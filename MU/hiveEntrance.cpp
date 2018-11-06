#include <Arduino.h>
#include "Data.h"

//Digital pins for hc-sr04 used to get the hive entrance status
const byte trigPin = 6;
const byte echoPin = 7;
//Time/soundConstant = distance in centimeters
const byte soundConstant = 58;

void setupHCSR04() {  
  //HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void hiveEntranceOpening() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned int timeTravel = pulseIn(echoPin, HIGH);
  Data.hiveEntrance = timeTravel / soundConstant;
  if(Data.hiveEntrance > 10) Data.safe -= hiveTrapRetired;
}

