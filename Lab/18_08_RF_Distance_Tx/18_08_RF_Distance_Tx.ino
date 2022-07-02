//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "GRP05"; //ได้สูงสุด 5 ตัวอักษร


void setup()
{
  Serial.begin(9600);
  delay(2000);

  if (!radio.begin()) {  //เปิดการใชงาน nrf24 และตรวจสอบการเชื่อมต่อ
    Serial.println("radio hardware is not responding!!");
    while (1) {} // hold in infinite loop
  }

  //set the address
  radio.openWritingPipe(address);

  //Set module as transmitter
  radio.stopListening();

  Serial.println("Trasmitter");
  Serial.println(radio.isChipConnected());
}
void loop()
{
  Serial.println("Send");
  const char text[] = "111";
  bool ret = radio.write(&text, sizeof(text));
  Serial.print("Write ret: ");
  Serial.println(ret);

  delay(1000);
}
