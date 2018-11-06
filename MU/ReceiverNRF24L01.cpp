#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Data.h"
#include "ReceiverNRF24L01.h"

#define CE_PIN 9
#define CSN_PIN 10

const byte enableAnotherSPIPin = 8;
byte channel[5] = {'v', 'a', 'l', 't', 't'};

RF24 radio(CE_PIN, CSN_PIN);

const unsigned long timeoutRF24 = 40000; //40 seconds
unsigned long startTimeRF24;

/** void openPipe(bool openP) 
 * This function open or close a reading pipe depending on openP, to start 
 * or finish a rf reception
 */
void openPipe(bool openP);

void seturReceiverNRF24L01() {
  pinMode(enableAnotherSPIPin, OUTPUT); //pin used to disconnect the sd module from vcc because otherwise the nrf module won't work
  enableAnotherSPI(false); //Makes sure there is the only SPI device connected
  radio.begin();
}

void receiveData() {
  openPipe(true);
  startTimeRF24 = millis();
  while (millis() < startTimeRF24 + timeoutRF24) {
    if (radio.available()) {
      radio.read(&externalData, sizeof(externalData));
      //Serial.print("Recibido ");
      //Serial.println(externalData.temperature);
      openPipe(false);
      return;
    }
  }
  //Serial.println("No Recibio");
  openPipe(false);
}

void openPipe(bool openP) {
  if (openP) {
    enableAnotherSPI(false);
    delay(10);
    radio.openReadingPipe(1, channel);
    radio.startListening();
  } else {
    radio.closeReadingPipe(channel);
    radio.stopListening();
  }
}

void enableAnotherSPI(bool enable) {
  if (enable)
    digitalWrite(enableAnotherSPIPin, HIGH);
  else
    digitalWrite(enableAnotherSPIPin, LOW);
}

