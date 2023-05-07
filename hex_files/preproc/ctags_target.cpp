#line 1 "C:\\Users\\Cyril\\Documents\\Arduino\\HC11_receive\\HC11_receive.ino"
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900(9, 8); // RX, TX
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int pwrPin = 10;
const int v1 = A3;
const int v2 = A4;
const int v3 = A5;
const int amp = A2;
const int led = A1;
unsigned long p = 0;


char inchar;
String instring;
String subString;

void setup()
{
  pinMode(amp, OUTPUT);
  pinMode(pwrPin, OUTPUT);
  pinMode(v1, OUTPUT);
  pinMode(v2, OUTPUT);
  pinMode(v3, OUTPUT);
  pinMode(led, OUTPUT);

  SIM900power();

  Serial.begin(9600);
  lcd.begin(20, 4);
  SIM900.begin(19200); // for GSM shield

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WIRELESS");
  lcd.setCursor(0, 1);
  lcd.print("SMART FIRE ALARM");
  lcd.setCursor(0, 2);
  lcd.print("SYSTEM");
  delay(3000);

  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print(F("LOGGING ON TO"));
  lcd.setCursor(3, 2);
  lcd.print(F("NETWORK"));
  for (uint8_t i = 0; i < 6; i++)
  {
    delay(10000 / 7);
    lcd.print('.');
  }
  delay(10000 / 7);

  SIM900.print("AT+CMGF=1\r");// set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);

  lcd.clear();
  lcd.print(" TEMP.(" + String((char)223) + "C)  STATUS ");


  for (size_t j = 1; j < 4; j++)
  {
    display(0, j, String(j));
  }
  sendSMSstartUP();
}

void loop()
{
  if (Serial.available() > 0)
  {
    inchar = Serial.read();
    delay(10);
    if (inchar == '*')
    {
      instring = "";
      while (inchar != '#')
      {
        inchar = Serial.read();
        delay(10);
        if (inchar != '#')
        {
          instring += inchar;
        }
      }
      instring.trim();
    }

    subString = instring.substring(3);
    subString.trim();
    if (subString != "NAN")
    {
      if (instring.charAt(0) == 'T')
      {
        display(2, (int)instring.charAt(1) - 48, instring.substring(3) + "  ");
        String temps = instring.substring(3);
        float temp = temps.toFloat();
        if (temp > 40.0)
        {
          ampON();
          playAudio((int)instring.charAt(1) - 48);
          sendSMS((int)instring.charAt(1) - 48);
          flash();
        }
      }

      if (instring.charAt(0) == 'G')
      {
        display(10, (int)instring.charAt(1) - 48, instring.substring(3));
        if (subString == "Fire!!!  " || subString == "Gas   !!!" || subString == "High temp")
        {
          ampON();
        }
        else
        {
          ampOFF();
        }
      }
    }
  }
  if (digitalRead(amp))
  {
    playAudio((int)instring.charAt(1) - 48);
    sendSMS((int)instring.charAt(1) - 48);
    flash();
  }
}

void display(int  i, int j, String inscription)
{
  lcd.setCursor(i, j);
  lcd.print(inscription);
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(pwrPin, 0);
  delay(1000);
  digitalWrite(pwrPin, 1);
}

void sendSMS(int room)
{
  SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+233266854047\"");
  //SIM900.println("AT + CMGS = \"+233546797793\"");// recipient's mobile number,
  delay(100);

  switch (room)
  {
  case 1:
	  SIM900.print("Fire detected in room 1!!!");
	  break;
  case 2:
	  SIM900.print("Gas detected in room 2!!!");
	  break;
  case 3:
	  SIM900.print("High temperature detected in room 3!!!");
	  break;
  default:
	  break;
  }

  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS
}

void sendSMSstartUP()
{
  SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+233266854047\"");
  delay(100);

  SIM900.println("system started succesfully");

  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS
}


void playAudio(int node)
{
  if (node == 1)
  {
    PORTC |= 1 << PC3;
    PORTC &= ~(1 << PC3);
  }

  if (node == 2)
  {
    PORTC |= 1 << PC4;
    PORTC &= ~(1 << PC4);
  }

  if (node == 3)
  {
    PORTC |= 1 << PC5;
    PORTC &= ~(1 << PC5);
  }
}

void pulse(int v)
{
  digitalWrite(v, 1);
  digitalWrite(v, 0);
}

void flash()
{
  if (millis() - p >= 1000)
  {
    digitalWrite(led, !digitalRead(led));
    p += 1000;
  }
}

void ampON()
{
  digitalWrite(amp, 1);
}

void ampOFF()
{
  digitalWrite(amp, 0);
}




