int heater = 15; // D8
int x;
void setup() {
  pinMode(heater, OUTPUT);
  Serial.begin(9600);
}

void Led_test(){
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle < 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(heater, dutyCycle);
    delay(1);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle > 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(heater, dutyCycle);
    delay(1);
  } 
}

void loop() {
  Led_test();
//    x = 500;
//    analogWrite(heater, x);
//
//    Serial.println(x);
//    delay(500);

}
