#include <HCSR04.h>
#include <SimpleKalmanFilter.h>

UltraSonicDistanceSensor distanceSensor(5, 4);  // Initialize sensor that uses digital pins 13 and 12.
int indexev = 0;
float sumDistance = 0;
float est_Distance=0;
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.1);
/* SimpleKalmanFilter(e_mea, e_est, q);
 e_mea: Measurement Uncertainty 
 e_est: Estimation Uncertainty 
 q: ;Process Noise
*/

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
}

void loop () {
    // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
    float dist=distanceSensor.measureDistanceCm();
    if(dist>0){
      sumDistance = sumDistance + dist;
      indexev = indexev + 1;
      delay(100);
    }
    if(indexev >= 10){
        est_Distance = simpleKalmanFilter.updateEstimate(dist);  
        float avrDistance = sumDistance/10;
        Serial.print(dist);
        Serial.print(",");
        Serial.print(avrDistance);
        Serial.print(",");
        Serial.println(est_Distance);
        
        indexev = 0;
        sumDistance = 0;

    }

    
}
