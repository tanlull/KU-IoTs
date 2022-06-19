//#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5 // D1 = 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
//ขา i2c จอ LCD
//#define SDA_PIN D4
//#define SCL_PIN D5

//LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address อาจแตกต่างกันไป
//byte Temp[] = {  //https://maxpromer.github.io/LCD-Character-Creator/
//  B00100,
//  B01010,
//  B01010,
//  B01110,
//  B01110,
//  B11111,
//  B11111,
//  B01110
//};
void setup(void) {
  Serial.begin(9600);
 // Wire.begin(SDA_PIN, SCL_PIN);
  
//  lcd.init();
//  lcd.backlight();
//  lcd.setCursor(2, 0);
//  lcd.print("Temperature");
//  lcd.setCursor(3, 1);
//  lcd.print("DS18B20");
//  delay(2000);
//  lcd.clear();
//  lcd.createChar(0, Temp);
//  lcd.setCursor(0, 0);
//  lcd.write(0);
//  lcd.setCursor(1, 0);
//  lcd.print("Temp:");
//  lcd.setCursor(14, 0);
//  lcd.print(char(B11011111));
//  lcd.print("C");
  
  Serial.println("Temperature IC");
  DS18B20.begin();
}

void loop(void) {
  Serial.println("Requesting temperatures...");
  DS18B20.requestTemperatures();
  Serial.print("Temperature is: ");
  float t=DS18B20.getTempCByIndex(0);
  Serial.print(t); //อุณหภูมิ 
  Serial.println(" *C");

//  lcd.setCursor(7, 0);
//  lcd.print(t);
  delay(1000);
}
