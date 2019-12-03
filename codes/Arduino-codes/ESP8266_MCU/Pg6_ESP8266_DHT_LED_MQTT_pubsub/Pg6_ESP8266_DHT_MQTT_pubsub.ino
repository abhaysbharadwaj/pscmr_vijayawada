/*****
  DHT11 + Relay + ESP + MQTT
abhaysbharadwaj@gmail.com
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
  Serial.println("");
  Serial.println("");
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId += macToStr(mac);
  Serial.println(deviceId);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
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
    Serial.print("DHT Data:\r\nTemp: " + String(t) + "*C\t");
    Serial.println("Humd: " + String(h) + " % RH\t");
    // Publishes Temperature and Humidity values
    client.publish(statusTopic, "1");
    client.publish(tempTopic, String(t).c_str());
    delay(100);
    client.publish(humdTopic, String(h).c_str());
  }
  client.loop();
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
      int deviceStatus = 1;
      client.subscribe(switchTopic);
      client.publish(statusTopic, "1");
      //client.publish(lwtTopic, "1");
      Serial.println("Published to Status Topic");
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
  Serial.println(topic);
  Serial.print(". Message: ");
  String payload;

  for (int i = 0; i < length; i++)
  {
    payload += (char)message[i];
  }
  Serial.print(payload);
  Serial.println();
  if (String(payload) == "1")
  {
    digitalWrite(relay, HIGH);
    Serial.println("Relay is ON");
  }
  else
  {
    digitalWrite(relay, LOW);
    Serial.println("Relay is OFF");
  }
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
