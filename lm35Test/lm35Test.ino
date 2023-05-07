#include "DHT.h"
#define DHTPIN  18  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
//#define LM1 A5
//float temp;
//
//void setup() {
//  // put your setup code here, to run once:
//Serial.begin(9600);
//}
//
//void loop() {
// temp = readLM1();
// Serial.print("temperature: ");
// Serial.println(temp);
// delay(1000);
//
//}
//
//float readLM1()
//{
//  float t = analogRead(LM1);
//  t = (t*500)/1023;
//  return t;
//}


////////////////////////////////////////////////////
#define tempPin A5
float tempC;
int reading;


void setup()
{
analogReference(INTERNAL);
Serial.begin(9600);
dht.begin();
}

void loop()
{
reading = analogRead(tempPin);
tempC = reading / 9.31;
Serial.print("LM35 temp:");
Serial.println(tempC);
delay(2000);
float t = dht.readTemperature();
Serial.print("dht22 temp:");
Serial.println(t);
delay(1000);

}
