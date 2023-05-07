#include<LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const uint8_t valve = 6;
const uint8_t RedLed = 7;
const uint8_t YeLLed = 8;
const uint8_t GreenLed = 9;
const uint8_t buzz = 10;
int sms_count_1 = 0;
int sms_count_2 = 0;
int sensorValue;

void setup()
{
  Serial.begin(9600);     //Setting baud rate of GSM Module 
  pinMode(RedLed, OUTPUT);
  pinMode(YeLLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(valve, OUTPUT);

  digitalWrite(buzz, 1);
  delay(100);
  digitalWrite(buzz, 0);
  lcd.begin(16, 2);
  lcd.setCursor(6, 0);
  lcd.print("LPG ");
  delay(100000000);
}

void loop()
{

  sensorValue = analogRead(A0);
  //sensorValue = 300;
  //Serial.print(sensorValue);
  //Serial.println(" ppm");

  if (sensorValue < 400)
  {
    digitalWrite(YeLLed, LOW);
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, HIGH);
    digitalWrite(valve, HIGH);
    lcd.setCursor(3, 1);
    lcd.print("NO LEAKAGE  ");
  }


  else if ((sensorValue >= 400) && (sensorValue <= 600))
  {
    digitalWrite(YeLLed, HIGH);
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, LOW);
    digitalWrite(valve, HIGH);
    beep(1500);
    lcd.setCursor(3, 1);
    lcd.print("LOW LEAKAGE  ");

  }

  else if (sensorValue >= 600)
  {
    digitalWrite(RedLed, HIGH);
    digitalWrite(YeLLed, LOW);
    digitalWrite(GreenLed, LOW);
    digitalWrite(valve, LOW);
    beep(50);
    lcd.setCursor(3, 1);
    lcd.print("HIGH LEAKAGE  ");
    send_sms1();
    send_sms2();
  }
}

void send_sms1()
{
  if (sms_count_1<1)
  {
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");    //To send SMS in Text Mode
    delay(1000);
    Serial.print("AT+CMGS=\"+233501358919\"\r"); // change to the phone number you using 
    delay(1000);
    Serial.print("Gas Leaking!");//the content of the message
    delay(1000);
    Serial.write(0x1A);//the stopping character
    delay(1000);
    sms_count_1++;
  }
}

void send_sms2()
{
  if (sms_count_2<1)
  {
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");    //To send SMS in Text Mode
    delay(1000);
    Serial.print("AT+CMGS=\"+233262451066\"\r"); // change to the phone number you using 
    delay(1000);
    Serial.print("Gas Leaking!");//the content of the message
    delay(1000);
    Serial.write(0x1A);//the stopping character
    delay(1000);
    sms_count_2++;
  }
}

void beep(unsigned char delayms)
{
  digitalWrite(buzz, 1);
  delay(delayms);
  digitalWrite(buzz, 0);
  delay(delayms);
}


