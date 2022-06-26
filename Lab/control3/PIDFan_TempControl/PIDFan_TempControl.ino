#include <AutoPID.h>
#include "DHT.h"
#define DHTPIN 5   //D1 
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define TEMP_READ_DELAY 1000 //can only read digital temp sensor every ~750ms

//pid settings and gains
#define OUTPUT_MIN 0
#define OUTPUT_MAX 1023
#define KP -2000 
#define KI 0
#define KD 0

int IN1 = 4; //D2;
int pwm_speed = 0;

double temperature, setPoint, controlSig;

DHT dht(DHTPIN, DHTTYPE);
//input/output variables passed by reference, so they are updated automatically
AutoPID myPID(&temperature, &setPoint, &controlSig, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

unsigned long lastTempUpdate; //tracks clock time of last temp update

//call repeatedly in loop, only updates after a certain time interval
//returns true if update happened
bool updateTemperature() {
  if ((millis() - lastTempUpdate) > TEMP_READ_DELAY) {
    temperature = dht.readTemperature(); //get temp reading
    Serial.print(setPoint); 
    Serial.print(",");
    Serial.print(controlSig*10/1000); 
    Serial.print(",");
    Serial.println(temperature);
    lastTempUpdate = millis();
    return true;
  }
  return false;
}//end updateTemperature


void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  //myPID.SetMode(AUTOMATIC);
  dht.begin();
  while (!updateTemperature()) {} //wait until temp sensor updated
  //if temperature is more than 2 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  //myPID.setBangBang(5);
  //set PID update interval to 1000ms
  myPID.setTimeStep(1000);

}//end setup


void loop() {
  updateTemperature();
  if (Serial.available())
  {
    int buf = Serial.parseInt();
    if (buf != 0) setPoint = buf; //33c
    delay(200);
  }
  myPID.run(); //call every loop, updates automatically at certain time interval
  analogWrite(IN1, controlSig);// set Speed(0-1024);
}
