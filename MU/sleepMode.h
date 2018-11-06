#ifndef SLEEPMODE_H
#define SLEEPMODE_H

//byte firstMeasures = 3;
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

/** time_t getTime()
 * This functin returns the current date and time as a string in the
 * following format "yyyy/mm/dd hh:mm"
 */
String getTime();

/** bool hasBeenAnHour()
 * This function returns true if has been an hour since the sleeping-waking
 * routine has been working 
 */
bool hasBeenAnHour();

/**void sleepForWorking()
 * This function put the microcontroller on sleep mode for almost an hour,
 * so the beekeeprs can work on the hives, without trigger any alarm.
 */
void sleepForWorking();

/**void stopSleeping()
 * This function detach the interruption triggered by the RTC
 */
void stopSleeping();

/** void setNextAlarm() 
 * This function set the next alarm for the rtc module based on the actual time
 */
void setNextAlarm();

#endif

