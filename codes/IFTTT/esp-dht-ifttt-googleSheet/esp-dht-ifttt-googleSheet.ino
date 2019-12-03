#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTTYPE DHT11 // DHT11 or DHT22
#define DHTPIN  D4 //D3 on nodemcu
DHT dht(DHTPIN, DHTTYPE, 11);

// Replace with your SSID and Password
const char* ssid     = "flip-test";
const char* password = "flip1234";

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/dht11_readings/with/key/cC_wsSzEFvrIT7kRyX-su6";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

unsigned long nowTime = 0;
unsigned long lastTime = 0;
long interval = 60; // - 60 seconds between reports
float hum, tc, tf;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  delay(2000);
  dht.begin();
  delay(10);
  wifiConnect();
  //makeIFTTTRequest();
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    nowTime = millis();
    unsigned long x = (nowTime - lastTime);
    Serial.println("--------------------------------");
    Serial.print(F("POSTING Time (sec) = "));
    Serial.println(interval);
    Serial.print(F("last POST (sec) = "));
    Serial.println(x / 1000);
    hum = dht.readHumidity();
    tc = dht.readTemperature();
    tf = dht.readTemperature(true);
    Serial.println("Humidity = " + String(hum));
    Serial.println("Temperature *C = " + String(tc));
    Serial.println("Temperature *F = " + String(tf));
    Serial.println("--------------------------------");
    if (x > (interval * 1000))
    {
      Serial.println("Time to post");
      lastTime = nowTime;
      makeIFTTTRequest();
    }
  }
  else
  {
    wifiConnect();
  }
  delay(2000);
}

// Establish a Wi-Fi connection with your router
void wifiConnect()
{
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20 * 4; // 20 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: ");
  Serial.print(millis());
  Serial.print(", IP address: ");
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest()
{
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);
  String jsonObject = String("{\"value1\":\"") + (hum) + "\",\"value2\":\"" + (tc) + "\",\"value3\":\"" + (tf) + "\"}";
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0))
  {
    delay(100);
  }
  if (!!!client.available())
  {
    Serial.println("No response...");
  }
  while (client.available())
  {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}
