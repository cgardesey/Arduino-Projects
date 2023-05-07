#include <Arduino.h>
#line 1
#line 1 "C:\\Users\\Cyril\\Documents\\Arduino\\clock\\clock.ino"
#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>
#include <SevenSeg.h>
#include <IRremote.h>

#define RECV_PIN           2
#define BUZZER_PIN         1  
#define _12H               16
#define COLON              5
#define LDR            	   A6
#define MANUAL_BRIGHTNESS  20


IRrecv myremote(RECV_PIN);
decode_results results;

#define ZERO         16738455
#define ONE          16724175
#define TWO          16718055
#define THREE        16743045
#define FOUR         16716015
#define FIVE         16726215
#define SIX          16734885
#define SEVEN        16728765
#define EIGHT        16730805
#define NINE         16732845
#define MINUS        16769055
#define PLUS         16754775
#define PREVIOUS     16720605
#define NEXT         16712445
#define EQ           16748655
#define CH           16736925
#define CHMINUS      16753245
#define CHPLUS 	     16769565
#define HPLUS        16750695
#define TWOHPLUS     16756815
#define PLAY         16761405

#define H_FORMAT            0
#define DC                  1
#define B_ADJ_MODE          2
#define SCREEN              3

#define MANUAL              0
#define AUTO                1
#define _12_H               0
#define _24_H               1

				     
#define TIME_SCREEN         0
#define DATE_SCREEN         1
#define STOPWATCH_SCREEN    2
#define COUNTDOWN_SCREEN    3


#define digit1 10
#define digit2 A3
#define digit3 12
#define digit4 13
#define digit5 14
#define digit6 15

SevenSeg disp(3, 4, 11, 6, 7, 8, 9);

RTC_DS1307 RTC;
static DateTime now;

unsigned long currentMillis, previousMillis;
const int digits = 6;
int digitPins[digits] = { digit1, digit2, digit3, digit4, digit5, digit6 };
static String time;
int tot_overflow1, num1 = 0, num2 = 0, num3 = 0, cur_pos = 1, anim_pos = 1, dc = 100;
int day_tens, day_units;
uint8_t prev_sec, current_sec;
bool timer_started;
bool leave = true;
bool alarm_on;

void setup();
void loop();
void displayTime();
void displayDate();
void displayCounter();
void stopWatch();
void CountDown();
void setTime();
void setCountDown();
void setDate();
int blink(String time);
uint8_t setTens(uint8_t timeParam, uint8_t key);
uint8_t setOnes(uint8_t timeParam, uint8_t key);
void resetCounter1();
int numberValue();
String printDigits(byte digits);
String getHour(uint8_t hour);
int getIntHour(uint8_t hour);
int setDayLimits(int month, int year);
int setHourLimits();
void cursor();
void pulseBuzzer();
void addOntoTime(uint8_t anim_pos);
void setCharAt(uint8_t anim_pos);
void buttonPressedHelper(bool on_timer_screen);
void homeHelper(bool on_time_screen);
void alarm();
#line 80
void setup()
{
	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, 1);
	pinMode(_12H, OUTPUT);
	pinMode(LDR, INPUT);
	pinMode(COLON, OUTPUT);
	pinMode(MANUAL_BRIGHTNESS, OUTPUT);
	Wire.begin();
	RTC.begin();
	disp.setDigitPins(digits, digitPins);
	disp.setCommonCathode();
	//disp.setActivePinState(HIGH, HIGH);
	disp.setRefreshRate(200);
	myremote.enableIRIn();
	now = RTC.now();
	RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()));
	if (EEPROM[DC] > 100)
	{
		EEPROM.update(DC, 100);
	}
	dc = EEPROM[DC];

	if (EEPROM[SCREEN] > 3)
	{
		EEPROM.update(SCREEN, 0);
	}

	//counter1 init
	noInterrupts();           // disable all interrupts
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;                // initialize counter
	TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler 
	TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	interrupts();             // enable all interrupts
	tot_overflow1 = 0;
}

