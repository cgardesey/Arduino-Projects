// The SparkFun MG2639 Cellular Shield uses SoftwareSerial
// to communicate with the MG2639 module. Include that
// library first:
#include <SoftwareSerial.h>
// Include the MG2639 Cellular Shield library
#include <SFE_MG2639_CellShield.h>
// This example also requires the Phant Arduino library.
// Download the library from our GitHub repo:
// https://github.com/sparkfun/phant-arduino
//#include <Phant.h>

#include <Wire.h> //I2C needed for sensors
#include "SparkFunMPL3115A2.h" //Pressure sensor - Search "SparkFun MPL3115" and install from Library Manager
#include "SparkFunHTU21D.h" //Humidity sensor - Search "SparkFun HTU21D" and install from Library Manager

MPL3115A2 myPressure; //Create an instance of the pressure sensor
HTU21D myHumidity; //Create an instance of the humidity sensor

/////////////////////////////////////////////////////////////
const byte STAT_BLUE = 12;
const byte STAT_GREEN = 13;
/////////////////////////////////////////////////////////////
const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte BATT = A2;
////////////////////////////////////////////////

float humidity;
float temp_h;
float tempf;
float light_lvl;
float batt_lvl;
float pressure;
float batt1=6.31, hum1=65.14, temp1=84.54, lit1=0.01;
long lastSecond; //The millis counter to see when a second rolls by
//IPAddress myIP; // IPAddress to store the local IP

/////////////////////
// Phant Constants //
/////////////////////
// Phant detsination server:
//const char server[] = "data.sparkfun.com";
// Phant public key:
//const char publicKey[] = "DJjNowwjgxFR9ogvr45Q";
//// Phant private key:
//const char privateKey[] = "P4eKwGGek5tJVz9Ar84n";
// Create a Phant object, which we'll use from here on:

//const char publicKey[] = "jqNYyZqVawTDY3QnQ6dr";
// Phant private key:
//const char privateKey[] = "zzZgyvzj6pIrxyq9qYwm";
//Phant phant(server, publicKey, privateKey);

//int a=20, b=30, c=40, d=50;

