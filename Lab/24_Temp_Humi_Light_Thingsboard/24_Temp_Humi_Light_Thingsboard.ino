#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>
#include <Wire.h>
#include <BH1750.h>
#include <BlynkSimpleEsp8266.h>

#define WIFI_AP "Pakhun"
#define WIFI_PASSWORD "022798129"
#define TOKEN "X5OCQ5XI8qk4IhaEbcqh"
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
}

void loop()
{
  if ( !tb.connected() ) {
    reconnect();
  }
  tb.loop();
   timer.run(); 
   timer2.run(); 


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
  Serial.print("\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C \t");
  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" Lux ");

  tb.sendTelemetryFloat("temperature", temperature);
  tb.sendTelemetryFloat("humidity", humidity);
  tb.sendTelemetryFloat("light", light);
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
