
// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11

// DHT Sensor
const int DHTPin = D4;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// only runs once on boot
void setup()
{
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
}

// runs over and over again
void loop()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp, "Failed");
    strcpy(fahrenheitTemp, "Failed");
    strcpy(humidityTemp, "Failed");
  }
  else
  {
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    float hic = dht.computeHeatIndex(t, h, false);
    dtostrf(hic, 6, 2, celsiusTemp);
    float hif = dht.computeHeatIndex(f, h);
    dtostrf(hif, 6, 2, fahrenheitTemp);
    dtostrf(h, 6, 2, humidityTemp);
    // You can delete the following Serial.print's, it's just for debugging purposes
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C \t Temperature: ");
    Serial.print(f);
    Serial.print(" *F\t");
  }
}
