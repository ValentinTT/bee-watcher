#ifndef SLEEPMODE_H
#define SLEEPMODE_H
/** void setupSleepMode
 * The RTC must be the first thing to initialize to try to be as accurate as possible
 * with the compilation time, which is the time used to set the RTC time. 
 * In case the gap between times is too much, there will be use other mechanism to set
 * the RTC time
 */
void goToSleep();

/** void goToSleep()
 * This function put the microcontroller on sleep mode to save energy.
 * The only way to wake the arduino up is to fire the interruption (connected to the RTC
 * module) which takes the code to the function wakeUp.
 */
void setupSleepMode();
/** bool hasBeenAnHour()
 * This function returns true if has been an hour since the sleeping-waking
 * routine has been working 
 */
bool hasBeenAnHour();
#endif

