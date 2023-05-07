#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int RedLed = 7;
int YeLLed = 8;
int GreenLed=9;


void setup() {
  pinMode(RedLed, OUTPUT);
  pinMode(YeLLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("lcd test");
}

void loop() {
digitalWrite(RedLed, 1);
digitalWrite(YeLLed, 1);
digitalWrite(GreenLed, 1);
delay(1000);
digitalWrite(RedLed, 0);
digitalWrite(YeLLed, 0);
digitalWrite(GreenLed, 0);
delay(1000);
}
