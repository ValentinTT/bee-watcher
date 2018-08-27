#ifndef HIVEENTRANCE_H
#define HIVEENTRANCE_H
/** void setupHCSR4()
 * This function set the operation's mode for the pins that work with 
 * the HC-SR04, in order to messure the hive entrance opening
 */
void setupHCSR04();

/** void hiveEntranceOpening() 
 * This function uses the HC-SR04 to get the opening at the hive entrance
 * to calculate it it use a constant soundConstant and the time in which 
 * the sound travels from the trigger echo to the hive entrance and back 
 * to the echo pin
 */
void hiveEntranceOpening();
#endif

