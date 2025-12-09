#include <WiFi.h>
#include "esp_wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);

void listConnectedClients() {
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;

  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  Serial.println("\n--- Connected Clients ---");
  if (adapter_sta_list.num == 0) {
    Serial.println("No devices connected.");
  }

  for (int i = 0; i < adapter_sta_list.num; i++) {
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

    Serial.print("Client ");
    Serial.print(i + 1);
    Serial.print(" MAC: ");

    for (int b = 0; b < 6; b++) {
      Serial.printf("%02X", station.mac[b]);
      if (b < 5) Serial.print(":");
    }

    Serial.print("  IP: ");
    Serial.println(IPAddress(station.ip.addr));
  }
  Serial.println("-------------------------\n");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.println("\nConfiguring Access Point...");

  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Soft AP creation failed");
    while (1);
  }

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Check and display connected clients every 5 seconds
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 5000) {
    lastTime = millis();
    listConnectedClients();
  }

  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected");

    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);

        // Basic web response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();
        client.println("<html><body><h2>Hello from ESP32 AP Server</h2>");
        client.println("<p>Check Serial Monitor for connected devices</p>");
        client.println("</body></html>");
        break;
      }
    }

    delay(1);
    client.stop();
    Serial.println("Client Disconnected");
  }
}
