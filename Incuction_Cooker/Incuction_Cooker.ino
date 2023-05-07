#include <Timer.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const uint8_t low_power  = 8;
const uint8_t medium_power  = 9;
const uint8_t high_power  = 10;
const uint8_t sqw_wave  = 12;

bool low = false;
bool medium = false;
bool high = false;


void setup() {
  pinMode(low_power,INPUT_PULLUP);
  pinMode(medium_power,INPUT_PULLUP);
  pinMode(high_power,INPUT_PULLUP);
  pinMode(sqw_wave, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(" Power FRQ(KHz)");
}

void loop()
{
if(!digitalRead(low_power))
  {
    low = true;
    medium = false;
    high = false;
  }
  if (!digitalRead(medium_power))
  {
    low = false;
    medium = true;
    high = false;
  }
  if (!digitalRead(high_power))
  {
    low = false;
    medium = false;
    high = true;
  }

  
  if(low)
  {
	  tone(sqw_wave, 30000);
	  lcd.setCursor(0, 1);
	  lcd.print("  LOW      30");
  }
  if (medium)
  {
	  tone(sqw_wave, 45000);
	  lcd.setCursor(0, 1);
	  lcd.print("MEDIUM     45");
  }
  if (high)
  {
	  tone(sqw_wave, 45000);
	  lcd.setCursor(0, 1);
	  lcd.print(" HIGH      65");
  }
}
