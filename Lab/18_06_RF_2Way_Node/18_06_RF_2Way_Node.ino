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
const byte addresses [][6] = {"GRP51", "GRP52"};  //Setting the two addresses. One for transmitting and one for receiving
int relay_pin = 16; //D0
boolean button_state = 0;
boolean tog;

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
  pinMode(relay_pin, OUTPUT);
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
  radio.openWritingPipe(addresses[1]);     //ตั้งค่า address สำหรับส่ง
  radio.openReadingPipe(1, addresses[0]);  //ตั้งค่า address สำหรับรับ

   //radio.setChannel(115);
   //radio.setPALevel(RF24_PA_MIN); //RF24_PA_MAX เป็นค่าเริ่มต้น
   //radio.setDataRate(RF24_250KBPS);
   //radio.openReadingPipe(1, addresses[0]);


}
void loop()
{
  static uint32_t oldtime=millis();
  if ( (millis()-oldtime) > 5000) { //ตั้งเวลาให้ทำงานทุกๆ 2วินาที
    oldtime = millis();
    radio.stopListening();
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

  }

  radio.startListening(); 
  if(radio.available()){
    radio.read(&button_state, sizeof(button_state)); //Reading the data
    Serial.println("get button status");
    if (button_state){
      tog = !tog; 
      Serial.println(tog);
      if (tog) digitalWrite(relay_pin, HIGH); 
      else digitalWrite(relay_pin, LOW);
      button_state = 0;
    }

  }
  delay(1000);
}
