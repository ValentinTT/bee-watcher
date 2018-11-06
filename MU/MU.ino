#include "Data.h"
struct packageMainUnit Data;
struct package externalData;
#include "ADXL335.h"
#include "innerDHT22.h"
#include "hiveEntrance.h"
#include "ReceiverNRF24L01.h"
#include "sleepMode.h"
#include "datalogger.h"
#include "SIM800L.h"
#include "fume.h"

void setup() {
  setupSIM800L();
  setupSleepMode();
  seturReceiverNRF24L01();
  setupDatalogger();
  setupInnerDHT22();
  setupHCSR04();
}

void loop() {
  goToSleep();
  delay(100);
  Serial.println("Wake up");
  shouldWaitSMS();
  readADXL();
  readFumeLevel();
  bool isBeenAnHour = true;//hasBeenAnHour();
  if (isBeenAnHour) {
    readTemperatureAndHumidity();
    hiveEntranceOpening();
  }
  receiveData();
  if (externalData.safe != hiveSafe) {
    sendMessage(externalData.safe);
  } else if(Data.safe != hiveSafe) {
    sendMessage(Data.safe);
  } 
  if (isBeenAnHour) {
    logData(true); //Log SU data
    logData(false); //Log MU data
  }
  resetMainData();
  resetData();
}


