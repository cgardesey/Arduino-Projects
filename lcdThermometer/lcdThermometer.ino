#include<LiquidCrystal.h>

#include "DHT.h"

#define DHTPIN 19

#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(9,10,11,12,13,16);

void setup() {
  lcd.begin(20,4);
  lcd.clear();
  dht.begin();
}

void loop() {
 delay(2000);
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0,0);
  lcd.print("TEMPERATURE:");
  lcd.setCursor(13,0);
  lcd.print(t);
  lcd.setCursor(0,1);
  lcd.print("HUMIDITY:");
  lcd.setCursor(10,1);
  lcd.print(h);
  
}
