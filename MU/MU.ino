#include "Data.h"
struct package Data;
struct package externalData;
#include "ADXL335.h"
#include "innerDHT22.h"
#include "hiveEntrance.h"
#include "ReceiverNRF24L01.h"
//#include "receiver.h"
#include "sleepMode.h"
#include "datalogger.h"

void setup() {
  setupSleepMode();
  seturReceiverNRF24L01();
  setupDatalogger();
  setupInnerDHT22();
  setupHCSR04();  
}

void loop() {  
  goToSleep();
  readADXL();
  bool isBeenAnHour = hasBeenAnHour();
  if(isBeenAnHour) {
    readTemperatureAndHumidity();
    hiveEntranceOpening();
  }
  receiveData();
  if(externalData.safe != 3) {
    if(externalData.safe = 2); //Send fire alarm
    if(externalData.safe = 1); //Send movement alarm
    if(externalData.safe = 0); //Send both alarms
  } else if(isBeenAnHour) {
    logData(true); //Log SU data
    logData(false); //Log MU data
  }
}

