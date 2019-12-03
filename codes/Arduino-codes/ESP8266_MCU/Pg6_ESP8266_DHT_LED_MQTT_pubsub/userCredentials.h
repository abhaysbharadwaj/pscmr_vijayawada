// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "abs_pocof1";
const char* password = "abs@1234";
String nodeId = "node01";
const char* mqtt_server = "23.20.0.61";
const char* tempTopic = "abhay/home/room1/temp";//topic to publish temperature sensor data
const char* humdTopic = "abhay/home/room1/humd";//topic to publish humidity sensor data
const char* statusTopic = "abhay/home/room1/status";//topic to publish temperature sensor data
const char* lwtTopic = "abhay/home/room1/lwt";//topic to publish lwt

const char* switchTopic = "abhay/home/room1/switch";//topic to subscribe for switch data from app

long interval = 10000; //(ms) - 10 sec between reports

// DHT Sensor - GPIO 5 = D1 on ESP-12E NodeMCU board
const int DHTPin = D4;
#define relay D8
