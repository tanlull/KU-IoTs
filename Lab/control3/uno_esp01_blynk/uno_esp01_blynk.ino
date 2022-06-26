#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = "Cd9jZ8iv25Xbpmsr-BgIvz9jqnkDxpp1";

char ssid[] = "Tump_2.4G";
char pass[] = "Tump2519";

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX , mega:10,11  uno:2,3

ESP8266 wifi(&EspSerial);

void setup()
{
 
  // Debug console
  Serial.begin(9600); 

  // Set ESP8266 baud rate
  EspSerial.begin(9600); //ค่าเริ่มต้นของ esp-01 คือ 115200
  delay(10);

  //Blynk.begin(auth, wifi, ssid, pass);
    Blynk.begin(auth,wifi, ssid, pass, "blynk.iot-cm.com", 8080);
  EspSerial.print("AT\r\n");
}

void loop()
{
  Blynk.run();
}
