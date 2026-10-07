#include <ESP8266WiFi.h>        // Core Wi-Fi functions
#include <WiFiClient.h>         // Provides WiFiClient
#include <ESP8266HTTPClient.h>  // Provides HTTPClient
#include <ArduinoJson.h>        // Provides ArduinoJson library for JSON parsing
#include "credentials.h"        // Include your URL credentials
#include "fetchTramData.h"     // Include the header file for fetchTramData

const char* API_BASE_URL = "http://transport.opendata.ch/v1/stationboard";
const char* API_PARAMS   = "&transportations%5B%5D=tram&limit=3";

//String url = String(API_BASE_URL) + "?station=" + TARGET_STATION + API_PARAMS;

// const char* TARGET_STATION = "YOUR STATION NAME"; // Replace with your desired station name

void fetchTramDepartures() {
  WiFiClient client;
  HTTPClient http;

  int numDestinations = sizeof(destinations) / sizeof(destinations[0]);

  for (int i = 0; i < numDestinations; i++) {

    String url = String(API_BASE_URL) + 
                 "?station=" + TARGET_STATION + 
                 API_PARAMS + 
                 "&direction=" + destinations[i];

    Serial.printf("Sende HTTP GET-Anfrage an: %s\n", url.c_str());


    if (http.begin(client, url)) {
      //int httpCode = http.GET();
      
      if (http.GET() == HTTP_CODE_OK) {
        WiFiClient *stream = http.getStreamPtr();

        // 1. Tell ArduinoJson to KEEP the fields from the stream
        JsonDocument filter;
        filter["stationboard"][0]["to"] = true;
        filter["stationboard"][0]["number"] = true;
        filter["stationboard"][0]["stop"]["prognosis"]["departure"] = true;

        // 2. Parse from stream directly
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, *stream, DeserializationOption::Filter(filter));

        if (!error) {

          for (JsonObject boardItem : doc["stationboard"].as<JsonArray>()) {
              const char* toStation = boardItem["to"];
              const char* lineNumber = boardItem["number"];
              const char* depTime   = boardItem["stop"]["prognosis"]["departure"];
              Serial.printf("Abfahrt: Linie %s nach %s um %s\n", lineNumber, toStation, depTime);
          }
        } else {
          Serial.printf("Fehler beim Parsen der JSON-Daten: %s\n", error.c_str());
        }
      } else {
        Serial.printf("Fehler bei der HTTP-Anfrage: %d\n", http.GET());
      }

      http.end();
  
      
      } else {
        Serial.printf("Fehler bei der HTTP-Anfrage");
      }

      http.end();
    
    }
  http.end();
}