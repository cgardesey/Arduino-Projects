#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 2    
#define DHTTYPE DHT22 
const byte heater1 = 4;
const byte heaterFan = 5;
const byte waterlevel = 3;
const byte heater2 = 7;
const byte humidifier = 17;
const byte humidFan = 6;
const byte exhaustFan = 9;
const byte coolingFan = 10;
const byte buzzer = 16;
float temperature, humidity;
LiquidCrystal lcd(8,11,12,13,14,15);
DHT dht(DHTPIN, DHTTYPE);


void setup() {
 Serial.begin(9600);
 lcd.begin(16,2);
 dht.begin();
 pinMode(waterlevel, INPUT_PULLUP);
 pinMode(buzzer,OUTPUT);
 pinMode(heater1,OUTPUT);
 pinMode(heaterFan,OUTPUT);
 pinMode(heater2, OUTPUT);
 pinMode(humidFan,OUTPUT);
 pinMode(humidifier,OUTPUT);
 pinMode(exhaustFan,OUTPUT);
 pinMode(coolingFan,OUTPUT);
 digitalWrite(buzzer, HIGH);
 delay(300);
 digitalWrite(buzzer, LOW);
 lcd.setCursor(0,0);
 lcd.print("DAAS BIOMEDICAL");
 lcd.setCursor(0,1);
 lcd.print("SYSTEMS");
 delay(1500);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("INFANT");
 lcd.setCursor(0,1);
 lcd.print("INCUBATOR");
 delay(1500);
 lcd.clear();
 selfTest();  
 
}

void loop() {
  
  humidity= dht.readHumidity();
  temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
    lcd.print("");
    return;
  }
 lcd.setCursor(0,0);
 lcd.print("TEMPERATURE:");
 lcd.setCursor(12,0);
 lcd.print(temperature);
 lcd.setCursor(0,1);
 lcd.print("HUMIDITY:");
 lcd.setCursor(10,1);
 lcd.print(humidity);
 lcd.setCursor(14,1);
 lcd.print("%");
 Serial.println(temperature);
 Serial.println(humidity);
 delay(1000);
//
// while(digitalRead(waterlevel))
// {
//  lcd.clear();
//  digitalWrite(buzzer, HIGH);
//  delay(1000);
//  lcd.setCursor(0,0);
//  lcd.print("LOW WATER LEVEL");
//  digitalWrite(buzzer, LOW);
//  delay(1000);
// }


 if (temperature< 36.0)
 {
  heatOn();
 }

 if (temperature> 37.0)
 {
  heatOff();
 }

 if(temperature>37.6)
 {
    coolOn();
    exhaustOn();
 }
   else
 {
       coolOff();
       exhaustOff();
 }

 while(temperature>37.9)
 {
  lcd.clear();
  digitalWrite(buzzer, HIGH);
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("OVER TEMPERATURE");
  digitalWrite(buzzer, LOW);
  delay(1000);

 }

 if (humidity< 70)
 {
  humidOn();
  exhaustOff();
 }

 if (humidity> 75)
 {
  humidOff();
  exhaustOn();
 }
  
}

void selfTest()
{
  digitalWrite(heater1,1);
  delay(2000);
  digitalWrite(heaterFan,1);
  delay(2000);
  digitalWrite(heater2,1);
  delay(2000);
  digitalWrite(humidFan,1);
  delay(2000);
  digitalWrite(exhaustFan,1);
  delay(2000);
  digitalWrite(coolingFan,1);
  delay(5000);
  digitalWrite(heater1,0);
  digitalWrite(heaterFan,0);
  digitalWrite(heater2,0);
  digitalWrite(humidFan,0);
  digitalWrite(exhaustFan,0);
  digitalWrite(coolingFan,0);
}

void heatOn()
{
  digitalWrite(heater1,1);
  digitalWrite(heater2,1);
  digitalWrite(heaterFan,1);
}

void heatOff()
{
  digitalWrite(heater1,0);
  digitalWrite(heater2,0);
  digitalWrite(heaterFan,0);
}

void exhaustOn()
{
  digitalWrite(exhaustFan,1);
}

void exhaustOff()
{
  digitalWrite(exhaustFan,0);
}

void coolOn()
{
  digitalWrite(coolingFan,1);
}

void coolOff()
{
  digitalWrite(coolingFan,0);
}

void humidOn()
{
  digitalWrite(humidifier,1);  
  digitalWrite(humidFan,1);
}

void humidOff()
{
  digitalWrite(humidifier,0);
  digitalWrite(humidFan,0);
}
