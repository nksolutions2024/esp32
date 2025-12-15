#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include "config.h"

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

long lastReconnectAttempt = 0;

/* ---------- MQTT RECONNECT ---------- */
boolean reconnect() {
  if (client.connect(MQTT_CLIENT_ID)) {
    Serial.println("Connected to MQTT broker");
    client.subscribe(MQTT_TOPIC_SUBSCRIBE);
    Serial.println("Subscribed to topic");
  }
  return client.connected();
}

/* ---------- MESSAGE CALLBACK ---------- */
void handleIncomingMessage(char* topic, byte* payload, unsigned int length) {
  String message;

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);

  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(handleIncomingMessage);

  lastReconnectAttempt = 0;
}

/* ---------- LOOP ---------- */
void loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();  // Keep MQTT connection alive
  }
}

