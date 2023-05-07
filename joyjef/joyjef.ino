
#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(12, 11, 14, 15, 16, 17);

RTC_DS1307 RTC;

char ZAKS[] = "ZAKS ELECTRONICS";
String menu[5] = { "SET TIME","LIST ALARM TIME", "ADD  ALARM TIME", "EDIT ALARM TIME", "REMOVE ALARM TIME" };
const int controlButtons[5] = { 6, 7, 8, 9, 10 };
const int menuPin = 10;
const int alarmPin = 13;
const int moveUP = 9;
static String action = "add";
const int moveDwon = 8;
const int scroll = 7;
static bool canWrite = true;
const int back = 6;
const int debounceDelay = 250;
volatile uint32_t tot_overflow;
volatile uint32_t tot_overflow_2;
bool cleared = false;
bool addAlarmScreenCleared = false;
bool goHome = false;
static int increment = 0;
static int blinkInt = 0;
static DateTime now;
const int timePlacements[4] = { 6,9,12,14 };
const int timePlacementsAddAlarmTime[3] = { 6, 9, 12 };
const int datePlacements[3] = { 10,13,16 };
const int alarmX[8] = { 1, 1, 1, 1, 11, 11, 11, 11 };
const int alarmY[8] = { 0, 1, 2, 3, 0, 1, 2, 3 };
static int sel = 0;
static int addAlarmTimesel = 0;
static int h, m; static String tm;
static int _tm = 0;
static int temp;
static int index;
int intValue;
byte loByte, hiByte;
static int start = 0;
String TM;
int H, M;
String text[2] = { "Time: ", "Date: " };
static int count = 0;
int alarmTime;

#define eepromSize 32
static int eepromMirror[eepromSize];
static int ans[eepromSize / 2];
static int ansUnsorted[eepromSize / 2];
int timeMode;
static bool counterInitialized = false;
static bool timeUp = false;

String line1 = "JOYJEFF  ACADEMY    JOYJEFF  ACADEMY    JOYJEFF  ACADEMY";
int stringStart, stringStop = 0;
int scrollCursor = 20;

void setup() 
{
  for (int button = 0; button < 5; button++)
    pinMode(controlButtons[button], INPUT);
  pinMode(alarmPin, OUTPUT);
  digitalWrite(alarmPin, HIGH);


  lcd.begin(20, 4);
  Wire.begin();
  RTC.begin();
   systemInit();
  /*attachInterrupt(0, alarm, RISING);*/
  CounterInit();
}

void loop()
{
  now = RTC.now();
  Home();
  alarm();

  if (tot_overflow == 10)
  {
    resetCounter();
  }

  if (digitalRead(menuPin) == HIGH)
  {
    CounterInit();
    lcd.clear();
    while (1)
    {
      menuInterrupt();
      blinks(menu[increment], centerText(menu[increment].length()), (increment % 4));

      if (digitalRead(moveUP) == HIGH)
      {
        resetCounter();
        increment--;
        if (increment == -1)
          increment = 4;
      }

      if (digitalRead(moveDwon) == HIGH)
      {
        resetCounter();
        increment++;
        if (increment == 5)
          increment = 0;
      }

      if (tot_overflow == 6 || digitalRead(back))
      {
        /*TIMSK1 &= ~(1 << TOIE1);*/
        lcd.clear();
        break;
      }

      if (digitalRead(menuPin))
      {
        bool Cleard = false;
        if (!Cleard)
        {
          lcd.clear();
          Cleard = true;
        }
        menuItem(increment);
        if (goHome)
        {
          goHome = false;
          break;
        }
      }
    }
  }
}


String printDigits(byte digits)
{
  if (digits < 10)
    return ('0' + String(digits));
  else
    return String(digits);
  //digits < 10 ? ('0' + String(digits)) : String(digits);
  //return String(digits);
}

