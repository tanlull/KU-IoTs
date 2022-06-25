
int ENA = 14; //D5;
int IN1 = 12; //D6;
int IN2 = 13; //D7;

void setup() {
// set all the motor control pins to outputs
pinMode(ENA, OUTPUT);//ขา enable
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
}

//ฟังชั่น testOne เป็นการควบคุมทิศทางการหมุนด้วยความเร็วคงที่

void testOne() {
// ขับมอเตอร์
digitalWrite(ENA, HIGH);  
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);

delay(5000); 
// หมุนไป 5วินาทีแล้วกลับทางหมุน
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);

delay(5000); // now turn off motors
//หมุนอีก 5วินาที และหยุดหมุน 
//digitalWrite(ENA, LOW);
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
}

// ฟังชั่นควบคุมความเร็ว
void testTwo() {

// หมุนมอเตอร์
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);

// เพิ่มความเร็วจากช้าไปจนถึงเร็วสุด
for (int i = 500; i < 1023; i++)
  {
    analogWrite(ENA, i);
    delay(50);
   }

// ลดความเร็วจากสูงสุดจนช้าสุด
for (int i = 1023; i >= 500; --i)
    {
      analogWrite(ENA, i);
      delay(50);
     }

// หยุดมอเตอร์
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
}

void loop() {
  testOne();   
  delay(1000);   
  testTwo();   
  delay(1000);
}
