//Contain the data structure
#include "Data.h"
struct package Data;
#include "ADXL335.h"
#include "innerDHT22.h"
#include "hiveEntrance.h"
#include "TransmitterNRF24L01.h"
#include "sleepMode.h"

void setup() {
  Serial.begin(9600);
  setupSleepMode();
  setupTransmitterNRF24L01();
  setupInnerDHT22();
  //setupHCSR04();
}

void loop() {
  goToSleep();
  Serial.println("Woke up");
  readADXL(); //This and the fumes sensor should be checked every 5 minutes;
  if(Data.safe != hiveSafe) Serial.println("Movimiento");
  if (true){//hasBeenAnHour()) { //This data is taken just once per hour
    readTemperatureAndHumidity();
    Serial.println("Measures taken");
  }
  Serial.println("Sending Data");
  sendData();
}