String getDayOfWeek(uint8_t dayOfWeek)
{
  switch (dayOfWeek)
  {
  case 1:
    return("MON");
    break;

  case 2:
    return("TUE");
    break;

  case 3:
    return("WED");
    break;

  case 4:
    return("THU");
    break;

  case 5:
    return("FRI");
    break;

  case 6:
    return("SAT");
    break;

  default:
    return("SUN");
    break;
  }
}

void menuInterrupt()
{
  if (!cleared)
  {
    lcd.clear();
    cleared = true;
  }

  if (increment == 4)
  {
    lcd.clear();
    lcd.setCursor((20 - menu[4].length()) / 2, 0);
    lcd.print(menu[4]);
    cleared = false;
  }
  else
  {
    for (size_t i = 0; i < 4; i++)
    {
      lcd.setCursor((20 - menu[i].length()) / 2, i);
      lcd.print(menu[i]);
    }
  }
}

void systemInit()
{
  for (size_t limit = 0; limit < 17; limit++)
  {
    lcd.clear();
    lcd.setCursor(2, 1);
    for (size_t i = 0; i < limit; i++)
    {
      lcd.print(ZAKS[i]);
    }
    if (limit != 5)
      delay(100);
  }

  delay(1000);

  for (size_t i = 0; i < 10; i++)
  {
    if (i % 2)
      lcd.print("ZAKS ELECTRONICS");
    else
      lcd.clear();
    lcd.setCursor(2, 1);
    delay(250);
  }

  delay(1000);

  for (size_t i = 0; i < 18; i++)
  {
    lcd.scrollDisplayLeft();
    delay(100);
  }
  lcd.clear();
  delay(1000);
}

void homeScroll()
{
  lcd.setCursor(scrollCursor, 0);
  lcd.print(line1.substring(stringStart, stringStop));
  lcd.setCursor(1, 1);
  lcd.print("------------------");
  showDateTime(2);
  delay(50);
  lcd.clear();
  if (stringStart == 0 && scrollCursor > 0)
  {
    scrollCursor--;
    stringStop++;
  }
  else if (stringStart == stringStop) 
  {
    stringStart = stringStop = 0;
    scrollCursor = 20;
  }
  else if (stringStop == line1.length() && scrollCursor == 0) 
  {
    stringStart++;
  }
  else 
  {
    stringStart++;
    stringStop++;
  }
}

void home()
{
  lcd.setCursor(2, 0);
  lcd.print("JOYJEFF  ACADEMY");
  lcd.setCursor(1, 1);
  lcd.print("------------------");
  showDateTime(2);
}

void Home()
{
  if (tot_overflow <= 5 && tot_overflow >= 0)
  {
    homeScroll();
  }
  else
  {
    home();
  }
}

void CounterInit()
{
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  tot_overflow = 0;
}

void CounterInit_2()
{
  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;

  TCNT2 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR2B |= (1 << CS12);    // 256 prescaler 
  TIMSK2 |= (1 << TOIE2);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  tot_overflow_2 = 0;
}

void resetCounter()
{
  TCNT1 = 34286;
  tot_overflow = 0;
}

void resetCounter_2()
{
  TCNT2 = 34286;
  tot_overflow_2 = 0;
}

ISR(TIMER1_OVF_vect)          // interrupt service routine 
{
  tot_overflow++;
}

ISR(TIMER2_OVF_vect)          // interrupt service routine 
{
  tot_overflow_2++;
}

String getHour(int hour)
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

int getHour2(int hour)
{
  if (hour > 12)
    hour -= 12;
  if (hour == 0)
    hour = 12;
  return hour;
}

String getMinute(int minute) {

  if (minute < 10)
    return String("0" + String(minute));
  else
    return String(minute);
}

String getSecond(int second) {

  if (second < 10)
    return String("0" + String(second));
  else
    return String(second);
}

String getDay(int day) {

  if (day < 10)
    return String("0" + String(day));
  else
    return String(day);
}

String getTimeMode(int hour)
{
  if (hour  > 12 || hour == 12)
    return(" pm");
  else
    return(" am");
}

