#include <RBDdimmer.h>//
#define outputPin  14  //D5
#define zerocross  12 // D6 for boards with CHANGEBLE input pins

dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
int outVal = 0;

void setup() {
  Serial.begin(9600); 
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
  Serial.println("Dimmer Program is starting...");
  Serial.println("Set value");
}

void printSpace(int val)
{
  if ((val / 100) == 0) Serial.print(" ");
  if ((val / 10) == 0) Serial.print(" ");
}

void loop() {
  int preVal = outVal;

  if (Serial.available())
  {
    int buf = Serial.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
  dimmer.setPower(outVal); // setPower(0-100%);

  if (preVal != outVal)
  {
    Serial.print("lampValue -> ");
    printSpace(dimmer.getPower());
    Serial.print(dimmer.getPower());
    Serial.println("%");

  }
  delay(50);
}
