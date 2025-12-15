#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

const char* WIFI_SSID = "TP-Link_AB3A";
const char* WIFI_PASSWORD = "63104066" ;

const char* MQTT_BROKER = "192.168.0.109";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID  = "iotnode-08";

const char* MQTT_USERNAME = "desd8";
const char* MQTT_PASSWORD = "desd1234";


const char* MQTT_TOPIC_PUBLISH = "cdac/diot/telemetry";
const char* MQTT_TOPIC_SUBSCRIBE = "cdac/diot/led/control";

const uint8_t NEW_MAC_ADDRESS[] = {0xdc, 0x1b, 0xa1, 0x64, 0x5e, 0xbf};


// Smaller buffer for ESP8266
DynamicJsonDocument sensor_data_payload(128);
char sensor_data_format_for_mqtt_publish[128];

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastReconnectAttempt = 0;
unsigned long counter = 0;

boolean reconnect() {
  if (client.connect(MQTT_CLIENT_ID)) {
    Serial.println("Connected to MQTT broker");
  }
  return client.connected();
}

void handleIncomingMessage(char* topic, byte* payload, unsigned int length) {
  // No incoming message handling required
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("Connecting to WiFi...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(handleIncomingMessage);
}

void loop() {
  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();

    counter++;

    sensor_data_payload.clear();
    sensor_data_payload["counter"] = counter;

    serializeJson(sensor_data_payload, sensor_data_format_for_mqtt_publish);

    client.publish(MQTT_TOPIC_PUBLISH, sensor_data_format_for_mqtt_publish);

    Serial.print("Counter sent: ");
    Serial.println(counter);

    delay(8);   // publish every 5 seconds
  }

  yield();  // important for ESP8266 stability
}
