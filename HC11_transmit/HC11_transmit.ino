void setup() {
Serial.begin(9600);
}

void loop() {
  Serial.print("G1:");
  Serial.print(analogRead(A0));
  Serial.print(" G2:");
  Serial.println(analogRead(A1));
  delay(2000);
}
