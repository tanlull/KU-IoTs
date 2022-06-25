
#define BLYNK_PRINT Serial
#include "Arduino.h"
#include <RBDdimmer.h>//ใช้ตัวที่แก้บักแล้ว
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define outputPin  14  //D5
#define zerocross  12 // D6 for boards with CHANGEBLE input pins

BlynkTimer timer;

char auth[] = "6cqHCTiRdV6kifNVpYsvNo6PzW-A3bJ_";
char ssid[] = "Tump";
char pass[] = "11223344";

dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
int outVal = 0;

BLYNK_WRITE(V0) {
  outVal=param.asInt();
  dimmer.setPower(outVal); // setPower(0-100%);
  Blynk.virtualWrite(V1, dimmer.getPower());
  Serial.print(dimmer.getPower());
  Serial.println("%");
}

//void dim(){
  //dimmer.setPower(outVal); // setPower(0-100%);
  //Serial.print(dimmer.getPower());
  //Serial.println("%");
//}

void setup() {
  Serial.begin(9600); 
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
//  timer.setInterval(100L, dim);
}

void loop() {
  Blynk.run();
  timer.run();
}
