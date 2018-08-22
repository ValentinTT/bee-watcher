#ifndef TRANSMITTER_H
#define TRANSMITTER_H
/** void setupTranmitter()
 * This function set the pins neccesary for the 433MHz transmitter to work.
 */
void setupTransmitter();

/** void sendData()
 * This function send the Data package(temp, hum, hiveEntrance) to the Main Unit after all the
 * messures (including acelerometer and fume sensors) were taken.
 */
void sendData();
#endif

