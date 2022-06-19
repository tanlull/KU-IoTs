
#include <SimpleKalmanFilter.h>


float calibration = 21.40; //change this value to calibrate
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.3);

void setup() {
 Serial.begin(9600);
}
 
void loop() {
 for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(analogInPin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*3.3/1024/6;
 Serial.print("Volt = ");
 Serial.println(pHVol);
 
 float phValue =  -15.23529412 * pHVol + 54.28; // Must be Adjust slope -15.23529412 and  54.28
 float estimated_ph = simpleKalmanFilter.updateEstimate(phValue);
 //Serial.print("sensor = ");
 //Serial.println(phValue);
 Serial.print(estimated_ph); Serial.print("\t");
 Serial.print(phValue); Serial.println("\t");
 //Serial.print("\r\n");
 delay(20);
}