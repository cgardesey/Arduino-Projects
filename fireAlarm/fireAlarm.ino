//MQ2 and LM1 are assumed to be in the same vicinity
//MQ6 and LM2 are assumed to be in the same vicinity

#include <LiquidCrystal.h>
#define MQ2 5
#define MQ6 4
#define LM1 A0
#define LM2 A1
#define ISD1 3
#define ISD2 2
float temp1;
float temp2;


LiquidCrystal lcd(13,12,11,10,9,6);
void setup() {
  pinMode(MQ2, INPUT);
  pinMode(MQ6, INPUT);
  pinMode(ISD1, OUTPUT);
  pinMode(ISD2, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("SMART ");
  lcd.setCursor(0,1);
  lcd.print("FIRE ALARM");
  delay(1000);
  lcd.clear();
  
}

void loop() {
 temp1 = readLM1();
 temp2 = readLM2();
 lcd.setCursor(0,0);
 lcd.print("temp1:");
 lcd.setCursor(6,0);
 lcd.print(temp1);
 lcd.setCursor(0,1);
 lcd.print("temp2:");
 lcd.setCursor(6,1);
 lcd.print(temp2);

 //monitor room 1
 if(digitalRead(MQ2)||temp1>45)
 {
  digitalWrite(ISD1, HIGH);
  Serial.println("FIRE AT ROOM 1");
 }
 else
  digitalWrite(ISD1, LOW);

//monitor room 2
 if(digitalRead(MQ6)||temp2>45)
 {
  digitalWrite(ISD2, HIGH);
  Serial.println("FIRE AT ROOM 2");
 }
 else
  digitalWrite(ISD2, LOW);

}

float readLM1()
{
  float t = analogRead(LM1);
  t = (t*500)/1023;
  return t;
}

float readLM2()
{
  float t = analogRead(LM2);
  t = (t*500)/1023;
  return t;
}
