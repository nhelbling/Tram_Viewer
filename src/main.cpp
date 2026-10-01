#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define TFT_CS   D8
#define TFT_DC   D3
#define TFT_RST  D4


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "FibreBox_X6-522207-Guest";
const char* password = "CJd69F5aCC5C";

const int stationId = 8591329; // ID der Haltestelle (z.B. 8591329 für "Saalsporthalle")

void fetchTramDepartures() {
  WiFiClient client;
  HTTPClient http;

  String url = "http://transport.opendata.ch/v1/stationboard?station=Zuerich,Saalsporthalle&transportations[]=tram&limit=4";

  if(http.begin(client, url)) {
    int httpCode = http.GET();

    if(httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("Fehler bei der HTTP-Anfrage: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Fehler beim Initialisieren der HTTP-Verbindung");
  }

  http.end();
}

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
  
  // -------------------------------------------------------

  // WLAN-Verbindung herstellen --------------------------
  Serial.println();
  Serial.print("Verbinde mit: ");
  Serial.println(ssid);

  // WLAN-Modus auf Station (Client) setzen
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Warten bis die Verbindung steht
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // -------------------------------------------------------

  Serial.println();
  Serial.println("WLAN erfolgreich verbunden!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  fetchTramDepartures();
}


void loop() {
  for (int i = 0; i < 4; i++) {
    delay(60000); // 60 Sekunden warten
  }
  fetchTramDepartures();
}

