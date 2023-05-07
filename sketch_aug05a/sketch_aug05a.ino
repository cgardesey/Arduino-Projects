#include <EEPROM.h>

int address, dataToWrite, space1, space2;

void setup()
{
	Serial.begin(9600);
}

void loop()
{ 
	String buffer = "";
	buffer = Serial.readString();
	buffer.trim();
	if (buffer.startsWith("read"))
	{
		address = buffer.substring(5).toInt();
		Serial.print("reading data from address: ");
		Serial.println(address);
		Serial.println(EEPROM.read(address));

	}
	else if (buffer.startsWith("write"))
	{
		space1 = buffer.indexOf(' ');
		space2 = buffer.indexOf(' ', space1 + 1);
		address = buffer.substring(space1, space2).toInt();
		dataToWrite = buffer.substring(space2).toInt();
		Serial.print("writing data: ");
		Serial.println(dataToWrite);
		Serial.print("to address: ");
		Serial.println(address);
		EEPROM.write(address, dataToWrite);
	}
}
