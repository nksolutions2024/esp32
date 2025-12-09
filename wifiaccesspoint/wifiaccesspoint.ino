#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_netif.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);

void listConnectedClients() {
  wifi_sta_list_t wifi_sta_list;
  esp_netif_sta_list_t netif_sta_list;

  // Get MACs of connected stations
  esp_wifi_ap_get_sta_list(&wifi_sta_list);

  // Convert MAC list to IP list
  esp_netif_get_sta_list(&wifi_sta_list, &netif_sta_list);

  Serial.println("\n--- Connected Clients ---");
  if (netif_sta_list.num == 0) {
    Serial.println("No devices connected.");
  }

  for (int i = 0; i < netif_sta_list.num; i++) {
    esp_netif_sta_info_t station = netif_sta_list.sta[i];

    Serial.print("Client ");
    Serial.print(i + 1);
    Serial.print(" MAC: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X",
                  station.mac[0], station.mac[1], station.mac[2],
                  station.mac[3], station.mac[4], station.mac[5]);

    Serial.print("  IP: ");
    Serial.println(IPAddress(station.ip.addr));
  }
  Serial.println("-------------------------\n");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Configuring Access Point...");

  if (!WiFi.softAP(ssid, password)) {
    Serial.println("AP setup failed!");
    while (1);
  }

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  Serial.println("Server started");
}

void loop() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 5000) {
    lastCheck = millis();
    listConnectedClients();
  }

  WiFiClient client = server.accept();

  if (client) {
    Serial.println("Client Connected");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println
