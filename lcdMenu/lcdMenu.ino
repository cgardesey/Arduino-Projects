//a menu of four items, when each is selected displays a submenu.
#include<LiquidCrystal.h>

LiquidCrystal lcd(9,10,11,12,13,16);


const int backButton = 14;
const int downButton = 15;
const int menuButton = 2;

const int led1 = 7;
const int led2 = 6;
const int led3 = 3;
const int led4 = 4;

const int led[4] = {led1,led2,led3,led4};
 byte ledState[4]= {0,0,0,0};
String ledMenu[]= {"led_1","led_2","led_3","led_4"};
byte pointerPos = 0;
byte pointerPos1 = 1;

void setup() {
  lcd.begin(20,4);
  lcd.clear();

    for(uint8_t x=0; x<4; x++)
  {
    pinMode(led[x],OUTPUT);
  }

  pinMode(backButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(menuButton, INPUT_PULLUP);

}

void loop() {

 menu();
   if(!digitalRead(downButton))
    {
      lcd.clear();
      menu();
      lcd.setCursor(3,pointerPos);
      lcd.print(">");
      pointerPos++;
      if(pointerPos==4)
         {

           pointerPos=0;
         }
         

         delay(200);
    }

  if(!digitalRead(menuButton))
  {
    lcd.clear();
    while(digitalRead(backButton))
        {
          if(pointerPos==1)
             {
                
                lcd.setCursor(7,0);
                lcd.print("led_1:");
                lcd.setCursor(9,1);
                lcd.print("ON");
                if(ledState[0]==0)
                 {
                   lcd.setCursor(7,2);
                   lcd.print("=>");
                 }
                 else
                 {
                   lcd.setCursor(7,1);
                   lcd.print("=>");
                 }

                lcd.setCursor(9,2);
                lcd.print("OFF");
             }

            if(pointerPos==2)
             {
                //lcd.clear();
                lcd.setCursor(7,0);
                lcd.print("led_2:");
                lcd.setCursor(9,1);
                lcd.print("ON");
                if(ledState[1]==0)
                 {
                   lcd.setCursor(7,2);
                   lcd.print("=>");
                 }
                 else
                 {
                   lcd.setCursor(7,1);
                   lcd.print("=>");
                 }
                lcd.setCursor(9,2);
                lcd.print("OFF");
             }

            if(pointerPos==3)
             {
                //lcd.clear();
                lcd.setCursor(7,0);
                lcd.print("led_3:");
                lcd.setCursor(9,1);
                lcd.print("ON");
                if(ledState[2]==0)
                 {
                   lcd.setCursor(7,2);
                   lcd.print("=>");
                 }
                 else
                 {
                   lcd.setCursor(7,1);
                   lcd.print("=>");
                 }
                lcd.setCursor(9,2);
                lcd.print("OFF");
             }

            if(pointerPos==0)
             {
                //lcd.clear();
                lcd.setCursor(7,0);
                lcd.print("led_4:");
                lcd.setCursor(9,1);
                lcd.print("ON");
                if(ledState[3]==0)
                 {
                   lcd.setCursor(7,2);
                   lcd.print("=>");
                 }
                 else
                 {
                   lcd.setCursor(7,1);
                   lcd.print("=>");
                 }
                lcd.setCursor(9,2);
                lcd.print("OFF");
             }
             if(!digitalRead(backButton))
                lcd.clear();

             if(!digitalRead(downButton))
                {
                  lcd.clear();
                  if(pointerPos==1&& ledState[0]==0)
                    {
                      lcd.setCursor(7,1);
                      lcd.print("=>");
                      digitalWrite(led1,HIGH);
                      ledState[0]=1;
                    }

                   if(pointerPos==1&& ledState[0]==1)
                    {
                      lcd.setCursor(7,2);
                      lcd.print("=>");
                      digitalWrite(led1,LOW);
                      ledState[0]=0;
                    }
                  /////////////////////////////////////////
                   if(pointerPos==2&& ledState[1]==0)
                    {
                      lcd.setCursor(7,1);
                      lcd.print("=>");
                      digitalWrite(led2,HIGH);
                      ledState[1]=1;
                    }
                  if(pointerPos==1&& ledState[1]==1)
                    {
                      lcd.setCursor(7,2);
                      lcd.print("=>");
                      digitalWrite(led2,LOW);
                      ledState[1]=0;
                    }
                  ///////////////////////////////////////////
                  if(pointerPos==3&& ledState[2]==0)
                    {
                      lcd.setCursor(7,1);
                      lcd.print("=>");
                      digitalWrite(led3,HIGH);
                      ledState[1]=1;
                    }
                  if(pointerPos==3&& ledState[2]==1)
                    {
                      lcd.setCursor(7,1);
                      lcd.print("=>");
                      digitalWrite(led3,LOW);
                      ledState[2]=0;
                    }
                 ////////////////////////////////////////////////
                 if(pointerPos==0&& ledState[3]==0)
                    {
                      lcd.setCursor(7,1);
                      lcd.print("=>");
                      digitalWrite(led4,HIGH);
                      ledState[3]=1;
                    }
                  if(pointerPos==0&& ledState[3]==1)
                    {
                      lcd.setCursor(7,1);
                      lcd.print("=>");
                      digitalWrite(led4,LOW);
                      ledState[3]=0;
                    }
                    delay(200);
                }
          
        }
    
    
  }
 

}

void menu()
{
  for(byte y=0; y<4; y++)
  {
     lcd.setCursor(4,y);
     lcd.print(ledMenu[y]);
  }
  
}

