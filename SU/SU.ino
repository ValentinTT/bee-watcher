//Contain the data structure
#include "Data.h"
struct package Data;
#include "ADXL335.h"
#include "innerDHT22.h"
#include "hiveEntrance.h"
#include "transmitter.h"
#include "sleepMode.h"

void setup() {
  setupSleepMode();  
  setupTransmitter();
  setupInnerDHT22();
  setupHCSR04();
}

void loop() {
  goToSleep();
  readADXL(); //This and the fumes sensor should be checked every 5 minutes;
    readTemperatureAndHumidity();
    hiveEntranceOpening();
  }
  sendData();
}

