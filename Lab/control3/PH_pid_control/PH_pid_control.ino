#include <AutoPID.h>
#include <SimpleKalmanFilter.h>

#define PH_PIN A0
double PHvoltage;
float PHcurrent;
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.3);

//pid settings and gains
#define OUTPUT_MIN 200
#define OUTPUT_MAX 1023
#define KP 500 
#define KI 0
#define KD 0


int IN1 = 4; //D2;
int pwm_speed = 0;

double estimated_PH, controlSig;
double setpointPH=6.5; //สลัด 6-7pH 

void measurePH();
AutoPID myPID(&estimated_PH, &setpointPH, &controlSig, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

void measurePH(){
    double PHvoltage;
    for(int i=0;i<10;i++) {
     PHvoltage+=analogRead(PH_PIN)*3.3/1023.0;
     delay(50);
   }
    PHvoltage=PHvoltage/10;
    //Serial.print("PH voltage:");
    //Serial.println(PHvoltage,3);

   // m = (pH6.86 - pH4) / (Vph6.86 - Vph4)=12.83
    double PH = 7 + ((PHvoltage-1.948)*12);
    Serial.print(PH,2);  //two decimal
    Serial.print(",");
    estimated_PH = simpleKalmanFilter.updateEstimate(PH);
    //Serial.println("estimate");
    Serial.print(controlSig);
    Serial.print(",");
    Serial.println(estimated_PH,2);  //two decimal
    
}

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(IN1, OUTPUT);
  myPID.setTimeStep(1000);
}
void loop(){
  measurePH();
  myPID.run(); //call every loop, updates automatically at certain time interval
  analogWrite(IN1, controlSig);// set Speed(0-1024);
}
