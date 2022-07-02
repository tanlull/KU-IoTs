//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "GRP05"; //ได้สูงสุด 5 ตัวอักษร

struct package
{
    int id = 1;
    float temp = 0.0;
    float humi = 0.0;
    char name[10] = "GROUP 05";
};
typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  if (! sht31.begin(0x44)) { // Set to 0x45 for alternate i2c addr
  Serial.println("Couldn't find SHT31");
  while (1) delay(1);
  }
  if (!radio.begin()) {  //เปิดการใชงาน nrf24 และตรวจสอบการเชื่อมต่อ
    Serial.println("radio hardware is not responding!!");
    while (1) delay(1); // hold in infinite loop
  }

  //set the address
  radio.openWritingPipe(address);

   //radio.setChannel(115);
   //radio.setPALevel(RF24_PA_LOW); //RF24_PA_MAX เป็นค่าเริ่มต้น
   //radio.setDataRate(RF24_250KBPS);
   //radio.openReadingPipe(1, addresses[0]);

  //Set module as transmitter
  radio.stopListening();

  Serial.println("trasmettere");
  Serial.println(radio.isChipConnected());
}
void loop()
{
  
  data.temp = sht31.readTemperature();
  delay(20);
  Serial.print("Temp *C = "); Serial.println(data.temp);
  data.humi = sht31.readHumidity();
  delay(20);
  Serial.print("Hum. % = "); Serial.println(data.humi);
  Serial.println("Send");
  bool ret = radio.write(&data, sizeof(data));
  Serial.print("Write ret: ");
  Serial.println(ret);
  data.id+=1;
  delay(1000);
}
