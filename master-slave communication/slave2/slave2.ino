#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 6);
const int ledPin = 13;
int inchar;

void setup() {
  lcd.begin(20, 4);
  lcd.clear();
  Serial.begin(9600);
  Serial.print('H');
}

void loop() 
{
  if(Serial.available() > 0)
  {
    inchar = Serial.read();
    if(inchar == 'H')
    {
    lcd.print("H");
    delay(2000);
    Serial.print('H');
    }
  }
}
