#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);
 
int led1 = 6;
int led2 = 5;
int led3 = 4;
int led4 = 3;
int led5 = 2;
 
void setup()
{
  Serial.begin(19200);
  // set up the digital pins to control
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
   digitalWrite(led5, LOW);
 
  // wake up the GSM shield
  SIM900power(); 
  SIM900.begin(19200);
  delay(20000);  // give time to log on to network.
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
  Serial.println("Ready...");
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}
 
void loop() 
{
  //If a character comes in from the cellular module...
  if(SIM900.available() >0)
  {
    inchar=SIM900.read();
    delay(10);
    if(inchar=='a')
    {
      delay(10);

      inchar=SIM900.read();
      if(inchar=='1')
      {
        digitalWrite(led1, HIGH);
        Serial.println("LED ON");
      }

      else if(inchar=='0')
      {
        digitalWrite(led1, LOW);
        Serial.println("LED OFF");
      }
    }
    
    if(inchar=='b')
    {
       delay(10);

      inchar=SIM900.read();
      if(inchar=='1')
      {
        digitalWrite(led2, HIGH);
        Serial.println("LED ON");
      }

      else if(inchar=='0')
      {
        digitalWrite(led2, LOW);
        Serial.println("LED OFF");
      }
    }
    SIM900.println("AT+CMGD=1,4"); 
  }
}

