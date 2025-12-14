#include <Wifi.h>
#include<PubSubClient.h>
#include<esp_wifi.h>
#include<ArduinoJson.h>
#include"config.h"

DynamicJsonDocument sensor_data_payload(1024);
char sensor_data_format_for_mqtt_publish[1024];

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

long lastReconnectAttempt = 0;

boolean reconnect(){
	if (client.connect(MQTT_CLIENT_ID))
	{
		Serial.println("attempt to connect to broker");
		client.subscribe(MQTT_TOPIC_SUBSCRIBE);
	}
	return client.connected();
		
}

void handleIncomingMessage(char* topic, byte* payload, unsigned int length){
	String message;
	for (unsigned int i=0; i<length; i++)
	{
		message += (char)payload[i];
	}

	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("]");
	Serial.println(message);

	if(String(topic) == MQTT_TOPIC_SUBSCRIBE)
	{
		if(message == "1"){
			digitalWrite(LED_BUILTIN, HIGH);
		} else if (message == "0"){
			digitalWrite(LED_BUILTIN, LOW);
		}
	}
}

void setup(){
	Serial.begin(115200);
	Serial.println("attempting to connect to WiFi....");
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID , WIFI_PASSWORD);
	if(WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		Serial.println("couldnt connect to wifi.");
	}
	
	Serial.println(WiFi.localIP());
	String macAddress = WiFi.macAddress();

	Serial.println("MAC address: "+macAddress);
	client.setServer(MQTT_BROKER, MQTT_PORT);
	client.setCallback(handleIncomingMessage);
	lastReconnectAttempt=0;

	pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
	if (!client.connected() )
	{
		long now = millis();
		if (now - lastReconnectAttempt > 5000)
		{
			lastReconnectAttempt = now;
			if(reconnect())
			{
				lastReconnectAttempt = 0;
			}
		}
	} 
	else 
	{
		Serial.println("connected to broker---!!!");
		client.loop();

		float humidity = random(0, 100 );
		float temperature = random(-16,56);

		sensor_data_payload["temperature"] = temperature;
		sensor_data_payload["humidity"] = humidity;

		serializeJson(sensor_data_payload, sensor_data_format_for_mqtt_publish);

		delay(2000);

		Serial.println("Humidity:" + String(humidity));
		Serial.println("Temperature:" + String(temperature));
		client.publish(MQTT_TOPIC_PUBLISH, sensor_data_format_for_mqtt_publish);
		Serial.println("Sensor data sent to broker");		

		delay(5000);
	}
}
