#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Abhay_ACT";
const char* password = "bruno3370";

void setup ()
{
  Serial.begin(115200);
  Serial.println("Connecting to " + String (ssid) );
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.print("Connected to " + String (ssid) + " with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    Serial.println("Connecting to website");
    delay(1000);
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://frugal-labs.com/site/time.php");  //Specify request destination
    int httpCode = http.GET(); //Send the request
    //Check the returning code
    if (httpCode > 0)
    {
      Serial.println("Connection successful. web page data is: ");
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
  }
}

void loop()
{

}
