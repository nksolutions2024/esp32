#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(11520);
  wifiMulti.addAP("KCH40", "123456788");

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;

  if (!client.connect("192.168.1.10", 1337)) {
    Serial.println("Connect fail");
    delay(3000);
    return;
  }

  client.println("Hello Server");

  unsigned long t = millis();
  while (!client.available() && millis() - t < 2000) delay(10);

  if (client.available()) {
    Serial.println(client.readString());
  } else {
    Serial.println("No data");
  }

  client.stop();
  delay(3000);
}