void showDateTime(int y)
{
  now = RTC.now();
  lcd.setCursor(0, y);
  lcd.print("Time: ");

  lcd.print(getHour(now.hour()) + ":" + getMinute(now.minute()) + ":" + getSecond(now.second()) + getTimeMode(now.hour()));

  lcd.setCursor(0, (y + 1));
  lcd.print("Date: " + getDayOfWeek(now.dayOfWeek()) + " " + getDay(now.day()) + '/' + printDigits(now.month()) + '/');
  lcd.print(now.year(), DEC);
}

void blinks(String text, int x, int y)
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

void selectScroll(int count)
{
  //DateTime now = RTC.now();
  showDateTime(1);
  if (count == 0)
  {
    switch (sel) {
    case 0:
      blinks(getHour(now.hour()), timePlacements[sel], 1);
      /*RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));*/
      break;
    case 1:
      blinks(getMinute(now.minute()), timePlacements[sel], 1);
      break;
    case 2:
      blinks(getSecond(now.second()), timePlacements[sel], 1);
      break;
    case 3:
      blinks(getTimeMode(now.hour()), timePlacements[sel], 1);
      break;
    }

    if (digitalRead(moveUP) == HIGH)
    {
      resetCounter();
      switch (sel) {
      case 0:
        RTC.adjust(now + TimeSpan(0, 1, 0, 0));
        //blinks(getHour(now.hour()), timePlacements[sel], 1);
        /*RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));*/
        break;
      case 1:
        RTC.adjust(now + TimeSpan(0, 0, 1, 0));
        //blinks(String(now.minute()), timePlacements[sel], 1);
        break;
      case 2:
        RTC.adjust(now + TimeSpan(0, 0, 0, 1));
        //blinks(String(now.second()), timePlacements[sel], 1);
        break;
      case 3:
        if (now.hour() > 11) {
          RTC.adjust(now + TimeSpan(0, -6, 0, 0));
          now = RTC.now();
          RTC.adjust(now + TimeSpan(0, -6, 0, 0));
        }
        else {
          RTC.adjust(now + TimeSpan(0, 6, 0, 0));
          now = RTC.now();
          RTC.adjust(now + TimeSpan(0, 6, 0, 0));
        }
        //blinks(getTimeMode(now.hour()), timePlacements[sel], 1);
        break;
      }
      now = RTC.now();
    }

    if (digitalRead(moveDwon) == HIGH)
    {
      resetCounter();
      switch (sel) {
      case 0:
        RTC.adjust(now + TimeSpan(0, -1, 0, 0));
        //blinks(getHour(now.hour()), timePlacements[sel], 1);
        /*RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));*/
        break;
      case 1:
        RTC.adjust(now + TimeSpan(0, 0, -1, 0));
        //blinks(String(now.minute()), timePlacements[sel], 1);
        break;
      case 2:
        RTC.adjust(now + TimeSpan(0, 0, 0, -1));
        //blinks(String(now.second()), timePlacements[sel], 1);
        break;
      case 3:
        if (now.hour() > 11) {
          RTC.adjust(now + TimeSpan(0, -6, 0, 0));
          now = RTC.now();
          RTC.adjust(now + TimeSpan(0, -6, 0, 0));
        }
        else {
          RTC.adjust(now + TimeSpan(0, 6, 0, 0));
          now = RTC.now();
          RTC.adjust(now + TimeSpan(0, 6, 0, 0));
        }
        //blinks(getTimeMode(now.hour()), timePlacements[sel], 1);
        break;
      }
      now = RTC.now();
    }

    if (digitalRead(scroll))
    {
      resetCounter();
      sel++;
      if (sel == 4)
        sel = 0;
    }
  }

  if (count == 1)
  {
    switch (sel) {

    case 0:
      blinks(getDay(now.day()), datePlacements[sel], 2);
      break;
    case 1:
      blinks(getDay(now.month()), datePlacements[sel], 2);
      break;
    case 2:
      blinks(getDay(now.year()), datePlacements[sel], 2);
      break;
    }


    if (digitalRead(moveUP) == HIGH)
    {
      resetCounter();
      switch (sel) {
      case 0:
        RTC.adjust(now + TimeSpan(1, 0, 0, 0));
        //blinks(getHour(now.hour()), timePlacements[sel], 1);
        /*RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));*/
        break;
      case 1:
        RTC.adjust(DateTime(now.year(), now.month() == 12 ? 1 : (now.month() + 1), now.day(), now.hour(), now.minute(), now.second()));
        //blinks(String(now.minute()), timePlacements[sel], 1);
        break;
      case 2:
        RTC.adjust(DateTime((now.year() + 1), now.month(), now.day(), now.hour(), now.minute(), now.second()));
        //blinks(String(now.second()), timePlacements[sel], 1);
        break;

      }
      now = RTC.now();
    }

    if (digitalRead(moveDwon) == HIGH)
    {
      resetCounter();
      switch (sel) {
      case 0:
        RTC.adjust(now + TimeSpan(-1, 0, 0, 0));
        //blinks(getHour(now.hour()), timePlacements[sel], 1);
        /*RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));*/
        break;
      case 1:
        RTC.adjust(DateTime(now.year(), now.month() == 1 ? 12 : (now.month() - 1), now.day(), now.hour(), now.minute(), now.second()));
        //blinks(String(now.minute()), timePlacements[sel], 1);
        break;
      case 2:
        RTC.adjust(DateTime((now.year() - 1), now.month(), now.day(), now.hour(), now.minute(), now.second()));
        //blinks(String(now.second()), timePlacements[sel], 1);
        break;

      }
      now = RTC.now();
    }

    if (digitalRead(scroll))
    {
      resetCounter();
      sel++;
      if (sel == 3)
        sel = 0;
    }
  }
}

