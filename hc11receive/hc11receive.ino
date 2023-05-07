#include <SoftwareSerial.h> 
SoftwareSerial HC11(7,8);
char incoming_char=0;

void setup() {
Serial.begin(9600);
HC11.begin(9600);

}

void loop() {
  
 if(HC11.available() >0)
  {
    incoming_char=HC11.read(); //Get the character from the cellular serial port.
    Serial.print(incoming_char); //Print the incoming character to the terminal.
  }
}
