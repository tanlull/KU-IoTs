//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

const byte address[][6] = {"00001", "00002"};

struct package
{
    int id = 1;
    float temp = 0.0;
    float humi = 0.0;
};
typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  if (!radio.begin()) { //เปิดการใชงาน nrf24 และตรวจสอบ
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} //หมุ่นค้างรอแก้ไขวงจร
  }

  //set the address
  radio.openReadingPipe(1, address[0]); //  ใช่ pipe 1
  radio.openReadingPipe(2, address[1]); //  ใช่ pipe 2
  
  radio.startListening(); //เป็นโหมด รับ
  Serial.println("is Receive");
  Serial.println(radio.isChipConnected());
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&data, sizeof(data));
    Serial.println("Received");
    if(data.id==1){
      Serial.print("ID : 1");
      //Serial.println(data.id);
    
      Serial.print("Temp : ");
      Serial.print(data.temp);
      Serial.println(" °C ");
    
      Serial.print("Hum. : ");    
      Serial.print(data.humi);
      Serial.println(" %");   
    }
    if(data.id==2){
      Serial.print("ID : 2");
      //Serial.println(data.id);
    
      Serial.print("Temp : ");
      Serial.print(data.temp);
      Serial.println(" °C ");
    
      Serial.print("Hum. : ");    
      Serial.print(data.humi);
      Serial.println(" %");   
    }
     
  }
}
