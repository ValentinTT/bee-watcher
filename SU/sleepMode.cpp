#include <Arduino.h>
//Library for the controlling the sleep mode
#include <avr/sleep.h>
/* As JChristensen says: "DS3231RTC.h is an Arduino library for the 
 * Maxim Integrated DS3232 and DS3231", which allows setting interruptions
 */
#include <DS3232RTC.h> 

//Pin to attach interruption, and interruption number bind to that pin
const byte interruptionPin = 2;
const byte interruptionNumber = 0;
const byte sleepingMinutes = 5;
byte interruptAtMinute = 0;

//Private function's declaration
time_t compileTime();
void wakeUp();

void setupSleepMode() {  
  // Initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);

  // set the RTC time and date to the compile time
  RTC.set(compileTime());
  // set Alarm 1 to occur every each sleepingMinutes minutes
  RTC.setAlarm(ALM1_MATCH_MINUTES, 0, interruptAtMinute, 0, 1);
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  RTC.alarmInterrupt(ALARM_1, true);

  pinMode(interruptionPin, INPUT_PULLUP);
}

void goToSleep() {
  sleep_enable();
  attachInterrupt(interruptionNumber, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(100);
  sleep_cpu();
  RTC.setAlarm(ALM1_MATCH_MINUTES, 0, interruptAtMinute, 0, 1);
  RTC.alarm(ALARM_1);
}

bool hasBeenAnHour() {
  return interruptAtMinute == sleepingMinutes;
}

/** void wakeUp()
 * This function is the ISR, in charge of disable the sleep mode on the microcontroller.
 * It also changes the time in which the next alarm should be set.
 */
void wakeUp() {
  sleep_disable();
  interruptAtMinute += sleepingMinutes;
  if(interruptAtMinute >= 60) interruptAtMinute -= 60;
  detachInterrupt(interruptionNumber);
}

/** time_t compileTime() By JChristen 
 * This function return a time_t object necesary to set the time in the RTC to the compilation
 * time, using the values saved by C++ in __DATE__ and __TIME__ 
 */
time_t compileTime() {
  const time_t FUDGE(10);    //fudge factor to allow for upload time, etc. (seconds, YMMV)
  const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char compMon[3], *m;

  strncpy(compMon, compDate, 3);
  compMon[3] = '\0';
  m = strstr(months, compMon);

  tmElements_t tm;
  tm.Month = ((m - months) / 3 + 1);
  tm.Day = atoi(compDate + 4);
  tm.Year = atoi(compDate + 7) - 1970;
  tm.Hour = atoi(compTime);
  tm.Minute = atoi(compTime + 3);
  tm.Second = atoi(compTime + 6);

  time_t t = makeTime(tm);
  return t + FUDGE;        //add fudge factor to allow for compile time
}

