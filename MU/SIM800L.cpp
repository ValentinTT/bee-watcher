#include <Arduino.h>
#include <SoftwareSerial.h>
#include "sleepMode.h"
#include "SIM800L.h"

#define SMSFumeMovementTrap 2
#define SMSFumeMovement 3
#define SMSMovementTrap 4
#define SMSMovement5
#define SMSFumeTrap 7
#define SMSFume 8
#define SMSTrap 9
#define SMSStopButtonPressed 10
#define SMSRestart 11
#define SMSPause 12
#define SMSResume 13

//SoftwareSerial mySerial(5, 7); // RX, TX

const byte interruptionPin = 3;
const unsigned long timeoutSIM800L = 60000; //Wait 1 minute;
unsigned long startTimeSIM800L;
String msgReceived;
bool shouldReceiveSMS = false;

void waitForMessage();
void startSIM800L();
void closeSIM800L();

void setupSIM800L() {
  pinMode(interruptionPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptionPin), waitForMessage, LOW);
  Serial.begin(9600);
  sendMessage(SMSRestart);
}

void startSIM800L() {
  //mySerial.begin(9600);
}

void closeSIM800L() {
  //mySerial.end();
}

void sendMessage(int alarmMsg) {
  //closeSIM800L();
  //Serial.println("Enviando msj");
  delay(100);
  //startSIM800L();
  Serial.println(F("AT+CMGF=1")); //Text Mode
  delay(1000);
  Serial.println(F("AT+CMGS=\"2644550816\"\r")); //<-here goes beekeeper number 2644834024 //2645055525 //Chepi 154042489
  delay(1000);
  switch (alarmMsg) {//10 means ok, 9 means trap retired, 8 means fume, 7 means fume and trap, 5 means movement, 4 means trap and movement, 3 means fume and movement, 2 means all
    case 2:
      Serial.println(F("Problemas en la colmena: Humo, movimiento, trampa."));
      break;
    case 3:
      Serial.println(F("Humo y movimiento en las colmenas."));
      break;
    case 4:
      Serial.println(F("Movimiento en las colmenas. Trampa retirada."));
      break;
    case 5:
      Serial.println(F("Movimiento en las colmenas."));
      break;
    case 7:
      Serial.println(F("Humo en las colmenas. Trampa retirada."));
      break;
    case 8:
      Serial.println(F("Humo en las colmenas."));
      break;
    case 9:
      Serial.println(F("Trampa retirada."));
      break;
    case 10:
      Serial.println(F("Alguien ha presionado el boton de parada."));
      break;
    case 11:
      Serial.println(F("El sistema se ha reiniciado."));
      break;
    case 12:
      Serial.println(F("El sistema se ha desactivado para trabajar."));
      break;
    case 13:
      Serial.println(F("El sistema se ha activado."));
      break;
  }
  delay(100);
  Serial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
  //closeSIM800L();
}

void shouldWaitSMS() {
  if (!shouldReceiveSMS) return; //If there is no need for waiting for a sms
  shouldReceiveSMS = false;
  //Serial.println("Esperando sms");
  //startSIM800L();
  
  Serial.println(F("AT")); //checking
  delay(1000);
  Serial.println(F("AT+CMGF=1"));  //Set to Text mode
  delay(1000);
  Serial.println(F("AT+CNMI=1,2,0,0,0")); // AT command to receive SMS
  delay(1000);

  startTimeSIM800L = millis();
  while (millis() < startTimeSIM800L + timeoutSIM800L) {
    if (Serial.available() > 0) {
      msgReceived = Serial.readString();
      if (msgReceived.indexOf(F("Hive off 123")) > -1) {
        sendMessage(SMSPause);

        //closeSIM800L();
        //Serial.println("A trabajar");       
        //sleepForWorking();
        sendMessage(SMSResume);
        attachInterrupt(digitalPinToInterrupt(interruptionPin), waitForMessage, LOW);
        //closeSIM800L();
        return;
      }
    }
  }
  sendMessage(SMSStopButtonPressed);
  attachInterrupt(digitalPinToInterrupt(interruptionPin), waitForMessage, LOW);  
  goToSleep();
}

void waitForMessage() {
  stopSleeping();
  shouldReceiveSMS = true;
  detachInterrupt(digitalPinToInterrupt(interruptionPin));
}

