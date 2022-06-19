
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <LCD_I2C.h>
#include <SimpleKalmanFilter.h>

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();
LCD_I2C lcd(0x27 , 16, 4); // Default address of most PCF8574 modules, change according

// PH
float calibration = 21.40; //change this value to calibrate
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;


SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.3);

uint8_t degree[8] =
{
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
};


byte termometru[8] = //icon for termometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte picatura[8] = //icon for water droplet
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};


float phRead(){
  for(int i=0;i<10;i++) 
   { 
   buf[i]=analogRead(analogInPin);
   delay(30);
   }
   for(int i=0;i<9;i++)
   {
   for(int j=i+1;j<10;j++)
   {
   if(buf[i]>buf[j])
   {
   temp=buf[i];
   buf[i]=buf[j];
   buf[j]=temp;
   }
   }
   }
   avgValue=0;
   for(int i=2;i<8;i++)
   avgValue+=buf[i];
   float pHVol=(float)avgValue*5.0/1024/6;
   //Serial.print("Volt = ");
   //Serial.println(pHVol);
   
   float phValue = -5.70 * pHVol + calibration;
   float estimated_ph = simpleKalmanFilter.updateEstimate(phValue);
   //Serial.print("sensor = ");
   //Serial.println(phValue);
   Serial.print(estimated_ph); Serial.print("\t");
   Serial.print(phValue); Serial.println("\t");
   //Serial.print("\r\n");
   delay(20);
   return phValue;
}

void setup() {
  
  Serial.begin(9600);
  Serial.println("SHT31 test1");
  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens
 
  lcd.begin(); 
  lcd.backlight();


  
  Serial.println("SHT31 test2");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

    lcd.createChar(0, degree);
    lcd.createChar(1, termometru); // termometer
    lcd.createChar(2, picatura);
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }

  // PH
  float phValueRead = phRead();
  
  delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Temp ="); 
    lcd.setCursor(7, 0);
    lcd.print(t);
    lcd.setCursor(12, 0);
    lcd.write(0);
    lcd.setCursor(13, 0);
    lcd.print("C");
    lcd.setCursor(15, 0);
    lcd.write(1);
    lcd.setCursor(0, 1);
    lcd.print("Hum. ="); 
    lcd.setCursor(7, 1); 
    lcd.print(h);
    lcd.setCursor(12, 1); 
    lcd.print(" %"); 
    lcd.setCursor(15, 1);
    lcd.write(2);

    // PH Display
    lcd.setCursor(0, 2);
    lcd.print("PH.  ="); 
    lcd.setCursor(7, 2); 
    lcd.print(phValueRead);
    lcd.setCursor(12, 2); 
    lcd.print(" pH"); 
    
    delay(1000); 
//    lcd.backlight();
//    lcd.clear();
}
