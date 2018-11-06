#include <Arduino.h>
#include "Data.h"
#include "sleepMode.h"

const byte mqPin = A3;
int totalReading = 0;

void readFumeLevel() {
  totalReading = 0;
  for(byte i = 0; i < 10; i++) {
    totalReading += analogRead(mqPin);
    delay(100);    
  }
  totalReading /= 10;
  Data.fumesLevel = totalReading;
  //if(firstMeasures > 0) return;
  if(totalReading > 300) Data.safe -= hiveDangerFume;
  
}

