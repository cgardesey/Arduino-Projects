#include <SoftwareSerial.h>
#include <Password.h>
#include <Keypad.h>
#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

SoftwareSerial SIM900(7, 8); // RX, TX

char inchar; // Will hold the incoming character from the GSM shield

LiquidCrystal lcd(13, 12, 11, 10, 9, 6);
RTC_DS1307 RTC;
//#define eepromSize 48
const uint8_t eepromSize PROGMEM = 48;
static DateTime now;

const char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[4] = { 2, 3, 4, 5 }, colPins[4] = { 14, 15, 16, 17 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
Password password = Password("1111");


uint8_t tot_overflow1;

uint8_t sel = 0, sel_numberList, sel_PO = 0, sel_del = 0, scroll_mainMenu = 0, blinkInt = 0;
uint8_t h = 9, m = 0, s = 0;
String tm = "am";

static volatile char keypressed;

uint8_t _tm;
boolean cleared_menuScreen1 = false, cleared_menuScreen2 = false;

String passString;
char pass[4], newPass[4], confirmPass[4];
bool passwordSuccessfullyChanged = false, oldPassForLoopBreak = false, oldPassForLoopBreak_main = false, newPassForLoopBreak = false, goHome = false, leaveSelectScroll_PO = false, leaveSelectScroll_timeout = false, powerTimeLeave = false;
uint8_t i_contact = 3;
char contact[12] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t start = 0;

int eepromMirror[eepromSize];
String ansString[eepromSize / 12];
byte incomingByte = 0;
bool listTimeout, listLeave;
uint8_t powerTime = 0;
String receivedSMS;
uint8_t States[6];
//bool alarmOn, powerOn;


void setup()
{
  //Serial.begin(9600); // for serial monitor

  SIM900.begin(19200); // for GSM shield

  delay(10000);  // give time to log on to network.
  SIM900.print(F("AT+CMGF=1\r"));// set SMS mode to text
  delay(100);
  SIM900.print(F("AT+CNMI=2,2,0,0,0\r")); // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);

  if (word(EEPROM[506], EEPROM[507]) == 65535)
  {
    writePasswordToEeprom(1111, 506, 507);
  }
  passwordSet();


  //counter1 init
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  tot_overflow1 = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    States[i] = 0;
  }
  States[4] = 1;
  States[5] = 0;
  sendMultipleSMS();

  /*alarmOn = false;
  powerOn = true;*/
  lcd.begin(20, 4);
  lcd.clear();
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(0, INPUT);
}

