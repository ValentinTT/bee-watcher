#ifndef DATA_H
#define DATA_H
#include <Arduino.h>

//Struct for storing and sending data 
#define hiveSafe 3
#define hiveDangerFume 1
#define hiveDangerMovement 2
#define dataInitialValue -100

struct package {
  float temperature = dataInitialValue;
  float humidity = dataInitialValue;
  float hiveEntrance = dataInitialValue;
  byte safe = hiveSafe; //3 means ok, 2 means fume, 1 means movement, 0 means both
  const byte id = 1; //This change in different SU
};
extern struct package Data;

/** void resetData() 
 * This function reset the Data package to his initial value. This value is know for the 
 * Main Unit, so they are not taken into acount
 */
void resetData();
#endif

