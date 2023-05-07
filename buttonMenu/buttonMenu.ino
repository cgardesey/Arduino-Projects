const int buttonPin1 = 2;
const int buttonPin2 = 14;
const int buttonPin3 = 15;

const int ledPin1 = 7;
const int ledPin2 = 6;
const int ledPin3 = 3;
const int ledPin4 = 4;
const int ledPin5 = 5;

const int buzzerPin = 9;

uint8_t scrollPos = 0;

const int led[5]={ledPin1, ledPin2, ledPin3, ledPin4, ledPin5};

void setup() {
for (uint8_t x=0; x<5; x++)
{
  pinMode(led[x],OUTPUT);
}

 pinMode(buttonPin1,INPUT_PULLUP);
 pinMode(buttonPin2,INPUT_PULLUP);
 pinMode(buttonPin3,INPUT_PULLUP);
}


void loop() {
  
 if(!digitalRead(buttonPin3))
  {
    delay(200);
    scrollDown();
  }

 if(!digitalRead(buttonPin2))
  {
    delay(200);
    SrollUp();
  }

}

void SrollUp()
{
  if(scrollPos>0)
  {
    digitalWrite(led[scrollPos-1],LOW);
  }
  
    digitalWrite(led[scrollPos],HIGH);
  
  scrollPos++;
  if(scrollPos==6)
  {
    digitalWrite(led[4],LOW);
    scrollPos=0;
  }
}

void scrollDown()
{
  digitalWrite(led[scrollPos],HIGH);

  if(scrollPos==0)
  {
    digitalWrite(led[scrollPos],LOW);
    scrollPos=5;
  }

  if(scrollPos<5){
     digitalWrite(led[scrollPos+1],LOW);
    }
 
  if(scrollPos>0)
  {
      scrollPos--;
  }


}

void scroll_up()
{
  if(scrollPos<5)
   {
    digitalWrite(led[scrollPos],LOW);
    digitalWrite(led[scrollPos],HIGH);
   }
}
