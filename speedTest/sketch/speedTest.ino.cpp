#include <Arduino.h>
#line 1
#line 1 "C:\\Users\\Ayatullah\\Documents\\Arduino\\speedTest\\speedTest.ino"
void setup();
void loop();
#line 1
void setup() {
  // put your setup code here, to run once:
pinMode(14, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(14, HIGH);
delay(1000);
digitalWrite(14, LOW);
delay(1000);
}

