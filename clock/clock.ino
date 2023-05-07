#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>
#include <SevenSeg.h>
#include <IRremote.h>


const int RECV_PIN = 2;
const int BUZZER_PIN = 1;
const int _12H = 16;
const int HYPHEN = A7;
const int COLON = 5;
const int LDR = A6;
const int MANUAL_BRIGHTNESS = 0;

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

#define digit1 10
#define digit2 A3
#define digit3 12
#define digit4 13
#define digit5 14
#define digit6 15

SevenSeg disp(11, 4, 3, 6, 7, 8, 9);

RTC_DS1307 RTC;
static DateTime now;

const int digits = 6;
int digitPins[digits] = { digit1, digit2, digit3, digit4, digit5, digit6 };
int digitPinsRev[digits] = { digit6, digit5, digit4, digit3, digit2, digit1 };
static String time;
uint8_t tot_overflow1, num1 = 9, num2 = 0, num3 = 0, cur_pos = 1, anim_pos = 1, dc = 100;
int day_tens, day_units;

void setup()
{
	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, 1);
	pinMode(_12H, OUTPUT);
	pinMode(LDR, INPUT);
	pinMode(COLON, OUTPUT);
	pinMode(HYPHEN, OUTPUT);
	pinMode(MANUAL_BRIGHTNESS, OUTPUT);
	Wire.begin();
	RTC.begin();
	disp.setDigitPins(digits, digitPins);
	//disp.setCommonCathode();
	//disp.setActivePinState(LOW, HIGH);
	myremote.enableIRIn();
	now = RTC.now();
	RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()));
	//RTC.writeSqwPinMode(SquareWave1HZ);
	if (EEPROM[1] > 100)
	{
		EEPROM.update(1, 100);
	}
	dc = EEPROM[1];
	//counter1 init
	noInterrupts();           // disable all interrupts
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;                // initialize counter
	TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler 
	TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	interrupts();             // enable all interrupts
	tot_overflow1 = 0;
	//Serial.begin(9600);
}

void loop()
{
	homeHelper(true);

	if (now.minute() % 15 == 0 && now.second() < 7)
	{
		disp.write(time);
	}
	else
	{
		disp.setDigitPins(digits, digitPins);
		disp.setCommonCathode();
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

	digitalWrite(HYPHEN, 0);

	if (myremote.decode(&results))
	{
		myremote.resume();
		if (String(results.value).startsWith("167"))
		{
			pulseBuzzer();
		}

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
			resetCounter1();
			digitalWrite(HYPHEN, 1);
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

					buttonPressedHelper();
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
						break;
					}
				}
				if (tot_overflow1 > 57)
				{
					break;
				}
			}
		}
		buttonPressedHelper();
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
			buttonPressedHelper();
			if (results.value == EQ)
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
						else if (key == 0 && EEPROM[0] != 1)
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
					else if (num1 / 10 == 0 && EEPROM[0] != 1)
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
			buttonPressedHelper();
			if (results.value == EQ || results.value == PLAY)
			{
				tot_overflow1 = 58;
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
					day_tens = 2;
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
					day_tens = 2;
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


				if (num2 == 9 || num2 == 4 || num2 == 6 || num2 == 11 && num1 == 31)
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


	if ((now.minute() == 14 || now.minute() == 29) && now.second() == 59)
	{
		time = "";
		anim_pos = 0;
	}
	if (now.minute() == 44 && now.second() == 59)
	{
		time = getHour(now.hour()) + printDigits(now.minute()) + printDigits(now.second());
	}


	if (now.minute() == 15 && now.second() < 7)
	{
		if (time.length() == 6)
		{
			time = "";
			anim_pos = 0;
		}
		anim_pos += 1;
		addOntoTime(anim_pos);

	}
	if (now.minute() == 30 && now.second() < 7)
	{
		disp.setDigitPins(digits, digitPinsRev);
		disp.setCommonCathode();

		if (time.length() == 6)
		{
			time = "";
			anim_pos = 0;
		}
		anim_pos += 1;
		addOntoTime(7 - anim_pos);
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
	if (EEPROM[0] != 1)
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
		time += String(now.hour() / 10);
		break;
	case 2:
		time += String(now.hour() % 10);
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

void buttonPressedHelper()
{
	if (EEPROM[2] != 1)
	{
		if (results.value == PLUS)
		{
			dc = (dc == 100) ? 0 : dc += 10;
			disp.setDutyCycle(dc);
			EEPROM.update(1, dc);
		}
		if (results.value == MINUS)
		{
			dc = (dc == 0) ? 100 : dc -= 10;
			disp.setDutyCycle(dc);
			EEPROM.update(1, dc);
		}
	}
	if (results.value == CHMINUS)
	{
		if (EEPROM[0] != 1)
		{
			EEPROM.update(0, 1);
		}
		else
		{
			EEPROM.update(0, 0);
		}
	}
	if (results.value == HPLUS)
	{
		dc = 50;
		disp.setDutyCycle(dc);
	}
	if (results.value == TWOHPLUS)
	{
		dc = 100;
		disp.setDutyCycle(dc);
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
		if (EEPROM[2] != 1)
		{
			EEPROM.update(2, 1);
		}
		else
		{
			EEPROM.update(2, 0);
		}
	}
}

void homeHelper(uint8_t is_time)
{
	//separator
	if (is_time)
	{
		now = RTC.now();
		if (now.second() % 2 == 0)
		{
			if (EEPROM[2] != 1)
			{
				analogWrite(COLON, map(dc, 0, 100, 0, 1023));
			}
			else
			{
				analogWrite(COLON, analogRead(LDR));
			}
		}
		else
		{
			analogWrite(COLON, 0);
		}
	}
	else
	{
		//digitalWrite(COLON, 0);
		analogWrite(COLON, 0);
	}

	//time mode
	if (EEPROM[0] != 1)
	{
		digitalWrite(_12H, 1);
	}
	else
	{
		digitalWrite(_12H, 0);
	}

	//brightness adjustment mode
	if (EEPROM[2] != 1)
	{
		digitalWrite(MANUAL_BRIGHTNESS, 1);
	}
	else
	{
		digitalWrite(MANUAL_BRIGHTNESS, 0);
	}

	//Adjust brightness automatically
	if (EEPROM[2] != 1)
	{
		disp.setDutyCycle(dc);
	}
	else
	{
		
		disp.setDutyCycle((int)map(analogRead(LDR), 0, 1023, 0, 100) / 10 * 10);
		//Serial.println((int)map(analogRead(LDR), 0, 1023, 0, 100) / 10 * 10);
	}
}
