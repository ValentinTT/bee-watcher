//Library necessary for the rf transmitter
#include <VirtualWire.h>
#include "Data.h"

//Pin used for rf transmission
const byte transmissionPin = 4;

void setupTransmitter() {
  //Transmitter configuration
  vw_set_tx_pin(transmissionPin);
  vw_set_ptt_inverted(true);
  vw_setup(500); //Bits per second
}

void sendData() {
  vw_send((uint8_t *)&Data, sizeof(Data));
  vw_wait_tx();
  resetData();
}

