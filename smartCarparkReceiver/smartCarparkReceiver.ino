#include <SoftwareSerial.h>

char inchar;
String instring;
String subString;

void setup()
{
	Serial.begin(9600);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
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

		digitalWrite((int)instring.charAt(0) - 48, (int)instring.charAt(1) - 48);
	}
}
