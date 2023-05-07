void setup() {
  Serial.begin(9600);

}

void loop() {
  int val = analogRead(A4);
Serial.println(val);
delay(1000);
}
