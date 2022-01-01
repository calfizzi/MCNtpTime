# MCNtpTime
**Get ESP Time from Internet to have an updated clock on your ESP!**

This Library works on ESP8266 and ESP32 to manage Ntp Time from internet granting regular time update.Between an update and the next one (e.i. each 60 minutes) it still works with internal Clock so in any case you can get the updated time.
You can regulate the update interval by begin function.


## Methods:
  ### ESP8266
```c++
  void begin ( const char *zoneCode, uint32_t updateIntervalTimeSeconds);
```
  ### ESP32
```c++
  void begin ( const char *zoneCode, bool useTask, uint32_t updateIntervalTimeSeconds);
```
```c++
  void                        update                  ( bool forceNtpUpdate, uint32_t retryOnFailMillis);
  struct tm                   getTime                 ( void );
  void                        setTimeZone             ( const char *Code);
  void                        setTimeZone             ( int minutes);
  void                        setDaylightOffset       ( int seconds);
  String                      getTimeZoneCode         ( int index);
  String                      getTimeZoneDescription  ( int index);
  String                      getTimeZoneGMT          ( int index);
  std::vector<timeZonesData>  getTimeZones            ( void );
  void                        printLocalTime          ( void ); // Nice Serial Print
```
## Example of use
```c++
void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  // .... connect to your WiFi
  // .... connect to your WiFi
  // .... connect to your WiFi
#if defined(ESP8266)
  MCNTPTime.begin("CET", 10);   // updateIntervalTimeSeconds  = 10 seconds
#elif defined(ESP32)
  MCNTPTime.begin("CET",true, 10);    // useTask = true, updateIntervalTimeSeconds  = 10 seconds
#endif
  MCNTPTime.printLocalTime();
}

void loop() {
  delay(1000);
#if defined (ESP8266)
  MCNTPTime.update();
#endif
  MCNTPTime.printLocalTime();
  
}
```



## Time Zones accepted:
```
    GMT  -  Greenwich Mean Time                           GMT 0:00  
    UTC  -  Universal Coordinated Time                    GMT 0:00  
    CET  -  Central European Time                         GMT +1:00
    EET  -  Eastern European Time                         GMT +2:00 
    ART  -  (Arabic) Egypt Standard Time                  GMT +2:00
    EAT  -  Eastern African Time                          GMT +3:00
    MET  -  Middle East Time                              GMT +3:30
    NET  -  Near East Time                                GMT +4:00
    PLT  -  Pakistan Lahore Time                          GMT +5:00
    IST  -  India Standard Time                           GMT +5:30
    BST  -  Bangladesh Standard Time                      GMT +6:00
    VST  -  Vietnam Standard Time                         GMT +7:00
    CTT  -  China Taiwan Time                             GMT +8:00
    JST  -  Japan Standard Time                           GMT +9:00
    ACT  -  Australia Central Time                        GMT +9:30
    AET  -  Australia Eastern Time                        GMT +10:00
    SST  -  Solomon Standard Time                         GMT +11:00
    NST  -  New Zealand Standard Time                     GMT +12:00
    MIT  -  Midway Islands Time                           GMT -11:00
    HST  -  Hawaii Standard Time                          GMT -10:00
    AST  -  Alaska Standard Time                          GMT -9:00 
    PST  -  Pacific Standard Time                         GMT -8:00 
    PNT  -  Phoenix Standard Time                         GMT -7:00 
    MST  -  Mountain Standard Time                        GMT -7:00 
    CST  -  Central Standard Time                         GMT -6:00 
    EST  -  Eastern Standard Time                         GMT -5:00 
    IET  -  Indiana Eastern Standard Time                 GMT -5:00 
    PRT  -  Puerto Rico and US Virgin Islands Time        GMT -4:00 
    CNT  -  Canada Newfoundland Time                      GMT -3:30 
    AGT  -  Argentina Standard Time                       GMT -3:00 
    BET  -  Brazil Eastern Time                           GMT -3:00 
    CAT  -  Central African Time                          GMT -1:00 
```
