#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,1,2,3,4);
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
    lcd.print("imtrusion detected");
}
