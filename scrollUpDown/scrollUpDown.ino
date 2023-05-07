const int upButton = 14;
const int downButton = 15;
const int menuButton = 2;

const int led1 = 7;
const int led2 = 6;
const int led3 = 3;
const int led4 = 4;
const int led5 = 5;

const int led[5] = {led1,led2,led3,led4,led5};

int scrollPos=0;

void setup() {
  for(uint8_t x=0; x<5; x++)
  {
    pinMode(led[x],OUTPUT);
  }

  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(menuButton, INPUT_PULLUP);
}

void loop() {
 if(!digitalRead(downButton))
 {
  delay(200 );
  scrollDown();
 }

 if(!digitalRead(upButton))
 {
  delay(200);
  scrollUp();
 }

 if(!digitalRead(menuButton))
 {
  
   while(digitalRead(menuButton))
   {
    blinck(80);
   }
   
 }

}

void scrollDown()
{
  digitalWrite(led[scrollPos],LOW);//off the current led 
  if(scrollPos==0)
  {
    digitalWrite(led[scrollPos+4],HIGH);//if the previous led was the first, ON the last 
  }
  else
  {
    digitalWrite(led[scrollPos-1],HIGH);//on the next lower led
  }
  
  scrollPos--;
  if(scrollPos==-1)
  {
    scrollPos=4;
  }
}

void scrollUp()
{
  digitalWrite(led[scrollPos],LOW);
  if(scrollPos==4)
  {
    digitalWrite(led[scrollPos-4],HIGH);
  }
  else
  {
    digitalWrite(led[scrollPos+1],HIGH);
  }
  scrollPos++;
  if(scrollPos==5)
  {
    scrollPos=0;
  }
}

void blinck(int rate)
{
  digitalWrite(led[scrollPos],HIGH);
  delay(rate);
  digitalWrite(led[scrollPos],LOW);
  delay(rate);
}
