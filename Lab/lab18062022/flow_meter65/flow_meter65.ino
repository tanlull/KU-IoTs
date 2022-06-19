/*
   YF-S201 Hall Effect Water Flow Meter / Sensor
   http://www.hobbytronics.co.uk/yf-s201-water-flow-meter
   
   Read Water Flow Meter and output reading in litres/hour

*/

int  flow_frequency;  // Measures flow meter pulses
int  l_hour;          // Calculated litres/hour  
int l_minute;  
float l_total;   //Total Litres      
float l_at_a_time;   //Liters at time             
int flowmeter = 5;  // Flow Meter Pin number D1
int resetswitch = 0;  // Flow Meter Pin number D3 => GPIO0
long currentTime;
long cloopTime;


ICACHE_RAM_ATTR void flow ()                  // Interruot function
{ 
   flow_frequency++;
} 

ICACHE_RAM_ATTR void resetPress ()                  // Interruot function
{ 
   l_total=0;
} 

void setup()
{ 
   pinMode(flowmeter, INPUT);
   Serial.begin(9600); 
   attachInterrupt(flowmeter, flow, RISING); // Setup Interrupt : attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)               
   attachInterrupt(resetswitch, resetPress, FALLING);             
  
   sei();                            // Enable interrupts  
   currentTime = millis();
   cloopTime = currentTime;
   l_total;
} 

void loop ()    
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {     
      cloopTime = currentTime;              // Updates cloopTime
      //YF-S201 : Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
      //YF-S201C : Frequency: F = 5Q  -3   Accuracy: ± 2% 
      //FS300A : Frequency : F=4.5*Q)-3  Accuracy: ±2%
      //YF-DN40 1.5 inch : Flow Pulse: F(Hz)=(0.5xQ) Accuracy: ±3% 
      //YF-DN50 2 inch : Flow Pulse: F(Hz)=(0.2xQ) Accuracy: ±3% 
      l_minute = (flow_frequency / 7.5);
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      l_at_a_time = (flow_frequency / 7.5) / 60;
      l_total = l_total + l_at_a_time;
      flow_frequency = 0;                   // Reset Counter
      Serial.print(l_minute, DEC);
      Serial.print(" L/min");             // Print litres/min
      Serial.print(" , ");
      Serial.print(l_hour, DEC);            // Print litres/hour
      Serial.print(" L/hour");
      Serial.print(" , ");
      Serial.print(l_at_a_time);            // Print lit at 1 sec
      Serial.print(" L/Time");
      Serial.print(" , ");
      Serial.print(l_total);            // Print total litres
      Serial.println(" L Total");
   }
}
