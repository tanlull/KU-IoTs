 /*  +---------------+-------------------------+-------------------------+
 *  |   Board       | INPUT Pin               | OUTPUT Pin              |
 *  |               | Zero-Cross              |                         |
 *  +---------------+-------------------------+-------------------------+
 *  | ESP8266       | D1(IO5),    D2(IO4),    | D0(IO16),   D1(IO5),    |
 *  |               | D5(IO14),   D6(IO12),   | D2(IO4),    D5(IO14),   |
 *  |               | D7(IO13),   D8(IO15),   | D6(IO12),   D7(IO13),   |
 *  |               |                         | D8(IO15)                |
 *  +---------------+-------------------------+-------------------------+*/
#define Output_pin 14 //D5
#define Zerocross_pin 12 //D6
int dimming = 100;  // ระดับ Dimming  (0-100)  0 = เปิดเต็มที่, 100 = ปิด
void ICACHE_RAM_ATTR zero_intr(); 
void setup()
{
  Serial.begin(9600);
  pinMode(Output_pin, OUTPUT);// Set AC Load pin as output
  attachInterrupt(Zerocross_pin, zero_intr, RISING);  // เลือกขา GPIO7 เป็นขาอินเตอรัพตรวจจับการเกิด zero-crossing
}

void zero_intr()  // interupt function กระตุ่น triac ตามช่วงเวลาที่ dimming กำหนด
{
  // Firing angle calculation : 1 full 50Hz wave T =1/50=20ms  
  // zerocrossing จะเกิดทุกๆ: 10ms (1/2 Cycle)=10000us
  // (10000us - 10us) / 100 = 95us (ประมาณ)
  int dimtime = (95*dimming); //ช่วงปิด triac     
  delayMicroseconds(dimtime);        // รอเวลากระตุ่น
  digitalWrite(Output_pin, HIGH);   // กระตุ่น triac
  delayMicroseconds(20);             // หน่วงเวลาให้ triac เริ่มนำกระแส
  digitalWrite(Output_pin, LOW);    // หยุดกระตุ่น แต่ triac ยังคงนำกระแส
}
void loop()  {
  for (int i=20; i <= 99; i++) //ปรับค่า dimming จากเปิดเต็มจนปิด
  {
    dimming=i;
    delay(100);
  }
}
