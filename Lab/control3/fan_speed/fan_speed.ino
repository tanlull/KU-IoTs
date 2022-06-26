
//int ENA = 14; //D5;
int IN1 = 4; //D2;
//int IN2 = 13; //D7;
int pwm_speed = 600;
void setup() {
// set all the motor control pins to outputs
  Serial.begin(9600);
  delay(500);
 // pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
 // pinMode(IN2, OUTPUT);
 // analogWrite(ENA, pwm_speed);
  digitalWrite(IN1, pwm_speed);
 // digitalWrite(IN2, LOW);
  Serial.println("+ - for sets Speed of motors");
}

void loop()
{
  if ( Serial.available()) {
    char ch = Serial.read();
    if (ch == '+')
    {
        pwm_speed=pwm_speed+100;
        if (pwm_speed>1000)
        {
          pwm_speed = 1000;
        }
        analogWrite(IN1, pwm_speed);
        delay(50);
        Serial.print("pwm = ");
        Serial.println(pwm_speed);
    }
    else if (ch == '-')
    {
        pwm_speed=pwm_speed-100;
        if (pwm_speed<0)
        {
          pwm_speed = 0;
        }
        analogWrite(IN1, pwm_speed);
        delay(50);
        Serial.print("pwm = ");
        Serial.println(pwm_speed);
    }
  }
}
