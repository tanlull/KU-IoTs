//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

//address through which two modules communicate.
const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving
int  button_pin = 5; //D1
boolean button_state = 1;

struct package //เพคเกรดรับค่าเซนเซอร์จาก node
{
    int id = 1;
    float temp = 0.0;
    float humi = 0.0;
};
typedef struct package Package;
Package data;

ICACHE_RAM_ATTR void push_button() { //อินเตอร์รัพฟังชั่น ทำงานเมื่อมีการกดปุ่มลง
  radio.stopListening();
  radio.write(&button_state, sizeof(button_state));
  Serial.println("Push button");
}



void setup()
{
  pinMode(button_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button_pin), push_button, RISING);

  Serial.begin(9600);
  delay(1000);
  if (!radio.begin()) {  //เปิดการใชงาน nrf24 และตรวจสอบการเชื่อมต่อ
    Serial.println("radio hardware is not responding!!");
    while (1) delay(1); // hold in infinite loop
  }
  
  //set the address  
  radio.openWritingPipe(addresses[0]);     //ตั้งค่า address สำหรับส่ง
  radio.openReadingPipe(1, addresses[1]);  //ตั้งค่า address สำหรับรับ

   //radio.setChannel(115);
   //radio.setPALevel(RF24_PA_MIN); //RF24_PA_MAX เป็นค่าเริ่มต้น
   //radio.setDataRate(RF24_250KBPS);
   //radio.openReadingPipe(1, addresses[0]);


}
void loop()
{
  radio.startListening(); 
  if(radio.available()){
    radio.read(&data, sizeof(data));
    Serial.println("Received");
    
    Serial.print("ID : ");
    Serial.println(data.id);
    
    Serial.print("Temp : ");
    Serial.print(data.temp);
    Serial.println(" °C ");
    
    Serial.print("Hum. : ");    
    Serial.print(data.humi);
    Serial.println(" %");    
  }
  delay(1000);
}
