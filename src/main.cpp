#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "credentials.h"
#include "network_client.h"
#include "fetchTramData.h"

#define TFT_CS   D8
#define TFT_DC   D3
#define TFT_RST  D4


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  delay(100);

  // TFT-Display initialisieren --------------------------
  tft.begin();
  tft.setRotation(1); // 1 = Querformat (320x240)
  tft.fillScreen(ILI9341_BLACK);

  // Titelzeile zeichnen
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TRAM ABFAHRTEN");
  
  // WIFI CONNECTION -------------------------------------------------------
  setup_Wifi();

}


void loop() {
  for (int i = 0; i < 4; i++) {
    delay(1000); // 60 Sekunden warten
  }
  fetchTramDepartures();
}

