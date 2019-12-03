String userId = "user01";
unsigned long intervalSec = 30;

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

String postUrl = "http://23.20.0.61:8000/api/sensor";
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
#define dht_pin D4
DHT dht(dht_pin, DHTTYPE);
const char* ssid = "abs_pocof1";
const char* password = "abs@1234";
unsigned long previousMillis = 0, interval = intervalSec * 1000;
float h, t;
String clientName = "";
unsigned long currentMillis = 0;
void setup()
{
  dht.begin();
  Serial.begin(115200);                                  //Serial connection
  WiFi.begin(ssid, password);   //WiFi connection

  Serial.println("Waiting for WiFi connection");
  while (WiFi.status() != WL_CONNECTED)   //Wait for the WiFI connection completion
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() == WL_CONNECTED)
  {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    //clientName += "-";
    clientName += macToStr(mac);
    Serial.print("Device Mac: ");
    Serial.println(clientName);
    Serial.println("");
    Serial.print("Connected to " + String (ssid) + " with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    previousMillis = millis();
  }
}

void loop()
{
  if (dht.read())
  {
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.print("DHT Data:\r\nTemperature: " + String(t) + "*C\t");
    Serial.println("Humidity: " + String(h) + " % RH\t");
  }
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    Serial.println("Its time to POST data");
    previousMillis = millis();
    if (WiFi.status() == WL_CONNECTED)  //Check WiFi connection status
    {
      HTTPClient http;    //Declare object of class HTTPClient

      http.begin(postUrl);      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header
      String my_http_data;
      my_http_data = "{\"userId\":\"";
      my_http_data += userId;
      my_http_data += "\",";
      my_http_data += "\"mac\":\"";
      my_http_data += clientName;
      my_http_data += "\",";
      my_http_data += "\"temperature\":\"";
      my_http_data += String(t);
      my_http_data += "\",";
      my_http_data += + "\"humidity\":\"";
      my_http_data += String(h);
      my_http_data += "\"}";
      Serial.println("Payload is: ");
      Serial.println(my_http_data);
      int httpCode = http.POST(my_http_data);   //Send the request
      String payload = http.getString();                  //Get the response payload

      Serial.println("HTTP Response is: " + String(httpCode));   //Print HTTP return code
      if (String(httpCode) == "-1")
        Serial.println("No internet connection");
      Serial.println(payload);    //Print request response payload

      http.end();  //Close connection

    }
    else
    {

      Serial.println("Error in WiFi connection");

    }
  }
  delay(5000);  //Send a request every 5 seconds
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i] < 0x10 ? "0" : "");
    result += String(mac[i], HEX);
    if (i < 5)
      result += ':';
  }
  return result;
}
