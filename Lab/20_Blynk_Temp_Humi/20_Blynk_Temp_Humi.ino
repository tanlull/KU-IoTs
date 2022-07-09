#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLgYgVXgrp"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "eUFlE7qMZSjkl7HEyV3s177OfCUqSP8x"
#define ledBuildin 16
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
float sensorVal;
float humi;
float temp_c;
float temp_f;
float hif;
float hic;

BlynkTimer timer;
#define DHTPIN 4 //D2 Plug Temp to this pin

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
void myTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V1, temp_c);   // Write temp
  Blynk.virtualWrite(V4, humi);   // Write humidity
}

const char* ssid = "Pakhun";
const char* password = "022798129";
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  timer.setInterval(1000L, myTimer); 

}
void loop()
{
// Reading sensor from hardware analog pin A0
  readTemp();
  
  // Runs all Blynk stuff
  Blynk.run(); 
  
  // runs BlynkTimer
  timer.run(); 
  
}



void readTemp(){

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humi = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp_c = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  temp_f = dht.readTemperature(true);
  
  // Compute heat index in Fahrenheit (the default)
  hif = dht.computeHeatIndex(temp_f, humi);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht.computeHeatIndex(temp_c, humi, false);

  Serial.print(F("Humidity: "));
  Serial.print(humi);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp_c);
  Serial.print(F("°C "));
  Serial.print(temp_f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(ledBuildin,HIGH);  // Set digital pin 16 HIGH Build in LED
    Serial.println("V0 ON");
    Blynk.virtualWrite(V7,1);
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(ledBuildin,LOW);  // Set digital pin 16 LOW     Build in LED
    Serial.println("V0 OFF");
    Blynk.virtualWrite(V7,0);
  }
}
