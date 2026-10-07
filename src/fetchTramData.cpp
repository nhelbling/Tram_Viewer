#include <ESP8266WiFi.h>        // Core Wi-Fi functions
#include <WiFiClient.h>         // Provides WiFiClient
#include <ESP8266HTTPClient.h>  // Provides HTTPClient
#include <ArduinoJson.h>        // Provides ArduinoJson library for JSON parsing
#include "credentials.h"        // Include your URL credentials
#include "fetchTramData.h"     // Include the header file for fetchTramData

const char* API_BASE_URL = "http://transport.opendata.ch/v1/stationboard";
const char* API_PARAMS   = "&transportations[]=tram&limit=4";

String url = String(API_BASE_URL) + "?station=" + TARGET_STATION + API_PARAMS;

// const char* TARGET_STATION = "YOUR STATION NAME"; // Replace with your desired station name

void fetchTramDepartures() {
  WiFiClient client;
  HTTPClient http;

  Serial.printf("Sende HTTP GET-Anfrage an: %s\n", url.c_str());

  if(http.begin(client, url)) {
    int httpCode = http.GET();
    
    if(httpCode > 0) {
      String payload = http.getString();
      Serial.printf("Empfangene Daten");
    } else {
      Serial.printf("Fehler bei der HTTP-Anfrage: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("Fehler beim Initialisieren der HTTP-Verbindung");
  }

  http.end();
}