void loop()
{
  now = RTC.now();
  //show time
  lcd.setCursor(4, 1);
  /*lcd.print(getHour(now.hour()) + ":" + printDigits(now.minute()) + ":" + printDigits(now.second()) + " " + getTimeMode(now.hour()));*/
  lcd.print(getHour(now.hour()));
  lcd.print(':');
  lcd.print(printDigits(now.minute()));
  lcd.print(':');
  lcd.print(printDigits(now.second()));
  lcd.print(' ');
  lcd.print(getTimeMode(now.hour()));

  keypressed = keypad.getKey();
  if (keypressed == 'A')
  {
    resetCounter1();
    lcd.clear();
    password.reset();
    goHome = false;
    while (true)
    {
      if (goHome)
      {
        break;
      }
      lcd.setCursor(2, 1);
      lcd.print(F("Enter Password:"));
      uint8_t i_p;
      for (i_p = 0; i_p < 4; i_p++)
      {
        keypressed = keypad.getKey();
        if (goBack())
        {
          oldPassForLoopBreak_main = true;
          break;
        }

        if (tot_overflow1 > 14)
        {
          lcd.clear();
          oldPassForLoopBreak_main = true;
          break;
        }

        if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9'))
        {
          resetCounter1();
          password.append(keypressed);
          lcd.setCursor((7 + i_p), 2);
          lcd.print('#');
        }
        else
        {
          i_p--;
        }
      }

      if (oldPassForLoopBreak_main)
      {
        oldPassForLoopBreak_main = false;
        break;
      }

      if (password.evaluate())
      {
        goHome = true;
        password.reset();
        scroll_mainMenu = 0;
        lcd.clear();
        while (1)
        {
          keypressed = keypad.getKey();
          //menu interrupt
          const String menuItemArray[5] PROGMEM = { "1. SET TIME","2. TEST ALARMS", "3. POWER OPTIONS", "4. CHANGE PASSWORD", "5. NOTIFICATION NO." };
          if (scroll_mainMenu == 0)
          {
            if (!cleared_menuScreen1)
            {
              lcd.clear();
              cleared_menuScreen1 = true;
            }
            cleared_menuScreen2 = false;

            for (uint8_t i = 0; i < 4; i++)
            {
              lcd.setCursor((20 - menuItemArray[i].length()) / 2, i);
              lcd.print(menuItemArray[i]);
            }
          }
          else if (scroll_mainMenu == 1)
          {

            if (!cleared_menuScreen2)
            {
              lcd.clear();
              cleared_menuScreen2 = true;
            }
            cleared_menuScreen1 = false;
            lcd.setCursor((20 - menuItemArray[4].length()) / 2, 0);
            lcd.print(menuItemArray[4]);
          }
          keypressed = keypad.getKey();

          if (keypressed == 'B')
          {
            resetCounter1();
            scroll_mainMenu--;
            if (scroll_mainMenu == -1)
              scroll_mainMenu = 1;
          }

          if (keypressed == 'C')
          {
            resetCounter1();
            scroll_mainMenu++;
            if (scroll_mainMenu == 2)
              scroll_mainMenu = 0;
          }
          if (goBack())
          {
            break;
          }
          if (tot_overflow1 > 14)
          {
            lcd.clear();
            break;
          }
          menuItem(keypressed);
        }
      }
      else
      {
        if (goBack())
        {
          break;
        }
        if (tot_overflow1 > 14)
        {
          lcd.clear();
          break;
        }
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print(F("Wrong Password!"));
        delay(1000);
        lcd.clear();
        password.reset();
      }

      if (goBack())
      {
        break;
      }
      if (tot_overflow1 > 14)
      {
        lcd.clear();
        break;
      }
    }
  }
  if (keypressed == '*')
  {
    digitalWrite(1, LOW);
    delay(20);
    digitalWrite(1, HIGH);
  }
  if (keypressed == '#')
  {
    digitalWrite(1, LOW);
    delay(10);
    digitalWrite(1, HIGH);
  }
  //auto power
  if (EEPROM[505] == 1)
  {
    if (word(EEPROM[510], EEPROM[511] == timeNow()))
    {
      Serial.write('G');
    }
    if (word(EEPROM[508], EEPROM[509] == timeNow()))
    {
      Serial.write('H');
    }
  }

  //serial receive
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    if (digitalRead(0) == LOW)
    {
      for (uint8_t i = 0; i < 6; i++)
      {
        States[i] = 1;
      }
      sendMultipleSMS();
    }
  if (digitalRead(0) == HIGH)
    {
      for (uint8_t i = 0; i < 4; i++)
      {
        States[i] = 0;
      }
      States[4] = 1;
      States[5] = 0;
      sendMultipleSMS();
    }
  }

  receiveSMS();
  if (receivedSMS != "")
  {
    if (receivedSMS == "t")
    {
      digitalWrite(1, LOW);
      delay(5);
      digitalWrite(1, HIGH);
      States[0] = 1;
      sendMultipleSMS();
      delay(1000);
      States[0] = 0;
      sendMultipleSMS();
      receivedSMS = "";

    }
    if (receivedSMS == "a")
    {
      digitalWrite(1, LOW);
      delay(100);
      digitalWrite(1, HIGH);
      lcd.setCursor(0, 3);
      lcd.print(receivedSMS);

      for (size_t i = 0; i < 4; i++)
      {
        States[i] ^= 1;
      }
      sendMultipleSMS();
      receivedSMS = "";
    }
    if (receivedSMS == "p")
    {
      digitalWrite(1, LOW);
      delay(200);
      digitalWrite(1, HIGH);
      States[4] ^= 1;
      sendMultipleSMS();
      receivedSMS = "";
    }
    if (receivedSMS.substring(0) == "b")
    {
      sendMultipleSMS();
      receivedSMS = "";
    }
    else
    {
    dataCall(receivedSMS);
    sendMultipleSMS();
    receivedSMS = "";
    }
  }
}

String printDigits(byte digits)
{
  if (digits < 10)
    return ('0' + String(digits));
  else
    return String(digits);
}

String getHour(uint8_t hour)
{
  if (hour > 12) {
    hour -= 12;
  }
  else if (hour == 0)
    return String(hour + 12);

  if (hour < 10)
    return String("0" + String(hour));
  else
    return String(hour);

}

String getTimeMode(uint8_t hour)
{
  if (hour  > 11)
    return("pm");
  else
    return("am");
}

//void Counter1Stop()
//{
//  TCCR1A = 0;
//  TCCR1B = 0;
//  tot_overflow1 = 0;
//}

void resetCounter1()
{
  TCNT1 = 34286;
  tot_overflow1 = 0;
}

ISR(TIMER1_OVF_vect)
{
  tot_overflow1++;
  keypressed = keypad.getKey();
  if (keypressed == 'D' || tot_overflow1 == 250)
  {
    resetCounter1();
  }
}

