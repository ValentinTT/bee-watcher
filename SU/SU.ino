//Contain the data structure
#include "Data.h"
struct package Data;
#include "ADXL335.h"
#include "innerDHT22.h"
#include "hiveEntrance.h"
#include "TransmitterNRF24L01.h"
//#include "transmitter.h"
#include "sleepMode.h"

void setup() {
  setupSleepMode();  
  setupTransmitterNRF24L01();
  setupInnerDHT22();
  setupHCSR04();
  //Test
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  goToSleep();
  readADXL(); //This and the fumes sensor should be checked every 5 minutes;
  if(hasBeenAnHour()) { //This data is taken just once per hour
    readTemperatureAndHumidity();
    hiveEntranceOpening();
  }
  sendData();
}

