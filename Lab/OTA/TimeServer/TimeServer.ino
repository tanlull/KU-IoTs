#include <ESP8266WiFi.h>


const char* ssid = "TOT_ARM";
const char* password = "034352523";

char ntp_server1[20] = "pool.ntp.org";
char ntp_server2[20] = "clock.nectec.or.th";
char ntp_server3[20] = "time.uni.net.th";

String NowString() {

  int getcount = 1;
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);
  String myyear = String(newtime->tm_year + 1900);
  
  while (myyear == "1970" && getcount <= 4) {
    
    time_t now = time(nullptr);
    struct tm* newtime = localtime(&now);
    myyear = String(newtime->tm_year + 1900);
    //vTaskDelay(100 / portTICK_PERIOD_MS);
    
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


void setup() {
  Serial.begin(9600);
  while (!Serial); // time to get serial running
  
  WiFi.begin(ssid, password);
  Serial.print("WiFi Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
  Serial.println("Connected");

  configTime(7 * 3600, 0, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("Waiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(500);

  Serial.println();
  Serial.println("Now: " + NowString());
  }

}

void loop() {
 Serial.println("Now: " + NowString());
 delay(1000);
}