void loop()
{
	switch (EEPROM[SCREEN])
	{
	case DATE_SCREEN:
		resetCounter1();
		while (true)
		{
			homeHelper(false);
			displayDate();
			if (myremote.decode(&results))
			{
				myremote.resume();
				if (String(results.value).startsWith("167"))
				{
					pulseBuzzer();
				}

				buttonPressedHelper(true);
				if (results.value == PREVIOUS)
				{
					cur_pos = 1;
					setDate();
				}
				if (results.value == NEXT)
				{
					cur_pos = 6;
					setDate();
				}
				if (results.value == PLAY)
				{
					return;
				}
			}
		}
		break;
	case STOPWATCH_SCREEN:
		num1 = num2 = num3 = 0;
		now = RTC.now();
		prev_sec = current_sec = now.second();
		timer_started = false;
		leave = false;
		alarm_on = false;
		while (true)
		{
			homeHelper(false);
			displayCounter();
			if (timer_started)
			{
				stopWatch();
			}
			if (alarm_on)
			{
				if (now.second() % 2 == 0)
				{
					disp.write("      ");
				}
				else
				{
					displayCounter();
				}
			}

			if (myremote.decode(&results))
			{
				myremote.resume();
				if (String(results.value).startsWith("167"))
				{
					pulseBuzzer();
					alarm_on = false;
				}

				buttonPressedHelper(true);
				if (results.value == EQ)
				{
					timer_started = !timer_started;
				}
				if (results.value == PLAY)
				{
					return;					
				}
			}
		}
		break;
	case COUNTDOWN_SCREEN:
		num1 = num2 = num3 = 0;
		timer_started = false;
		alarm_on = false;
		cur_pos = 1;
		setCountDown();
		while (true)
		{
			now = RTC.now();
			if (leave)
			{
				return;
			}
			homeHelper(true);
			if (alarm_on)
			{
				if (now.second() % 2 == 0)
				{
					disp.write("      ");
				}
				else
				{
					displayCounter();
				}
			}
			else
			{
				displayCounter();
			}

			if (timer_started)
			{
				CountDown();
			}
			if (alarm_on)
			{
				alarm();
			}
			if (myremote.decode(&results))
			{
				myremote.resume();
				if (String(results.value).startsWith("167"))
				{
					pulseBuzzer();
					alarm_on = false;
				}

				buttonPressedHelper(true);
				if (results.value == EQ)
				{
					timer_started = !timer_started;
				}
				if (results.value == CHMINUS)
				{
					cur_pos = 1;
					setCountDown();
				}
				if (results.value == PREVIOUS)
				{
					cur_pos = 1;
					setCountDown();
				}
				if (results.value == NEXT)
				{
					cur_pos = 6;
					setCountDown();
				}
				if (results.value == PLAY)
				{
					return;
				}
			}
		}
		break;
	default:
		resetCounter1();
		while (true)
		{
			homeHelper(true);

			if (now.minute() % 15 == 0 && now.second() < 7)
			{
				disp.write(time);
			}
			else
			{
				displayTime();
			}

			if (now.minute() == 0 && now.second() < 3)
			{
				digitalWrite(BUZZER_PIN, 1);
			}
			else
			{
				digitalWrite(BUZZER_PIN, 0);
			}
			if (myremote.decode(&results))
			{
				myremote.resume();
				if (String(results.value).startsWith("167"))
				{
					pulseBuzzer();
				}

				buttonPressedHelper(false);
				if (results.value == PREVIOUS)
				{
					cur_pos = 1;
					setTime();
				}
				if (results.value == NEXT)
				{
					cur_pos = 6;
					setTime();
				}
				if (results.value == PLAY)
				{
					return;
				}
				if (results.value == EQ)
				{
					if (now.hour() > 11)
					{
						RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() - 12, now.minute(), now.second()));
					}
					else
					{
						RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() + 12, now.minute(), now.second()));
					}
					now = RTC.now();
				}				
			}
		}
		break;
	}
}

void displayTime()
{
	disp.write(getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second()));
}

void displayDate()
{
	now = RTC.now();
	disp.write(printDigits(now.day()) + printDigits(now.month()) + String(now.year()).substring(2));
}

void displayCounter()
{
	disp.write(printDigits(num1) + printDigits(num2) + printDigits(num3));
}

void stopWatch()
{
	now = RTC.now();

	currentMillis = millis();
	if (currentMillis - previousMillis > 99)
	{
		previousMillis = currentMillis;
		num3 += 10;
	}

	current_sec = now.second();
	if (prev_sec != current_sec)
	{
		num3 = 0;
		prev_sec = current_sec;
		++num2;
		if (num2 == 60)
		{
			num2 = 0;
			++num1;
			num1 = (num1 == 100) ? 0 : num1;
			if (num1 == 100)
			{
				num1 = 0;
				timer_started = false;
				alarm_on = true;
			}
		}
	}
}

