#include <Arduino.h>
//Library for the controlling the sleep mode
#include <avr/sleep.h>
/* As JChristensen says: "DS3231RTC.h is an Arduino library for the 
 * Maxim Integrated DS3232 and DS3231", which allows setting interruptions
 */
#include <DS3232RTC.h> 
#include "sleepMode.h"

//Pin to attach interruption, and interruption number bind to that pin
const byte interruptionPin = 2;
const byte interruptionNumber = 0;
const byte sleepingMinutes = 3;
byte interruptAtMinute = 0;
bool isAwake;
byte alarmsEach5[12] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 0};
byte alarmsEach3[20] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 0};

//Private function's declaration
void wakeUp();
void setAwake(bool newAwakeValue);

void setupSleepMode() {  
  // Initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);
  
  RTC.alarmInterrupt(ALARM_1, true);
  pinMode(interruptionPin, INPUT_PULLUP);
}

void goToSleep() {
  setAwake(false);
  sleep_enable();
  // set Alarm 1 to occur every each sleepingMinutes minutes
  setNextAlarm();
  RTC.setAlarm(ALM1_MATCH_MINUTES, 0, interruptAtMinute, 0, 1);
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  RTC.alarmInterrupt(ALARM_1, true);
  attachInterrupt(interruptionNumber, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(100);
  sleep_cpu();
}

void sleepForWorking() {
  setAwake(false);
  sleep_enable();
  // set Alarm 1 to occur in the next hour
  setNextAlarm();
  //Change the alarm so it happend within an hour
  interruptAtMinute -= (sleepingMinutes - 1); 
  if(interruptAtMinute < 0) interruptAtMinute = 60 - sleepingMinutes;
  RTC.setAlarm(ALM1_MATCH_MINUTES, 0, interruptAtMinute, 0, 1);
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  RTC.alarmInterrupt(ALARM_1, true);
  attachInterrupt(interruptionNumber, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(100);
  sleep_cpu();
}

/** void wakeUp()
 * This function is the ISR, in charge of disable the sleep mode on the microcontroller.
 * It also changes the time in which the next alarm should be set.
 */
void wakeUp() {
  setAwake(true);
  sleep_disable();
  detachInterrupt(interruptionNumber);
}

void setNextAlarm() {
  byte plusSecond = second(RTC.get())>50? 1 : 0; //in case that for example the time is 59:50 it set the next alarm not to 0:00 but for 5:00 or 3:00
  /*if(firstMeasures > 0) {
    firstMeasures--;
  }
  */
  interruptAtMinute = alarmsEach3[(minute(RTC.get()) + plusSecond) / sleepingMinutes]; 
  Serial.println(plusSecond);
  Serial.println(minute(RTC.get()));
  Serial.println((minute(RTC.get()) + plusSecond) / sleepingMinutes);
  Serial.println(interruptAtMinute);
}

void stopSleeping() {
  if(isAwake) {
    interruptAtMinute -= sleepingMinutes;
    if(interruptAtMinute < 0) interruptAtMinute += 60;
  }
  setAwake(true);
  sleep_disable();
  detachInterrupt(interruptionNumber);
}

void setAwake(bool newAwakeValue) {
  isAwake = newAwakeValue;
}

bool hasBeenAnHour() {
  return interruptAtMinute == sleepingMinutes;
}

String getTime() {
  time_t tm = RTC.get(); //unix time
  if(minute(tm) < 10) 
    return String(year(tm)) + "/" + String(month(tm)) + "/" + String(day(tm)) + " " + String(hour(tm)) + ":0" + String(minute(tm));
  return String(year(tm)) + "/" + String(month(tm)) + "/" + String(day(tm)) + " " + String(hour(tm)) + ":" + String(minute(tm));
}

