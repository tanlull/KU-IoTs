#include <PZEM004Tv30.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char ssid[] = "TOT_ARM";            
char pass[] = "xxxxxxxxx";            
char server[] = "blynk.iot-cm.com"; //Blynk local
int port = 8080;
#define AUTH "your AUTH"
float voltage;
float current;
float power;
float energy;
float frequency;
float pf;


/* Use software serial for the PZEM
 * Pin 5 Rx (Connects to the Tx pin on the PZEM)
 * Pin 4 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(5, 4);
BlynkTimer timer;

void updateBlynk() {
  Blynk.virtualWrite(V1,voltage);
  Blynk.virtualWrite(V2,current);
  Blynk.virtualWrite(V3,power);
  Blynk.virtualWrite(V4,frequency);
  Blynk.virtualWrite(V5,pf);
  Blynk.virtualWrite(V6,energy);
  Blynk.virtualWrite(V7,energy*4.5);
}

BLYNK_WRITE(V10)
{
  int buttonVal=param.asInt();
  if(buttonVal==HIGH)
  {
    pzem.resetEnergy();
    Blynk.virtualWrite(V6,0);
    Blynk.virtualWrite(V7,0);
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(AUTH, ssid , pass, server, port);
  timer.setInterval(5000L, updateBlynk);
}

void loop() {
    Blynk.run();
    timer.run();
    voltage = pzem.voltage();
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    current = pzem.current();
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    power = pzem.power();
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }

    energy = pzem.energy();
    if( !isnan(energy) ){
        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
    } else {
        Serial.println("Error reading energy");
    }

    frequency = pzem.frequency();
    if( !isnan(frequency) ){
        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
    } else {
        Serial.println("Error reading frequency");
    }

    pf = pzem.pf();
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();
    delay(2000);
}