void CountDown()
{
	now = RTC.now();
	current_sec = now.second();
	if (prev_sec != now.second())
	{
		prev_sec = current_sec;
		if (num3 != 0)
		{
			--num3;
			if (num1 == 0 && num2 == 0 && num3 == 0)
			{
				timer_started = false;
				alarm_on = true;
			}
		}
		else
		{
			num3 = 59;
			if (num2 != 0)
			{
				--num2;
			}
			else
			{
				num2 = 59;
				if (num1 != 0)
				{
					--num1;
				}
				else
				{
					num1 = 0;
				}
			}
		}
	}
}

void setTime()
{
	resetCounter1();
	now = RTC.now();
	while (true)
	{
		num1 = getHour(now.hour()).toInt();
		num2 = now.minute();
		num3 = now.second();
		if (myremote.decode(&results))
		{
			myremote.resume();
			if (String(results.value).startsWith("167"))
			{
				pulseBuzzer();
			}

			cursor();
			buttonPressedHelper(false);
			if (results.value == EQ)
			{
				results.value = 0;
				break;
			}
			if (results.value == PLAY)
			{
				break;
			}
			if (results.value == ZERO || results.value == ONE || results.value == TWO || results.value == THREE || results.value == FOUR || results.value == FIVE || results.value == SIX || results.value == SEVEN || results.value == EIGHT || results.value == NINE)
			{
				resetCounter1();
				uint8_t key = numberValue();

				switch (cur_pos)
				{
				case 1:
					setHourLimits();
					if (key < day_tens + 1)
					{
						if (key == day_tens)
						{
							if (num1 % 10 < day_units + 1)
							{
								num1 = setTens(num1, key);
							}
						}
						else if (key == 0 && EEPROM[H_FORMAT] != _24_H)
						{
							if (num1 % 10 != 0)
							{
								num1 = setTens(num1, key);
							}
						}
						else
						{
							num1 = setTens(num1, key);
						}
					}
					break;
				case 2:
					setHourLimits();
					if (num1 / 10 == day_tens)
					{
						if (key < day_units + 1)
						{
							num1 = setOnes(num1, key);
						}
					}
					else if (num1 / 10 == 0 && EEPROM[H_FORMAT] != _24_H)
					{
						if (key != 0)
						{
							num1 = setOnes(num1, key);
						}
					}
					else
					{
						num1 = setOnes(num1, key);
					}
					break;
				case 3:
					if (key < 6)
					{
						num2 = setTens(num2, key);
					}
					break;
				case 4:
					num2 = setOnes(num2, key);
					break;
				case 5:
					if (key < 6)
					{
						num3 = setTens(num3, key);
					}
					break;
				case 6:
					num3 = setOnes(num3, key);
					break;
				default:
					break;
				}
				RTC.adjust(DateTime(now.year(), now.month(), now.day(), num1, num2, num3));
				now = RTC.now();
			}
		}
		homeHelper(true);
		if (tot_overflow1 > 57)
		{
			break;
		}
		now = RTC.now();
		blink(getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second()));
	}
}

void setCountDown()
{
	resetCounter1();
	while (true)
	{
		if (myremote.decode(&results))
		{
			myremote.resume();
			if (String(results.value).startsWith("167"))
			{
				pulseBuzzer();
			}

			cursor();
			buttonPressedHelper(true);
			if (results.value == EQ)
			{
				timer_started = true;
				break;
			}
			if (results.value == ZERO || results.value == ONE || results.value == TWO || results.value == THREE || results.value == FOUR || results.value == FIVE || results.value == SIX || results.value == SEVEN || results.value == EIGHT || results.value == NINE)
			{
				resetCounter1();
				uint8_t key = numberValue();

				switch (cur_pos)
				{
				case 1:
					num1 = setTens(num1, key);
					break;
				case 2:
					setHourLimits();
					num1 = setOnes(num1, key);
					break;
				case 3:
					if (key < 6)
					{
						num2 = setTens(num2, key);
					}
					break;
				case 4:
					num2 = setOnes(num2, key);
					break;
				case 5:
					if (key < 6)
					{
						num3 = setTens(num3, key);
					}
					break;
				case 6:
					num3 = setOnes(num3, key);
					break;
				default:
					break;
				}
			}
			if (results.value == PLAY)
			{
				leave = true;
				break;
			}
		}
		homeHelper(false);
		if (tot_overflow1 > 57)
		{
			break;
		}
		now = RTC.now();
		blink(printDigits(num1) + printDigits(num2) + printDigits(num3));
	}
}

