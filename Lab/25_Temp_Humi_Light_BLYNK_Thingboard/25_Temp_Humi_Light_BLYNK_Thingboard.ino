// Board => Node MCU 1.0 


#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLgYgVXgrp"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "eUFlE7qMZSjkl7HEyV3s177OfCUqSP8x"

char thingsboardServer[] = "totws.com";
#define TOKEN "X5OCQ5XI8qk4IhaEbcqh"

#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"
#include <BlynkSimpleEsp8266.h>
#include <ThingsBoard.h>

BlynkTimer timer;
BlynkTimer timer2;

ThingsBoard tb(_blynkWifiClient);

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
long readDataTime = 5000; // 50 s
long ledBlinkTime = 2000; // 50 s

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  while (!Serial); // time to get serial running
  Serial.println(F("Serial Ready!"));
  //attachInterrupt(ON_OFF_SWITCH, switchLED, RISING); 
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
     if ( !tb.connected() ) {
      //Serial.println("Thingsboard NOT Connected");
     reconnect();
  }
   tb.loop();
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
  Blynk.virtualWrite(V6, real_c);   // Real feel


  tb.sendTelemetryFloat("temperature", temp_c);
  tb.sendTelemetryFloat("humidity", humi);
  tb.sendTelemetryFloat("light", light_lux);
  tb.sendTelemetryFloat("real_feel", real_c);
}


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


void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer,TOKEN, 1883,"esp8266-th-client",NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
