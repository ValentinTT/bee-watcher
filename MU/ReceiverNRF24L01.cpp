#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Data.h"

#define CE_PIN 9
#define CSN_PIN 10

byte channel[5] = {'v', 'a', 'l', 't', 't'};

RF24 radio(CE_PIN, CSN_PIN);

const int timeout = 40 * 1000; //40 milliseconds
unsigned long startTime;

void seturReceiverNRF24L01() {
  radio.begin();
  radio.openReadingPipe(1, channel);
  radio.startListening();
}

void receiveData() {
  startTime = millis();
  while(!radio.available() && millis() < startTime + timeout) {
    if(radio.available()) 
      radio.read(&externalData, sizeof(externalData));
  }
}

