
int ENA = 14; //D5;
int IN1 = 12; //D6;
int IN2 = 13; //D7;
int pwm_speed = 600;
int button_pin_up = 5; //D1
int button_pin_down = 4; //D2
void ICACHE_RAM_ATTR button_up(); 
void ICACHE_RAM_ATTR button_down(); 


void setup() {
// set all the motor control pins to outputs
  Serial.begin(9600);
  delay(500);
  pinMode(button_pin_up, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(button_pin_up), button_up, RISING); //and the interrupt is attached
  pinMode(button_pin_down, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(button_pin_down), button_down, RISING); //and the interrupt is attached
 
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  analogWrite(ENA, pwm_speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  Serial.println("+ - for sets Speed of motors");
}

void button_up()     //This is the function that the interupt calls
{

pwm_speed +=100;
rotate_motor();
}

void button_down()     //This is the function that the interupt calls
{

pwm_speed -=100;
rotate_motor();
}

void rotate_motor(){

        if (pwm_speed<0)
        {
          pwm_speed = 0;
        }
        if (pwm_speed>1000)
        {
          pwm_speed = 1000;
        }
        analogWrite(ENA, pwm_speed);
        delay(50);
        Serial.print("pwm = ");
        Serial.println(pwm_speed);
}

void loop()
{
  if ( Serial.available()) {
    char ch = Serial.read();
    if (ch == '+')
    {
        pwm_speed=pwm_speed+100;
        rotate_motor();
    }
    else if (ch == '-')
    {
        pwm_speed=pwm_speed-100;
        rotate_motor();
    }
  }
}
