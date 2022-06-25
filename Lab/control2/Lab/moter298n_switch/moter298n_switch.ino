
int ENA = 14; //D5;
int IN1 = 12; //D6;
int IN2 = 13; //D7;
int button_pin = 5; //D1
bool direct=0;

void ICACHE_RAM_ATTR button(); 

void button()     //This is the function that the interupt calls
{
  direct=!direct; //กลับทิศทางการหมุน
  if(direct)
  {
   digitalWrite (ENA, HIGH);  
   digitalWrite (IN1, HIGH);
   digitalWrite (IN2, LOW);    
  }else 
  {
   digitalWrite (ENA, HIGH);
   digitalWrite (IN1, LOW);
   digitalWrite (IN2, HIGH);
  }
}

void setup() {
// set all the motor control pins to outputs
  pinMode(button_pin, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(button_pin), button, RISING); //and the interrupt is attached
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite (ENA, LOW);
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, LOW);
}

void loop() {  
  delay(1000);
}
