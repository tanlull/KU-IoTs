#define relay11 5
#define relay12 4
void setup() {
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);
  digitalWrite(relay11,HIGH);
  digitalWrite(relay12,HIGH);
}

void anti_cw(int tdelay){
  digitalWrite(relay11,HIGH);
  digitalWrite(relay12,LOW);
  delay(tdelay);
}

void cw(int tdelay){
  digitalWrite(relay11,LOW);
  digitalWrite(relay12,HIGH);
  delay(tdelay);
}

void turnoff(int tdelay){
  digitalWrite(relay11,LOW);
  digitalWrite(relay12,LOW);
  delay(tdelay);
}

void loop() {

  cw(3000);
  turnoff(500);
  anti_cw(3000);
  turnoff(500);


}