void menuItem(char menuItem)
{
  keypressed = keypad.getKey();

  switch (menuItem)
  {
  case '1':
    resetCounter1();
    lcd.clear();
    h = getHour(now.hour()).toInt();
    m = printDigits(now.minute()).toInt();
    s = printDigits(now.second()).toInt();
    tm = getTimeMode(now.hour());
    sel = 0;
    while (1)
    {
      if (goBack())
      {
        break;
      }
      if (tot_overflow1 > 14)
      {
        lcd.clear();
        break;
      }
      keypressed = keypad.getKey();
      //select scroll
      /*keypressed = keypad.getKey();*/
      //sell scroll
      //keypressed = keypad.getKey();
      if (keypressed == 'B')
      {
        resetCounter1();
        sel--;
        if (sel == -1)
        {
          sel = 6;
        }
      }
      /*keypressed = keypad.getKey();*/
      if (keypressed == 'C')
      {
        resetCounter1();
        sel++;
        if (sel == 7)
        {
          sel = 0;
        }
      }
      if (h > 12)
      {
        h = 12;
      }

      lcd.setCursor(4, 1);
      //show time
      /*  lcd.print(printDigits(h) + ":" + printDigits(m) + ":" + printDigits(s) + " " + tm);*/
      lcd.print(printDigits(h));
      lcd.print(':');
      lcd.print(printDigits(m));
      lcd.print(':');
      lcd.print(printDigits(s));
      lcd.print(' ');
      lcd.print(tm);

      uint8_t timePlacements[7] = { 4, 5, 7, 8, 10, 11, 13 };
      switch (sel)
      {
      case 0:
        blinks(String(h / 10), timePlacements[sel], 1);
        break;
      case 1:
        blinks(String(h % 10), timePlacements[sel], 1);
        break;
      case 2:
        blinks(String(m / 10), timePlacements[sel], 1);
        break;
      case 3:
        blinks(String(m % 10), timePlacements[sel], 1);
        break;
      case 4:
        blinks(String(s / 10), timePlacements[sel], 1);
        break;
      case 5:
        blinks(String(s % 10), timePlacements[sel], 1);
        break;
      case 6:
        blinks(tm, timePlacements[sel], 1);
        break;
      }
      if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9'))
      {
        resetCounter1();
        char key = String(keypressed).toInt();

        switch (sel)
        {
        case 0:
          /*RTC.adjust(DateTime(now.year(), now.month(), now.day(), keypressed, m, s));*/
          if (key < 2)
          {
            h = setTens(h, key);
          }
          break;
        case 1:
          h = setOnes(h, key);
          break;
        case 2:
          if (key < 6)
          {
            m = setTens(m, key);
          }
          break;
        case 3:
          m = setOnes(m, key);
          break;
        case 4:
          if (key < 6)
          {
            s = setTens(s, key);
          }
          break;
        case 5:
          s = setOnes(s, key);
          break;
        case 6:
          switch (key)
          {
          case 1:
            tm = "am";
            break;
          case 2:
            tm = "pm";
            break;
          default:
            break;
          }
          /*RTC.adjust(now + TimeSpan(12, 0, 0, 0));*/
          break;
        default:
          break;
        }
      }
      if (keypressed == 'A')
      {
        if (tm == "pm")
        {
          h += 12;
        }
        RTC.adjust(DateTime(now.year(), now.month(), now.day(), h, m, s));
        now = RTC.now();
        break;
      }
    }
    break;

  case '2':
    digitalWrite(1, LOW);
    delay(10);
    digitalWrite(1, HIGH);    
    return;

  case '3':
    resetCounter1();
    lcd.clear();
    while (true)
    {
      lcd.setCursor(2, 1);
      lcd.print(F("1. MANUAL MODE"));
      lcd.setCursor(1, 2);
      lcd.print(F("2. AUTOMATIC MODE"));

      if (goBack())
      {
        break;
      }
      if (tot_overflow1 > 14)
      {
        lcd.clear();
        break;
      }
      keypressed = keypad.getKey();

      switch (keypressed)
      {
      case '1':
        lcd.clear();
        //manual mode
        lcd.setCursor(3, 1);
        lcd.print(F("Manual mode"));
        lcd.setCursor(4, 2);
        lcd.print(F("activated"));
        EEPROM.update(505, 0);
        delay(1000);
        lcd.clear();
        break;
      case '2':
        lcd.clear();
        automaticMode();
        break;
      default:
        break;
      }
    }
    break;

  case '4':
    resetCounter1();
    lcd.clear();
    password.reset();
    while (true)
    {
      if (passwordSuccessfullyChanged)
      {
        passwordSuccessfullyChanged = false;
        break;
      }
      lcd.setCursor(0, 1);
      lcd.print(F("Enter Old Password:"));
      uint8_t i_op;
      for (i_op = 0; i_op < 4; i_op++)
      {
        keypressed = keypad.getKey();
        if (goBack())
        {
          oldPassForLoopBreak = true;
          break;
        }
        if (tot_overflow1 > 14)
        {
          lcd.clear();
          oldPassForLoopBreak = true;
          break;
        }
        if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9'))
        {
          resetCounter1();
          password.append(keypressed);
          lcd.setCursor((7 + i_op), 2);
          lcd.print('#');
        }
        else
        {
          i_op--;
        }
      }
      if (oldPassForLoopBreak)
      {
        oldPassForLoopBreak = false;
        break;
      }
      if (password.evaluate())
      {
        lcd.clear();
        password.reset();
        while (true)
        {
          lcd.setCursor(0, 1);
          lcd.print(F("Enter New Password:"));
          uint8_t i_np;
          for (i_np = 0; i_np < 4; i_np++)
          {
            keypressed = keypad.getKey();
            if (goBack())
            {
              newPassForLoopBreak = true;
              break;
            }
            if (tot_overflow1 > 14)
            {
              lcd.clear();
              newPassForLoopBreak = true;
              break;
            }
            if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9'))
            {
              resetCounter1();
              newPass[i_np] = keypressed;
              lcd.setCursor((7 + i_np), 2);
              lcd.print('#');
            }
            else
            {
              i_np--;
            }
          }
          if (newPassForLoopBreak)
          {
            newPassForLoopBreak = false;
            break;
          }

          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print(F("Confirm New Password"));
          uint8_t i_cp;
          for (i_cp = 0; i_cp < 4; i_cp++)
          {
            keypressed = keypad.getKey();
            if (goBack())
            {
              newPassForLoopBreak = true;
              break;
            }
            if (tot_overflow1 > 14)
            {
              lcd.clear();
              newPassForLoopBreak = true;
              break;
            }
            if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9'))
            {
              resetCounter1();
              confirmPass[i_cp] = keypressed;
              lcd.setCursor((7 + i_cp), 2);
              lcd.print('#');
            }
            else
            {
              i_cp--;
            }
          }
          if (newPassForLoopBreak)
          {
            newPassForLoopBreak = false;
            break;
          }

          if (String(newPass).substring(0, 4) == String(confirmPass).substring(0, 4))
          {
            lcd.clear();
            lcd.setCursor(5, 1);
            lcd.print(F("Password"));
            lcd.setCursor(0, 2);
            lcd.print(F("successfully changed"));
            writePasswordToEeprom(String(confirmPass).substring(0, 4).toInt(), 506, 507);
            passwordSet();
            delay(1000);
            lcd.clear();
            passwordSuccessfullyChanged = true;
            break;
          }
          else
          {
            lcd.clear();

            /*lcd.setCursor(0, 0);
            lcd.print(String(newPass).substring(0, 4));
            lcd.setCursor(0, 1);
            lcd.print(String(confirmPass).substring(0, 4));
            delay(1000000000);*/
            lcd.setCursor(1, 1);
            lcd.print(F("Passwords do not"));
            lcd.setCursor(6, 2);
            lcd.print(F("match"));
            delay(1000);
            lcd.clear();
            password.reset();
          }
        }

      }
      else
      {
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print(F("Wrong Password!"));
        delay(1000);
        lcd.clear();
        password.reset();
      }
    }
    break;

  case '5':
    resetCounter1();
    lcd.clear();
    while (1)
    {
      lcd.setCursor(2, 0);
      lcd.print(F("1. ADD NUMBER"));
      lcd.setCursor(2, 1);
      lcd.print(F("2. LIST NUMBERS"));
      lcd.setCursor(2, 2);
      lcd.print(F("3. DELETE NUMBER"));

      keypressed = keypad.getKey();
      if (goBack())
      {
        break;
      }
      if (tot_overflow1 > 14)
      {
        lcd.clear();
        break;
      }

      menuItem_numberAction(keypressed);
    }
    break;

  default:
    break;
  }
}

