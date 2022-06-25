#define BLYNK_PRINT Serial
#include <ArduinoJson.h>
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
   DynamicJsonDocument doc(200);
   deserializeJson(doc, DataString);
   String Name=doc["nodeName"];
   String temp=doc["Temp"];
   String humidity=doc["Humi"];
   Serial.println(temp);
   Blynk.virtualWrite(V3, Name);//Label value
   Blynk.virtualWrite(V4, temp); //gauge
   Blynk.virtualWrite(V5, humidity); //gauge
   Blynk.virtualWrite(V1, DataString);  //Terminal
  }
  DataString = "";
}
void setup() {


  Serial.begin(9600);//hardware serial 
  software_Serial.begin(115200);//software serial
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  timer.setInterval(1000L, serialRead);
}
void loop() {
  Blynk.run();
  timer.run();
}
