#include <Arduino.h>
//Library for the DHT 22 sensor
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include "Data.h"

//DHT creation
const byte DHTPin = 5;
DHT dht22(DHTPin, DHT22);

void setupInnerDHT22() {
  //Starts the dht sensor
  dht22.begin();
}

void readTemperatureAndHumidity() {
  float t = dht22.readTemperature();
  //If there was an error try again
  if(isnan(t)) t = dht22.readTemperature();
  float h = dht22.readHumidity();
  //If there was an error try again
  if(isnan(h)) h = dht22.readHumidity();

  //If there was no problem save the data
  if(!isnan(t)) Data.temperature = t;
  if(!isnan(h)) Data.humidity = h;
}