void menuItem_numberAction(char menuItem)
{
  keypressed = keypad.getKey();
  switch (menuItem)
  {
  case '1':
    if (systemMemmoryFull())
    {
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print(F("System memmory"));
      lcd.setCursor(7, 2);
      lcd.print(F("full!"));
      delay(1000);
      lcd.clear();
      return;
    }
    resetCounter1();
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print(F("Enter Contact No.:"));
    i_contact = 4;
    resetContactHolder();

    while (true)
    {
      if (goBack())
      {
        break;
      }
      if (tot_overflow1 > 14)
      {
        lcd.clear();
        break;
      }
      keypressed = keypad.getKey();
      if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9') && i_contact < 16)
      {
        resetCounter1();
        lcd.setCursor(i_contact, 2);
        lcd.print(keypressed);
        contact[i_contact - 4] = keypressed;
        i_contact++;
      }
      if (keypressed == 'A' && contact[0] != ' ')
      {
        resetCounter1();
        String tempString = "+";
        for (uint8_t i = 0; i < 12; i++)
        {
          if (contact[i] == ' ')
          {
            continue;
          }
          tempString += contact[i];
        }
        if (numberExist(tempString))
        {
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print(F("Number already"));
          lcd.setCursor(6, 2);
          lcd.print(F("exist!"));
          delay(1000);
          lcd.clear();
          return;
        }
        else
        {
          for (uint8_t i = 0; i < eepromSize; i += 12)
          {
            if (EEPROM[i] == 255)
            {
              delay(1000);
              for (uint8_t j = 0; j < 12; j++)
              {
                EEPROM[i + j] = contact[j];
              }
              lcd.clear();
              lcd.setCursor(6, 1);
              lcd.print(F("Number"));
              lcd.setCursor(1, 2);
              lcd.print(F("successfully added"));
              resetContactHolder();
              delay(1000);
              lcd.clear();
              start = 0;
              while (true)
              {
                listAlarmTimeScreen();
                if (goBack())
                {
                  keypressed = 'D';
                  break;
                }
                if (tot_overflow1 > 14)
                {
                  keypressed = 'D';
                  lcd.clear();
                  break;
                }
              }
              break;
            }
            if (goBack())
            {
              keypressed = 'D';
              break;
            }
            if (tot_overflow1 > 14)
            {
              keypressed = 'D';
              lcd.clear();
              break;
            }
          }
        }
      }
      if (keypressed == 'C' && contact[0] != ' ')
      {
        resetCounter1();
        i_contact--;
        lcd.setCursor(i_contact, 2);
        lcd.print(" ");
        contact[i_contact - 4] = ' ';
      }
    }
    break;

  case '2':
    resetCounter1();
    lcd.clear();
    start = 0;
    listTimeout = false;
    listLeave = false;
    while (true)
    {
      if (!eepromEmpty())
      {
        listAlarmTimeScreen();
        if (listTimeout)
        {
          /*resetCounter1();*/
          lcd.clear();
          return;
        }
        if (listLeave)
        {
          resetCounter1();
          lcd.clear();
          return;
        }
      }
      else
      {
        lcd.clear();
        alarmTimeListEmptyScreen();
        keypressed = keypad.getKey();
        break;
      }
      keypressed = keypad.getKey();
      if (goBack())
      {
        break;
      }
      if (tot_overflow1 > 14)
      {
        lcd.clear();
        break;
      }
    }
    break;

  case '3':
    resetCounter1();
    sel_del = 0;
    start = 0;
    lcd.clear();
    /*  alarmTimelist = 0;
    action = "remove";*/
    if (!eepromEmpty())
    {
      while (1)
      {
        listAlarmTimeScreen();
        blinks_del(ansString[sel_del], sel_del % 4);
        keypressed = keypad.getKey();
        if (keypressed == 'A')
        {
          resetCounter1();
          lcd.clear();
          uint8_t selection = getSelection();
          for (uint8_t i = 0; i < 12; i++)
          {
            EEPROM[selection + i] = 255;
          }

          if (eepromEmpty())
          {
            lcd.setCursor(1, 1);
            lcd.print(F("Alarm time list is"));
            lcd.setCursor(4, 2);
            lcd.print(F("now empty!"));
            delay(1000);
            lcd.clear();
            break;
          }

          while (true)
          {
            listAlarmTimeScreen();
            if (goBack())
            {
              break;
            }
            if (tot_overflow1 > 14)
            {
              lcd.clear();
              break;
            }
          }
        }

        //moveUpDown
        if (keypressed == 'B')
        {
          resetCounter1();
          if (sel_del == start)
          {
            //move down helper
            for (uint8_t i = start + 3; i > start; i--)
            {
              if (ansString[i] == "+")
              {
                continue;
              }
              sel_del = i;
              return;
            }
          }
          else
          {
            sel_del--;
          }
        }

        if (keypressed == 'C')
        {
          resetCounter1();

          if (sel_del == (start + 3) || ansString[sel_del + 1] == "+")
          {
            sel_del = start;
          }
          else
          {
            sel_del++;
          }

        }
        if (goBack())
        {
          break;
        }
        if (tot_overflow1 > 14)
        {
          lcd.clear();
          break;
        }
      }
    }
    else
    {
      lcd.clear();
      alarmTimeListEmptyScreen();
      break;
    }
    break;
  default:
    break;
  }
}

