void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print('H');
  delay(3000);
  Serial.print('L');
  delay(3000);
}
