//Library for the controlling the sleep mode
#include <avr/sleep.h>
/* As JChristensen says: "DS3231RTC.h is an Arduino library for the 
 * Maxim Integrated DS3232 and DS3231", which allows setting interruptions
 */
#include <DS3232RTC.h> 
//Library necessary for the rf transmitter
#include <VirtualWire.h>
//Library for the DHT 22 sensor
#include <Adafruit_Sensor.h>
#include "DHT.h"

//Struct for sending data 
#define hiveSafe 3
#define hiveDangerFume 1
#define hiveDangerMovement 2
#define dataInitialValue -100
struct package {
  float temperature = dataInitialValue;
  float humidity = dataInitialValue;
  float hiveEntrance = dataInitialValue;
  byte safe = hiveSafe; //3 means ok, 2 means fume, 1 means movement, 0 means both
  const byte id = 1; //This change in different SU
};
struct package Data;

//Pin used for rf transmission
const byte transmissionPin = 4;

//DHT creation
const byte DHTPin = 5;
DHT dht22(DHTPin, DHT22);

//Analog pins for the ADXL335 
const byte xPin = 0;
const byte yPin = 1;
const byte zPin = 2;
//Sensibility for the ADXL335
const byte XYSensibility = 15; //Higher difference allowed between x and y axes
const byte ZXYSensibility = 50; //Higher difference allowed between x-z axes or y-z axes
//Variables for the ADXL335 reading
short xValue;
short yValue;
short zValue;

//Digital pins for hc-sr04 used to get the hive entrance status
const byte trigPin = 6;
const byte echoPin = 7;
//Time/soundConstant = distance in centimeters
const byte soundConstant = 58;

//Pin to attach interruption, and interruption number bind to that pin
const byte interruptionPin = 2;
const byte interruptionNumber = 0;
const byte sleepingMinutes = 5;
byte interruptAtMinute = 0;

void setup() {
  /**The RTC must be the first thing to initialize to try to be as accurate as possible
   * with the compilation time, which is the time used to set the RTC time. 
   * In case the gap between times is too much, there will be use other mechanism to set
   * the RTC time
   */
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
  
  //Transmitter configuration
  vw_set_tx_pin(transmissionPin);
  vw_set_ptt_inverted(true);
  vw_setup(500); //Bits per second

  //Starts the dht sensor
  dht22.begin();

  //HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  goToSleep();
  readADXL(); //This and the fumes sensor should be checked every 5 minutes;
  if(interruptAtMinute == 5) { //This data is taken just once per hour
    readTemperatureAndHumidity();
    hiveEntranceOpening();
  }
  sendData();
}

/** void goToSleep()
 * This function put the microcontroller on sleep mode to save energy.
 * The only way to wake the arduino up is to fire the interruption (connected to the RTC
 * module) which takes the code to the function wakeUp.
 */
 void goToSleep() {
  sleep_enable();
  attachInterrupt(interruptionNumber, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(100);
  sleep_cpu();
  RTC.setAlarm(ALM1_MATCH_MINUTES, 0, interruptAtMinute, 0, 1);
  RTC.alarm(ALARM_1);
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

/** void sendData()
 * This function send the Data package(temp, hum, hiveEntrance) to the Main Unit after all the
 * messures (including acelerometer and fume sensors) were taken.
 */
void sendData() {
  vw_send((uint8_t *)&Data, sizeof(Data));
  vw_wait_tx();
  resetData();
}

/** void resetData() 
 * This function reset the Data package to his initial value. This value is know for the 
 * Main Unit, so they are not taken into acount
 */
void resetData() {
  Data.temperature = dataInitialValue;
  Data.humidity = dataInitialValue;
  Data.hiveEntrance = dataInitialValue;
  Data.safe = hiveSafe;
}

/** void readTemperatureAndHumidity() 
 * This function read the temperature and humidity of the dht22 sensor in about 250ms to 500ms
 * and then save if on the Data package.
 */
void readTemperatureAndHumidity() {
  float t = dht22.readTemperature();
  //If there was an error try again
  if(isnan(t)) t = dht22.readTemperature();
  float h = dht22.readHumidity();
  //If there was an error try again
  if(isnan(h)) h = dht22.readHumidity();

  //If there was no problem save the data
  if(!isnan(t)) Data.temperature = t;
  if(!isnan(h)) Data.humidity = h;
}

/** void readADXL()
 *  This function executes in at most 1 second and read 10 times the 
 *  result of the ADXL335 detecting any kind of rotation or movement
 *  it reads 10 times to avoid interference. 
 */
void readADXL() {
  byte bentCounter = 0; 
  for(byte i = 0; i < 10; i++){ 
    xValue = analogRead(xPin);
    yValue = analogRead(yPin);
    zValue = analogRead(zPin);
    if(abs(xValue - yValue) > XYSensibility) { //Checks the sensor's position
      if(abs(xValue - zValue) > ZXYSensibility ||
        abs(yValue - zValue) > ZXYSensibility) {
        bentCounter++;
      }
    } else if(abs(xValue - yValue) + abs(yValue - zValue) < 10) { 
      //If the three axes are almost identical it means there is no sensor connected
      bentCounter = 6;
      break;
    }
    delay(100); //Delay in between messures to avoid electrical interfirence
  }
  if(bentCounter > 5) Data.safe -= hiveDangerMovement;
}

/** void hiveEntranceOpening() 
 * This function uses the HC-SR04 to get the opening at the hive entrance
 * to calculate it it use a constant soundConstant and the time in which 
 * the sound travels from the trigger echo to the hive entrance and back 
 * to the echo pin
 */
void hiveEntranceOpening() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned int timeTravel = pulseIn(echoPin, HIGH);
  Data.hiveEntrance = timeTravel / soundConstant;
}

