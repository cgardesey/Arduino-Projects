#include <NewPing.h>
#include <SoftwareSerial.h>



#define TRIGGER_PIN1  2  
#define ECHO_PIN1     3

#define TRIGGER_PIN2  4
#define ECHO_PIN2     5

#define TRIGGER_PIN3  6
#define ECHO_PIN3     7

#define TRIGGER_PIN4  8
#define ECHO_PIN4     9

unsigned int distance1;
unsigned int distance2;
unsigned int distance3;
unsigned int distance4;
#define MAX_DISTANCE 200 

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN4, ECHO_PIN4, MAX_DISTANCE);

void setup()
{
	Serial.begin(9600);
}

void loop() {
  delay(50);                      
  unsigned int uS1 = sonar1.ping(); 
  distance1 = uS1 / US_ROUNDTRIP_CM;
 /* Serial.print("distance1: ");
  Serial.print(distance1); 
  Serial.println("cm");*/
 

  delay(50);                      
  unsigned int uS2 = sonar2.ping();
  distance2 = uS2 / US_ROUNDTRIP_CM; 
  /*Serial.print("distance2: ");
  Serial.print(distance2); 
  Serial.println("cm");*/
 

  delay(50);                      
  unsigned int uS3 = sonar3.ping();
  distance3 =  uS3 / US_ROUNDTRIP_CM;
  /*Serial.print("distance3: ");
  Serial.print(distance3); 
  Serial.println("cm");*/
 


  delay(50);                      
  unsigned int uS4 = sonar4.ping();
  distance4 = uS4 / US_ROUNDTRIP_CM;  
  /*Serial.print("distance4: ");
  Serial.print(distance4); 
  Serial.println("cm");*/
  delay(1000);

  if(distance1 < 6)
	  Serial.print("*20#"); 
   else 			 
      Serial.print("*21#");


   if(distance2 < 6)
      Serial.print("*30#");
       else 		 
      Serial.print("*31#");

   
   if(distance3 < 6)
      Serial.print("*40#");
       else 		 
      Serial.print("*41#");

    
   if(distance4 < 6)
      Serial.print("*50#");
    else 			 
	  Serial.print("*51#");
   
  }
