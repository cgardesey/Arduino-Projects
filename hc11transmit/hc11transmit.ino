#include <SoftwareSerial.h> 
SoftwareSerial HC11(7,8);

void setup() {
Serial.begin(9600);
HC11.begin(9600);

}

void loop() {
  
HC11.println("hello");
delay(1000);
}
