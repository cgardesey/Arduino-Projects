#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);
RTC_DS1307 rtc;

void setup () {
  Serial.begin(57600);
  lcd.begin(20,4);
  lcd.clear();
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); 
#endif
  rtc.begin();
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}


void loop() {
     DateTime now = rtc.now();
    
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(' ');
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();
    lcd.setCursor(2,0);
    lcd.print(now.hour());
    //lcd.setCursor(4,0);
    lcd.print(":");
    //lcd.setCursor(5,0);
    lcd.print(printDigits(now.minute()));
    //lcd.setCursor(7,0);
    lcd.print(":");
    //lcd.setCursor(8,0);
    lcd.print(printDigits(now.second()));
//    if(now.second()==59)
//    {
//      lcd.setCursor(8,0);
//      lcd.print("00");
//    }
    delay(1000);
}


String printDigits(byte digits)
{
  if (digits < 10)
  {
    return ('0' + String(digits));
  }
  else
  {
    return String(digits);
  }
}
