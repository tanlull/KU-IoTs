#include <RBDdimmer.h>
#include "DHT.h"
#define DHTPIN 5   //D1 
#define outputPin  14  //D5 node mcu
#define zerocross  12 // D6 for boards with CHANGEBLE input pins
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
double temperature, control_signal;
double setPoint;
double Kp=50; //proportional gain
double Ki=0.002; //integral gain
double Kd=0; //derivative gain
int T=1000; //sample time in milliseconds (ms)
unsigned long last_time;
double total_error, last_error;
int max_control=99;
int min_control=10;

DHT dht(DHTPIN, DHTTYPE);
dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards


void setup(){ 
  Serial.begin(9600);
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 
  dht.begin();
}

void loop(){
  if (Serial.available())
  {
    int buf = Serial.parseInt();
    if (buf != 0) setPoint = buf; //33C
    delay(200);
  }
   PID_Control(); //calls the PID function every T interval and outputs a control signal 
   delay(500);
}

void PID_Control(){
    unsigned long current_time = millis(); //returns the number of milliseconds passed since the Arduino started running the program
    int delta_time = current_time - last_time; //delta time interval 
    if (delta_time >= T){
      temperature = dht.readTemperature(); //get temp reading
      delay(100);
      
      Serial.print(setPoint); 
      Serial.print(",");
      Serial.print(control_signal); 
      Serial.print(",");
      Serial.println(temperature);
      double error = setPoint - temperature;
      //Serial.println(error);
      total_error += error; //accumalates the error - integral term
      if (total_error >= max_control) total_error = max_control;
      else if (total_error <= min_control) total_error = min_control;
      
      double delta_error = error - last_error; //difference of error for derivative term
      control_signal = Kp*error + (Ki*T)*total_error + (Kd/T)*delta_error; //PID control compute
      if (control_signal >= max_control) control_signal = max_control;
      else if (control_signal <= min_control) control_signal = min_control;
      
      last_error = error;
      last_time = current_time;
      dimmer.setPower(control_signal); // setPower(0-100%);
  } 
}
