#include <Arduino.h>
#include "Data.h"

//Analog pins for the ADXL335 
const byte xPin = 0;
const byte yPin = 1;
const byte zPin = 2;
//Sensibility for the ADXL335
const byte XYSensibility = 15; //Higher difference allowed between x and y axes
const byte ZXYSensibility = 50; //Higher difference allowed between x-z axes or y-z axes
//Variables for the ADXL335 reading
short xValue;
short yValue;
short zValue;

int readADXL() {
  byte bentCounter = 0; 
  for(byte i = 0; i < 10; i++){ 
    xValue = analogRead(xPin);
    yValue = analogRead(yPin);
    zValue = analogRead(zPin);
    if(abs(xValue - yValue) > XYSensibility) { //Checks the sensor's position
      if(abs(xValue - zValue) > ZXYSensibility ||
        abs(yValue - zValue) > ZXYSensibility) {
        bentCounter++;
      }
    } else if(abs(xValue - yValue) + abs(yValue - zValue) < 10) { 
      //If the three axes are almost identical it means there is no sensor connected
      bentCounter = 6;
      break;
    }
    delay(100); //Delay in between messures to avoid electrical interfirence
  }
  if(bentCounter > 5) Data.safe -= hiveDangerMovement;
}

