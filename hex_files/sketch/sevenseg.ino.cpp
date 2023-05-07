#include <Arduino.h>
#line 1
#line 1 "C:\\Users\\Cyril\\Documents\\Arduino\\sevenseg\\sevenseg.ino"
#include <RTClib.h>
#include <Wire.h>
#include <SevenSeg.h>

SevenSeg sev(2,3,4,5,6,7,8);

RTC_DS1307 RTC;
static DateTime now;

const int digits = 4;
int digitPins[digits]={A0,A1,A2,A3};

int time;

void setup();
void loop();
#line 15
void setup()
{
  Wire.begin();
  RTC.begin();
  sev.setDigitPins (digits , digitPins );
  sev.setCommonCathode();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop() 
{
  now = RTC.now();
  time = now.minute() / 10 * 1000 + (now.minute() % 10) * 100 + now.second() / 10 * 10 + now.second() % 10;
  sev.write(time);
    //sev.write(1234); 
}


