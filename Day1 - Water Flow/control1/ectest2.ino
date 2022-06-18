#include <OneWire.h>
#define StartConvert 0
#define ReadTemperature 1
#define ECsensorPin A0  //EC Meter analog output,pin on analog 4::P32
 // 12.88ms/cm->1.84v  1.413ms/cm->0.217v      0.084ms/cm->0.0225v
const byte numReadings = 20;     //the number of sample times

byte DS18B20_Pin = 2; //DS18B20 signal, pin on digital 2
unsigned int AnalogSampleInterval=25,printInterval=700;  //analog sample interval;serial print interval;temperature sample interval
unsigned int readings[numReadings];      // the readings from the analog input
int indexec = 0;                  // the indexec of the current reading
unsigned long AnalogValueTotal = 0;                  // the running total
unsigned int AnalogAverage = 0,averageVoltage=0;                // the average
unsigned long AnalogSampleTime,printTime;
float ECcurrent; 
 
//Temperature chip i/o
OneWire ds(DS18B20_Pin);  // on digital pin 2
 
void setup() {
  Serial.begin(115200);
  // initialize all the readings to 0:
  for (byte thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  AnalogSampleTime=millis();
  printTime=millis();
}
 
void loop() {
  /*
   Every once in a while,sample the analog value and calculate the average.
  */
  if(millis()-AnalogSampleTime>=AnalogSampleInterval)  
  {
    AnalogSampleTime=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - readings[indexec];
    // read from the sensor:
    readings[indexec] = analogRead(ECsensorPin);
    //Serial.println(readings[indexec]);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + readings[indexec];
    // advance to the next position in the array:
    indexec = indexec + 1;
    // if we're at the end of the array...
    if (indexec >= numReadings)
    // ...wrap around to the beginning:
    indexec = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / numReadings;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */

   /*
   Every once in a while,print the information on the serial monitor.
  */
  if(millis()-printTime>=printInterval)
  {
    printTime=millis();
    averageVoltage=150+AnalogAverage*(float)3300/4095;
    Serial.print("Analog value:");
    Serial.print(AnalogAverage);   //analog average,from 0 to 4095
    Serial.print("    Voltage:");
    Serial.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
    Serial.print("mV    ");

    float TempCoefficient=1.0;
    //float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
    if(CoefficientVolatge<150)Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)Serial.println("Out of the range!");  //>20ms/cm,out of the range
    else
    { 
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-84.32;   //1ms/cm<EC<=3ms/cm
     // if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+3500;                           //10ms/cm<EC<20ms/cm     
      //else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
      ECcurrent/=1000;    //convert us/cm to ms/cm
      Serial.print(ECcurrent,2);  //two decimal
      Serial.println("ms/cm");
    }
  }
 
}