void blinks(String text, uint8_t x, uint8_t y)
{
  if (blinkInt % 2)
  {
    lcd.setCursor(x, y);
    lcd.print(text);
  }
  else
  {
    lcd.setCursor(x, y);
    lcd.print(getWhiteSpaces(text));
  }
  blinkInt++;
  delay(250);
}

void blinks_del(String text, uint8_t x)
{
  if (blinkInt % 2)
  {
    lcd.setCursor(4, x);
    lcd.print(text);
  }
  else
  {
    lcd.setCursor(4, x);
    lcd.print(F("             "));
  }
  blinkInt++;
  delay(250);
}

String getWhiteSpaces(String input)
{
  String result;
  for (uint8_t i = 0; i < input.length(); i++)
  {
    result = (result + " ");
  }
  return result;
}

int setTens(uint8_t timeParam, uint8_t key)
{
  return  (10 * key + timeParam % 10);
}

uint8_t setOnes(uint8_t timeParam, uint8_t key)
{
  uint8_t temp = timeParam / 10;
  return  (10 * temp + key);
}

void automaticMode()
{
  if (word(EEPROM[510], EEPROM[511]) == 65535)
  {
    writePasswordToEeprom(500, 510, 511);
  }
  int x = word(EEPROM[510], EEPROM[511]);
  h = (getTime(x).substring(0, 2)).toInt();
  m = (getTime(x).substring(3, 5)).toInt();
  tm = getTime(x).substring(6);
  leaveSelectScroll_PO = false;
  powerTimeLeave = false;
  powerTime = 0;
  while (true)
  {
    keypressed = keypad.getKey();
    lcd.setCursor(0, 1);
    lcd.print(F("Enter Power on Time:"));
    selectScroll_PO();
    if (leaveSelectScroll_PO)
    {
      return;
    }
    if (powerTimeLeave)
    {
      break;
    }
  }

  if (word(EEPROM[508], EEPROM[509]) == 65535)
  {
    writePasswordToEeprom(11000, 508, 509);
  }
  x = word(EEPROM[508], EEPROM[509]);
  h = (getTime(x).substring(0, 2)).toInt();
  m = (getTime(x).substring(3, 5)).toInt();
  tm = getTime(x).substring(6);
  leaveSelectScroll_PO = false;
  powerTimeLeave = false;
  powerTime = 1;
  while (true)
  {
    keypressed = keypad.getKey();
    lcd.setCursor(0, 1);
    lcd.print(F("Enter Shutdown Time:"));
    selectScroll_PO();
    if (leaveSelectScroll_PO)
    {
      return;
    }
    if (powerTimeLeave)
    {
      break;
    }
  }

  lcd.setCursor(2, 1);
  lcd.print(F("Automatic mode"));
  lcd.setCursor(4, 2);
  lcd.print(F("activated"));
  EEPROM.update(505, 1);
  delay(1000);
  lcd.clear();
}

