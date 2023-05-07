#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
void setup()
{
mySerial.begin(19200); // the GPRS baud rate
Serial.begin(19200); // the GPRS baud rate
}

void loop()
{
if (mySerial.available())
Serial.write(mySerial.read());
if (Serial.available())
mySerial.write(Serial.read());
}

