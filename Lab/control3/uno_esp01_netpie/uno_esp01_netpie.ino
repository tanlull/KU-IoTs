//ดัดแปลงจากที่ใช้ nodemcu เป็น arduino uno + esp-01
#include "SoftwareSerial.h"
#include "WiFiEsp.h" //***สำหรับ uno + esp-01
//#include <ESP8266WiFi.h> //ของ nodemcu
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 7
#define DHTTYPE DHT11
#define LED1 16
SoftwareSerial Serial1(2, 3); // RX, TX , mega:10,11  uno:2,3 หรือ 8,9
//Not all pins on the Mega and Mega 2560 support change interrupts
//so only the following can be used for RX: 10, 11, 12, 13, 14, 15,
//50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), 
//A13 (67), A14 (68), A15 (69).
// The circuit: * RX is digital pin 10 (connect to TX of other device)
//              * TX is digital pin 11 (connect to RX of other device)
const char* ssid = "Tump_2.4G";
const char* password = "Tump2519";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "a03eafcf-b2d3-4cd9-b2c0-ddb863bd44ae";
const char* mqtt_username = "y7462Vn6YuU481JQhVuLYhMcegQEwiNa";
const char* mqtt_password = "VuFG-GWeoMfQ6aQJigX~IDOy1~8S_nax";

WiFiEspClient espClient;//****ก็คือ WiFiClient ของ nodemcu
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
char msg[50];

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection…");
        if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
            Serial.println("connected");
            client.subscribe("@msg/led");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("try again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char)payload[i];
    }
    Serial.println(message);
    if(String(topic) == "@msg/led") {
        if(message == "on"){
            digitalWrite(LED1,0);
            client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
            Serial.println("LED on");
        }
        else if (message == "off"){
            digitalWrite(LED1,1);
            client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
            Serial.println("LED off");
        }
    }
}

void setup() {
    pinMode(LED1,OUTPUT);
    Serial.begin(9600);
    Serial1.begin(9600);
    WiFi.init(&Serial1); //***เพิ่มมากรณีใช้ uno + esp-01
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    dht.begin();
}

void loop() {
    int humidity = random(0,100);//dht.readHumidity();
    int temperature = random(0,100);//dht.readTemperature();
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    String data = "{\"data\": {\"humidity\":" + String(humidity) + ", \"temperature\":" + String(temperature) + "}}";
    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);
    delay(2000);
}
