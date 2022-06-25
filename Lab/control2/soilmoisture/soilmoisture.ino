#define SECONDS_DS(seconds) ((seconds)*1000000UL)
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
long distanc=0;
char auth[] = "TRcjbQQrN_uMAmoi8DxD2hlk3VQEwqKU";
char ssid[] = "TOT_ARM";
char pass[] = "034352523";

int sensorPin = A0;
int sensorValue = 0;
int moistureValue = 0;
int Valv_pin = D5;//สั่งงานrelay ควบคุมวาล์ว
WidgetLED led(V11);

void moisture(){
  sensorValue = analogRead(sensorPin);
  moistureValue = map(sensorValue,300,750,100,0);
  Blynk.virtualWrite(V10, moistureValue);
  Serial.print(moistureValue);
  Serial.print("%");
  Serial.println();
  
}

void Valv(){
  if(moistureValue<40){
    digitalWrite(Valv_pin, LOW);
    led.on();
    Serial.print("Open Valv");
  }
  if(moistureValue>80){ 
    digitalWrite(Valv_pin, HIGH);
    led.off();
    Serial.println("Close Valv");
  }
  //ESP.deepSleep(SECONDS_DS(15));
  ESP.deepSleep(15e6); 
}
void setup() {
  Serial.begin(9600);
  while(!Serial) { 
  }
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  pinMode(Valv_pin, OUTPUT);
  timer.setInterval(500L, moisture); //วัดระดับความชื้นทุกๆ 0.5 วินาที
  timer.setInterval(1000L, Valv); //เช็คระดับความชื้นเพื่อเปิดปิดวาล์วทุกๆ 1 วินาที


}
void loop()
{
  Blynk.run();
  timer.run();
}