void setDate()
{
	resetCounter1();
	now = RTC.now();
	while (true)
	{
		num1 = now.day();
		num2 = now.month();
		num3 = now.year() % 100;
		if (myremote.decode(&results))
		{
			myremote.resume();
			if (String(results.value).startsWith("167"))
			{
				pulseBuzzer();
			}

			cursor();
			buttonPressedHelper(false);
			if (results.value == EQ || results.value == PLAY)
			{
				break;
			}
			if (results.value == ZERO || results.value == ONE || results.value == TWO || results.value == THREE || results.value == FOUR || results.value == FIVE || results.value == SIX || results.value == SEVEN || results.value == EIGHT || results.value == NINE)
			{
				resetCounter1();
				uint8_t key = numberValue();

				switch (cur_pos)
				{
				case 1:
					setDayLimits(num2, num3);
					if (key < day_tens + 1)
					{
						if (key == day_tens)
						{
							if (num1 % 10 < day_units + 1)
							{
								num1 = setTens(num1, key);
							}
						}
						else if (key == 0)
						{
							if (num1 % 10 != 0)
							{
								num1 = setTens(num1, key);
							}
						}
						else
						{
							num1 = setTens(num1, key);
						}
					}
					break;
				case 2:
					setDayLimits(num2, num3);
					if (num1 / 10 == day_tens)
					{
						if (key < day_units + 1)
						{
							num1 = setOnes(num1, key);
						}
					}
					else if (num1 / 10 == 0)
					{
						if (key != 0)
						{
							num1 = setOnes(num1, key);
						}
					}
					else
					{
						num1 = setOnes(num1, key);
					}
					break;
				case 3:
					day_tens = 1;
					day_units = 2;
					if (key < day_tens + 1)
					{
						if (key == day_tens)
						{
							if (num2 % 10 < day_units + 1)
							{
								num2 = setTens(num2, key);
							}
						}
						else if (key == 0)
						{
							if (num2 % 10 != 0)
							{
								num2 = setTens(num2, key);
							}
						}
						else
						{
							num2 = setTens(num2, key);
						}
					}
					break;
				case 4:
					day_tens = 1;
					day_units = 2;
					if (num2 / 10 == day_tens)
					{
						if (key < day_units + 1)
						{
							num2 = setOnes(num2, key);
						}
					}
					else if (num2 / 10 == 0)
					{
						if (key != 0)
						{
							num2 = setOnes(num2, key);
						}
					}
					else
					{
						num2 = setOnes(num2, key);
					}
					break;
				case 5:
					num3 = setTens(num3, key);
					break;
				case 6:
					num3 = setOnes(num3, key);
					break;
				default:
					break;
				}


				if ((num2 == 9 || num2 == 4 || num2 == 6 || num2 == 11) && num1 == 31)
				{
					num1 = 30;
				}
				if (num1 > 28 && num2 == 2)
				{
					if ((2000 + num3) % 4 == 0)
					{
						num1 = 29;
					}
					else
					{
						num1 = 28;
					}
				}
				RTC.adjust(DateTime(num3, num2, num1, now.hour(), now.minute(), now.second()));
				now = RTC.now();
			}
		}
		homeHelper(false);
		if (tot_overflow1 > 57)
		{
			break;
		}
		now = RTC.now();
		blink(printDigits(now.day()) + printDigits(now.month()) + String(now.year()).substring(2));
	}
}

int blink(String time)
{
	if (tot_overflow1 % 2)
	{
		time.setCharAt(cur_pos - 1, ' ');
	}
	disp.write(time);
}

uint8_t setTens(uint8_t timeParam, uint8_t key)
{
	return  (10 * key + timeParam % 10);
}

uint8_t setOnes(uint8_t timeParam, uint8_t key)
{
	uint8_t temp = timeParam / 10;
	return  (10 * temp + key);
}

void resetCounter1()
{
	TCNT1 = 0;
	tot_overflow1 = 0;
}

