#ifndef DATA_H
#define DATA_H
#include <Arduino.h>

//Struct for storing and sending data 
#define hiveSafe 10
#define hiveTrapRetired 1
#define hiveDangerFume 2
#define hiveDangerMovement 5
#define dataInitialValue -100

struct package {
  float temperature = dataInitialValue;
  float humidity = dataInitialValue;
  float hiveEntrance = dataInitialValue;
  byte safe = hiveSafe; //10 means ok, 9 means trap retired, 8 means fume, 7 means fume and trap, 5 means movement, 4 means trap and movement, 3 means fume and movement, 2 means all
  const byte id = 1; //This change in different SU
};

struct packageMainUnit {
  float temperature = dataInitialValue;
  float humidity = dataInitialValue;
  float hiveEntrance = dataInitialValue;
  float fumesLevel = dataInitialValue;
  byte safe = hiveSafe; //3 means ok, 2 means fume, 1 means movement, 0 means both
  const byte id = 0; //0 if for the MU
};
extern struct packageMainUnit Data;
extern struct package externalData;

void resetData();
void resetMainData();
#endif

