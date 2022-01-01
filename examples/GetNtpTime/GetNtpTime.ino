#if defined(ESP8266)
  #include <ESP8266wifi.h>
  #include <ESP8266mDNS.h>
#else
  #include <wifi.h>
  #include <ESPmDNS.h>
#endif
#include <MCNTPTime.h>

const char* hostname  = "GetNTPTime-Example";
const char* ssid      = ""; // Please change me
const char* password  = "";

void startWiFi()
{
  uint32_t ms = millis();
	if (strlen(ssid)==0)
		Serial.println("****** PLEASE MODIFY ssid/password *************");

  WiFi.mode        ( WIFI_STA);
  WiFi.begin       ( ssid, password);
  WiFi.setHostname ( hostname);
 
  Serial.print("WiFi connection..");
  while (WiFi.status() != WL_CONNECTED && millis()-ms <10000) {   
    Serial.print('.');
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED )
  {
    #if defined(ESP8266)
        ESP.restart();
    #elif defined(ESP32)
        esp_restart();
    #endif
  }
  Serial.println();
  WiFi.setHostname(hostname);
  if (!MDNS.begin    ( hostname )) 
  {
    Serial.println("Error setting up MDNS responder ( " + String(hostname) +  " )!");
  }else
    Serial.println("mDNS responder started ( " + String(hostname) +  " )");

  Serial.println( "\r\nConnected to " + String(ssid) + ", local IP address: " + String(WiFi.localIP().toString()));  

}


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Started!");
  startWiFi();
#if defined(ESP8266)
  MCNTPTime.begin("CET", 60);   
#elif defined(ESP32)
  MCNTPTime.begin("CET",true, 60);    // useTask = true, updateIntervalTimeSeconds  = 60 seconds
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
