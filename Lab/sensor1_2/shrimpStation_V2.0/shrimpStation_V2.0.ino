// shrimp.kps@gmail.com pw:gump2519
// blynk pw : shrimp2565
// firebase aut :KC9R1cvYBLh8qzLAaG12WtK5xgU2ctThJ0XJJ5sq
// https://shrimp-kps-ae987-default-rtdb.asia-southeast1.firebasedatabase.app/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
char auth[] = "y5ssiljQHUB7NEXcNQC2q4zagzjnNKki";
//char ssid[] = "Tump";
//char pass[] = "11223344";
char ssid[] = "TOT_ARM";
char pass[] = "034352523";
//char ssid[] = "ENG@kps4G";
//char pass[] = "engkps2022";
//char ssid[] = "SINGHA_2G";
//char pass[] = "singha606";

#include "Arduino.h"

//#include <Firebase_ESP_Client.h>
//#include <FirebaseArduino.h>
#include <FirebaseESP8266.h>
#include <time.h>
#define FIREBASE_HOST "https://shrimp-kps-ae987-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "KC9R1cvYBLh8qzLAaG12WtK5xgU2ctThJ0XJJ5sq"
FirebaseData firebaseData;
FirebaseJson json;

#include <U8g2lib.h>
#include <avr/pgmspace.h>
//#define GRAPH_BAR
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R2, 12, 13, 15, U8X8_PIN_NONE);

String inputString = "";
char buf[30];
float DO_val;
float pH_val;
float Temp_val;
float Bat_val;
float Wifi_val;

int inp=0 ;

//#define MAX_%DO 200
//#define MIN_%DO 0
#define MAX_DO 20
#define MIN_DO 0
#define MAX_pH 14
#define MIN_pH 0
#define MAX_TEMP 50
#define MIN_TEMP 0
#define MAX_BATT 14
#define MIN_BATT 0
#define NUM_VAL (16)

int DOTab[NUM_VAL];
int pHTab[NUM_VAL];
int TempTab[NUM_VAL];
int BattTab[NUM_VAL];
int i,ght=31-7;
int x=0;

static unsigned char drawwifi[]={  0, 0, 0, 0, 0 };
static unsigned char wifi3[] = {  0x07, 0x08, 0x13, 0x14, 0x15 };
static unsigned char wifi2[] = {  0x00, 0x00, 0x03, 0x04, 0x05 };
static unsigned char wifi1[] = {  0x00, 0x00, 0x00, 0x00, 0x01 };

static unsigned char drawbatt[]={  0, 0, 0, 0 };
static unsigned char batt0[] = {  0x3F, 0x61, 0x61, 0x3F };
static unsigned char batt1[] = {  0x3F, 0x63, 0x63, 0x3F };
static unsigned char batt2[] = {  0x3F, 0x67, 0x67, 0x3F };
static unsigned char batt3[] = {  0x3F, 0x6F, 0x6F, 0x3F };
static unsigned char batt4[] = {  0x3F, 0x7F, 0x7F, 0x3F };

char ntp_server1[20] = "pool.ntp.org";
char ntp_server2[20] = "clock.nectec.or.th";
char ntp_server3[20] = "time.uni.net.th";

boolean backlight = true;

int menuitem = 1;
int page = 1;
volatile boolean up = false;
volatile boolean down = false;
volatile boolean enter = false;
int downButtonState = 0;
int upButtonState = 0;  
int enterButtonState = 0;          
int lastDownButtonState = 0;
int lastEnterButtonState = 0;
int lastUpButtonState = 0;
#define enterPin 10 //S3
#define downPin 5
#define upPin 4
#define backlightPin 14

