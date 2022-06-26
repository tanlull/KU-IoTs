#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(D2, D3); // RX, TX
byte Read_address[] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x85, 0xDB};
byte Set_address[] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x03, 0xEB, 0xC1};

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


//mySerial.write(Set_address, sizeof(Set_address));
delay(10);
Serial.println("Read address");
mySerial.write(Read_address, sizeof(Read_address));
address_values = mySerial.readString(); 
Serial.println(address_values[4],HEX);

delay(3000);
}
