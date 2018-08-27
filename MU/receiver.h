#ifndef RECEIVER_H
#define RECEIVER_H
/** void setupReceiver()
 * This function sets the rf receiver configuration, pin, bound rate, etc
 */
void setupReceiver();

/** void readData() 
 * This function waits until the data from a SU has been read, and then
 * saves it into the externalData struct
 */
void readData();
#endif