void drawMenu()
{
  downButtonState = digitalRead(downPin);
  enterButtonState = digitalRead(enterPin);
  upButtonState =   digitalRead(upPin);

  checkButton();
      
  if (page==1) 
  { 
    u8g2.drawHLine(0,8,127);
    int rssi = WiFi.RSSI();
    if(rssi > -40) rssi = -40;
    if(rssi < -90) rssi = -90;
    int WifiSignal = map(rssi, -40, -90, 100, 0);
    //Serial.println(WifiSignal);
    if(0<=WifiSignal && WifiSignal<=33) u8g2.drawXBMP( 110, 0, 5, 5, wifi1);
    if(34<=WifiSignal && WifiSignal<=66) u8g2.drawXBMP( 110, 0, 5, 5, wifi2);
    if(67<=WifiSignal) u8g2.drawXBMP( 110, 0, 5, 5, wifi3);
     
    float Volt = analogRead(A0)*3.3/1023;
    float batt_Volt = 5.1*Volt; //250k/50k
    //Serial.println(batt_Volt);
    if(0 <= batt_Volt && batt_Volt< 10.7) u8g2.drawXBMP( 120, 1, 7, 4, batt0);
    if(10.7<=batt_Volt && batt_Volt<11.7) u8g2.drawXBMP( 120, 1, 7, 4, batt1);
    if(11.7<=batt_Volt && batt_Volt<12.7) u8g2.drawXBMP( 120, 1, 7, 4, batt2);
    if(12.7<=batt_Volt && batt_Volt<13.7) u8g2.drawXBMP( 120, 1, 7, 4, batt3);
    if(13.7<=batt_Volt) u8g2.drawXBMP( 120, 1, 7, 4, batt4);
    
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(40, 7);
    u8g2.print("MAIN MENU");
    u8g2.setFont(u8g2_font_5x7_t_cyrillic);

    if (menuitem==1) 
    { 
      u8g2.drawButtonUTF8(63, 25, U8G2_BTN_HCENTER|U8G2_BTN_INV, 0,  16,  2, "Station" );
    }
    else 
    {
      u8g2.drawButtonUTF8(63, 25, U8G2_BTN_HCENTER|U8G2_BTN_BW2, 0,  14,  1, "Station" );
    }
    if (menuitem==2) 
    { 
      u8g2.drawButtonUTF8(63, 40, U8G2_BTN_HCENTER|U8G2_BTN_INV, 0,  14,  2, " Pond#1 " );
    }
    else 
    {
      u8g2.drawButtonUTF8(63, 40, U8G2_BTN_HCENTER|U8G2_BTN_BW2, 0,  12,  1, " Pond#1 " );
    }
    if (menuitem==3) 
    { 
      u8g2.drawButtonUTF8(63, 55, U8G2_BTN_HCENTER|U8G2_BTN_INV, 0,  9,  2, " Setting  " );
    }
    else 
    {
      u8g2.drawButtonUTF8(63, 55, U8G2_BTN_HCENTER|U8G2_BTN_BW2, 0,  7,  1, " Setting  " );
    }
  }
  else if (page==2) 
  {
    

  }
 
}

void checkButton(){
 if (downButtonState != lastDownButtonState) //checkIfDownButtonIsPressed
  {
    if (downButtonState == 0) 
    {
      down=true;
    }
    delay(50);
  }
   lastDownButtonState = downButtonState; 
 
 if (upButtonState != lastUpButtonState)  //checkIfUpButtonIsPressed
  {
    if (upButtonState == 0) {
      up=true;
    }
    delay(50);
  }
   lastUpButtonState = upButtonState;
  
 if (enterButtonState != lastEnterButtonState) //checkIfEnterButtonIsPressed
  {
    if (enterButtonState == 0) {
      enter=true;
    }
    delay(50);
  }
   lastEnterButtonState = enterButtonState;
  
  
  
  if (up && page == 1 ) {
    up = false;
    menuitem--;
    if (menuitem==0)
    {
      menuitem=3;
    }      
  }//else if (up && page == 2 ) {
  //  up = false;
  //  contrast--;
  //  setContrast();
 // }


  if (down && page == 1) {
    down = false;
    menuitem++;
    if (menuitem==4) 
    {
      menuitem=1;
    }      
  }//else if (down && page == 2 ) {
  //  down = false;
  //  contrast++;
  //  setContrast();
 // }


  if (enter==1) {
    enter = false;
    
    if (page == 1 && menuitem==2) 
    {        
      Serial.print("2");
      if (backlight) 
      {
        backlight = false;

       // turnBacklightOff();
        }
      else 
      {
        backlight = true; 
      //  turnBacklightOn();
       }
    }

    if(page == 1 && menuitem ==3)
    {
     // resetDefaults();
    }


else if (page == 1 && menuitem==1) {
      page=2;
     }
else if (page == 2) {
      page=1;
     }
   }
   
}


String NowString() {
  int getcount = 1;
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);
  String myyear = String(newtime->tm_year + 1900);
  
  while (myyear == "1970" && getcount <= 4) {
    
    time_t now = time(nullptr);
    struct tm* newtime = localtime(&now);
    myyear = String(newtime->tm_year + 1900);
    delay(50);
   // vTaskDelay(100 / portTICK_PERIOD_MS);
    
    getcount++;
  }

  String tmpNow = "";
  tmpNow += String(newtime->tm_year + 1900);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mon + 1);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mday);
  tmpNow += " ";
  tmpNow += String(newtime->tm_hour);
  tmpNow += ":";
  tmpNow += String(newtime->tm_min);

  return tmpNow;
}



