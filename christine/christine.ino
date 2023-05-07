#include <SoftwareSerial.h>

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
int value= analogRead(A0);
Serial.println(value);
if(value>655)
{
Serial.println("diper is saturated");
delay(500);
}

}
