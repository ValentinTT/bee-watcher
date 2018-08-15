

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

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  readADXL();
}

void readADXL() {
  byte bentCounter = 0; 
  for(byte i = 0; i < 15; i++){ 
    xValue = analogRead(xPin);
    yValue = analogRead(yPin);
    zValue = analogRead(zPin);
    delay(100);
    
    if(abs(xValue - yValue) > XYSensibility) {
      if(abs(xValue - zValue) > ZXYSensibility ||
        abs(yValue - zValue) > ZXYSensibility) {
        bentCounter++;
      }    
    }
  }

  if(bentCounter > 5) digitalWrite(LED_BUILTIN, HIGH);
  else digitalWrite(LED_BUILTIN, LOW);
}



