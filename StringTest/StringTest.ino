char charVal=69;
int  intVal= 70;
float flaotVal= 69.0;
void setup() {
 Serial.begin(9600);

}

void loop() {
// Serial.println("charVal");
// Serial.println(charVal);
// Serial.println(charVal,BIN);
// Serial.println(charVal,DEC);
// Serial.println(charVal,HEX);

 Serial.println("intVal");
 Serial.print("intVal: ");
 Serial.println(intVal);
 Serial.print("BIN: ");
 Serial.println(intVal,BIN);
 Serial.print("DEC: ");
 Serial.println(intVal,DEC);
 Serial.print("HEX: ");
 Serial.println(intVal,HEX);
 Serial.print("CHAR: ");
 Serial.print((char)intVal);
 delay(1000);

}
