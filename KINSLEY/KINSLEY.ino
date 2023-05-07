#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);


void setup() {

   Serial.begin(19200);
  // put your setup code here, to run once:
pinMode(6, INPUT);
pinMode(13, OUTPUT);

  // wake up the GSM shield
 // SIM900power(); 
  SIM900.begin(19200);
  delay(20000);  // give time to log on to network.
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
  Serial.println("Ready...");
}


void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+233267894128\"");                                     // recipient's mobile number, in international format
  delay(100);
  SIM900.println(" Intrusion detected at FNF hostel, Ayeduase New Site with coordinates 6*40'24.61\"N 1*33'00.63\"W ");        // message to send
  delay(100);
  SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                     // give module time to send SMS
 // SIM900power();                                   // turn off module
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(6)==0)
  {
    digitalWrite(13, HIGH);
    sendSMS();
  }
  else
  digitalWrite(13, LOW);
  
}


