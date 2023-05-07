char incomingByte;      // a variable to read incoming serial data into
int pirPins[12] = { 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 19 };
bool intruMsgSent;

void setup()
{
  for (int i = 0; i < 12; i++)
  {
    pinMode(pirPins[i], INPUT);
    digitalWrite(pirPins[i], HIGH);
  }
  digitalWrite(3, HIGH);
  DDRC |= 0b00001111;
  PORTC |= 0b00001111;
  delay(500);
  PORTC &= 0b11110000;
  //Serial.begin(9600); // for serial monitor
  SIM900power();  // turn on shield
  delay(45000);  // give time to log on to network.
  intruMsgSent = true;
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  PORTD |= 1 << PD1;
}

void loop()
{
  intrusionDetected();
  serialReceive();
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  PORTC |= 1 << PC4;
  delay(1000);
  PORTC &= ~(1 << PC4);
  delay(7000);
}

void intrusionDetected()
{
  if (pirTrigered())
  {
    PORTC |= 0b00001111;
    PORTD &= ~(1 << PD1);
    intruMsgSent = false;
    delay(3000);
  }
  else
  {
    PORTC &= 0b11110000;
    if (!intruMsgSent)
    {
      PORTD |= 1 << PD1;
      intruMsgSent = true;
    }
  }
}

void serialReceive()
{
  if (digitalRead(0) == LOW)
  {
    int count = 0;  //TEST
    delay(60);
    if (digitalRead(0) == LOW)
    {
      count++; //SET
      delay(80);
      if (digitalRead(0) == LOW)
      {
        count++;//POWER
        delay(90);
      }
    }
////////////////////////////////////

 while(digitalRead(0) == LOW)
  {
    int count=0;
    delay(60);
    count++;
    delay(80);
    count++;
    delay(90);
  
  }
    switch (count)
    {
    case 0://TEST
      PORTC |= 0b00001111;
      delay(2000);
      PORTC &= 0b11110000;
    case 1://SET
      /*PORTC ^= 1 << PC0; 
      PORTC ^= 1 << PC1;
      PORTC ^= 1 << PC2;
      PORTC ^= 1 << PC3;*/
      PORTC |= 0b00001111;

      break;
    case 2://POWER
      PORTD ^= 1 << PD3;
      break;
    default:
      break;
    }
  }
}

boolean pirTrigered()
{
  for (int i = 0; i < 12; i++)
  {
    if (digitalRead(pirPins[i]) == LOW)
    {
      return true;
    }
  }
  return false;
}