String getTime(int index)
{
  int H, M, Mode, intValue; String TM;

  intValue = index;
  Mode = index / 10000;
  H = (intValue - 10000 * Mode) / 100;
  M = (intValue - 10000 * Mode) - 100 * H;

  switch (Mode)
  {
  case 0:
    TM = "am";
    break;

  case 1:
    TM = "pm";
    break;

  default:
    break;
  }
  return(printDigits(H) + ":" + printDigits(M) + " " + TM);
}

boolean eepromEmpty()
{
  for (uint8_t i = 0; i < eepromSize; i += 12)
  {
    if (EEPROM[i] != 255)
    {
      return false;
    }
  }
  return true;
}

void alarmTimeListEmptyScreen()
{
  lcd.setCursor(2, 1);
  lcd.print(F("Number list is"));
  lcd.setCursor(6, 2);
  lcd.print(F("empty!"));
  delay(1000);
  lcd.clear();
}

void listAlarmTimeScreen()
{
  eepromCopy_and_convert();
  for (uint8_t i = start, j = start; i < (start + 4); i++, j++)
  {
    if (ansString[i] == "+")
    {
      j--;
      continue;
    }
    lcd.setCursor(4, j % 4);
    lcd.print(ansString[i]);
    keypressed = keypad.getKey();
    if (keypressed == 'D')
    {
      listLeave = true;
      return;
    }
    if (tot_overflow1 > 14)
    {
      listTimeout = true;
      return;
    }
    keypressed = keypad.getKey();
    if (keypressed == 'C')
    {
      resetCounter1();

      if (nextPageEmpty() || start == eepromSize / 48 - 1)
      {
        start = 0;
        sel_numberList = 0;
      }
      else
      {
        start += 4;
        sel_numberList = start;
        lcd.clear();
      }
    }

    keypressed = keypad.getKey();
    if (keypressed == 'B')
    {
      resetCounter1();

      if (start == 0)
      {
        start = 4;
        sel_numberList = 4;
      }
      else
      {
        start -= 4;
        sel_numberList = start;
        lcd.clear();
      }
    }
  }
}

boolean nextPageEmpty()
{
  for (uint8_t i = (start + 4); i < eepromSize / 12; i++)
  {
    if (ansString[i] != "+")
      return false;
  }
  return true;
}

void passwordSet()
{
  passString = String(word(EEPROM[506], EEPROM[507]));
  passString.toCharArray(pass, passString.length() + 1); //convert string to char array
  password.set(pass);
}

