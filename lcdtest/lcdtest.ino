#include<LiquidCrystal.h>

LiquidCrystal lcd(9,10,11,12,13,16);


const int upButton = 14;
const int downButton = 15;
const int menuButton = 2;

const int led1 = 7;
const int led2 = 6;
const int led3 = 3;
const int led4 = 4;
const int led5 = 5;

const int led[5] = {led1,led2,led3,led4,led5};
String ledMenu[]= {"led_1","led_2","led_3","led_4","led_5"};
byte pointerPos = 0;



void setup() {

  lcd.begin(20,4);
  lcd.clear();

    for(uint8_t x=0; x<5; x++)
  {
    pinMode(led[x],OUTPUT);
  }

  pinMode(upButton, INPUT_PULLUP);
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
               for(byte y=0; y<4; y++)
                  {
                       lcd.setCursor(4,y);
                       lcd.print(ledMenu[y+1]);
                    }
                    pointerPos=0;
         }
         

         delay(200);
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

