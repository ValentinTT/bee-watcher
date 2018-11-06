#ifndef SIM800L_H
#define SIM800L_H

/** void setupSIM800L()
 * This function start the serial communication with the SIM800L module,
 * set the pin 3 as INPUT_PULLUP, and attach an interruption to it.
 */
void setupSIM800L();

/** void sendMessage(int alarmMsg)
 * This function send a message based on the alarmMsg 
 * @param alarmMsg an integer between 0-3 indicating the typeofAlarm to send
 * via sms.
 */
void sendMessage(int alarmMsg);

/** void shouldWaitSMS()
 * This function check if it should wait for a message, and waits to be set into sleep mode
 * so the beekeeper can do his/her work.
 */
void shouldWaitSMS();

#endif

