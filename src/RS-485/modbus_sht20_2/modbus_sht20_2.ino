#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(D2, D3); // RX, TX
//byte readTemp[] = {0x01, 0x04,  0x00,  0x01,  0x00,  0x01,  0x60,  0x0a};
//byte readHumi[] = {0x01, 0x04,  0x00,  0x02,  0x00,  0x01,  0xC1,  0xCA};
byte readTempHumi[] = {0x01, 0x04,  0x00,  0x01,  0x00,  0x02,  0x20,  0x0B};

String Temp_values; 
String Humi_values; 
String Temp_Humi_values; 
 

void setup()
{
  Serial.println("start init serial 0");
  Serial.begin(9600);
 
  while (!Serial) {
    Serial.println("loop for init serial 0");
  }
  Serial.println("start init software serial");
  mySerial.begin(9600);
  while (!mySerial) {
    Serial.println("loop for init software serial");
  }
 
}
 
void loop()
{
Serial.println("Read data");
mySerial.write(readTempHumi, sizeof(readTempHumi));
delay(10);
Temp_Humi_values = mySerial.readString(); 
int Temp=Temp_Humi_values[3]<<8|Temp_Humi_values[4];
int Humi=Temp_Humi_values[5]<<8|Temp_Humi_values[6];
float Temperature=(int(Temp))/10.0f;
float Humidity=(int(Humi)/10.0f);
Serial.print("Temp: ");
Serial.print(Temperature);
Serial.println(" C");
Serial.print("Humi: ");
Serial.print(Humidity);
Serial.println(" %");
//mySerial.flush();

delay(2000);
}