ISR(TIMER1_OVF_vect)
{
	tot_overflow1++;

	if (now.minute() == 14 && now.second() == 59)
	{
		time = getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second());
		anim_pos = 0;
	}
	if (now.minute() == 29 && now.second() == 59)
	{
		time = "      ";
		anim_pos = 7;
	}
	if (now.minute() == 44 && now.second() == 59)
	{
		time = getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second());
	}


	if (now.minute() == 15 && now.second() < 7)
	{
		time.setCharAt(anim_pos, ' ');
		++anim_pos;
		if (anim_pos == 6)
		{
			time = getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second());
			anim_pos = 0;
		}
	}
	if (now.minute() == 30 && now.second() < 7)
	{

		if (anim_pos == 1)
		{
			time = "      ";
			anim_pos = 7;
		}
		anim_pos = anim_pos - 1;
		setCharAt(anim_pos - 1);
	}
	if (now.minute() == 45 && now.second() < 7)
	{
		time.remove(time.length() - 1);
		if (time.length() == 0)
		{
			time = getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second());
		}
	}
	if (now.minute() == 00 && now.second() < 7)
	{
		if (time == "      ")
		{
			time = getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second());
		}
		else
		{
			time = "      ";
		}

	}

	if (tot_overflow1 == 250)
	{
		resetCounter1();
	}
}

