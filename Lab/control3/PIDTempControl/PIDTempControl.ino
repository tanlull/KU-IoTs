#include <AutoPID.h>
#include <RBDdimmer.h>
#include "DHT.h"
#define DHTPIN 5   //D1 
#define outputPin  14  //D5 node mcu
#define zerocross  12 // D6 for boards with CHANGEBLE input pins
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define TEMP_READ_DELAY 800 //can only read digital temp sensor every ~750ms

//pid settings and gains
#define OUTPUT_MIN 10
#define OUTPUT_MAX 99
#define KP 40 //p-control 50
#define KI 1.35
#define KD 0

double temperature, setPoint, controlSig;

DHT dht(DHTPIN, DHTTYPE);
dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
//int controlSig = 0; // ระดับ Dimming  (0-100)  0 =ปิด , 100 = เปิดเต็มที่

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
    Serial.print(controlSig); 
    Serial.print(",");
    Serial.println(temperature);
    lastTempUpdate = millis();
    return true;
  }
  return false;
}//end updateTemperature


void setup() {
  Serial.begin(9600);
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
  //myPID.SetMode(AUTOMATIC);
  dht.begin();
  while (!updateTemperature()) {} //wait until temp sensor updated
  //if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  myPID.setBangBang(4);
  //set PID update interval to 4000ms
  myPID.setTimeStep(1000);

}//end setup


void loop() {
  updateTemperature();
  if (Serial.available())
  {
    int buf = Serial.parseInt();
    if (buf != 0) setPoint = buf; //33C
    delay(200);
  }
  myPID.run(); //call every loop, updates automatically at certain time interval
 
  dimmer.setPower(controlSig); // setPower(0-100%);

}
