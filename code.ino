#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiS3.h>                        // ← ADDED (built-in for Uno R4 WiFi)

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TDS_PIN A0
#define ONE_WIRE_BUS 2

// ═══════════════════════════════════════════════
//   FILL THESE 3 THINGS — everything else is auto
// ═══════════════════════════════════════════════
const char* SSID        = "Parnika";       // ← your WiFi name
const char* PASSWORD    = "Vansh@123";   // ← your WiFi password
const char* TS_API_KEY  = "XGZ8HRQ75VZNILTF";   // ← from ThingSpeak channel
// ═══════════════════════════════════════════════

const char* TS_HOST     = "api.thingspeak.com";
const int   TS_PORT     = 80;
const int   SEND_EVERY  = 10;   // seconds (ThingSpeak free = min 15s)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiClient client;                                 // ← ADDED

unsigned long lastSend = 0;                        // ← ADDED timer

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  sensors.begin();

  // ── Connect WiFi ───────────────────────────── ADDED
  Serial.print("Connecting to WiFi");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
  // ───────────────────────────────────────────────────
}

void loop() {
  // ─────────────────────────────────────────────
  // 🌡️ Read temperature  ← YOUR CODE, unchanged
  // ─────────────────────────────────────────────
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // ─────────────────────────────────────────────
  // 🧪 Read TDS           ← YOUR CODE, unchanged
  // ─────────────────────────────────────────────
  int sensorValue = analogRead(TDS_PIN);
  float voltage = sensorValue * (5.0 / 1024.0);
  float compensation = 1.0 + 0.02 * (tempC - 25.0);
  float tds = (voltage / compensation) * 500;

  // ─────────────────────────────────────────────
  // 📺 Display            ← YOUR CODE, unchanged
  // ─────────────────────────────────────────────
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(tempC);
  display.println(" C");
  display.setCursor(0, 20);
  display.print("TDS:");
  display.setTextSize(2);
  display.setCursor(0, 35);
  display.print(tds);
  display.print(" ppm");
  display.display();

  // ─────────────────────────────────────────────
  // 📡 Send to ThingSpeak every 15s  ← ADDED
  // field1 = TDS (ppm)
  // field2 = Temperature (°C)
  // field3 = Raw TDS analog value
  // ─────────────────────────────────────────────
  unsigned long now = millis() / 1000;
  if (now - lastSend >= SEND_EVERY) {
    lastSend = now;

    if (WiFi.status() == WL_CONNECTED) {
      if (client.connect(TS_HOST, TS_PORT)) {

        String url = "/update?api_key=" + String(TS_API_KEY)
                   + "&field1=" + String(tds, 2)
                   + "&field2=" + String(tempC, 2)
                   + "&field3=" + String(sensorValue);

        client.println("GET " + url + " HTTP/1.1");
        client.println("Host: api.thingspeak.com");
        client.println("Connection: close");
        client.println();

        delay(500);
        String response = "";
        while (client.available()) {
          response = client.readStringUntil('\n');
        }
        client.stop();

        // ThingSpeak returns entry number on success (e.g. "5")
        // Returns "0" if rate limited or error
        Serial.println("ThingSpeak response: " + response);
        Serial.println("Sent → TDS:" + String(tds,1)
                     + " Temp:" + String(tempC,1)
                     + " Raw:" + String(sensorValue));

      } else {
        Serial.println("ThingSpeak connection failed");
      }
    } else {
      Serial.println("WiFi lost — reconnecting...");
      WiFi.begin(SSID, PASSWORD);
    }
  }
  // ─────────────────────────────────────────────

  delay(10000);   // ← YOUR delay, unchanged
}
