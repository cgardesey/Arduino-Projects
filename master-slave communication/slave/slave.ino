char incomingByte;      // a variable to read incoming serial data into

bool alarmsSet;

//static volatile uint64_t tot_overflow1 = 0;

void setup()
{
  Serial.begin(9600); // for serial monitor

  //PIR PIN
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);

  //POWER PIN
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  //BUZZER PINS
  DDRC |= 0b00001111;

  //BUZZER TEST
  PORTC |= 0b00001111;
  delay(500);
  PORTC &= 0b11110000;

  SIM900power();  // turn on shield
  delay(50000);  //allow pir to wake from sleep 
  alarmsSet = false;
}

void loop()
{
  if (alarmsSet == false)
  {
    intrusionDetected();
  }
  
  serialReceive();
}

void intrusionDetected()
{
  if (digitalRead(9) == LOW)
  {
    PORTC |= 0b00001111;
    Serial.print('Z');
    delay(7000);
  }
  else
  {
    PORTC &= 0b11110000;
  }
}

void serialReceive()
{
  if (Serial.available() > 0)
  {
     incomingByte = Serial.read();
    switch (incomingByte)
    {
    case 'A'://TEST
      PORTC |= 0b00001111;
      delay(2000);
      PORTC &= 0b11110000;
      break;
    case 'B'://ALARMS ON
      PORTC |= 0b00001111;
      alarmsSet = true;
      break;
    case 'C'://ALARMS OFF
      PORTC &= 0b11110000;
      alarmsSet = false;
      break;
    case 'D'://POWER ON
      PORTD |= 1 << PD3;
      delay(40000);
      break;
    case 'E'://POWER OFF
      PORTD &= ~(1 << PD3);
      PORTC &= 0b11110000;
      alarmsSet = false;
      break;
    case 'F'://POWER TOGGLE
      PORTD ^= 1 << PD3;
      if (digitalRead(3) == HIGH)
      {
        delay(40000);
      }   
      break;
    case 'G'://ALARM TOGGLE
      PORTC ^= 1 << PC3 | 1 << PC2 | 1 << PC1 | 1 << PC0;
      alarmsSet = !alarmsSet;
      break;
    default:
      break;
    }
  }
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  PORTC |= 1 << PC4;
  delay(1000);
  PORTC &= ~(1 << PC4);
  delay(7000);
}

//void Counter1Start()
//{
//  noInterrupts();           // disable all interrupts
//  TCCR1A = 0;
//  TCCR1B = 0;
//
//  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
//  TCCR1B |= (1 << CS12);    // 256 prescaler 
//  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
//  interrupts();             // enable all interrupts
//  tot_overflow1 = 0;
//}
//
//void Counter1Stop()
//{
//  TCCR1A = 0;
//  TCCR1B = 0;
//  tot_overflow1 = 0;
//}
//
//void resetCounter1()
//{
//  TCNT1 = 34286;
//  tot_overflow1 = 0;
//}
//
//ISR(TIMER1_OVF_vect)
//{
//  tot_overflow1++;
//  if (tot_overflow1 == 10)
//  {
//    Counter1Stop();
//  }
//}