int numberValue()
{
	switch (results.value)
	{
	case ZERO:
		return 0;
	case ONE:
		return 1;
	case TWO:
		return 2;
	case THREE:
		return 3;
	case FOUR:
		return 4;
	case FIVE:
		return 5;
	case SIX:
		return 6;
	case SEVEN:
		return 7;
	case EIGHT:
		return 8;
	case NINE:
		return 9;
		break;
	default:
		break;
	}
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

String getHour(uint8_t hour)
{
	if (EEPROM[H_FORMAT] != _24_H)
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

int getIntHour(uint8_t hour)
{
	if (EEPROM[H_FORMAT] != _24_H)
	{
		if (hour > 12)
		{
			hour -= 12;
		}
		else if (hour == 0)
		{
			return(hour + 12);
		}
	}
	return (hour);
}

int setDayLimits(int month, int year)
{
	if (month == 2)
	{
		day_tens = 2;
		if ((2000 + year) % 4 == 0)
		{
			day_units = 9;
		}
		else
		{
			day_units = 8;
		}
	}
	else if (month == 9 || month == 4 || month == 6 || month == 11)
	{
		day_tens = 3;
		day_units = 0;
	}
	else
	{
		day_tens = 3;
		day_units = 1;
	}
}

int setHourLimits()
{
	if (EEPROM[H_FORMAT] != _24_H)
	{
		day_tens = 1;
		day_units = 2;
	}
	else
	{
		day_tens = 2;
		day_units = 3;
	}
}

void cursor()
{
	if (results.value == PREVIOUS)
	{
		resetCounter1();
		cur_pos = (cur_pos == 1) ? 6 : --cur_pos;
	}
	if (results.value == NEXT)
	{
		resetCounter1();
		cur_pos = (cur_pos == 6) ? 1 : ++cur_pos;
	}
}

void pulseBuzzer()
{
	digitalWrite(BUZZER_PIN, 1);
	delay(50);
	digitalWrite(BUZZER_PIN, 0);
}

void addOntoTime(uint8_t anim_pos)
{
	switch (anim_pos)
	{
	case 1:
		time += String((getIntHour(now.hour()) / 10));
		break;
	case 2:
		time += String((getIntHour(now.hour()) % 10));
		break;
	case 3:
		time += String(now.minute() / 10);
		break;
	case 4:
		time += String(now.minute() % 10);
		break;
	case 5:
		time += String(now.second() / 10);
		break;
	case 6:
		time += String(now.second() % 10);
		break;
	default:
		break;
	}
}

void setCharAt(uint8_t anim_pos)
{
	switch (anim_pos)
	{
	case 5:
		time.setCharAt(anim_pos, 48 + (now.second() % 10));
		break;
	case 4:
		time.setCharAt(anim_pos, 48 + (now.second() / 10));
		break;
	case 3:
		time.setCharAt(anim_pos, 48 + (now.minute() % 10));
		break;
	case 2:
		time.setCharAt(anim_pos, 48 + (now.minute() / 10));
		break;
	case 1:
		time.setCharAt(anim_pos, 48 + (now.hour() % 10));
		break;
	case 0:
		time.setCharAt(anim_pos, 48 + (getIntHour(now.hour()) / 10));
		break;
	default:
		break;
	}
}

void buttonPressedHelper(bool on_timer_screen)
{
	if (EEPROM[B_ADJ_MODE] != AUTO)
	{
		if (results.value == PLUS)
		{
			dc = (dc == 100) ? 0 : dc += 10;
			disp.setDutyCycle(dc);
			EEPROM.update(DC, dc);
		}
		if (results.value == MINUS)
		{
			dc = (dc == 0) ? 100 : dc -= 10;
			disp.setDutyCycle(dc);
			EEPROM.update(DC, dc);
		}
	}
	if (results.value == CHMINUS)
	{
		if (on_timer_screen)
		{
			num1 = num2 = num3 = 0;
			timer_started = false;
		}
		else
		{
			if (EEPROM[H_FORMAT] != _24_H)
			{
				EEPROM.update(H_FORMAT, _24_H);
			}
			else
			{
				EEPROM.update(H_FORMAT, _12_H);
			}
		}
	}
	if (results.value == HPLUS)
	{
		dc = 50;
		disp.setDutyCycle(dc);
		EEPROM.update(DC, dc);
	}
	if (results.value == TWOHPLUS)
	{
		dc = 100;
		disp.setDutyCycle(dc);
		EEPROM.update(DC, dc);
	}
	if (results.value == CH)
	{
		digitalWrite(MANUAL_BRIGHTNESS, 0);
		digitalWrite(_12H, 0);
		digitalWrite(COLON, 0);
		disp.clearDisp();
		while (true)
		{
			if (myremote.decode(&results))
			{
				myremote.resume();
				if (String(results.value).startsWith("167"))
				{
					pulseBuzzer();
				}

				if (results.value == CH)
				{
					break;
				}
			}
		}
	}
	if (results.value == CHPLUS)
	{
		if (EEPROM[B_ADJ_MODE] != AUTO)
		{
			EEPROM.update(B_ADJ_MODE, AUTO);
		}
		else
		{
			EEPROM.update(B_ADJ_MODE, MANUAL);
		}
	}
	if (results.value == PLAY)
	{
		if (EEPROM[SCREEN] == 2)
		{
			EEPROM.update(SCREEN, 0);
		}
		else
		{
			EEPROM.update(SCREEN, ++EEPROM[SCREEN]);
		}
	}
}

void homeHelper(bool on_time_screen)
{
	//separator
	if (on_time_screen)
	{
		now = RTC.now();
		if (now.second() % 2 == 0)
		{
			if (EEPROM[B_ADJ_MODE] != AUTO)
			{
				analogWrite(COLON, map(dc, 0, 100, 1, 255 / 6));
			}
			else
			{
				analogWrite(COLON, 1023 - analogRead(LDR));
			}
		}
		else
		{
			analogWrite(COLON, 0);
		}
	}
	else
	{
		if (EEPROM[B_ADJ_MODE] != AUTO)
		{
			analogWrite(COLON, map(dc, 0, 100, 1, 255 / 6));
		}
		else
		{
			analogWrite(COLON, 1023 - analogRead(LDR));
		}
	}

	//time mode
	if (on_time_screen && EEPROM[H_FORMAT] != _24_H)
	{
		if (now.hour() > 11)
		{
			if (now.second() % 2 == 0)
			{
				digitalWrite(_12H, 1);
			}
			else
			{
				digitalWrite(_12H, 0);
			}
		}
		else
		{
			digitalWrite(_12H, 1);
		}
	}
	else
	{
		digitalWrite(_12H, 0);
	}

	//brightness adjustment mode
	if (EEPROM[B_ADJ_MODE] != AUTO)
	{
		digitalWrite(MANUAL_BRIGHTNESS, 1);
	}
	else
	{
		digitalWrite(MANUAL_BRIGHTNESS, 0);
	}

	//Adjust brightness automatically
	if (EEPROM[B_ADJ_MODE] != AUTO)
	{
		disp.setDutyCycle(dc);
	}
	else
	{
		disp.setDutyCycle((int)map(1023 - analogRead(LDR), 0, 1023, 0, 100) / 10 * 10);
	}
}

void alarm()
{
	current_sec = now.second();
	if (current_sec != prev_sec)
	{
		prev_sec = current_sec;
		digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
	}
}

