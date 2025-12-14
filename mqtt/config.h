
#ifndef CONFIG_H
#define CONFIG_H

const char* WIFI_SSID = "KCH40";
const char* WIFI_PASSWORD = "123456788" ;

const char* MOTT_BROKER = "13.233.232.181";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID  = "iotnode-08";

const char* MQTT_USERNAME = "desd8";
const char* MQTT_PASSWORD = "desd1234";


const char* MQTT_TOPIC_PUBLISH = "cdac/diot/telemetry";
const char* MQTT_TOPIC_SUBSCRIBE = "cdac/diot/led/control";

const uint8_t NEW_MAC_ADDRESS[] = {0xdc, 0x1b, 0x64, 0x5e, 0xbf};

#endif
