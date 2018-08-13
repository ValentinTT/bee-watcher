#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#define interruptPin 2 //Pin we are going to use to wake up the Arduino


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  delay(5000);
  goingToSleep();
}

void goingToSleep() {
  sleep_enable();
  attachInterrupt(0, wakeUp, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Going To Sleep");
  delay(1000);
  sleep_cpu();
  Serial.println("Just Woke up");
  digitalWrite(LED_BUILTIN, HIGH);
}

void wakeUp() {
  Serial.println("Interrupt fired");
  sleep_disable();//Disable sleep mode
  detachInterrupt(0);
}

