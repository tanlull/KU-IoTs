
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <LCD_I2C.h>

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();
LCD_I2C lcd(0x27 , 16, 2); // Default address of most PCF8574 modules, change according

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

void setup() {
  
  Serial.begin(9600);
  Serial.println("SHT31 test1");
//  while (!Serial)
//    delay(10);     // will pause Zero, Leonardo, etc until serial console opens
// 
  lcd.begin(); 
  lcd.backlight();


//  
//  Serial.println("SHT31 test2");
//  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
//    Serial.println("Couldn't find SHT31");
//    while (1) delay(1);
//  }

    lcd.createChar(0, degree);
}


void loop() {
//  float t = sht31.readTemperature();
//  float h = sht31.readHumidity();

//  if (! isnan(t)) {  // check if 'is not a number'
//    Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
//  } else { 
//    Serial.println("Failed to read temperature");
//  }
//  
//  if (! isnan(h)) {  // check if 'is not a number'
//    Serial.print("Hum. % = "); Serial.println(h);
//  } else { 
//    Serial.println("Failed to read humidity");
//  }
  //delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Temp ="); 
//    lcd.setCursor(7, 0);
//    lcd.print(t);
//    lcd.setCursor(12, 0);
//    lcd.write(0);
//    lcd.setCursor(13, 0);
//    lcd.print("C");
//    lcd.setCursor(0, 1);
//    lcd.print("Hum. ="); 
//    lcd.setCursor(7, 1); 
//    lcd.print(h);
//    lcd.setCursor(12, 1); 
//    lcd.print(" %"); 
//    delay(1000); 
    //lcd.backlight();
    //lcd.clear();
}
