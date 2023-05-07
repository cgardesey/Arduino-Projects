#include <IRremote.h>
#include <IRremoteInt.h>

#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);

int RECV_PIN = 11; //output of ir receiver connects here
int led1 = 6;
int led2 = 5;
int led3 = 4;
int led4 = 3;
int led5 = 2;

int itsONled[] = {0,0,0,0,0,0};// to keep the initial state of the leds low

#define key1  16748655 //FF906F // code received from button 1
#define key2  16758855 //FFB847 // code received from button 2
#define key3  16775175 //FFF807 // code received from button 3
#define key4  16756815 //FFB04F // code received from button 4
#define key5  16750695 //FF9867 // code received from button 5

IRrecv irrecv(RECV_PIN);
 
decode_results results;

 
void setup()
{
  Serial.begin(19200);
  // set up the digital pins to control
  irrecv.enableIRIn();
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

void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+233266854047\"");                                     // recipient's mobile number, in international format
  delay(100);
  SIM900.println("System Reconfigured.");        // message to send
  delay(100);
  SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                     // give module time to send SMS
  SIM900power();                                   // turn off module
}

void receiveIR()
{
   if (irrecv.decode(&results)) {
    unsigned int value = results.value;
    switch(value) {
       case key1:
         if(itsONled[1] == 1) {        // if first led is on then
            digitalWrite(led1, LOW);   // turn it off when button is pressed
            itsONled[1] = 0;           // and set its state as off
         } else {                      // else if first led is off
             digitalWrite(led1, HIGH); // turn it on when the button is pressed
             itsONled[1] = 1;          // and set its state as on
         }
          break; 
       case key2:
         if(itsONled[2] == 1) {
            digitalWrite(led2, LOW);
            itsONled[2] = 0;
         } else {
             digitalWrite(led2, HIGH);
             itsONled[2] = 1;
         }
          break;
       case key3:
         if(itsONled[3] == 1) {
            digitalWrite(led3, LOW);
            itsONled[3] = 0;
         } else {
             digitalWrite(led3, HIGH);
             itsONled[3] = 1;
         }
          break;          

          case key4:
         if(itsONled[4] == 1) {
            digitalWrite(led4, LOW);
            itsONled[3] = 0;
         } else {
             digitalWrite(led4, HIGH);
             itsONled[4] = 1;
         }
          break; 

         case key5:
         if(itsONled[5] == 1) {
            digitalWrite(led5, LOW);
            itsONled[5] = 0;
         } else {
             digitalWrite(led5, HIGH);
             itsONled[5] = 1;
         }
          break;          
    }
    //Serial.println(value); // you can comment this line
    irrecv.resume(); // Receive the next value
  }
  }
 
void loop() 
{
  //If a character comes in from the cellular module...
  if(SIM900.available() >0)
  {
    inchar=SIM900.read(); 
    if (inchar=='#')
    {
      delay(10);
 
      inchar=SIM900.read(); 
      if (inchar=='a')
      {
        delay(10);
        inchar=SIM900.read();
        if (inchar=='0')
        {
          digitalWrite(led1, LOW);
        } 
        else if (inchar=='1')
        {
          digitalWrite(led1, HIGH);
        }
        delay(10);
        inchar=SIM900.read(); 
        if (inchar=='b')
        {
          inchar=SIM900.read();
          if (inchar=='0')
          {
            digitalWrite(led2, LOW);
          } 
          else if (inchar=='1')
          {
            digitalWrite(led2, HIGH);
          }
          delay(10);
          inchar=SIM900.read(); 
          if (inchar=='c')
          {
            inchar=SIM900.read();
            if (inchar=='0')
            {
              digitalWrite(led3, LOW);
            } 
            else if (inchar=='1')
            {
              digitalWrite(led3, HIGH);
            }
            delay(10);
            inchar=SIM900.read(); 

            
            if (inchar=='d')
            {
              delay(10);
              inchar=SIM900.read();
              if (inchar=='0')
              {
                digitalWrite(led4, LOW);
              } 
              else if (inchar=='1')
              {
                digitalWrite(led4, HIGH);
              }
              delay(10);

              inchar=SIM900.read(); 

            
            if (inchar=='e')
            {
              delay(10);
              inchar=SIM900.read();
              if (inchar=='0')
              {
                digitalWrite(led5, LOW);
              } 
              else if (inchar=='1')
              {
                digitalWrite(led5, HIGH);
              }
              delay(10);
            }
          }

          sendSMS();
          SIM900.println("AT+CMGD=1,4"); // delete all SMS
        }
        }
      }
    }
  }

  receiveIR();
}