void menuItem(int menuItem)
{
  switch (menuItem)
  {
  case 0:
    resetCounter();
    while (1)
    {
      showDateTime(1);
      blinks(text[count], 0, (count + 1));
      sel = 0;

      if (digitalRead(moveUP) == HIGH)
      {
        resetCounter();
        count--;
        if (count == -1)
          count = 1;
      }

      if (digitalRead(moveDwon) == HIGH)
      {
        resetCounter();
        count++;
        if (count == 2)
          count = 0;
      }

      if (digitalRead(scroll))
      {
        resetCounter();

        while (true)
        {
          selectScroll(count);
          if (digitalRead(menuPin) == HIGH || tot_overflow == 6 || digitalRead(back) == HIGH)
            break;
        }
      }

      if (tot_overflow == 6 || digitalRead(back))
      {
        /*TIMSK1 &= ~(1 << TOIE1);*/
        goHome = true;
        lcd.clear();
        break;
      }
    }
    break;

  case 1:
    resetCounter();
    start = 0;
    while (1)
    {
      if (!isPromEmpty())
      {
        listAlarmTimeScreen();
      }


      else {
        lcd.setCursor(1, 1);
        lcd.print("No alarm time has");
        lcd.setCursor(4, 2);
        lcd.print("been added");
        delay(600);
        lcd.clear();
        break;
      }
      if (tot_overflow == 6 || digitalRead(back))
      {
        /*TIMSK1 &= ~(1 << TOIE1);*/
        goHome = true;
        lcd.clear();
        break;
      }
    }
    break;

  case 2:
    resetCounter();
    start = 0;
    while (1)
    {
      if (!addAlarmScreenCleared)
      {
        lcd.clear();
        addAlarmScreenCleared = true;
      }
      h = 7; m = 0; tm = "am";
      addAlarmTimesel = 0;
      resetCounter();
      while (true)
      {
        action = "add";
        selectScrollAddAlarmTime();
        if (digitalRead(menuPin) == HIGH || tot_overflow == 6 || digitalRead(back) == HIGH)
          break;
      }

      if (tot_overflow == 6 || digitalRead(back))
      {
        /*TIMSK1 &= ~(1 << TOIE1);*/
        goHome = true;
        lcd.clear();
        break;
      }

    }
    break;

  case 3:
    resetCounter();
    sel = 0;
    start = 0;
    action = "edit";
    if (!isPromEmpty())
    {
      while (1)
      {
        listAlarmTimeScreen();
        blinks(getAlarm(sel), alarmX[sel % 8], alarmY[sel % 8]);

        if (digitalRead(moveUP) == HIGH)
        {
          resetCounter();
          if (isNextEntry())
          {
            if (sel == start)
            {
              sel = start + 7;
            }
            else
            {
              sel--;
            }
          }           
        }

        if (digitalRead(moveDwon) == HIGH)
        {
          resetCounter();         
          if (isNextEntry())
          {
            if (sel == (start + 7))
            {
              sel = start;
            }
            else
            {
              sel++;
            }
          }

        }

        if (digitalRead(menuPin) == HIGH)
        {
          resetCounter();

          lcd.clear();
          h = (getAlarm(sel).substring(0, 2)).toInt();
          m = (getAlarm(sel).substring(3, 5)).toInt();
          tm = getAlarm(sel).substring(6);

          while (1) 
          {
            selectScrollAddAlarmTime();
            if (tot_overflow == 6 || digitalRead(back))
            {
              /*TIMSK1 &= ~(1 << TOIE1);*/
              goHome = true;
              lcd.clear();
              break;
            }
          }
        }

        if (tot_overflow == 6 || digitalRead(back))
        {
          /*TIMSK1 &= ~(1 << TOIE1);*/
          goHome = true;
          lcd.clear();
          break;
        }
      }
    }
    break;

  case 4:
    resetCounter();
    sel = 0;
    start = 0;
    action = "remove";
    if (!isPromEmpty()) {
      while (1)
      {
        listAlarmTimeScreen();
        blinks(getAlarm(sel), alarmX[sel % 8], alarmY[sel % 8]);

        if (digitalRead(menuPin) == HIGH)
        {
          resetCounter();
          lcd.clear();
          int selection = getSelection();
          EEPROM[(selection * 2)] = 255;
          EEPROM[((selection * 2) + 1)] = 255;
          while (true) {
            listAlarmTimeScreen();
            if (tot_overflow == 6 || digitalRead(back))
            {
              /*TIMSK1 &= ~(1 << TOIE1);*/
              goHome = true;
              lcd.clear();
              break;
            }
          }

        }

        if (digitalRead(moveUP) == HIGH)
        {
          resetCounter();
          if (isNextEntry())
          {
            if (sel == start)
            {
              sel = start + 7;
            }
            else
            {
              sel--;
            }
          }
        }

        if (digitalRead(moveDwon) == HIGH)
        {
          resetCounter();
          if (isNextEntry())
          {
            if (sel == (start + 7))
            {
              sel = start;
            }
            else
            {
              sel++;
            }
          }

        }

        if (tot_overflow == 6 || digitalRead(back))
        {
          /*TIMSK1 &= ~(1 << TOIE1);*/
          goHome = true;
          lcd.clear();
          break;
        }
      }
    }
    break;
  default:
    break;
  }

}

