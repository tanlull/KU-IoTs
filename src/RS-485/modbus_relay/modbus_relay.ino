#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(D2, D3); // RX, TX
byte Read_address[] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x85, 0xDB};
//byte On_relay1[] = {0x01, 0x05, 0x00, 0x01, 0xFF, 0x00, 0x8C, 0xE4};
byte On_relay1[] = {0x03, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8D, 0xD8};
//byte On_relay1[] = {0xFF, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x99, 0xE4};
byte Off_relay1[] = {0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x28};
byte On_relayAll[] = {0xFF, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x01, 0xFF, 0x30, 0x1D};

String address_values; 

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  while (!mySerial) {
    Serial.println("loop for init software serial");
  }

}
 
void loop()
{
Serial.println("Read address");
//mySerial.write(Read_address, sizeof(Read_address));
mySerial.write(On_relay1, sizeof(On_relay1));
delay(10);
address_values = mySerial.readString(); 

//Serial.println(address_values[5],HEX);
Serial.println(address_values);
delay(3000);
mySerial.write(Off_relay1, sizeof(Off_relay1));

delay(3000);
}
