#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>
#include <Wire.h>
#include <BH1750.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLgYgVXgrp"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "eUFlE7qMZSjkl7HEyV3s177OfCUqSP8x"

#define WIFI_AP "Pakhun"
#define WIFI_PASSWORD "022798129"

//thingsboard Token
#define TOKEN "X5OCQ5XI8qk4IhaEbcqh"

//const char* WIFI_AP1 = "Pakhun";
//const char* WIFI_PASSWORD1 = "022798129";


BlynkTimer timer;
BlynkTimer timer2;
long readDataTime = 5000; // 50 s
long ledBlinkTime = 2000; // 50 s

// DHT
#define DHTPIN 14
#define D1 5  
#define D2 4  
#define DHTTYPE DHT22
#define LED_DISP 2 // build in LED 2 or 16
char thingsboardServer[] = "totws.com";
BH1750 lightMeter(0x23); // ADDR -> GND (0x23) , ADDR -> VCC (0x5C) , 
WiFiClient wifiClient;
long i=0;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

ThingsBoard tb(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  
  dht.begin();
  pinMode(LED_DISP, OUTPUT);
  delay(10);
  InitWiFi();
  lastSend = 0;
  Wire.begin(D2,D1); // SDA, SCL  
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
      Serial.println(F("BH1750 Advanced begin"));
    } else {
      Serial.println(F("Error initialising BH1750"));
    }

  timer.setInterval(readDataTime, getSendTempHumiLightData); 
  timer2.setInterval(ledBlinkTime, blinkLED); 
  //Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_AP1, WIFI_PASSWORD1);
}

void loop()
{
  if ( !tb.connected() ) {
    reconnect();
  }
  
  tb.loop();
  //Blynk.run();
   timer.run(); 
   timer2.run(); 

}

void update2BLYNK(float temp_c,float humi,float light_lux) 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V1, temp_c);   // Write temp
  Blynk.virtualWrite(V4, humi);   // Write humidity
  Blynk.virtualWrite(V5, light_lux);   // Write Light
}

void getSendTempHumiLightData()
{
  //Serial.println("Collecting temperature data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float light = readLux(); 
  i+=1;
  //Serial.println("Sending data to ThingsBoard:");
  Serial.print("id: ");
  Serial.print(i);
  Serial.print(", ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, ");
  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" Lux");

  // Thingsboard
  tb.sendTelemetryFloat("temperature", temperature);
  tb.sendTelemetryFloat("humidity", humidity);
  tb.sendTelemetryFloat("light", light);

  // Blynk
  update2BLYNK(temperature,humidity,light);
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
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

float readLux(){
  float lux = lightMeter.readLightLevel();
//  Serial.print("Light: ");
//  Serial.print(lux);
//  Serial.println(" lx");
  return lux;
}