void selectScrollAddAlarmTime()
{
  showAddTimeScreen();
  switch (addAlarmTimesel)
  {
  case 0:
    blinks(printDigits(h), timePlacementsAddAlarmTime[addAlarmTimesel], 2);
    break;
  case 1:
    blinks(printDigits(m), timePlacementsAddAlarmTime[addAlarmTimesel], 2);
    break;
  case 2:
    blinks(tm, timePlacementsAddAlarmTime[addAlarmTimesel], 2);
    break;
  }

  if (digitalRead(moveUP) == HIGH)
  {
    resetCounter();
    switch (addAlarmTimesel) {
    case 0:
      /*h = 12 ? 1 : h++;*/
      if (h == 12)
        h = 1;
      else
        h++;
      break;
    case 1:
      //m = 60 ? 0 : (m + 1);
      if (m == 59)
        m = 0;
      else
        m++;
      break;
    case 2:
      /*tm = "am" ? "pm" : "am";*/
      if (tm == "am")
        tm = "pm";
      else if (tm == "pm")
        tm = "am";
      break;
    }
  }

  if (digitalRead(moveDwon) == HIGH)
  {
    resetCounter();
    switch (addAlarmTimesel)
    {
    case 0:
      /*h = 1 ? 12 : h--;*/
      if (h == 1)
        h = 12;
      else
        h--;
      break;
    case 1:
      //m = 1 ? 60 : (m - 1);
      if (m == 0)
        m = 59;
      else
        m--;
      break;
    case 2:
      /*tm = "am" ? "pm" : "am";*/
      if (tm == "am")
        tm = "pm";
      else if (tm == "pm")
        tm = "am";
      break;
    }
  }

  if (digitalRead(scroll))
  {
    resetCounter();
    addAlarmTimesel++;
    if (addAlarmTimesel == 3)
      addAlarmTimesel = 0;
  }

  if (digitalRead(menuPin) == HIGH)
  {
    resetCounter();
    lcd.clear();

    if (tm == "am")
      _tm = 0;
    else
      _tm = 1;
    alarmTime = 1000 * h + 10 * m + _tm;
    if (action == "add")
    {
      for (int i = 0; i < eepromSize; i++)
      {
        if (alarmTime == word(EEPROM[i], EEPROM[i + 1]))

        {
          lcd.clear();
          lcd.setCursor(1, 1);
          lcd.print("Alarm time already");
          lcd.setCursor(6, 2);
          lcd.print("exists!");
          delay(600);
          canWrite = false;
          break;
        }
        else
          canWrite = true;
      }

      if (canWrite) {
        for (int i = 0; i < eepromSize; i++)

        {
          if (EEPROM[i] == 255)

          {
            EEPROM[i] = highByte(alarmTime);
            EEPROM[i + 1] = lowByte(alarmTime);
            start = 0;
            while (true)
            {
              listAlarmTimeScreen();
              if (tot_overflow == 6 || digitalRead(back))

              {
                /*TIMSK1 &= ~(1 << TOIE1);*/
                goHome = true;
                lcd.clear();
                return;
              }
            }
            break;
          }
        }
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("System memmory");
        lcd.setCursor(7, 2);
        lcd.print("full!");
        delay(600);
      }
    }
    else if (action == "edit") {

      for (int i = 0; i < eepromSize; i++)
      {
        if (alarmTime == word(EEPROM[i], EEPROM[i + 1]))

        {
          lcd.clear();
          lcd.setCursor(1, 1);
          lcd.print("Alarm time already");
          lcd.setCursor(6, 2);
          lcd.print("exists!");
          delay(600);
          lcd.clear();
          canWrite = false;
          break;
        }
        else
          canWrite = true;
      }

      if (canWrite) {

        int selection = getSelection();

        EEPROM.update((selection * 2), highByte(1000 * h + 10 * m + _tm));
        EEPROM.update(((selection * 2) + 1), lowByte(1000 * h + 10 * m + _tm));
      }
      while (true) {
        listAlarmTimeScreen();
        if (tot_overflow == 6 || digitalRead(back))
        {
          /*TIMSK1 &= ~(1 << TOIE1);*/
          goHome = true;
          lcd.clear();
          break;
        }
      }
    }

  }

}

