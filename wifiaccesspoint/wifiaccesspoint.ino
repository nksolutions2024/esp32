#include <WiFi.h>
#include "esp_wifi.h"

const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);

void listConnectedClients() {
  wifi_sta_list_t wifi_sta_list;
  esp_wifi_ap_get_sta_list(&wifi_sta_list);

  Serial.println("\n--- Connected Clients (MAC only) ---");
  if (wifi_sta_list.num == 0) {
    Serial.println("No devices connected.");
  }

  for (int i = 0; i < wifi_sta_list.num; i++) {
    uint8_t *mac = wifi_sta_list.sta[i].mac;

    Serial.printf("Client %d MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  i + 1,
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }
  Serial.println("------------------------------------\n");
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  server.begin();

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  static unsigned long timer = 0;
  if (millis() - timer > 5000) {
    timer = millis();
    listConnectedClients();
  }

  WiFiClient client = server.accept();
  if (client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: close");
    client.println();
    client.println("<h2>Hello from ESP32!</h2>");
    delay(1);
    client.stop();
  }
}
