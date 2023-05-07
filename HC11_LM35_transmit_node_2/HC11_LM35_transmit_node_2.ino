#include "DHT.h"

#define DHTPIN 8  
#define DHTTYPE DHT22 
#define GASPIN A5

byte g, g1;

float t1, t2, Dt1, Dt2;
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  Serial.begin(9600);
  dht.begin();

  pinMode(GASPIN, INPUT);
  delay(3000);
  
  g1 = digitalRead(GASPIN);
  t1 = t2 = dht.readTemperature();
  
  fireStatus();
  Serial.println("*T2:" + String(t2) + "#");
}

void loop()
{
  t1 = dht.readTemperature();
  g = digitalRead(GASPIN);

  Dt1 = abs(t2 - t1);

  delay(1000);

  t2 = dht.readTemperature();
  g1 = digitalRead(GASPIN);

  Dt2 = abs(t2 - t1);

//  if (digitalRead(GASPIN) == !g)
//  {
//    fireStatus();
//  }

  if(g1 != g)
  {
    fireStatus();
  }

  if (abs(Dt2 - Dt1) > 0)
  {
    Serial.println("*T2:" + String(t2) + "#");
  }
}

void fireStatus()   
{
  if (!g1)
  {
    Serial.println("*G2:Gas!!!   #");
  }
  else
  {
    Serial.println("*G2:No Gas   #");
  }
}
