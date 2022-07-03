#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>

#define LED_DISP 16 //D0

#define ON_OFF_SWITCH 5 //D1

const char* ssid = "TAN P30 Pro";
const char* password = "1234567890-=";
bool ledStatus = 0;


// Blynk
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLgYgVXgrp"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "eUFlE7qMZSjkl7HEyV3s177OfCUqSP8x"



void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.begin(9600);
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
  // ส่วนของ OTA *****************************************************************
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.setHostname("GROUP5");
  ArduinoOTA.begin();
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
  // put your main code here, to run repeatedly:
   ArduinoOTA.handle();
   Blynk.run();
   //disPlayLED();
}
