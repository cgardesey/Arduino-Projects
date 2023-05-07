#include <NewPing.h>
#include <SoftwareSerial.h>

#define TRIGGER_PIN  12 
#define ECHO_PIN     11
#define MAX_DISTANCE 400 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
SoftwareSerial mySerial(2, 3); //rx, tx

unsigned long currentMillis;
unsigned long previousMillis = 0;
long interval = 20000 ;
int distance = 0;
String tankid = "1";

void setup() {
  //mySerial.begin(38400); 
  Serial.begin(9600); 
  mySerial.begin(9600); 
  Serial.begin(9600); 
  delay(10000); 
  mySerial.print("AT+CMGF=1\r"); 
  delay(100);
  mySerial.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);
  Serial.println("GSM AT TEST, READY!!!");
  delay(2000);
  gprsInit();

}

void loop() {
  
  currentMillis = millis();               
  unsigned int uS = sonar.ping(); 
  int distance = uS / US_ROUNDTRIP_CM;
  Serial.println(String(distance) + "cm"); 
  
  delay(1000);
  
  if(distance != 0 && currentMillis - previousMillis >= interval)
  {
    upload(distance);
    previousMillis = currentMillis;
  }
  
}

void getResponse()
{
  while (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}

void manageCom()
{
  while (mySerial.available()>0)
  {
    Serial.write(mySerial.read());
  }

  while(Serial.available()>0)
  { 
    mySerial.write(Serial.read());
  }
}


void gprsInit()
{
  mySerial.println("AT\r\n");
  delay(500);
  manageCom();
  mySerial.println("AT+CFUN=1\r\n");
  delay(500);
  manageCom();
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
  delay(500);
  manageCom();
  mySerial.println("AT+CSTT=\"MTN\",\"\",\"\"\r\n");
  delay(3000);
  manageCom();
  mySerial.println("AT+SAPBR=1,1\r\n");
  delay(1000);
  manageCom();
}

void upload(int distance)
{
    Serial.println("uploading to server ******************************");
    mySerial.println(F("AT+HTTPINIT\r\n"));
    delay(500);
    manageCom();
    mySerial.println(F("AT+HTTPPARA=\"CID\",1\r\n"));
    delay(500);
    manageCom();
    mySerial.println("AT+HTTPPARA=\"URL\",\"http://2schooldirect.net:55554/store-measurement?tankid=" + tankid + "&waterlevel=" + String(distance) + "\"");
    delay(3000);
    manageCom();
    mySerial.println(F("AT+HTTPACTION=0\r\n"));
    delay(4000);
    manageCom();
    //getHttpActionCode();

    //Serial.println("***********");
    mySerial.println(F("AT+HTTPREAD\r\n"));
    delay(4000);
    manageCom();
    //getReply();
    mySerial.println(F("AT+HTTPTERM\r\n"));
    delay(500);
    manageCom();
    //delay(60000);
}
