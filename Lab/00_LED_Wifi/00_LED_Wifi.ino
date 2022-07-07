
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#define LED_DISP 2 // build in LED 2 or 16

#define ON_OFF_SWITCH 5 

const char* ssid = "Pakhun";
const char* password = "022798129";
bool ledStatus = 0;




void setup() {
  Serial.begin(115200);
  while (!Serial); // time to get serial running
  attachInterrupt(ON_OFF_SWITCH, switchLED, RISING); 
  WiFi.begin(ssid, password);
  pinMode(LED_DISP, OUTPUT);
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
  // จบ OTA *****************************************************************
}

void disPlayLED(){
    Serial.println("LED Blink OFF");
    digitalWrite(LED_DISP, LOW);
    delay(500);
    Serial.println("LED Blink ON");
    digitalWrite(LED_DISP, HIGH);
    delay(500);
    
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

void loop() {

   disPlayLED();
}
