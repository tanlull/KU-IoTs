#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
long distanc=0;
char auth[] = "6cqHCTiRdV6kifNVpYsvNo6PzW-A3bJ_";
char ssid[] = "Tump";
char pass[] = "11223344";

const int TRIGGER = D1;
int ECHO = D2;
int pump_pin = D5;//สั่งงานrelay ควบคุมปั้มน้ำ
WidgetLED led(V11);

void level(){
  digitalWrite(TRIGGER, LOW);   
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(5); 
  digitalWrite(TRIGGER, LOW);          //สร้างpule
  long duration = pulseIn(ECHO, HIGH); //จับเวลาสะท้อนกลับ
  distanc = (duration/2) / 29.41;      //ความเร็วเสียงในอากาศประมาณ 340 m/s หรือ 29.41 us/cm
  if (distanc<200){                    //ติดตั้งเซ็นเซอร์ที่ระดับ2เมตร
    Blynk.virtualWrite(V10, 200-distanc);
    Serial.print(distanc);
    Serial.print("cm");
    Serial.println();
  } 
}

void pump(){
  if((200-distanc)<20){ //เช็คระดับน้ำต่ำกว่า 20cm ให้เปิดปั้ม
    digitalWrite(pump_pin, LOW);
    led.on();
  }
  if((200-distanc)>180){ //เช็คระดับน้ำสูงกว่า 180cm ให้ปิดปั้ม
    digitalWrite(pump_pin, HIGH);
    led.off();
  }
}
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  pinMode(TRIGGER, OUTPUT); 
  pinMode(ECHO, INPUT);
  pinMode(pump_pin, OUTPUT);
  timer.setInterval(500L, level); //วัดระดับน้ำทุกๆ 0.5 วินาที
  timer.setInterval(1000L, pump); //เช็คระดับน้ำทุกๆ 1 วินาที
}
void loop()
{
  Blynk.run();
  timer.run();
}
