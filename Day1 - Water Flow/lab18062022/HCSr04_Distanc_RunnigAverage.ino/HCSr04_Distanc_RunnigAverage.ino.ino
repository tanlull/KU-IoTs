#include <HCSR04.h>
#include <RunningAverage.h>

UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.
RunningAverage myRA(10);
int samples = 0;
float avrDistance = 0;

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
    myRA.clear();
}

void loop () {
    // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
    float dist=distanceSensor.measureDistanceCm();
    if(dist>0){
        myRA.addValue(dist);
        samples++;
        avrDistance = myRA.getAverage();
        delay(100);
        
    }
    if(samples == 20){
         samples = 0;
         myRA.clear();
        Serial.print(dist);
        Serial.print(",");
        Serial.println(avrDistance);
    }

    
}