void listAlarmTimeScreen()
{
  eepromCopy();
  convert(eepromMirror);
  sortEEProm(ans, 16);

  for (int i = start; i < (start + 8); i += 1)
  {
    if (ans[i] == 25200)
      continue;


    switch (i % 8)
    {
    case 0:
      lcd.setCursor(1, 0);
      break;
    case 1:
      lcd.setCursor(1, 1);
      break;
    case 2:
      lcd.setCursor(1, 2);
      break;
    case 3:
      lcd.setCursor(1, 3);
      break;
    case 4:
      lcd.setCursor(11, 0);
      break;
    case 5:
      lcd.setCursor(11, 1);
      break;
    case 6:
      lcd.setCursor(11, 2);
      break;
    case 7:
      lcd.setCursor(11, 3);
      break;
    default:
      break;
    }


    lcd.print(getAlarm(i));
  }

  if (debounce(scroll) && !isPageEmpty())
  {
    resetCounter();
    start += 8;
    sel += 8;
    lcd.clear();
    if (start == eepromSize / 2)
    {
      start = 0;
      sel = 0;
    }
  }
}

void showAddTimeScreen()
{
  lcd.clear();
  lcd.setCursor(1, 1);
  if (action == "edit")
    lcd.print(" Edit Alarm Time:");
  else
    lcd.print("Enter Alarm Time:");
  lcd.setCursor(6, 2);
  lcd.print(printDigits(h) + ":" + printDigits(m) + " " + tm);
}

