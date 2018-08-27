#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "sleepMode.h"
#include "Data.h"

File logFile;

const byte csPin = 13; //The hardware ss line of the SPI bus, that is the same that call SD.begin();

void setupDatalogger() {
  if (!SD.begin(csPin)) 
    return;
}

void logData(bool isExternalData) {
  logFile = SD.open("data.txt", FILE_WRITE);
  if (logFile) {
    logFile.print("{\"time\": ");
    logFile.print(getTime());
    logFile.print(",\"id\": ");
    if(isExternalData) {
      logFile.print(externalData.id);
      logFile.print(",\"data\": {\"temperature\":");
      logFile.print(externalData.temperature);
      logFile.print(",\"humidity\": ");
      logFile.print(externalData.humidity);
      logFile.print(",\"hiveEntrance\": ");
      logFile.print(externalData.hiveEntrance);
      logFile.print(",\"safe\": ");
      logFile.print(externalData.safe);
    }
    else {      
      logFile.print(Data.id);
      logFile.print(",\"data\": {\"temperature\":");
      logFile.print(Data.temperature);
      logFile.print(",\"humidity\": ");
      logFile.print(Data.humidity);
      logFile.print(",\"hiveEntrance\": ");
      logFile.print(Data.hiveEntrance);
      logFile.print(",\"safe\": ");
      logFile.print(Data.safe);
    }
    logFile.println("}}");
    logFile.close();
  }
}