void readFilebase() 
{
  if (Firebase.ready()) {
   
        Firebase.getFloat(firebaseData,"Senser/Pound1/DO");
        DO_val=firebaseData.floatData(); 
        if(DO_val<0 || DO_val>20)DO_val=0;       
        Serial.println(DO_val);
        //inp=3;
        Firebase.getFloat(firebaseData,"Senser/Pound1/pH");
        pH_val=firebaseData.floatData();
        if(pH_val<0 || pH_val>14)pH_val=0;   
        Serial.println(pH_val);
         // inp=2;
        Firebase.getFloat(firebaseData,"Senser/Pound1/Temperature");
        Temp_val=firebaseData.floatData();
        if(Temp_val<0 || Temp_val>50)Temp_val=0;
        Serial.println(Temp_val);
         // inp=1;
        Firebase.getFloat(firebaseData,"Senser/Pound1/BattVolt");
        Bat_val=firebaseData.floatData();
        if(Bat_val<0 || Bat_val>14)Bat_val=0;
        Serial.println(Bat_val);

        Firebase.getFloat(firebaseData,"Senser/Pound1/wifi");
        Wifi_val=firebaseData.floatData();
        if(Wifi_val<0 || Bat_val>100)Wifi_val=0;
        Serial.println(Wifi_val);
 
      //return 1;
    }
 // return 0;
}

void addDO()
{
  for(i=0;i<NUM_VAL-1;i++) DOTab[i]=DOTab[i+1];
  int t=DO_val;
  if(t<MIN_DO) t=MIN_DO;
  if(t>MAX_DO) t=MAX_DO;
  DOTab[NUM_VAL-1] = (long)(t)*ght/(MAX_DO+1);
}

void addpH()
{
  for(i=0;i<NUM_VAL-1;i++) pHTab[i]=pHTab[i+1];
  int t=pH_val;
  if(t<MIN_pH) t=MIN_pH;
  if(t>MAX_pH) t=MAX_pH;
  pHTab[NUM_VAL-1] = (long)(t)*ght/(MAX_pH+1);
}

void addTemp()
{
  for(i=0;i<NUM_VAL-1;i++) TempTab[i]=TempTab[i+1];
  int t=Temp_val;
  if(t<MIN_TEMP) t=MIN_TEMP;
  if(t>MAX_TEMP) t=MAX_TEMP;
  TempTab[NUM_VAL-1] = (long)(t-MIN_TEMP)*ght/(MAX_TEMP-MIN_TEMP+1);
}

void addBatt()
{
  for(i=0;i<NUM_VAL-1;i++) BattTab[i]=BattTab[i+1];
  int t=Bat_val;
  if(t<MIN_BATT) t=MIN_BATT;
  if(t>MAX_BATT) t=MAX_BATT;
  BattTab[NUM_VAL-1] = (long)(t-MIN_BATT)*ght/(MAX_BATT-MIN_BATT+1);
}


void drawValScreen()
{
  //u8g2.drawRFrame(0,7,127,63-7,1);
  u8g2.setFont(u8g2_font_tinytim_tf );
  u8g2.drawStr (0,6,"Water Quality");

  if(Wifi_val>=0 && Wifi_val<33) memcpy(drawwifi , wifi1,sizeof(wifi1));
  else if(Wifi_val>=33 && Wifi_val<66) memcpy(drawwifi , wifi2,sizeof(wifi2));
  else if(Wifi_val>=66 && Wifi_val<101) memcpy(drawwifi , wifi3,sizeof(wifi3));
  u8g2.drawXBMP( 105, 0, 5, 5, drawwifi);
  
  if(Bat_val>=10.7 && Bat_val<11.4) memcpy(drawbatt ,batt1,sizeof(batt1));
  else if(Bat_val>=11.4 && Bat_val<12.1) memcpy(drawbatt ,batt2,sizeof(batt2));
  else if(Bat_val>=12.1 && Bat_val<12.8) memcpy(drawbatt ,batt3,sizeof(batt3));
  else if(Bat_val>=12.8) memcpy(drawbatt ,batt4,sizeof(batt4));
  u8g2.drawXBMP( 114, 1, 7, 4, drawbatt);
    
  u8g2.drawHLine(3,7,57);
  u8g2.drawBox(36,7,24,6);
  
  u8g2.drawHLine(63+3,7,57);
  u8g2.drawBox(36+63,7,24,6);
  
  u8g2.drawHLine(3,37,57);
  u8g2.drawBox(36,37,24,6);
  
  u8g2.drawHLine(63+3,37,57);  
  u8g2.drawBox(36+63,37,24,6); 
   
  u8g2.setDrawColor(0);   
  u8g2.setFont(u8g2_font_u8glib_4_tf); 
  u8g2.drawStr (37,7+5,"   DO");
  u8g2.drawStr (37+64,7+5,"   pH");
  u8g2.drawStr (37,37+5," TEMP");
  u8g2.drawStr (37+64,37+5," BATT"); 
   
  u8g2.setFont(u8g2_font_courB08_tf);
  u8g2.setDrawColor(1);  
  u8g2.drawStr(0+37,7+14,"mg/L");
  u8g2.drawStr(63+40,7+14,"pH");
  u8g2.drawUTF8(0+41,7+26+5+14,"° ");  
  u8g2.drawStr(0+46,7+26+5+14,"C");
  u8g2.drawStr(63+37,7+26+5+14,"Volt");

   
   drawValue(0,7,DO_val,MAX_DO,MIN_DO);
   drawValue(63,7,pH_val,MAX_pH,MIN_pH);
   drawValue(0,7+26+5,Temp_val,MAX_TEMP,MIN_TEMP);
   drawValue(63,7+26+5,Bat_val,MAX_BATT,MIN_BATT);  
}
void drawValue(int xg, int yg, float val, float max_val, float min_val)
{
  u8g2.setFont(u8g2_font_ncenB12_tf);



  u8g2.drawFrame(xg+3,yg+26-4,57,4);
  //u8g2.drawHLine(xg+3+56,yg+26-5,1);  
  //u8g2.drawPixel(xg+3,yg+26-5);
  
  u8g2.drawBox(xg+3,yg+26-4,int(val*57/(max_val-min_val)),4);
  
  u8g2.setCursor(xg+3,yg+14);
  u8g2.print(val,1);
  
  u8g2.setFont(u8g2_font_u8glib_4_hr);
  u8g2.setCursor(xg+3,yg+26-5);
  u8g2.print(min_val,0);
  u8g2.setCursor(xg+53,yg+26-5);
  u8g2.print(max_val,0);
}

