
int prevVal, newVal, prevDiff, newDiff;
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  prevVal = newVal = analogRead(A0);
  Serial.println(prevVal);
}

void loop() {
  prevVal = analogRead(A0);
  prevDiff = abs(newVal - prevVal);
  delay(2000);
  newVal = analogRead(A0);
  newDiff = abs(newVal - prevVal);
 if(abs(newDiff - prevDiff) > 0)
 {
  Serial.println(newVal);
 }
}
