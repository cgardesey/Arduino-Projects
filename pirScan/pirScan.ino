char incomingByte;      // a variable to read incoming serial data into
bool intruMsgSent;
bool done= false;
int count;

void setup()
{

  //PIR
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  DDRC |= 0b00001111;
  //PORTC |= 0b00001111;
  PORTC |= 1 << PC0;
  delay(500);
  //PORTC &= 0b11110000;
  PORTC &= ~(1 << PC0);

  SIM900power();  // turn on shield
  //delay(45000);  // give time to log on to network.
  intruMsgSent = true;
  pinMode(0, INPUT);
  digitalWrite(0, HIGH);

  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  //PORTD |= 1 << PD1;
}

void loop()
{
  intrusionDetected();
  if (!digitalRead(0))
  {
    //test();
    last();
  }


  /* if (!digitalRead(0))
  {
    serialReceive();
  }*/
  //serialReceive();
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
  if (digitalRead(9) == LOW)
  {
    PORTC |= 0b00001111;
    PORTD &= ~(1 << PD1);
    intruMsgSent = false;
    //delay(5000);
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
  if (!digitalRead(0))
  {
    count = 0;  //TEST    
    delay(1500);
  }
  
  if (!digitalRead(0))
  {
    count=1; //SET  
    delay(2000);
  }
  
  if (!digitalRead(0))
  {
    count=2;//POWER
    delay(100);
  }
  
  
  switch (count)
  {
  case 0://TEST
    test();
    //PORTC |= 0b00001111;
    //PORTC |= (1 << PC0);
    //delay(2000);
    //////PORTC &= 0b11110000;
    //PORTC &= ~(1 << PC0);
    break;
  case 1://SET
    PORTC |= (1 << PC0);
    /*PORTC ^= 1 << PC0;
    PORTC ^= 1 << PC1;
    PORTC ^= 1 << PC2;
    PORTC ^= 1 << PC3;*/
    /*PORTC |= 0b00001111;*/

    break;
  case 2://POWER
    //PORTC |= (1 << PC0);
    PORTD ^= (1 << PD3);
    break;
  default:
    //test();
    break;
 }
}

void test()
{
  PORTC |= (1 << PC0);
  delay(300);
  PORTC &= ~(1<<PC0);
  delay(300);

}

int serialReceive1()
{
  if(!digitalRead(0))
  {
    count = 0;
    delay(1000);
    if (!digitalRead(0))
    {
      count = 1;
      delay(2000);
      if (!digitalRead(0))
      {
        count = 2;
        delay(3000);
      }
    }
   }
  return count;
}


void last()
{
  bool done=false;
  while(!digitalRead(0))
  {
    pinMode(1, INPUT);
    while(!done)
      {
        if(digitalRead(0)&& digitalRead(1))//IF BOTH INPUTS ARE HELD HIGH
          {
            PORTD ^= (1 << PD3);
            delay(100);
            done = true;
          }
        if(!digitalRead(0)&& !digitalRead(1))//IF BOTH INPUTS ARE HELD LOW
          {
            PORTC |=(1<<PC0);
            delay(10000  );
            done = true;
          }

        if(!digitalRead(0)&& digitalRead(1))//IF D0 IS LOW AND D1 IS HIGH
          {
             PORTC |=(1<<PC0);
             delay(3000);
              PORTC &=~(1<<PC0);
             delay(100);
             done = true;
          }
          if(done)
          {
            pinMode(1,OUTPUT);
           }
            
      }
  }
}








