const int buttonPin = 2;
const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;
const int buzzerPin = 9;
#define CLICK  1
#define PRESS  2
uint8_t cnt=0;
int cliks = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
 cnt = klick();
 
//if(cnt==CLICK)
//
//  {
//     Serial.println("CLICK");
//  }

 if(cnt==PRESS)
 {
    Serial.println("PRESS");
 }
}

uint8_t klick()
{
  
  if (!digitalRead(buttonPin))
  {
    delay(1);
    cliks++;
    if (digitalRead(buttonPin))
        {
          Serial.println(cliks);
//          
         if(cliks>10 && cliks <100)
          {
            Serial.println("CLICK");
            cliks=0;
            return CLICK;
}
//             
          if(cliks>800)
          {
            cliks=0;
            return PRESS;
           
          }
//          else 
//          return 0;
             
          cliks=0;
          
        }
        
  }
}
