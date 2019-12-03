// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "flip-test";
const char* password = "flip1234";
String nodeId = "node0001";
const char* mqtt_server = "platform.myflip.io";
const char* publishTopic = "abhay/room1/sensor";//topic to publish temperature sensor data
const char* lwtTopic = "abhay/room1/lwt";//topic to publish temperature sensor data


long interval = 5000; //(ms) - 10 sec between reports

// DHT Sensor - GPIO 5 = D1 on ESP-12E NodeMCU board
const int DHTPin = 4;
