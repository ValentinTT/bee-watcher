//Library necessary for the rf transmitter
#include <VirtualWire.h>
//Pin Used for transmission
const byte transmissionPin = 4;
//Struct for sending data 
#define hiveSafe 3
#define hiveDangerFume 1
#define hiveDangerMovement 2
struct package {
  float temperature;
  float humidity;
  float piquera;
  byte safe = hiveSafe; // 3 means ok, 2 means fume, 1 means movement, 0 means both
  const byte id = 1; //This change in different SU
};
struct package Data;

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

void setup() {
  //Transmitter configuration
  vw_set_tx_pin(transmissionPin);
  vw_set_ptt_inverted(true);
  vw_setup(500); //Bits per second
}

void loop() {
  readADXL();
}


/* void sendData()
 * This function send the Data package(temp, hum, piq) to the Main Unit after all the
 * messures (including acelerometer and fume sensors) were taken.
 */
void sendData() {
  vw_send((uint8_t *)&Data, sizeof(Data));
  vw_wait_tx();
}

/** void readADXL()
 *  This function executes in about 1 second and read 10 times the 
 *  result of the ADXL335 detecting any kind of rotation or movement
 *  it reads 10 times to avoid interference. 
 */
void readADXL() {
  byte bentCounter = 0; 
  for(byte i = 0; i < 10; i++){ 
    xValue = analogRead(xPin);
    yValue = analogRead(yPin);
    zValue = analogRead(zPin);
    if(abs(xValue - yValue) > XYSensibility) { //Check the sensor's position
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



