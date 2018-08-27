#include <Arduino.h>
//Library necessary for the rf receiver
#include <VirtualWire.h>
#include "Data.h"

//Pin used for the receiver
const byte receiverPin = 4;

void setupReceiver() {
  // Initialise the IO and ISR
  vw_set_rx_pin(receiverPin);
  vw_setup(500);   // Bits per sec
  vw_rx_start();   // Start the receiver PLL running
}

void readData() {
  uint8_t buf[sizeof(externalData)];
  uint8_t buflen = sizeof(externalData);
  while(true) {
    if (vw_have_message()){  //Is there a package for us?
      vw_get_message(buf, &buflen);
      memcpy(&externalData, &buf, buflen);
      return;
    }    
  }
}