void writePasswordToEeprom(int param, int highByteAddr, int lowByteAddr)
{
  EEPROM.write(highByteAddr, highByte(param));
  EEPROM.write(lowByteAddr, lowByte(param));
}

void eepromCopy_and_convert()
{
  //eeprom copy
  for (uint8_t i = 0; i < eepromSize; i++)
  {
    eepromMirror[i] = EEPROM[i];
  }

  //reset ans string
  for (uint8_t i = 0; i < eepromSize / 12; i++)
  {
    ansString[i] = "+";
  }
  //convert
  for (uint8_t i = 0; i < eepromSize; i += 12)
  {
    if (eepromMirror[i] == 255)
    {
      ansString[i / 12] = "+";
    }
    else
    {
      for (uint8_t j = 0; j < 12; j++)
      {
        if (EEPROM[i + j] == 32)
        {
          continue;
        }
        ansString[i / 12] += String(EEPROM[i + j] - 48);
      }
    }
  }
}

uint8_t getSelection()
{
  for (uint8_t i = 0; i < eepromSize; i += 12)
  {
    if (EEPROM[i] == 255)
    {
      continue;
    }
    String tempStringArray = "+";
    for (uint8_t j = 0; j < 12; j++)
    {
      if (EEPROM[i + j] == 32)
      {
        continue;
      }
      tempStringArray += String(EEPROM[i + j] - 48);
    }
    if (ansString[sel_del] == tempStringArray)
    {
      return i;
    }
  }
}

void resetContactHolder()
{
  for (uint8_t i = 0; i < 12; i++)
  {
    contact[i] = ' ';
  }
}

boolean systemMemmoryFull()
{
  for (uint8_t i = 0; i < eepromSize; i += 12)
  {
    if (EEPROM[i] == 255)
    {
      return false;
    }
  }
  return true;
}

boolean numberExist(String input)
{
  eepromCopy_and_convert();
  for (uint8_t i = 0; i < 8; i++)
  {
    if (ansString[i] == "+")
    {
      continue;
    }
    if (ansString[i] == input)
    {
      return true;
    }
  }
  return false;
}

void sendMultipleSMS()
{
  /*float balance = getCreditBalance().toFloat();*/
  eepromCopy_and_convert();
  /*resetAnsString(ansString);*/
  for (uint8_t i = 0; i < 4; i++)
  {
    if (ansString[i] == "+")
    {
      continue;
    }
    sendSMS(1.2, ansString[i]);
  }
}

void sendSMS(float B, String U)
{
  String sms = "";
  sms += (char)123;
  sms += '"'; sms += 'S'; sms += '"';sms += ":{";
  sms += '"'; sms += 'A';sms += '"';sms += ":";
  sms += '[';
  sms += States[0]; sms += ',';
  sms += States[1]; sms += ',';
  sms += States[2]; sms += ',';
  sms += States[3];
  sms += ']';
  sms += ','; sms += '"'; sms += 'P'; sms += '"'; sms += ':'; sms += States[4];
  sms += ','; sms += '"'; sms += 'I'; sms += '"'; sms += ':'; sms += States[5];
  sms += ','; sms += '"'; sms += 'B'; sms += '"'; sms += ':'; sms += B;
  sms += ','; sms += '"'; sms += 'U'; sms += '"'; sms += ':'; sms += "\"""" "\"";
  sms += ','; sms += '"'; sms += 'V'; sms += '"'; sms += ':'; sms += 9;
  sms += "}}";

  SIM900.print(F("AT+CMGF=1\r")); // AT command to send SMS message
  delay(100);
  //SIM900.println("AT + CMGS = \"+12128675309\"");  // recipient's mobile number,
  SIM900.println("AT + CMGS = \"" + U + "\"");  // recipient's mobile number,
  delay(100);
  //SIM900.println("{"S":{"A":[0, 1, 1, 1],"P" : 0,"I" : 1,"B" : 1.2,"U" : "2554441","V" : 12}}"); // message to send
  //SIM900.println("{\"S\":{\"A\":[a, b, c, d],\"P\" : 0,\"I\" : 1,\"B\" : 1.2,\"U\" : \"" + U + "\",\"V\" : 12}}"); // message to send
  SIM900.println(sms);
  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS  
}

void receiveSMS()
{
  //SIM900.print("AT+CMGR=1\r\n");  // read the first SMS from the inbox
  if (SIM900.available() > 0)
  {
    inchar = SIM900.read();
    delay(10);
    if (inchar == '*')
    {
      inchar = SIM900.read();
      delay(10);
      receivedSMS = "";
      while (inchar != '#')
      {
        receivedSMS += inchar;
        inchar = SIM900.read();
        delay(10);
      }
      lcd.setCursor(15, 3);
      lcd.print(receivedSMS);
    }
  }
  else
  {
    receivedSMS = "";
  }
}

