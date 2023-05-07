/*
 Weather Shield Example
 By: Nathan Seidle
 SparkFun Electronics
 Date: June 10th, 2016
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example prints the current humidity, air pressure, temperature and light levels.

 The weather shield is capable of a lot. Be sure to checkout the other more advanced examples for creating
 your own weather station.

 */
#include <SoftwareSerial.h>
#include <SFE_MG2639_CellShield.h>
#include <Phant.h>



#include <Wire.h> //I2C needed for sensors
#include "SparkFunMPL3115A2.h" //Pressure sensor - Search "SparkFun MPL3115" and install from Library Manager
#include "SparkFunHTU21D.h" //Humidity sensor - Search "SparkFun HTU21D" and install from Library Manager

MPL3115A2 myPressure; //Create an instance of the pressure sensor
HTU21D myHumidity; //Create an instance of the humidity sensor

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//const byte STAT_BLUE = 7;
//const byte STAT_GREEN = 8;
/////////////////////////////////////////////////////////////
const byte STAT_BLUE = 12;
const byte STAT_GREEN = 13;
/////////////////////////////////////////////////////////////
const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte BATT = A2;
////////////////////////////////////////////////
const char server[] = "api.thingspeak.com";
float humidity;
float temp_h;
float tempf;
float light_lvl;
float batt_lvl;
float pressure;
////////////////////////////////////////////////
//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by
IPAddress myIP; 

void setup()
{
  Serial.begin(9600);
  Serial.println("Weather Shield Example");

  pinMode(STAT_BLUE, OUTPUT); //Status LED Blue
  pinMode(STAT_GREEN, OUTPUT); //Status LED Green

  pinMode(REFERENCE_3V3, INPUT);
  pinMode(LIGHT, INPUT);
  
  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
  myHumidity.begin();//Configure the humidity sensor
  lastSecond = millis();
  Serial.println("Weather Shield online!");

//  //////////////////////////////////////////////////////////////////////////////////////////////from phant example
//  int status;
//  status = cell.begin();//turn on shield and verfy communication
//  if (status <= 0)
//  {
//    Serial.println(F("Unable to communicate with shield. Looping"));
//    while(1)
//      ;
//  }
//  
//  while( gprs.open() <= 0)//establish connection 
//  {
//    delay(2000);
//  }
//  myIP = gprs.localIP(); // Get our assigned IP address and print it:
//  Serial.print(F("My IP address is: "));
//  Serial.println(myIP);
//  Serial.println(F("Press any key to post to Phant!"));/////////******//////////////////////////
//  //////////////////////////////////////////////////////////////////////////////////////////////
}

void loop()
{
  
  if (millis() - lastSecond >= 1000)//Print readings every second
  {
    digitalWrite(STAT_BLUE, HIGH); //Blink stat LED
     lastSecond += 1000;
     float humidity = myHumidity.readHumidity();//Check Humidity Sensor
    if (humidity == 998) //Humidty sensor failed to respond
    {
      Serial.println("I2C communication to sensors is not working. Check solder connections.");
      
      //Try re-initializing the I2C comm and the sensors
      myPressure.begin(); 
      myPressure.setModeBarometer();
      myPressure.setOversampleRate(7);
      myPressure.enableEventFlags();
      myHumidity.begin();
    }
    else
    {
      Serial.print("Humidity = ");
      Serial.print(humidity);
      Serial.print("%,"); 
      float temp_h = myHumidity.readTemperature();
      Serial.print(" temp_h = ");
      Serial.print(temp_h, 2);
      Serial.print("C,");

      //Check Pressure Sensor
      float pressure = myPressure.readPressure();
      Serial.print(" Pressure = ");
      Serial.print(pressure);
      Serial.print("Pa,");

      //Check tempf from pressure sensor
      float tempf = myPressure.readTempF();
      Serial.print(" temp_p = ");
      Serial.print(tempf, 2);
      Serial.print("F,");

      //Check light sensor
      float light_lvl = get_light_level();
      Serial.print(" light_lvl = ");
      Serial.print(light_lvl);
      Serial.print("V,");

      //Check batt level
      float batt_lvl = get_battery_level();
      Serial.print(" VinPin = ");
      Serial.print(batt_lvl);
      Serial.print("V");
      Serial.println();
      postToThingspeak();
    }

    digitalWrite(STAT_BLUE, LOW); //Turn off stat LED
  }

  delay(100);
}
//Returns the voltage of the light sensor based on the 3.3V rail
//This allows us to ignore what VCC might be (an Arduino plugged into USB has VCC of 4.5 to 5.2V)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void postToThingspeak()
{
  // Open GPRS. It's OK to call this if GPRS is already
  // open, it'll just return super-quickly.
  byte status = gprs.open();
  if (status <= 0)
  {
    Serial.print(F("Couldn't open GPRS"));
    return;
  }
  Serial.println(F("GPRS open!"));
  
  // Connect to the Phant server (data.sparkfun.com) on
  // port 80.
  status = gprs.connect(server, 80);
  if (status <= 0)
  {
    Serial.println(F("Error connecting."));
    return;
  }
  Serial.print(F("Connected to "));
  Serial.println(server);
  gprs.print("AT+ZIPSEND=80,100\r");
  delay(4000);
  gprs.print("GET http://api.thingspeak.com/update?key=GSU96L72FD1JBLK2&field1=" + String(humidity) +"&field2=" + String(temp_h) + "&field3=" +String(pressure)+"&field4=" + String(tempf) + "&field5=" +String(light_lvl)+ "&field6=" + String(batt_lvl));
  delay(5000);
  gprs.print((char)26);
  delay(5000);
  gprs.print("AT+ZPPPCLOSE\r");
 }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
