#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>
#include <IRremote.h>
#include <SevenSeg.h>

const int RECV_PIN                         = 2;
IRrecv myremote(RECV_PIN);
decode_results results;

#define digit1 10
#define digit2 A3
#define digit3 12
#define digit4 13
#define digit5 14
#define digit6 15

SevenSeg disp(3, 4, 5, 6, 7, 8, 9);
const int digits = 6;
int digitPins[digits] = { digit1, digit2, digit3, digit4, digit5, digit6 };
RTC_DS1307 RTC;
static DateTime now;

void setup() {
  // put your setup code here, to run once:
pinMode(3, OUTPUT);
disp.setCommonCathode();
disp.setDigitPins(digits, digitPins);
Wire.begin();
  RTC.begin();

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;                // initialize counter
  TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();
  
}

void loop() {
  now = RTC.now();
  displayTime();
  analogWrite(3, 10);

  if (myremote.decode(&results))
  {
    myremote.resume();
    if (results.value == 16738455)
    {
      ;
    }
  }
}

void displayTime()
{
  disp.write(getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second()));
}

String getHour(uint8_t hour)
{
  if (EEPROM[0] != 1)
  {
    if (hour > 12)
    {
      hour -= 12;
    }
    else if (hour == 0)
    {
      return String(hour + 12);
    }
  }

  return printDigits(hour);
}

String printDigits(byte digits)
{
  if (digits < 10)
  {
    return ('0' + String(digits));
  }
  else
  {
    return String(digits);
  }
}
