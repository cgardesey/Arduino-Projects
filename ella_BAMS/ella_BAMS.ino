#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const byte button = 10;

char inchar;
String instring;
String subString;

int id;

void setup()
{
	pinMode(button, INPUT_PULLUP);
	Serial.begin(9600);
	finger.begin(57600);
	lcd.begin(16, 2);

	if (finger.verifyPassword())
	{

		lcd.print(F("  Fingerprint   "));
		lcd.setCursor(0, 1);
		lcd.print(F(" sensor found!  "));
		delay(2500);
	}
	else
	{
		lcd.print(F("  Fingerprint   "));
		lcd.setCursor(0, 1);
		lcd.print(F("sensor not found"));
		while (1);
	}
}

void loop()
{
	lcd.setCursor(0, 0);
	lcd.print(F(" System idle... "));
	lcd.setCursor(0, 1);
	lcd.print(F("                "));
	if (Serial.available() > 0)
	{
		lcd.clear();
		inchar = Serial.read();
		//delay(10);
		if (inchar == '*')
		{
			instring = "";
			while (inchar != '#')
			{
				inchar = Serial.read();
				//delay(10);
				if (inchar != '#')
				{
					instring += inchar;
				}
			}
			instring.trim();
		}

		//digitalWrite((int)instring.charAt(0) - 48, (int)instring.charAt(1) - 48);
		action();
	}

	if (digitalRead(10) == LOW)
	{
		while (1)
		{
			if (getFingerprintID() == 0 || digitalRead(10) == LOW)
			{
				delay(800);
				break;

			}
		}

	}
}

void action()
{
	switch (instring.charAt(0))
	{
	case 'E':
		enrollStudent();
		break;
	case 'D':
		id = instring.substring(1).toInt();
		deleteFingerprint(id);
		break;
	default:
		//enrollStudent();
		break;
	}
}

void enrollStudent()
{
	id = instring.substring(1).toInt();

	while (getFingerprintEnroll() != 0);
}

