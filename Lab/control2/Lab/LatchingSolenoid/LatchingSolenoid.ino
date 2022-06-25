#define relay11 5
#define relay12 4
void setup() {
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);
  digitalWrite(relay11,HIGH);
  digitalWrite(relay12,HIGH);
}

void loop() {
  digitalWrite(relay11,LOW);
  delay(20);
  digitalWrite(relay11,HIGH);
  delay(3000);
  digitalWrite(relay12,LOW);
  delay(20);
  digitalWrite(relay12,HIGH);
  delay(3000);
}
