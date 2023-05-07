int incomingByte;      // a variable to read incoming serial data into
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
  Serial.begin(9600); // for serial monitor
  //delay(200); // wait for master
  //SIM900power();  // turn on shield
  //delay(20000);  // give time to log on to network.
  intruMsgSent = true;
}

void loop()
{
  serialSend();
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

void serialSend()
{
  if (pirTrigered())
  {
    PORTC |= 0b00001111;
    Serial.print('Z');
    intruMsgSent = false;
  }
  else
  {
    PORTC &= 0b11110000;
    if (!intruMsgSent)
    {
      Serial.print('Y');
      intruMsgSent = true;
    }
  }
}

void serialReceive()
{
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();

    switch (incomingByte)
    {
    case 'A':
      PORTC |= 1 << PC0;
      delay(10000);
      PORTC &= ~(1 << PC0);
      break;
    case 'B':
      PORTC |= 1 << PC1;
      delay(10000);
      PORTC &= ~(1 << PC1);
      break;
    case 'C':
      PORTC |= 1 << PC2;
      delay(10000);
      PORTC &= ~(1 << PC2);
      break;
    case 'D':
      PORTC |= 1 << PC3;
      delay(10000);
      PORTC &= ~(1 << PC3);
      break;
    case 'E':
      PORTC |= 0b00001111;
      break;
    case 'F':
      PORTC &= 0b11110000;
      break;
    case 'G':
      PORTC |= 0b00001000;
      break;
    case 'H':
      PORTC &= 0b11110111;
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