void setup() 
{
//  int status;
//  Serial.begin(9600);
//  
//  // To turn the MG2639 shield on, and verify communication
//  // always begin a sketch by calling cell.begin().
//  status = cell.begin();
//  if (status <= 0)
//  {
//    Serial.println(F("Unable to communicate with shield. Looping"));
//    while(1)
//      ;
//  }
//  
//  // Then call gprs.open() to establish a GPRS connection.
//  // gprs.open() can take upwards of 30 seconds to connect,
//  // so be patient.
//  while( gprs.open() <= 0)
//  {
//    delay(2000);
//  }
//  
//  // Get our assigned IP address and print it:
//  myIP = gprs.localIP();
//  Serial.print(F("My IP address is: "));
//  Serial.println(myIP);
  
  Serial.println(F("Weather Shield Example"));
  Serial.println(F("Starting the weather module............."));
  pinMode(STAT_BLUE, OUTPUT); //Status LED Blue
  pinMode(STAT_GREEN, OUTPUT); //Status LED Green

  pinMode(REFERENCE_3V3, INPUT);
  pinMode(LIGHT, INPUT);
  
  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
  myHumidity.begin(); //Configure the humidity sensor
  lastSecond = millis();

  pinMode(10,OUTPUT);//The default digital driver pins for the GSM and GPS mode
  pinMode(11,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  delay(1500);
  digitalWrite(5,LOW);
 
  digitalWrite(10,LOW);//Enable GSM mode
  digitalWrite(11,HIGH);//Disable GPS mode
  delay(2000);
  Serial.begin(115200); 
  delay(5000);//GPS ready
  Serial.println(F("Weather Shield online!"));

/////////////////////////////////////////////////////////////////////////////////////

  //Serial.println(F());
}

void loop()
{
//  // If data has been sent over a TCP link:
//  if (gprs.available())
//  {  // Print it to the serial monitor:
//    Serial.write(gprs.read());
//  }
//  // If a character has been received over serial:
//  if (Serial.available())
//  {
//    // Post to Phant!
//    postToPhant();
//    // Then clear the serial buffer:
//    while (Serial.available())
//      Serial.read();
//  }

  //if (millis() - lastSecond >= 1000)//Print readings every second
  //{
    digitalWrite(STAT_BLUE, HIGH); //Blink stat LED
     lastSecond += 1000;
     humidity = myHumidity.readHumidity();//Check Humidity Sensor
    if (humidity == 998) //Humidty sensor failed to respond
    {
      Serial.println(F("I2C communication to sensors is not working. Check solder connections."));
      
      //Try re-initializing the I2C comm and the sensors
      myPressure.begin(); 
      myPressure.setModeBarometer();
      myPressure.setOversampleRate(7);
      myPressure.enableEventFlags();
      myHumidity.begin();
    }
    else
    {
//      Serial.print(F("Humidity = "));
//      Serial.print(humidity);
//      Serial.print(F("%,")); 
     temp_h = myHumidity.readTemperature();
//      Serial.print(F(" temp_h = "));
//      Serial.print(temp_h, 2);
//      Serial.print(F("C,"));
//
//      //Check Pressure Sensor
      pressure = myPressure.readPressure();
//      Serial.print(F(" Pressure = "));
//      Serial.print(pressure);
//      Serial.print(F("Pa,"));
//
//      //Check tempf from pressure sensor
      tempf = myPressure.readTempF();
//      Serial.print(F(" temp_p = "));
//      Serial.print(tempf, 2);
//      Serial.print(F("F,"));
//
//      //Check light sensor
      light_lvl = get_light_level();
//      Serial.print(F(" light_lvl = "));
//      Serial.print(light_lvl);
//      Serial.print(F("V,"));
//
//      //Check batt level
     batt_lvl = get_battery_level();
//      Serial.print(F(" VinPin = "));
//      Serial.print(batt_lvl);
//      Serial.print(F("V"));
//      Serial.println();
}

    digitalWrite(STAT_BLUE, LOW); //Turn off stat LED
      delay(100);
  // If data has been sent over a TCP link:
  Serial.println(F("sending to thingspeak"));
  sendToThingspeak();

}

//void postToPhant()
//{
//  // Open GPRS. It's OK to call this if GPRS is already
//  // open, it'll just return super-quickly.
//  byte status = gprs.open();
//  if (status <= 0)
//  {
//    Serial.print(F("Couldn't open GPRS"));
//    return;
//  }
//  Serial.println(F("GPRS open!"));
//  
//  // Connect to the Phant server (data.sparkfun.com) on
//  // port 80.
//  status = gprs.connect(server, 80);
//  if (status <= 0)
//  {
//    Serial.println(F("Error connecting."));
//    return;
//  }
//  Serial.print(F("Connected to "));
//  Serial.println(server);
//  
//  // Set up our Phant post using the Phant library. For
//  // each field in the Phant stream we need to call
//  // phant.add([field], value).
//  // Value can be any data type, in this case we're only
//  // using integers.
//  phant.add(F("batt"), batt1);
//  phant.add(F("hum"), hum1);
//  phant.add(F("lit"), lit1);
//  phant.add(F("tem"), temp1);
//  //phant.add(F("analog4"), analogRead(A4));
//  //phant.add(F("analog5"), analogRead(A5));
//  // Storing fields in flash (F()) will save a good chunk
//  // of RAM, which is very precious.
//  
//  Serial.println(F("Posting to Phant!"));
//  // Encapsulate a phant.post() inside a gprs.print(). 
//  // phant.post() takes care of everything in the HTTP header
//  // including newlines.
//  gprs.print(phant.post());
//  
//  /*
//  // You can close here, but then won't be able to read any
//  // responses from the server in the loop().
//  status = gprs.close();
//  if (status <= 0)
//  {
//    Serial.println(F("Unable to close GPRS. Looping"));
//  }
//  */
//}

float get_light_level()
{
  float operatingVoltage = analogRead(REFERENCE_3V3);

  float lightSensor = analogRead(LIGHT);

  operatingVoltage = 3.3 / operatingVoltage; //The reference voltage is 3.3V

  lightSensor = operatingVoltage * lightSensor;

  return (lightSensor);
}
//Returns the voltage of the raw pin based on the 3.3V rail
//This allows us to ignore what VCC might be (an Arduino plugged into USB has VCC of 4.5 to 5.2V)
//Battery level is connected to the RAW pin on Arduino and is fed through two 5% resistors:
//3.9K on the high side (R1), and 1K on the low side (R2)
float get_battery_level()
{
  float operatingVoltage = analogRead(REFERENCE_3V3);

  float rawVoltage = analogRead(BATT);

  operatingVoltage = 3.30 / operatingVoltage; //The reference voltage is 3.3V

  rawVoltage = operatingVoltage * rawVoltage; //Convert the 0 to 1023 int to actual voltage on BATT pin

  rawVoltage *= 4.90; //(3.9k+1k)/1k - multiple BATT voltage by the voltage divider to get actual system voltage

  return (rawVoltage);
}

void sendToThingspeak()
{
//  digitalWrite(2, LOW); // ENABLE GSM SERIAL
//  digitalWrite(3, HIGH); // DISABLE GPS SERIAL

  delay(2000);
  

  Serial.println("AT+CPIN?");
  delay(1000);
  


  Serial.println("AT+CREG?");
  delay(1000);
  

  Serial.println("AT+CGATT?");
  delay(1000);


  Serial.println("AT+CIPSHUT");
  delay(1000);
  

  Serial.println("AT+CIPSTATUS");
  delay(2000);
  

  Serial.println("AT+CIPMUX=0");
  delay(2000);
 

  Serial.println("AT+CSTT=\"MTN\"");
  delay(1000);
 

  Serial.println("AT+CIICR");
  delay(3000);


  Serial.println("AT+CIFSR");
  delay(2000);
 

  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\", \"80\"");
  delay(6000);


  Serial.println("AT+CIPSEND");
  delay(4000);
 

  //Serial.println("GET http://api.thingspeak.com/update?key=W68QUVX7MAZT3NXY&field1=" + String(lati));
  Serial.println("GET http://api.thingspeak.com/update?key=AIQSYJHQBZFH7TM2&field1=" + String(humidity) +"&field2=" + String(tempf) + "&field3=" +String(temp_h)+ "&field4=" +String(pressure)+ "&field5=" +String(light_lvl)+ "&field6=" +String(batt_lvl));//+ "&field7=" +String(rainin));
  delay(4000);

  Serial.println((char)26);
  delay(5000);
 

  Serial.println("AT+CIPSHUT");
  delay(100);
}