uint8_t getFingerprintEnroll()
{

	int p = -1;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Enrolling"));
	lcd.setCursor(0, 1);
	lcd.print(F("student ID #"));
	lcd.print(id);
	delay(2500);
	while (p != FINGERPRINT_OK)
	{
		p = finger.getImage();
		switch (p)
		{
		case FINGERPRINT_OK:
			lcd.setCursor(0, 0);
			lcd.print(F("  Image taken   "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			delay(2500);
			break;
		case FINGERPRINT_NOFINGER:
			lcd.setCursor(0, 0);
			lcd.print(F("      ...       "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			lcd.setCursor(0, 0);
			lcd.print(F(" Communication  "));
			lcd.setCursor(0, 1);
			lcd.print(F("      error     "));
			break;
		case FINGERPRINT_IMAGEFAIL:
			lcd.setCursor(0, 0);
			lcd.print(F(" Imaging error  "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			break;
		default:
			lcd.setCursor(0, 0);
			lcd.print(F(" Unknown error  "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			break;
		}
	}

	// OK success!

	p = finger.image2Tz(1);
	switch (p)
	{
	case FINGERPRINT_OK:
		lcd.setCursor(0, 0);
		lcd.print(F("Image converted "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		break;
	case FINGERPRINT_IMAGEMESS:
		lcd.setCursor(0, 0);
		lcd.print(F("Image too messy "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("     error      "));
		delay(2500);
		return p;
	case FINGERPRINT_FEATUREFAIL:
		lcd.setCursor(0, 0);
		lcd.print(F("  FP features   "));
		lcd.setCursor(0, 1);
		lcd.print(F("   not found    "));
		delay(2500);
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		lcd.setCursor(0, 0);
		lcd.print(F("  FP features   "));
		lcd.setCursor(0, 1);
		lcd.print(F("   not found    "));
		delay(2500);
		return p;
	default:
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}
	lcd.setCursor(0, 0);
	lcd.print(F(" Remove finger  "));
	lcd.setCursor(0, 1);
	lcd.print(F("                "));
	delay(2500);
	p = 0;
	while (p != FINGERPRINT_NOFINGER) {
		p = finger.getImage();
	}
	//lcd.print(F("Student ID #"));
	//lcd.print(id);
	p = -1;
	lcd.setCursor(0, 0);
	lcd.print(F("   Place same  "));
	lcd.setCursor(0, 1);
	lcd.print(F("  finger again  "));
	delay(2500);
	while (p != FINGERPRINT_OK) {
		p = finger.getImage();
		switch (p)
		{
		case FINGERPRINT_OK:
			lcd.setCursor(0, 0);
			lcd.print(F("  Image taken   "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			delay(2500);
			break;
		case FINGERPRINT_NOFINGER:
			lcd.setCursor(0, 0);
			lcd.print(F("      ...       "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			lcd.setCursor(0, 0);
			lcd.print(F(" Communication  "));
			lcd.setCursor(0, 1);
			lcd.print(F("      error     "));
			break;
		case FINGERPRINT_IMAGEFAIL:
			lcd.setCursor(0, 0);
			lcd.print(F(" Imaging error  "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			break;
		default:
			lcd.setCursor(0, 0);
			lcd.print(F(" Unknown error  "));
			lcd.setCursor(0, 1);
			lcd.print(F("                "));
			break;
		}
	}

	// OK success!

	p = finger.image2Tz(2);
	switch (p)
	{
	case FINGERPRINT_OK:
		lcd.setCursor(0, 0);
		lcd.print(F("Image converted "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		break;
	case FINGERPRINT_IMAGEMESS:
		lcd.setCursor(0, 0);
		lcd.print(F("Image too messy "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("     error      "));
		delay(2500);
		return p;
	case FINGERPRINT_FEATUREFAIL:
		lcd.setCursor(0, 0);
		lcd.print(F("  FP features   "));
		lcd.setCursor(0, 1);
		lcd.print(F("   not found    "));
		delay(2500);
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		lcd.setCursor(0, 0);
		lcd.print(F("  FP features   "));
		lcd.setCursor(0, 1);
		lcd.print(F("   not found    "));
		delay(2500);
		return p;
	default:
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}

	// OK converted!
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Creating model  "));  
	lcd.setCursor(0, 1);
	lcd.print(F("    for id      "));
	lcd.print(id);

	p = finger.createModel();
	if (p == FINGERPRINT_OK) {
		lcd.setCursor(0, 0);
		lcd.print(F("Prints matched! "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("      error     "));
		delay(2500);
		return p;
	}
	else if (p == FINGERPRINT_ENROLLMISMATCH) {
		lcd.setCursor(0, 0);
		lcd.print(F("Fingerprints    "));
		lcd.setCursor(0, 1);
		lcd.print(F(" did not match  "));
		delay(2500);
		return p;
	}
	else {
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}

	/*lcd.print(F("Student Id "));
	lcd.print(id);*/
	p = finger.storeModel(id);
	if (p == FINGERPRINT_OK) {
		lcd.setCursor(0, 0);
		lcd.print(F("     Stored!    "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		/*instring = "*E";
		instring += id;
		instring += "#";
		Serial.print(instring);*/
		return p;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("      error     "));
		delay(2500);
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		lcd.setCursor(0, 0);
		lcd.print(F("Could not store "));
		lcd.setCursor(0, 1);
		lcd.print(F("in that location"));
		delay(2500);
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Error writing  "));
		lcd.setCursor(0, 1);
		lcd.print(F("    to flash    "));
		delay(2500);
		return p;
	}
	else {
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}
}

uint8_t deleteFingerprint(uint8_t id) {
	uint8_t p = -1;

	p = finger.deleteModel(id);

	if (p == FINGERPRINT_OK) {
		lcd.setCursor(0, 0);
		lcd.clear();
		lcd.print(F("FP with id #"));
		lcd.print(String(id));
		lcd.setCursor(0, 1);
		lcd.print(F("   Deleted!     "));
		delay(2500);
		/*instring = "*D";
		instring += id;
		instring += "#";
		Serial.print(instring);*/
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("      error     "));
		delay(2500);
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		lcd.setCursor(0, 0);
		lcd.print(F("Could not delete"));
		lcd.setCursor(0, 1);
		lcd.print(F("in that location"));
		delay(2500);
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Error writing  "));
		lcd.setCursor(0, 1);
		lcd.print(F("    to flash    "));
		delay(2500);
		return p;
	}
	else {
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}
}

uint8_t getFingerprintID() {
	uint8_t p = finger.getImage();
	switch (p) {
	case FINGERPRINT_OK:
		lcd.setCursor(0, 0);
		lcd.print(F("  Image taken   "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		break;
	case FINGERPRINT_NOFINGER:
		lcd.setCursor(0, 0);
		lcd.print(F("  Place finder   "));
		lcd.setCursor(0, 1);
		lcd.print(F("                 "));
		//delay(2500);
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("     error      "));
		delay(2500);
		return p;
	case FINGERPRINT_IMAGEFAIL:
		lcd.setCursor(0, 0);
		lcd.print(F("     Imaging    "));
		lcd.setCursor(0, 1);
		lcd.print(F("      error     "));
		delay(2500);
		return p;
	default:
		lcd.setCursor(0, 0);
		lcd.print(F("     Unknown    "));
		lcd.setCursor(0, 1);
		lcd.print(F("      error     "));
		delay(2500);
		return p;
	}

	// OK success!

	p = finger.image2Tz();
	switch (p)
	{
	case FINGERPRINT_OK:
		lcd.setCursor(0, 0);
		lcd.print(F("Image converted "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		break;
	case FINGERPRINT_IMAGEMESS:
		lcd.setCursor(0, 0);
		lcd.print(F("Image too messy "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("     error      "));
		delay(2500);
		return p;
	case FINGERPRINT_FEATUREFAIL:
		lcd.setCursor(0, 0);
		lcd.print(F("  FP features   "));
		lcd.setCursor(0, 1);
		lcd.print(F("   not found    "));
		delay(2500);
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		lcd.setCursor(0, 0);
		lcd.print(F("  FP features   "));
		lcd.setCursor(0, 1);
		lcd.print(F("   not found    "));
		delay(2500);
		return p;
	default:
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}

	// OK converted!
	p = finger.fingerFastSearch();
	if (p == FINGERPRINT_OK) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Found a match! "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		lcd.setCursor(0, 0);
		lcd.print(F(" Communication  "));
		lcd.setCursor(0, 1);
		lcd.print(F("     error      "));
		delay(2500);
		return p;
	}
	else if (p == FINGERPRINT_NOTFOUND) {
		lcd.setCursor(0, 0);
		lcd.print(F("  Did not find  "));
		lcd.setCursor(0, 1);
		lcd.print(F("    a match     "));
		delay(2500);
		return p;
	}
	else {
		lcd.setCursor(0, 0);
		lcd.print(F(" Unknown error  "));
		lcd.setCursor(0, 1);
		lcd.print(F("                "));
		delay(2500);
		return p;
	}

	// found a match!

	lcd.clear();
	lcd.print(F("Found ID #"));
	lcd.print((String(finger.fingerID)));
	lcd.setCursor(0, 1);
	lcd.print(F("with C of"));
	lcd.print((String(finger.confidence)));
	delay(2500);
	/*instring = "*V";
	instring += finger.fingerID;
	instring += "#";
	Serial.println(instring);*/
	Serial.println(finger.fingerID);
	return p;
}

int getFingerprintIDez() {
	uint8_t p = finger.getImage();
	if (p != FINGERPRINT_OK)  return -1;

	p = finger.image2Tz();
	if (p != FINGERPRINT_OK)  return -1;

	p = finger.fingerFastSearch();
	if (p != FINGERPRINT_OK)  return -1;

	// found a match!
	lcd.clear();
	lcd.print(F("Found ID #"));
	lcd.print((String(finger.fingerID)));
	lcd.setCursor(0, 1);
	lcd.print(F("with C of"));
	lcd.print((String(finger.confidence)));
	delay(2500);
	return finger.fingerID;
}
