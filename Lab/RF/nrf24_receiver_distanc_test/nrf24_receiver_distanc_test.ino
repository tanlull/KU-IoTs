//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";//ได้สูงสุด 5 ตัวอักษร

void setup()
{
  Serial.begin(9600);
  delay(2000);

  if (!radio.begin()) { //เปิดการใชงาน nrf24 และตรวจสอบ
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} //หมุ่นค้างรอแก้ไขวงจร
  }

  //set the address
  radio.openReadingPipe(0, address); //  ใช่ pipe 0

  radio.startListening(); //เป็นโหมด รับ
  //radio.stopListening(); //เป็นโหมด ส่ง
  Serial.println("is Receive");
  Serial.println(radio.isChipConnected());
  pinMode(16, OUTPUT);
}

void loop()
{
  char text[32] = {0}; //รับได้สุงสุด 32 ตัวอักษร
  //Read the data if available in buffer
  if (radio.available())
  {
    Serial.println("Receive");
    radio.read(&text, sizeof(text));
    digitalWrite(16, LOW); 
    Serial.println(text);
    delay(500);
  }else{
    digitalWrite(16, HIGH); 
  }
}