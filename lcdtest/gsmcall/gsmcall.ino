#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // configure software serial port
 
void setup()
{
  SIM900.begin(19200);               
  SIM900power();  
  delay(20000);  // give time to log on to network. 
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
}
 
void callSomeone()
{
  SIM900.println("ATD + +233546797793;"); // dial US (212) 8675309
  delay(100);
  SIM900.println();
  delay(30000);            // wait for 30 seconds...
  SIM900.println("ATH");   // hang up
}

void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+233268317286\"");                                     // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Hello, world. This is a text message from an Arduino Uno.");        // message to send
  delay(100);
  SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                     // give module time to send SMS
  SIM900power();                                   // turn off module
}
 
void loop()
{
  callSomeone(); // call someone
  //sendSMS();
  SIM900power();   // power off GSM shield
  do {} while (1); // do nothing
}
