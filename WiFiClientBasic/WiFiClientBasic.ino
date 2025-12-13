#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;
WiFiServer server(80);   // Web server on port 80

int counter = 1;
#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  wifiMulti.addAP("KCH40", "123456788");

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Start web server
}

void handleWebClient() {
  WiFiClient client = server.available();
  if (!client) return;

  // Wait until client sends data
  while (!client.available()) delay(1);

  // Read HTTP request (not used, but required)
  client.readStringUntil('\r');
  client.flush();

  // HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta http-equiv='refresh' content='1'>");
  client.println("<title>ESP32 Counter</title>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>ESP32 Counter</h1>");
  client.println("<h2>Counter value: " + String(counter) + "</h2>");
  client.println("</body>");
  client.println("</html>");

  client.stop();
}

void loop() {
  handleWebClient();  // Handle browser requests

  Serial.println(++counter);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(80);
  digitalWrite(LED_BUILTIN, LOW);
  delay(80);

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
