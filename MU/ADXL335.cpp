#include <Arduino.h>
#include "Data.h"

//Analog pins for the ADXL335 
const byte xPin = 0;
const byte yPin = 1;
const byte zPin = 2;
//Variables for the ADXL335 reading
int xValue;
int yValue;
int zValue;

double readToDeg;

void readADXL() {
  byte bentCounter = 0; 
  for(byte i = 0; i < 10; i++){ 
    xValue = analogRead(xPin);
    yValue = analogRead(yPin);
    zValue = analogRead(zPin);

    xValue = map(xValue, 265, 402, -90, 90);
    yValue = map(yValue, 265, 402, -90, 90);
    zValue = map(zValue, 265, 402, -90, 90);

    //X angle
    readToDeg = RAD_TO_DEG * (atan2(-yValue, -zValue) + PI);
    if(readToDeg > 70 && readToDeg < 320) { 
      Serial.print("x: ");
      Serial.println(readToDeg);
      bentCounter++;
      continue;
    }
    //Y angle
    readToDeg = RAD_TO_DEG * (atan2(-xValue, -zValue) + PI);
    if(readToDeg < 220 || readToDeg > 340) { 
      Serial.print("y: ");
      Serial.println(readToDeg);
      bentCounter++;
      continue;
    }
    delay(100); //Delay in between messures to avoid electrical interfirence
  }
  if(bentCounter > 5) Data.safe -= hiveDangerMovement;
}

