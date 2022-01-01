#include "MCNtpTime.h"

MCNTPTime_Class MCNTPTime;

#if defined(ESP8266)
  bool getLocalTime(struct tm *timeData)
  {
    time_t rawtime = 0; 
    struct tm* timeinfo = NULL; 
    time(&rawtime); 
    timeinfo = localtime(&rawtime); 
    if (rawtime >10000 && timeinfo!=NULL )
    {
      memcpy(timeData, timeinfo, sizeof(struct tm));
      return true;
    }
    return false;
  }
#endif
#if defined(ESP32)
                                              MCNTPTime_Class::~MCNTPTime_Class         ( void )
{
  if (_TaskOneHandle!=0)
    vTaskDelete(_TaskOneHandle);
}
#endif
#if defined(ESP8266)
void                                          MCNTPTime_Class::begin                    ( const char *zoneCode, uint32_t updateIntervalTimeSeconds)
{
  _setZoneCode(zoneCode);
  _updateIntervalTimeSeconds = updateIntervalTimeSeconds;
  update(true);
}
#elif defined(ESP32)
void                                          MCNTPTime_Class::begin                    ( const char *zoneCode, bool useTask, uint32_t updateIntervalTimeSeconds)
{
  _setZoneCode(zoneCode);
  _updateIntervalTimeSeconds = updateIntervalTimeSeconds;
  update(true);
  delay(500);
  if (useTask)
    xTaskCreate(MCNTPTime_Class::_TaskFunction, "MCNTPTime_Update", 10000, this, 1,&_TaskOneHandle);
}
#endif
void                                          MCNTPTime_Class::update                   ( bool forceNtpUpdate, uint32_t retryOnFailMillis)
{
  if (forceNtpUpdate || _getUpdateTimeElapsed()>_updateIntervalTimeSeconds*1000)
  {
    configTime(_gmtOffset_sec, _daylightOffset_sec, _ntpServer);
    if (WiFi.status() == WL_CONNECTED)
    {
      while(!getLocalTime(&_ntpTime)){
        Serial.println("Failed to obtain time");
        delay(retryOnFailMillis);
      }
    }else
      Serial.println("WiFi Not Connected!\r\n");
    _update_ms = millis();
    #if MCNTPTime_DEGUB
      Serial.println("\r\nNTP Time Updated!\r\n");
    #endif  
    _ms = millis()-700; // remove answer delay ~700 millis
    //Serial.println("NTP Time Got!");
  }
}
struct tm                                     MCNTPTime_Class::getTime                  ( void )
{
  time_t t = mktime(&_ntpTime) + _getElapsed() / 1000;
  return *localtime(&t);
}
void                                          MCNTPTime_Class::setTimeZone              ( const char *code)
{
  _setZoneCode(code);
  update();
}
void                                          MCNTPTime_Class::setTimeZone              ( int minutes)
{
  _gmtOffset_sec = (int32_t)minutes*60; // to seconds
  update();
}
void                                          MCNTPTime_Class::setDaylightOffset        ( int seconds)
{
  _daylightOffset_sec = seconds;
  update();
}
String                                        MCNTPTime_Class::getTimeZoneCode          ( int index)
{
  if (index<0 || index>TIME_ZONES_COUNT-1)
    index = _getCurrentTimeZoneIndex();
  char temp[50]="unknown";
  if(index>=0)
    strcpy ( temp,  timeZones[index].Code);
  return String (temp);
}
String                                        MCNTPTime_Class::getTimeZoneDescription   ( int index)
{
  if (index<0 || index>TIME_ZONES_COUNT-1)
    index = _getCurrentTimeZoneIndex();
  char temp[100]="unknown";
  if(index>=0)
    strcpy ( temp,  timeZones[index].Zone);
  return String (temp);
}
String                                        MCNTPTime_Class::getTimeZoneGMT           ( int index)
{
  if (index<0 || index>TIME_ZONES_COUNT-1)
    index = _getCurrentTimeZoneIndex();
  char temp[20]="unknown";
  if(index>=0)
    strcpy ( temp,  timeZones[index].GMT);
  return String (temp);
}
std::vector<MCNTPTime_Class::timeZonesData>   MCNTPTime_Class::getTimeZones             ( void )
{
  std::vector<timeZonesData> returnData(TIME_ZONES_COUNT);
  char temp[80];
  for (size_t i = 0; i < TIME_ZONES_COUNT; i++)
  {
    strcpy(temp, timeZones[i].Code);
    returnData[i].code = temp;
    strcpy(temp, timeZones[i].Zone);
    returnData[i].description = temp;
    strcpy(temp, timeZones[i].GMT);
    returnData[i].GMTOffset = temp;
    returnData[i].GMTOffsetMinutes = timeZones[i].GMTOffset;
  }
}
void                                          MCNTPTime_Class::printLocalTime           ( void )
{
  struct tm timeinfo = getTime();
#if defined(ESP8266)
  Serial.printf("%02d/%02d/%04d %02d:%02d:%02d", 
    timeinfo.tm_mday, 
    timeinfo.tm_mon+1,
    timeinfo.tm_year + 1900,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  );
  
#elif defined(ESP32)
  Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S");
#endif
  Serial.println(" (" + getTimeZoneCode() + ") " + getTimeZoneDescription()  +" "+ getTimeZoneGMT());
  Serial.println();
}
int                                           MCNTPTime_Class::_getCurrentTimeZoneIndex ( void )
{
  for (size_t i = 0; i < TIME_ZONES_COUNT; i++)
    if (timeZones[i].GMTOffset==_gmtOffset_sec/60)
      return i;
  return -1;
}
uint32_t                                      MCNTPTime_Class::_getElapsed              ( void )
{
  uint32_t returnValue;
  if (_ms>millis())
    returnValue = 0xFFFFFFFF -_ms + millis(); 
  else
    returnValue = millis()-_ms;
  return returnValue;
}
uint32_t                                      MCNTPTime_Class::_getUpdateTimeElapsed    ( void )
{
  uint32_t returnValue;
  if (_update_ms>millis())
    returnValue = 0xFFFFFFFF -_update_ms + millis(); 
  else
    returnValue = millis()-_update_ms;
  return returnValue;
}
 #if defined(ESP32)
void                                         MCNTPTime_Class::_TaskFunction            ( void *data)
{
  MCNTPTime_Class *NTPTime = (MCNTPTime_Class *)data;
  while(1)
  {
//Serial.println("Getting NTP time...");
    NTPTime->update(true);
//Serial.print("NTPTime->_updateIntervalTimeSeconds: "); Serial.println(NTPTime->_updateIntervalTimeSeconds);
    delay(NTPTime->_updateIntervalTimeSeconds*1000);
  }
}
#endif
 void                                         MCNTPTime_Class::_setZoneCode             ( const char *Code)
 {
  for (size_t i = 0; i < TIME_ZONES_COUNT; i++)
  {
    if (strcmp(Code,timeZones[i].Code) == 0)
    {
      _gmtOffset_sec = ((int32_t)timeZones[i].GMTOffset)*60; // minute to seconds
      return;
    }
  }

 }
