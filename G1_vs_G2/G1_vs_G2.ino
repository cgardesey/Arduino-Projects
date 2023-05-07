
int g11, g21, Dg11, Dg21;
int g11, g21, Dg11, Dg21;
void setup() 
{
  Serial.begin(9600);

  g11 = g21 = analogRead(A1);


  Serial.println("g111:" + String(g21));
}

void loop()
{
  g111 = analogRead(A1);

  Dg111 = abs(g21 - g111);

  delay(2000);

  g21 = analogRead(A1);

  Dg21 = abs(g21 - g111);

 if(abs(Dg21 - Dg111) > 0)
 {
	 Serial.println("g11:" + String(g21));
 }
}
