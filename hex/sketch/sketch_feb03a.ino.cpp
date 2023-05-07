#include <Arduino.h>
#line 1
#line 1 "C:\\Users\\Ayatullah\\Documents\\Arduino\\sketch_feb03a\\sketch_feb03a.ino"
#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3); 
// creates a "virtual" serial port/UART
// connect BT module TX to D2
// connect BT module RX to D3
// connect BT Vcc to 5V, GND to GND
void setup();
void loop();
#line 7
void setup()  
{
  // set digital pin to control as an output
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  delay(1000);
    digitalWrite(9, LOW);
  delay(1000);
  // set the data rate for the SoftwareSerial port
  BT.begin(9600);
  // Send test message to other device
  BT.println("Hello from Arduino");
}
char a; // stores incoming character from other device
void loop() 
{
  if (BT.available())
  // if text arrived in from BT serial...
  {
    a=(BT.read());
    if (a=='1')
    {
      digitalWrite(9, HIGH);
      BT.println("LED on");
    }
    if (a=='2')
    {
      digitalWrite(9, LOW);
      BT.println("LED off");
    }
    if (a=='?')
    {
      BT.println("Send '1' to turn LED on");
      BT.println("Send '2' to turn LED on");
    }   
    // you can add more "if" statements with other characters to add more commands
  }
}

