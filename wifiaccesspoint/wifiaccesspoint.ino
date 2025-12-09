#include <WiFi.h>
#include <esp_wifi.h>

const char* ssid = "yourAP";
const char* password = "yourPassword";

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  wifi_sta_list_t list;
  esp_wifi_ap_get_sta_list(&list);

  Serial.println("Connected Clients:");
  for (int i = 0; i < list.num; i++) {
    uint8_t* mac = list.sta[i].mac;
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }

  if (list.num == 0) {
    Serial.println("None");
  }

  Serial.println("----------------");
  delay(5000);
}