void drawGscreen()
{
    u8g2.setFont(u8g2_font_trixel_square_tr);
  
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0, 0, 62, 7);
    u8g2.setDrawColor(0);
    u8g2.drawStr (2,6,"O2 :                      mg/L");
    u8g2.setCursor(18,6);
    u8g2.print (DO_val);
    
    u8g2.setDrawColor(1);
    u8g2.drawBox(66, 0, 62, 7);
    u8g2.setDrawColor(0);
    u8g2.drawStr (68,6,"pH:     ");
    u8g2.setCursor(84,6);
    u8g2.print (pH_val);

    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0,32,62,7);
    u8g2.setDrawColor(0);
    u8g2.drawStr (2,38,"Temp :                       °C");
    u8g2.setCursor(25,38);
    u8g2.print (Temp_val);
    u8g2.setDrawColor(1);

    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(66,32,62,7);
    u8g2.setDrawColor(0);
    u8g2.drawStr (68,38,"Batt :                       Volt");
    u8g2.setCursor(89,38);
    u8g2.print (Bat_val);
    u8g2.setDrawColor(1);
    
   drawGraph(0,7,DOTab);
   drawGraph(66,7,pHTab);
   drawGraph(0,39,TempTab);
   drawGraph(66,39,BattTab);   
}
void drawGraph(int xg, int yg, int valTab[])
{
  
  u8g2.drawFrame(xg,yg,62,ght);
#ifdef GRAPH_BAR
  for(i=1;i<NUM_VAL;i++) {
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(1+xg+(i-1)*4, yg+ght-1-valTab[i], 3, valTab[i]);

  }
#else 
  for(i=0;i<NUM_VAL-1;i++) {
    int dy = valTab[i+1]-valTab[i];
    //for(int j=0;j<4;j++) u8g2_DrawVLine(1+xg+i*4+j,yg+ght-1-(valTab[i]+dy*j/4),yg+ght-1,1);
   for(int j=0;j<4;j++) u8g2.drawVLine(1+xg+i*4+j,yg+ght-1-valTab[i]-dy*j/4,valTab[i]+dy*j/4);
  }
#endif
}


void setup() ////////////////////////Setup/////////////////////////////////
{
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(enterPin, INPUT_PULLUP);//enter
  pinMode(downPin, INPUT_PULLUP);//down
  pinMode(upPin, INPUT_PULLUP);//up
  pinMode(backlightPin, OUTPUT);//Turn Backlight
  digitalWrite(backlightPin,HIGH); //Turn Backlight ON

  Blynk.begin(auth, ssid, pass,IPAddress(203,150,37,180), 8080);

  configTime(7 * 3600, 0, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("Waiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Now: " + NowString());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");  

  json.set("Station1/Date", NowString()); 
  Firebase.updateNode(firebaseData,"/Station",json);
 
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);

  u8g2.enableUTF8Print();
}

void loop() 
{
 if(1) 
  {
    readFilebase();
   Serial.println("Start plote");
   u8g2.clearBuffer();
    int xs=68;
   // u8g2.clear();
     addDO();
     addpH();
     addTemp();
     addBatt();
    drawValScreen();
    //drawGscreen();
     u8g2.sendBuffer();
  }

  //if(inp>=3) { delay(1000); inp=0; }
    delay(60000); 

}
