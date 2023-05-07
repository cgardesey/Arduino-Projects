#include "DHT.h"

#define DHTPIN 8  
#define DHTTYPE DHT11 
#define GASPIN A5

int g, g1;

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
  
  if (t2 > 40.0)
  {
	  Serial.println("*G3:High temp#");
  }
  else
  {
	  Serial.println("*G3:Norm temp#");
  }
  Serial.println("*T3:" + String(t2) + "#");
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
    //fireStatus();
  }

  if (abs(Dt2 - Dt1) > 0)
  {
    Serial.println("*T3:" + String(t2) + "#");
	if (t2 > 40.0)
	{
		Serial.println("*G3:High temp#");
	}
	else
	{
		Serial.println("*G3:Norm temp#");		
	}
  }
}

//void fireStatus()   
//{
//  if (!g1)
//  {
//    Serial.println("*G3:Fire!!!#");
//  }
//  else
//  {
//    Serial.println("*G3:No Fire#");
//  }
//}
