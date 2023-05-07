#include <SoftwareSerial.h>
 SoftwareSerial bluetooth(2,3);
void setup() {
  //char state;

 pinMode(4, OUTPUT);
 //pinMode(3, OUTPUT);
 digitalWrite(4, LOW);
 //digitalWrite(3, LOW);
 bluetooth.begin(9600);
 Serial.begin(9600);
}

void loop() {
if(bluetooth.available()>0)
{
  char state=bluetooth.read();
  switch(state){

    case 'A':
    digitalWrite(4, HIGH);
    //Serial.print("LED ON");
    bluetooth.print(" ENGINEER CYRIL IS AT ZAKS ELECTRONICS{}[[{()");
    break;
    case 'a':
    digitalWrite(4, LOW);
    //Serial.print("LED OFF");
    bluetooth.print(" LED OFF");
    break;
//    case 'B':
//    digitalWrite(3, HIGH);
//    Serial.print("LED3 ON");
//    break;
//    case 'b':
//    digitalWrite(3, LOW);
//    Serial.print("LED2 OFF");
//    break;
    default:
    break;
    
    
    }
  
  }

}
