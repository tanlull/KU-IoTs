
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <BH1750.h>
#include "DHT.h"

// DHT
#define DHTPIN 14   // D5
#define D1 5  
#define D2 4   
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#define LED_DISP 2 // build in LED 2 or 16

#define ON_OFF_SWITCH 5 

BH1750 lightMeter(0x23); // ADDR -> GND (0x23) , ADDR -> VCC (0x5C) , 

const char* ssid = "Pakhun";
const char* password = "022798129";
bool ledStatus = 0;
String payload = "json data";
int delayTime = 5000; // 5 s



void setup() {
  Serial.begin(115200);
  while (!Serial); // time to get serial running
  attachInterrupt(ON_OFF_SWITCH, switchLED, RISING); 
  startWifi();
  pinMode(LED_DISP, OUTPUT);
  dht.begin();
  Wire.begin(D2,D1); // SCL , SDA 
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
      Serial.println(F("BH1750 Advanced begin"));
    } else {
      Serial.println(F("Error initialising BH1750"));
    }
}

void loop() {
   readDHTData();
   readLux();
   disPlayLED();
   delay(delayTime);
}

void startWifi(){
  WiFi.begin(ssid, password);
  Serial.print("WiFi Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_DISP, LOW);
    delay(250);
    digitalWrite(LED_DISP, HIGH);
    delay(250);
  }
  Serial.println("Connected");

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
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

//  Serial.print(F("Humidity: "));
//  Serial.print(h);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(t);
//  Serial.print(F("°C "));
//  Serial.print(f);
//  Serial.print(F("°F  Heat index: "));
//  Serial.print(hic);
//  Serial.print(F("°C "));
//  Serial.print(hif);
//  Serial.println(F("°F"));
  payload="{\"temperature\":"+String(t)+",\"humidity\":"+String(h)+",\"realfeel\":"+String(hic)+"}";
  Serial.println(payload);
}

void disPlayLED(){
    //Serial.println("LED Blink OFF");
    digitalWrite(LED_DISP, LOW);
    delay(200);
    //Serial.println("LED Blink ON");
    digitalWrite(LED_DISP, HIGH);
    delay(200);
    
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
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  return lux;
}
