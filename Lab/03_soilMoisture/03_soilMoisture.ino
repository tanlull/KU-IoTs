#include <ESP8266WiFi.h>              // Include main library
//const char* ssid     = "Tump";    //ชื่อ wifi
//const char* password = "11223344";  // wifi password

int sensorValue = 0;
void setup() 
{
  Serial.begin(9600); 
//  pinMode(D1, OUTPUT);       //***
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) 
//  {
//    delay(500);
//  }
}
void loop() 
{
  sensorValue = analogRead(A0);       //อ่านค่า analog
  //****************************
 int  outputValue = map(sensorValue, 0, 1023, 100, 0); // Map 0 => 100 , 1023 => 0
 
  if(outputValue<30)
  {
      digitalWrite(D1, LOW);
  }else {
      digitalWrite(D1, HIGH);
  }
  //***************************
  Serial.print(outputValue);
  Serial.println(" %");
  delay(1000);                          //หน่วงเวลา 1 วินาที

}
