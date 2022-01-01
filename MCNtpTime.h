/*

  Module: MCNtpTime 
  Author: Mario Calfizzi (MC)

  Description:
      This Library works on ESP8266 and ESP32 to manage Ntp Time from internet granting regular time update.
      between the updates it works with internal Clock so in any time you can get updated current time.

   Location: https://github.com/calfizzi/MCNtpTime

*/
#pragma once

#ifndef MCNTPTime_h
#define MCNTPTime_h
#include <arduino.h>
#include <vector>
#include <string>
#include <sys/time.h>
#if defined(ESP8266)
  #include <ESP8266wifi.h>
#elif  defined(ESP32)
  #include <wifi.h>
  #include <freertos/task.h>
#endif

#define MCNTPTime_DEGUB 0

#define TIME_ZONES_COUNT 32
class MCNTPTime_Class
{
public:
  typedef struct timeZonesData_s
  {
    String  code;
    String  description;
    int32_t GMTOffsetMinutes;
    String  GMTOffset;
  }timeZonesData;
#if defined(ESP32)
  ~MCNTPTime_Class         ( void );
#endif
#if defined(ESP8266)
  void                        begin                   ( const char *zoneCode=(char*)"GMT", uint32_t updateIntervalTimeSeconds =  3600);
#elif defined(ESP32)
  void                        begin                   ( const char *zoneCode="GMT", bool useTask=true, uint32_t updateIntervalTimeSeconds =  3600);
#endif

  void                        update                  ( bool forceNtpUpdate = false, uint32_t retryOnFailMillis = 1000);
  struct tm                   getTime                 ( void );
  void                        setTimeZone             ( const char *Code);
  void                        setTimeZone             ( int minutes);
  void                        setDaylightOffset       ( int seconds = 3600);
  String                      getTimeZoneCode         ( int index=-1);
  String                      getTimeZoneDescription  ( int index=-1);
  String                      getTimeZoneGMT          ( int index=-1);
  std::vector<timeZonesData>  getTimeZones            ( void );
  void                        printLocalTime          ( void );
private:
  typedef struct timeZone_s
  {
    char *Code;
    char *Zone;
    int GMTOffset;
    char *GMT;
  }timeZone;
  const timeZone timeZones[TIME_ZONES_COUNT] = 
  {
    {  (char*) "GMT", (char*)"Greenwich Mean Time",                       0, (char*)"GMT 0:00"   },
    {  (char*) "UTC", (char*)"Universal Coordinated Time",                0, (char*)"GMT 0:00"   },
    {  (char*) "CET", (char*)"Central European Time",                    60, (char*)"GMT +1:00"  },
    {  (char*) "EET", (char*)"Eastern European Time",                   120, (char*)"GMT +2:00"  },
    {  (char*) "ART", (char*)"(Arabic) Egypt Standard Time",            120, (char*)"GMT +2:00"  },
    {  (char*) "EAT", (char*)"Eastern African Time",                    180, (char*)"GMT +3:00"  },
    {  (char*) "MET", (char*)"Middle East Time",                        210, (char*)"GMT +3:30"  },
    {  (char*) "NET", (char*)"Near East Time",                          240, (char*)"GMT +4:00"  },
    {  (char*) "PLT", (char*)"Pakistan Lahore Time",                    300, (char*)"GMT +5:00"  },
    {  (char*) "IST", (char*)"India Standard Time",                     330, (char*)"GMT +5:30"  },
    {  (char*) "BST", (char*)"Bangladesh Standard Time",                360, (char*)"GMT +6:00"  },
    {  (char*) "VST", (char*)"Vietnam Standard Time",                   420, (char*)"GMT +7:00"  },
    {  (char*) "CTT", (char*)"China Taiwan Time",                       480, (char*)"GMT +8:00"  },
    {  (char*) "JST", (char*)"Japan Standard Time",                     540, (char*)"GMT +9:00"  },
    {  (char*) "ACT", (char*)"Australia Central Time",                  590, (char*)"GMT +9:30"  },
    {  (char*) "AET", (char*)"Australia Eastern Time",                  600, (char*)"GMT +10:00" },
    {  (char*) "SST", (char*)"Solomon Standard Time",                   660, (char*)"GMT +11:00" },
    {  (char*) "NST", (char*)"New Zealand Standard Time",               720, (char*)"GMT +12:00" },
    {  (char*) "MIT", (char*)"Midway Islands Time",                    -660, (char*)"GMT -11:00" },
    {  (char*) "HST", (char*)"Hawaii Standard Time",                   -600, (char*)"GMT -10:00" },
    {  (char*) "AST", (char*)"Alaska Standard Time",                   -540, (char*)"GMT -9:00"  },
    {  (char*) "PST", (char*)"Pacific Standard Time",                  -480, (char*)"GMT -8:00"  },
    {  (char*) "PNT", (char*)"Phoenix Standard Time",                  -420, (char*)"GMT -7:00"  },
    {  (char*) "MST", (char*)"Mountain Standard Time",                 -420, (char*)"GMT -7:00"  },
    {  (char*) "CST", (char*)"Central Standard Time",                  -360, (char*)"GMT -6:00"  },
    {  (char*) "EST", (char*)"Eastern Standard Time",                  -300, (char*)"GMT -5:00"  },
    {  (char*) "IET", (char*)"Indiana Eastern Standard Time",          -300, (char*)"GMT -5:00"  },
    {  (char*) "PRT", (char*)"Puerto Rico and US Virgin Islands Time", -240, (char*)"GMT -4:00"  },
    {  (char*) "CNT", (char*)"Canada Newfoundland Time",               -210, (char*)"GMT -3:30"  },
    {  (char*) "AGT", (char*)"Argentina Standard Time",                -180, (char*)"GMT -3:00"  },
    {  (char*) "BET", (char*)"Brazil Eastern Time",                    -180, (char*)"GMT -3:00"  },
    {  (char*) "CAT", (char*)"Central African Time",                    -60, (char*)"GMT -1:00"  }
  };
  int                        _getCurrentTimeZoneIndex ( void );
  uint32_t                   _getElapsed              ( void );
  uint32_t                   _getUpdateTimeElapsed    ( void );
  void                       _setZoneCode             ( const char *Code);
#if defined(ESP32)
  static void                _TaskFunction            ( void *data);
#endif
  struct tm                  _ntpTime ={0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint32_t                   _ms = millis(); // last read millis;
  char                       _ntpServer[40] = "pool.ntp.org";
  long                       _gmtOffset_sec = 0;
  int                        _daylightOffset_sec = 0;
#if defined(ESP32)
  TaskHandle_t               _TaskOneHandle = 0;
#endif
  uint32_t                   _updateIntervalTimeSeconds =  3600;
  uint32_t                   _update_ms = 0;
};

extern MCNTPTime_Class MCNTPTime;
#endif