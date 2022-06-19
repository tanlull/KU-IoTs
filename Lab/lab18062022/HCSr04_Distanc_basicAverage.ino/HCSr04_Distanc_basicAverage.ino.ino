#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(5, 4);  // Trig (D1) 5 / echo (D2) 4.
int indexev = 0;
float sumDistance = 0;
float h_ground_2_sensor = 100; // Ground to censor height

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
        float avrDistance = sumDistance/10;
        float water_level = h_ground_2_sensor - dist;
        if(water_level <0) water_level =0;
        Serial.print(dist);
        Serial.print("cm , avg =");
        Serial.print(avrDistance);
        Serial.print("cm , water=");
        Serial.print(water_level);
        Serial.println("cm");
        indexev = 0;
        sumDistance = 0;
    }

    
}
