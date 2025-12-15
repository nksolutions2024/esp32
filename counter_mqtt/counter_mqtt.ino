#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

DynamicJsonDocument sensor_data_payload(256);
char sensor_data_format_for_mqtt_publish[256];

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

long lastReconnectAttempt = 0;
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

  Serial.println("Attempting to connect to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Could not connect to WiFi");
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(handleIncomingMessage);
}

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
    client.loop();

    // Increment counter
    counter++;

    // Create JSON payload
    sensor_data_payload.clear();
    sensor_data_payload["counter"] = counter;

    serializeJson(sensor_data_payload, sensor_data_format_for_mqtt_publish);

    // Publish counter
    client.publish(MQTT_TOPIC_PUBLISH, sensor_data_format_for_mqtt_publish);

    Serial.print("Counter sent: ");
    Serial.println(counter);

    delay(5000);  // publish every 5 seconds
  }
}