String getWhiteSpaces(String input)
{
  String result;
  for (int i = 0; i < input.length(); i++) {
    result = (result + " ");
  }

  return result;
}

int centerText(int pos)
{
  return ((20 - menu[increment].length()) / 2);
}

boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  previousState = digitalRead(pin); // store switch state
  for (int counter = 0; counter < debounceDelay; counter++)
  {
    delay(1); // wait for 1 millisecond
    state = digitalRead(pin); // read the pin
    if (state != previousState)
    {
      counter = 0; // reset the counter if the state changes
      previousState = state; // and save the current state
    }
  }
  // here when the switch state has been stable longer than the debounce period
  return state;
}

String getAlarm(int index)
{

  intValue = ans[index];
  H = intValue / 1000;
  M = (intValue - H * 1000) / 10;

  switch (intValue - (H * 1000 + M * 10))
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
  return(getMinute(H) + ":" + getMinute(M) + " " + TM);
}

boolean isPromEmpty()
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    if (EEPROM[i] != 255)
    {
      return false;
    }
  }
  return true;
}

boolean alarmTimeExists(int alarmTime)
{
  for (int i = 0; i < EEPROM.length(); i += 2)
  {
    if (word(EEPROM[i], EEPROM[i + 1]) == alarmTime)
    {
      return true;
    }
  }
  return false;
}

void eepromCopy()
{
  for (int i = 0; i < eepromSize; i++)

  {
    eepromMirror[i] = EEPROM[i];
  }
}

void sortEEProm(int a[], int size)
{

  for (int i = 0; i < size; i++)
  {
    if (a[i] % 10 == 1)
      a[i] += 12590;
  }

  for (int i = 0; i<(size - 1); i++)
  {
    for (int o = 0; o<(size - (i + 1)); o++)
    {
      if (a[o] > a[o + 1])
      {
        int t = a[o];
        a[o] = a[o + 1];
        a[o + 1] = t;
      }
    }
  }
  for (int i = 0; i < size; i++)

  {
    if (a[i] % 10 == 1)
      a[i] -= 12590;
  }
}

void convert(int input[])
{

  for (int i = 0; i < eepromSize; i += 2)
  {
    if (input[i] == 255)
      ansUnsorted[i / 2] = ans[i / 2] = 25200;
    else
      ansUnsorted[i / 2] = ans[i / 2] = word(input[i], input[i + 1]);
  }

}

int getSelection()
{
  for (int i = 0; i < eepromSize / 2; i++) {
    if (ans[sel] == ansUnsorted[i]) {
      return i;
    }
  }
}

boolean isPageEmpty()
{
  for (int i = 8; i < eepromSize / 2; i++) {
    if (ans[i] != 25200)
      return false;
  }
  return true;
}

boolean isNextEntry()
{
  return (!(ans[sel + 1] == 25200));
}

void alarm()
{
  now = RTC.now();

  if (getTimeMode(now.hour()) == " am")
    timeMode = 0;
  else
    timeMode = 1;
  for (int i = 0; i < eepromSize / 2; i++)
  {
    if (ans[i] == (getHour2(now.hour()) * 1000 + (10 * now.minute()) + timeMode))
    {
      digitalWrite(alarmPin, LOW);
      if (counterInitialized == false)
      {
        CounterInit();
        counterInitialized == true;
      }
      return;
    }
  }
  digitalWrite(alarmPin, HIGH);
}
