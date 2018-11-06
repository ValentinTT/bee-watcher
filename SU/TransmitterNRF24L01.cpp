#include <Arduino.h>
//Libraries necessary for the use of the module
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Data.h"

#define CE_PIN 9
#define CSN_PIN 10

byte channel[5] = {'v', 'a', 'l', 't', 't'};

RF24 radio(CE_PIN, CSN_PIN);

const int timeout = 40 * 1000; //40 seconds
unsigned long startTime;
bool dataSent;

void setupTransmitterNRF24L01() {
  radio.begin();
  radio.openWritingPipe(channel);
}

void sendData() {
  startTime = millis();
  do {
    dataSent = radio.write(&Data, sizeof(Data));
  } while(!dataSent && millis() < startTime + timeout);
  resetData();
}

