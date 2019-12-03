// Including the libraries
#include <ESP8266WiFi.h>

// Replace with your network details
const char* ssid = "flip";
const char* password = "flip";

void setup()
{
  Serial.begin(115200);
  delay(10);

  // Connecting to WiFi network
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
  Serial.println("WiFi connected");
  Serial.print("ESP IP is: ");
  Serial.println(WiFi.localIP());
}

void loop()
{

}
