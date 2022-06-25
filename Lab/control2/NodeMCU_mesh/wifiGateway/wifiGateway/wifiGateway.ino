#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
String temp;
char auth[] = "6cqHCTiRdV6kifNVpYsvNo6PzW-A3bJ_";
const char* ssid = "Tump";  
const char* pass = "11223344"; 


String DataString;
SoftwareSerial software_Serial(D1, D2); 
void serialRead(){
  while (software_Serial.available() > 0)
  {
    char readChar = software_Serial.read();  
    DataString += readChar; 
  }  
  if (DataString  != "")
    {
   Serial.println(DataString);
   temp=DataString.substring(29,31);
   Serial.println(temp);
   Blynk.virtualWrite(V4, temp);
   Blynk.virtualWrite(V1, DataString);  
  }
  DataString = "";
}
void setup() {
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  Serial.begin(9600);//hardware serial 
  software_Serial.begin(115200);//software serial
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  timer.setInterval(1000L, serialRead);
}
void loop() {
  Blynk.run();
  timer.run();
}
