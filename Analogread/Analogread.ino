#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
double val;
char str_temp[6];

void setup() {
  Serial.begin(9600);   
  lcd.begin(16, 2);

  lcd.print("ELECTRONIC PRESS");
  lcd.setCursor(0, 1);
  lcd.print("     GAUGE     ");
  delay(2000);
  lcd.clear();

  lcd.print("    GROUP     ");
  lcd.setCursor(0, 1);
  lcd.print("     4B      ");
  delay(2000);
  lcd.clear();

  lcd.print(" PRESSURE(PsiG)");
}

void loop() {
  lcd.setCursor(5, 1);
  //lcd.print((map(analogRead(A0), 0, 1023, 0.5, 4.5) - 0.5) / 4 * 1000);
  lcd.print(dtostrf(((float(analogRead(A0) / 1023.0 * 5.0)) - 0.5) * 250.0, 4, 2, str_temp));
  delay(1000);
}
