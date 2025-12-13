#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> //Benolt Blanchon
#include <esp_wifi.h>

// ----------- MAC Address (Spoofed) -----------
uint8_t newMACAddress[] = {0xF4, 0x96, 0x34, 0x9D, 0xE6, 0x2B};

// ----------- WiFi Credentials -----------
const char* ssid     = "KCH40";
const char* password = "123456788";

// ----------- ThingsBoard HTTP Telemetry URL -----------
const char* serverName =
  "http://thingsboard.cloud/api/v1/LMH7Owp5zlQJX5U18eYa/telemetry";

// ----------- Variables -----------
long humidityBetween1To100;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;  // 10 seconds

DynamicJsonDocument sensorData(1024);

// ----------- Setup -----------
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);

  WiFi.begin(ssid, password);

  Serial.print("ESP Board MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer Delay: 10 Seconds for sending second unit");
}

// ----------- Loop -----------
void loop() {
  humidityBetween1To100 = random(1, 100);

  sensorData["Location"]    = "Pune";
  sensorData["Temperature"] = 27;
  sensorData["Humidity"]    = humidityBetween1To100;
  sensorData["Pressure"]    = 960;

  if ((millis() - lastTime) > timerDelay) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String json;
      serializeJson(sensorData, json);

      int responseCode = http.POST(json);
      Serial.print("HTTP Response code: ");
      Serial.println(responseCode);

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    lastTime = millis();
  }
}