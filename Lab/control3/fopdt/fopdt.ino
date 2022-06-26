//First-Order-Plus-Time-Delay model
#include "DHT.h"

#define DHTPIN 5   //D1  
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
int RelayPin = 4; //D2
int RelayStatus=0;
float targetTemp = 32;
int i=0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  //Serial.println(F("DHTxx test!"));
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin,HIGH);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }  
  if(i>10){
  digitalWrite(RelayPin,LOW);
  RelayStatus=1;
  }
  Serial.print(t);
  Serial.print(",");
  Serial.println(RelayStatus);
  i++;
}
