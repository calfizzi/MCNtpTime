# MCNtpTime
This Library works on ESP8266 and ESP32 to manage Ntp Time from internet granting regular time update.
      between the updates it works with internal Clock so in any time you can get updated current time.


## Methods:
```
  ## ESP8266
    void                      begin                   ( const char *zoneCode=(char*)"GMT", uint32_t updateIntervalTimeSeconds =  3600);
  ## ESP32
    void                      begin                   ( const char *zoneCode="GMT", bool useTask=true, uint32_t updateIntervalTimeSeconds =  3600);

  void                        update                  ( bool forceNtpUpdate = false, uint32_t retryOnFailMillis = 1000);
  struct tm                   getTime                 ( void );
  void                        setTimeZone             ( const char *Code);
  void                        setTimeZone             ( int minutes);
  void                        setDaylightOffset       ( int seconds = 3600);
  String                      getTimeZoneCode         ( int index=-1);
  String                      getTimeZoneDescription  ( int index=-1);
  String                      getTimeZoneGMT          ( int index=-1);
  std::vector<timeZonesData>  getTimeZones            ( void );
  void                        printLocalTime          ( void ); // Nice Serial Print
```