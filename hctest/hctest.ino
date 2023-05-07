#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(8,9,4,5,6,7);
SoftwareSerial mySerial(2, 3);
SoftwareSerial hc(11, 12);


const byte button= 10;
String instring;
void setup() {
hc.begin(9600);
lcd.begin(16,2);
pinMode(button, INPUT_PULLUP);

}

void loop() {
 if(!digitalRead(button))
 {
    hc.print("hc test");
    lcd.print("hc test");
    delay(500);
    lcd.clear();
    
 }

 if(hc.available()>0)
 {
    instring = hc.readString();
    lcd.clear();
    instring.trim();
    lcd.print(instring);
 }
 

}
