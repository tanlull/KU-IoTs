// Board => Node MCU 1.0 


#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLgYgVXgrp"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "eUFlE7qMZSjkl7HEyV3s177OfCUqSP8x"

#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
BlynkTimer timer2;

// DHT
#define DHTPIN 14   // D5
#define D1 5  
#define D2 4   
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
long i=0;

#define LED_DISP 2 // build in LED 2 or 16

#define ON_OFF_SWITCH 5 

float humi;
float temp_c;
float real_c;
float light_lux;

BH1750 lightMeter(0x23); // ADDR -> GND (0x23) , ADDR -> VCC (0x5C) , 

//const char* ssid = "ALIEN_2_2.4G";
//const char* password = "FF5754755";
const char* ssid = "Pakhun";
const char* password = "022798129";
bool ledStatus = 0;
String payload = "json data";
long readDataTime = 15000; // 50 s
long ledBlinkTime = 2000; // 50 s


void setup() {
  Serial.begin(115200);
  while (!Serial); // time to get serial running
  Serial.println(F("Serial Ready!"));
  //attachInterrupt(ON_OFF_SWITCH, switchLED, RISING); 
  //startWifi();
  pinMode(LED_DISP, OUTPUT);
  dht.begin();
  Wire.begin(D2,D1); // SDA, SCL  
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
      Serial.println(F("BH1750 Advanced begin"));
    } else {
      Serial.println(F("Error initialising BH1750"));
    }
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(readDataTime, readSensorData); 
  timer2.setInterval(ledBlinkTime, blinkLED); 
}

void loop() {

   Blynk.run();
   timer.run(); 
   timer2.run(); 
}

void readSensorData(){
   readDHTData();
   readLux();
   disPlayLED(100);
   update2BLYNK();
}

void update2BLYNK() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V1, temp_c);   // Write temp
  Blynk.virtualWrite(V4, humi);   // Write humidity
  Blynk.virtualWrite(V5, light_lux);   // Write Light
}

//void startWifi(){
//  WiFi.begin(ssid, password);
//  Serial.print("WiFi Connecting..");
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    digitalWrite(LED_DISP, LOW);
//    delay(250);
//    digitalWrite(LED_DISP, HIGH);
//    delay(250);
//  }
//  Serial.println("Connected");
//
//  Serial.println("Ready");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//}

void readDHTData(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);



  float light = readLux(); 
  humi=h;
  temp_c=t;
  real_c=hic;
  light_lux=light;
  i += 1;
  payload="{\"id\":"+String(i)+",\"temperature\":"+String(t)+",\"humidity\":"+String(h)+",\"realfeel\":"+String(hic)+",\"light\":"+String(light)+"}";
  //publishString(payload);
  Serial.println(payload);
}

void publishString(String data){
  char Buf[100];
  data.toCharArray(Buf,100);
  //client.publish(topic, &Buf[0]);
}

void blinkLED(){
  disPlayLED(200);
}


void disPlayLED(int delayTime){
    //Serial.println("LED Blink OFF");
    digitalWrite(LED_DISP, LOW);
    delay(delayTime);
    //Serial.println("LED Blink ON");
    digitalWrite(LED_DISP, HIGH);
}

ICACHE_RAM_ATTR void switchLED(){
  if(ledStatus==false){
    digitalWrite(LED_DISP, HIGH);
    ledStatus=true;
  }else{
    digitalWrite(LED_DISP, LOW);
    ledStatus=false;
  }
}

float readLux(){
  float lux = lightMeter.readLightLevel();
//  Serial.print("Light: ");
//  Serial.print(lux);
//  Serial.println(" lx");
  return lux;
}
