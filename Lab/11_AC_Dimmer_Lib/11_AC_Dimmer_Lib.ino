#include <RBDdimmer.h>//
#define outputPin  14  //D5
#define zerocross  12 // D6 for boards with CHANGEBLE input pins
int button_pin_up = 5; //D1
int button_pin_down = 4; //D2
void ICACHE_RAM_ATTR button_up(); 
void ICACHE_RAM_ATTR button_down(); 
dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
int outVal = 50;

void setup() {
  Serial.begin(9600); 
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
 pinMode(button_pin_up, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(button_pin_up), button_up, RISING); //and the interrupt is attached
  pinMode(button_pin_down, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(button_pin_down), button_down, RISING); //and the interrupt is attached
 
  Serial.println("Dimmer Program is starting...");
  Serial.println("Set value");
}

void printSpace(int val)
{
  if ((val / 100) == 0) Serial.print(" ");
  if ((val / 10) == 0) Serial.print(" ");
}

void button_up()     //This is the function that the interupt calls
{
   int lampPower = dimmer.getPower();

      outVal = lampPower +10;
   //dimmer.setPower(lampPower);
   Serial.print(lampPower);
   Serial.println("%");

}

void button_down()     //This is the function that the interupt calls
{
     int lampPower = dimmer.getPower();
   outVal = lampPower -10;
  // dimmer.setPower(lampPower);
    Serial.print(dimmer.getPower());
    Serial.println("%");
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
