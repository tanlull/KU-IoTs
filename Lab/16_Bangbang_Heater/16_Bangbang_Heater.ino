#include "DHT.h"

 #define DHTPIN 5   //D1  
// Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
int RelayPin = 4; //D2
int RelayStatus=0;
float targetTemp = 32;
float bangOn=0.2;
float bangOff=0.2;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(RelayPin, OUTPUT);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.

  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    float hif = dht.computeHeatIndex(f, h);
//   Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  if (t < (targetTemp-bangOn)){ 
    digitalWrite(RelayPin,HIGH);
    Serial.println("Turn On Realy");
  delay(1000);

//   digitalWrite(RelayPin,LOW);
//     delay(1000);
 // RelayStatus=1;
  } 
  if(t > (targetTemp+bangOff)){
    digitalWrite(RelayPin,LOW);
    Serial.println("Turn Off Realy");
    RelayStatus=0;
    delay(1000);
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(",");
  Serial.println(RelayStatus);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
 
}
