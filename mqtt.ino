#include <Wifi.h>
#include<PubSubClient.h>
#include<esp_wifi.h>
#include<ArduinoJson.h>
#include"config.h"

DynamicJsonDocument sensor_data_payload(1024);
char sensor_data_format_for_mqtt-publish[1024];

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

long lastReconnectAttempt = 0;

boolean reconnect(){
	if (client.connect(MQTT_CLIENT_ID))
	{
		Serial.println();
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

	Serial.println(topic);
	Serial.println(message);

	if(String(topic) == MQTT_TOPIC_SUBSCRIBE)
	{
		if(message == "1"){
		digitalWrite(LED_BUILTIN, HIGH);
		} else if (message == "0"){
			digitalWrite(,LOW);
		}
	}
}

void setup(){
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid,password); //remaining
	if(WiFi.waitForConnectResult() != WL_CONNECTED){
	Serial.println("couldnt connect to wifi");
	}
	
	println(WiFi.localIP());
	String macAddress = WiFi.macAddress();

	Serial.println(""+macAddress);
	client.setServer(MQTT_BROKER, MQTT_PORT);
	client.setCallback(handleIncomingMessage);
	lastReconnectAttempt=0

	pinMode
}

void loop(){
	if (!client.connected()){
		long now = millis();
		if (now)
	



		if(reconnect()){
			lastReconnectAttempt = 0;
		}
		}
	} else {
		Serial broker
		client.loop();

		float humidity = random(0, 100 );
		float temperature = random(-16,56);

		sensor_data_payload["temperature"] = temperature;
		humidity;

		serializeJson(sensor_data_payload, sensor_data_format_for_mqtt_publish);

		delay(2000);

		Serial.println(String(humidity));
		temperature;
		client.publish(MQTT_TOPIC_PUBLISH, sensor_data_format_for_mqtt_publish);
		Serial.println("");		

		delay(5000);
}
}
	