void delSms()
{
  //SIM900.print("AT+CMGD=ALL\r\n");  // delete all first message from the inbox
  SIM900.println(F("AT+CMGD=1,4")); // delete all SMS
}

//void dial(String number)
//{
//  //SIM900.println("ATD + +12128675309;"); // dial US (212) 8675309
//  SIM900.println("ATD + " + number);
//  delay(100);
//  SIM900.println();
//  //delay(5000);            // wait for 5 seconds...
//}

String getCreditBalance()
{
  //AT+CUSD=1, "*#10#"
  dataCall("*124#");
  if (SIM900.available() > 0)
  {
    inchar = SIM900.read();
    if (inchar == 'G')
    {
      delay(10);
      inchar = SIM900.read();
      if (inchar == 'H')
      {
        delay(10);
        inchar = SIM900.read();
        if (inchar == 'C')
        {
          delay(10);
          inchar = SIM900.read();
          String balance = "";
          while (inchar != ',')
          {
            balance += inchar;
            delay(10);
            inchar = SIM900.read();
          }
          /*delSms();*/
          return balance;
        }
      }
    }
  }
}

void dataCall(String code)
{
  SIM900.println("AT+CUSD = 1, \"" + code + "\"");
}

int timeNow()
{
  now = RTC.now();

  if (getTimeMode(now.hour()) == " am")
  {
    return (100 * getHour(now.hour()).toInt() + now.minute());
  }
  else
  {
    return (10000 + 100 * getHour(now.hour()).toInt() + now.minute());
  }
}

boolean goBack()
{
  /*keypressed = keypad.getKey();*/
  if (keypressed == 'D')
  {
    resetCounter1();
    lcd.clear();
    return true;
  }
  return false;
}

void showTime_PO()
{
  lcd.setCursor(5, 2);
  //lcd.print(printDigits(h) + ":" + printDigits(m) + " " + tm);
  lcd.print(printDigits(h));
  lcd.print(":");
  lcd.print(printDigits(m));
  lcd.print(" ");
  lcd.print(tm);
}

void selectScroll_PO()
{
  showTime_PO();
  const uint8_t timePlacements_PO[5] = { 5, 6, 8, 9, 11 };
  switch (sel_PO)
  {
  case 0:
    blinks(String(h / 10), timePlacements_PO[sel_PO], 2);
    break;
  case 1:
    blinks(String(h % 10), timePlacements_PO[sel_PO], 2);
    break;
  case 2:
    blinks(String(m / 10), timePlacements_PO[sel_PO], 2);
    break;
  case 3:
    blinks(String(m % 10), timePlacements_PO[sel_PO], 2);
    break;
  case 4:
    blinks(tm, timePlacements_PO[sel_PO], 2);
    break;
  }
  keypressed = keypad.getKey();
  if ((keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9'))
  {
    resetCounter1();
    char key = String(keypressed).toInt();

    switch (sel_PO)
    {
    case 0:
      if (key < 2)
      {
        h = setTens(h, key);
      }
      break;
    case 1:
      h = setOnes(h, key);
      break;
    case 2:
      if (key < 6)
      {
        m = setTens(m, key);
      }
      break;
    case 3:
      m = setOnes(m, key);
      break;
    case 4:
      switch (key)
      {
      case 1:
        tm = "am";
        break;
      case 2:
        tm = "pm";
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
  else if (keypressed == 'B')
  {
    resetCounter1();
    sel_PO--;
    if (sel_PO == -1)
    {
      sel_PO = 4;
    }
  }
  else if (keypressed == 'C')
  {
    resetCounter1();
    sel_PO++;
    if (sel_PO == 5)
    {
      sel_PO = 0;
    }
  }
  else if (keypressed == 'D')
  {
    leaveSelectScroll_PO = true;
    return;
  }
  else if (keypressed == 'A')
  {
    if (tm == "am")
    {
      _tm = 0;
    }
    else
    {
      _tm = 1;
    }
    int y = 10000 * _tm + 100 * h + m;
    switch (powerTime)
    {
    case 0:
      EEPROM.update(510, highByte(y));
      EEPROM.update(511, lowByte(y));
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print(F("Power on time"));
      lcd.setCursor(1, 2);
      lcd.print(F("successfully set"));
      delay(1000);
      lcd.clear();
      powerTimeLeave = true;
      return;
    case 1:
      EEPROM.update(508, highByte(y));
      EEPROM.update(509, lowByte(y));
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print(F("Shutdown time"));
      lcd.setCursor(1, 2);
      lcd.print(F("successfully set"));
      delay(1000);
      lcd.clear();
      powerTimeLeave = true;
      return;
    default:
      break;
    }
  }

  if (goBack())
  {
    leaveSelectScroll_PO = true;
    return;
  }
}








