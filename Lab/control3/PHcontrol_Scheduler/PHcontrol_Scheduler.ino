
#include <SimpleKalmanFilter.h>
#include <TaskScheduler.h>
#define PH_PIN A0
double PHvoltage;
float PHcurrent;
float setpointPH=6.5; //สลัด 6-7pH 
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.3);
int RelayPin = 4; //D2
int RelayStatus=0;
float estimated_PH=0;

void Controller();
void measurePH();
//Tasks
Task t1(5000,TASK_FOREVER,&Controller);
Task t2(1000,TASK_FOREVER,&measurePH);

Scheduler SchedulRun;

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
    Serial.println(estimated_PH,2);  //two decimal
    
}

void Controller(){
 //********Bang Bang controller********
   if (estimated_PH < (setpointPH)){ 
     digitalWrite(RelayPin,LOW);
     delay(3000);
     digitalWrite(RelayPin,HIGH);
   }else{
     digitalWrite(RelayPin,HIGH);
   }
   //************************************
   //********Hysteresis controller********
   //if (estimated_PH < (setpointPH-0.3)){ 
   //  digitalWrite(RelayPin,LOW);
   //  delay(3000);
   //  digitalWrite(RelayPin,HIGH);
   //}   
   //if (estimated_PH > (setpointPH+0.3)){
   //  digitalWrite(RelayPin,HIGH);
   //}
   //************************************
}

void setup() {
  Serial.begin(115200);
  delay(100);
  SchedulRun.init();
  SchedulRun.addTask(t1);
  SchedulRun.addTask(t2);
  delay(5000);
  t1.enable();
  t2.enable();
}
void loop(){
   SchedulRun.execute();
}
