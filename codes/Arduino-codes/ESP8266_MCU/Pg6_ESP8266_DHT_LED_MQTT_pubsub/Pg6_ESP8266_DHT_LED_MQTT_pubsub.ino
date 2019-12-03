/*****
  DHT11 + Relay + ESP + MQTT

*****/

#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "DHT.h"
#include "userCredentials.h"

// Uncomment one of the lines bellow for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
char* macid;
String deviceId = "";
void setup()
{
  dht.begin();
  Serial.begin(115200);
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId += macToStr(mac);
  Serial.println(deviceId);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  mqtt_connect();
  Serial.println("Start");
}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop()
{
  if (!client.connected())
  {
    mqtt_connect();
  }
  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasure > interval)
  {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    String my_data;
    my_data = "{\"uid\":\"";
    my_data += nodeId;
    my_data += "\",";
    my_data += "\"temperature\":\"";
    my_data += String(t);
    my_data += "\",";
    my_data += + "\"humidity\":\"";
    my_data += String(h);
    my_data += "\"}";
    Serial.println(my_data);
    Serial.print("DHT Data:\r\nTemp: " + String(t) + "*C\t");
    Serial.println("Humi: " + String(h) + " % RH\t");
    // Publishes Temperature and Humidity values
    client.publish(publishTopic, String(my_data).c_str());
  }
}
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void mqtt_connect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(deviceId.c_str(), lwtTopic, 2, 0, "0"))
    {
      Serial.println("Connected to MQTT broker as: " + String(deviceId));
      deviceId = "";
    }
    else
    {
      Serial.println("MQTT connect failed");
      Serial.println("Will reset and try again...");
      delay(5000);
      abort();
    }
  }
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String payload;

  for (int i = 0; i < length; i++)
  {
    payload += (char)message[i];
  }
  Serial.print(payload);
  Serial.println();
  Serial.println();
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}
