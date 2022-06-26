
#include <SimpleKalmanFilter.h>
#include <TaskScheduler.h>
#define EC_PIN A0
double ECvoltage;
float ECcurrent;
float setpointEC=1.4; //สลัด 1.1-1.7 ms/cm
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.3);
int RelayPin = 4; //D2
int RelayStatus=0;
float estimated_EC=0;

void Controller();
void measureEC();
//Tasks
Task t1(5000,TASK_FOREVER,&Controller);
Task t2(1000,TASK_FOREVER,&measureEC);

Scheduler SchedulRun;

void measureEC(){
    ECvoltage=0;
   for(int i=0;i<10;i++) {  //เฉลี่ยค่าanalog`10ค่าที่วัดได้
     ECvoltage+=analogRead(EC_PIN)*3300/1023.0;
     delay(50);
   }   
   ECvoltage=ECvoltage/10;
   //Serial.println(ECvoltage,4); 
   if(ECvoltage<150)Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
   else if(ECvoltage>3300)Serial.println("Out of the range!");  //>20ms/cm,out of the range
   else
    {
      if(ECvoltage<=448)ECcurrent=6.8*ECvoltage-600;   //1ms/cm<EC<=3ms/cm
      else if(ECvoltage<=1457)ECcurrent=6.98*ECvoltage-600;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*ECvoltage+1500;                           //10ms/cm<EC<20ms/cm
      ECcurrent/=1000;    //convert us/cm to ms/cm
      Serial.print(ECcurrent,2);  //two decimal
      Serial.print(",");
      //Serial.println("ms/cm");
      estimated_EC = simpleKalmanFilter.updateEstimate(ECcurrent);
      //Serial.println("estimate");
      Serial.println(estimated_EC,2);  //two decimal
      //Serial.println("ms/cm");
    }
}

void Controller(){
 //********Bang Bang controller********
   if (estimated_EC < (setpointEC)){ 
     digitalWrite(RelayPin,LOW);
     delay(3000);
     digitalWrite(RelayPin,HIGH);
   }else{
     digitalWrite(RelayPin,HIGH);
   }
   //************************************
   //********Hysteresis controller********
   //if (estimated_EC < (setpointEC-0.3)){ 
   //  digitalWrite(RelayPin,LOW);
   //  delay(3000);
   //  digitalWrite(RelayPin,HIGH);
   //}   
   //if (estimated_EC > (setpointEC+0.3)){
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
