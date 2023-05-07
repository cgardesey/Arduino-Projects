char inchar;
void setup() {
  Serial.begin(9600);
pinMode(14, OUTPUT);
digitalWrite(14, HIGH);
delay(1000);
digitalWrite(14, LOW);
}

void loop() 
{
  if(Serial.available() > 0)
  {
  inchar = Serial.read();
   if(inchar == 'H')
  {
     digitalWrite(14, !digitalRead(14));
    delay(2000);
    Serial.print('H');
  }
  }
  
}

