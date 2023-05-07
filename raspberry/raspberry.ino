#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial SIM900(7, 8); // RX, TX

#define n1_H 0
#define n1_L 1
#define n2_H 2
#define n2_L 3
#define n3_H 4
#define n3_L 5

const uint8_t node1 = 4;
const uint8_t node2 = 5;
const uint8_t node3 = 6;

void setup()
{
	SIM900.begin(19200); // for GSM shield
	SIM900.print("AT+CMGF=1\r");// set SMS mode to text
	pinMode(node1, INPUT);
	pinMode(node2, INPUT);
	pinMode(node3, INPUT);
	/*if (word(EEPROM[n1_H], EEPROM[n1_L]) == 65535)
	{
	writeToEeprom(0, n1_H, n1_L);
	}
	if (word(EEPROM[n2_H], EEPROM[n2_L]) == 65535)
	{
	writeToEeprom(0, n2_H, n2_L);
	}
	if (word(EEPROM[n3_H], EEPROM[n3_L]) == 65535)
	{
	writeToEeprom(0, n3_H, n3_L);
	}*/
}

void loop()
{
	if (digitalRead(node1))
	{
		//writeToEeprom(word(EEPROM[n1_H], EEPROM[n1_L]) + 1, n1_H, n1_L);
		sendSMS(1);
	}
	if (digitalRead(node2))
	{
		//writeToEeprom(word(EEPROM[n2_H], EEPROM[n2_L]) + 1, n2_H, n2_L);
		sendSMS(2);
	}
	if (digitalRead(node3))
	{
		//writeToEeprom(word(EEPROM[n3_H], EEPROM[n3_L]) + 1, n3_H, n3_L);
		sendSMS(3);
	}
}

void sendSMS(int room)
{
	SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
	delay(100);
	//SIM900.println("AT + CMGS = \"+233546676098\"");
	SIM900.println("AT + CMGS = \"+12563694536\"");
	delay(100);

	switch (room)
	{
	case 1:
		SIM900.print("{");
		SIM900.print("\"");
		SIM900.print("detector");
		SIM900.print("\"");
		SIM900.print(":1,");
		SIM900.print("\"");
		SIM900.print("remarks");
		SIM900.print("\"");
		SIM900.print(":");
		SIM900.print("\"");
		SIM900.print("Fire started and spreading");
		SIM900.print("\"");
		SIM900.print("}");
		//SIM900.print(word(EEPROM[n1_H], EEPROM[n1_L]));
		break;
	case 2:
		SIM900.print("{");
		SIM900.print("\"");
		SIM900.print("detector");
		SIM900.print("\"");
		SIM900.print(":2,");
		SIM900.print("\"");
		SIM900.print("remarks");
		SIM900.print("\"");
		SIM900.print(":");
		SIM900.print("\"");
		SIM900.print("Fire started and spreading");
		SIM900.print("\"");
		SIM900.print("}");
		//SIM900.print(word(EEPROM[n2_H], EEPROM[n2_L]));
		break;
	case 3:
		SIM900.print("{");
		SIM900.print("\"");
		SIM900.print("detector");
		SIM900.print("\"");
		SIM900.print(":3,");
		SIM900.print("\"");
		SIM900.print("remarks");
		SIM900.print("\"");
		SIM900.print(":");
		SIM900.print("\"");
		SIM900.print("Fire started and spreading");
		SIM900.print("\"");
		SIM900.print("}");
		//SIM900.print(word(EEPROM[n3_H], EEPROM[n3_L]));
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

void writeToEeprom(int param, int highByteAddr, int lowByteAddr)
{
	EEPROM.write(highByteAddr, highByte(param));
	EEPROM.write(lowByteAddr, lowByte(param));
}