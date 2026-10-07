#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "credentials.h"
#include "network_client.h"

// Reference for WiFi credentials 
//constexpr const char* WIFI_SSID     = "YOUR WIFI_SSID";
//constexpr const char* WIFI_PASSWORD = "YOUR WIFI_PASSWORD";

void setup_Wifi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    Serial.println("WLAN erfolgreich verbunden! ");

    
}

