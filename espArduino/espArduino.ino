#include<SoftwareSerial.h>
char inchar;
SoftwareSerial ESP(2, 3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ESP.begin(115200);
}

void loop() {
  if(ESP.available())
   {
    while(ESP.available())
    {
      inchar = ESP.read();
      Serial.print(inchar);
    }
    
   }

   if(Serial.available())
   {
   // delay(1000);
    String command="";

    while(Serial.available())
    {
      command+=(char)Serial.read();
      delay(250);
    }
       ESP.println(command);
    }

}
