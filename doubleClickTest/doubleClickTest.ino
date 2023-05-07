#define CLICK 10
#define PRESS 20


const int buttonPin = 2;
const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;
const int buzzerPin = 9;

uint8_t pressCount ;
int klick = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  
pressCount = clik();

if(pressCount == CLICK)
  {
   Serial.println("Button Clicked");
   digitalWrite(ledPin1, !digitalRead(ledPin1));
  }
  
if(pressCount == PRESS)
  {
   Serial.println("Button Pressed");
   digitalWrite(ledPin2, !digitalRead(ledPin2));
  }
   
}

uint8_t clik()
{
  if(!digitalRead(buttonPin))
  {
   klick++;
   delay(1);
       if(digitalRead(buttonPin))
       {
          if(klick>0 && klick < 400)
          {
            //digitalWrite(ledPin1, !digitalRead(ledPin1));
            klick=0;
            return CLICK;
          }

          if(klick>700)
          {
            klick=0;
            return PRESS;
          }

          else 
            return 0;

           
       }
  }
  
}

void blinck(uint8_t times, int rate)
{
  for(uint8_t x=0; x<times; x++)
  {
    digitalWrite(ledPin1,1);
    delay(rate);
    digitalWrite(ledPin1,0);
    delay(rate);
    
  }
  
}

void beep()
{
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
}
