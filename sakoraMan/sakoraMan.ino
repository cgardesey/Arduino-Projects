void setup() {

DDRD=0XFF;
PORTD=0XFF;
}
/////////////////////////////////////////////////end setup()
void loop() {

animate1(5000);

for(int i=0; i<20; i++)
{
 animate1(40);
 blank(40);
}

for(int i=0; i<15; i++)
{
 animate1(80);
 blank(80);
}

for(int i=0; i<10; i++)
{
 animate1(160);
 blank(160);
}

for(int i=0; i<8; i++)
{
 animate1(300);
 blank(300);
}

for(int i=0; i<5; i++)
{
 animate1(600);
 blank(600);
}



  
animate1(5000);
blank(1000);
animate2(2000);
blank(1000);
animate3(2000);
blank(1000);
animate4(2000);
blank(1000);

animate1(2000);
blank(1000);
animate2(1000);
blank(1000);
animate3(1000);
blank(1000);
animate4(1000);
blank(1000);

animate1(1000);
blank(500);
animate2(1000);
blank(500);
animate3(2000);
blank(500);
animate4(2000);
blank(500);

animate1(1000);
blank(1000);
animate2(1000);
blank(1000);
animate3(500);
blank(1000);
animate4(500);
blank(1000);


animate1(1000);
//blank(1000);
animate2(1000);
//blank(1000);
animate3(500);
//blank(1000);
animate4(500);
//blank(1000);


animate1(1000);
blank(1000);
animate2(1000);
blank(1000);


for(int i=0; i<5; i++)
{
 animate3(500);
}
blank(1000);
for(int i=0; i<5; i++)
{
 animate4(500);
}

  blank(1000);


  animate1(3000);
  blank(3000);
  animate1(3000);
  blank(3000);

for(int i=0; i<5; i++)
{
  animate1(3000);
  blank(1000);
  animate1 (3000);
  blank(1000);
}

for(int i=0; i<10; i++)
{
  animate2(100);
}

blank(300);
animate1(3000);

for(int i=0; i<10; i++)
{
  animate3(100);
}

blank(300);
animate1(5000);

for(int i=0; i<10; i++)
{
  animate4(100);
}


}

/////////////////////////////////////////////////end loop()
void blank(int _delay)
{
 PORTD=0;
 delay(_delay); 
}
void animate1(int _delay)//all on
{
  PORTD=0Xff;
  delay(_delay); 
}

void animate2(int _delay)
{
   PORTD=0X03;      //SAKORA MAN
  delay(_delay);
  PORTD=0X1C;      //PHONE ACCESSORIES
  delay(_delay);
   PORTD=0X20;    //SIBI
  delay(_delay);
}

void animate3(int _delay)
{
  PORTD=0X03;
  delay(_delay);
  PORTD=0X1C|0X03; 
  delay(_delay);
  PORTD|=0X20|0X1C|0X03;
  delay(_delay);
}

void animate4(int _delay)
{
  PORTD=0X20;
  delay(_delay);
  PORTD=0X20|0X1C;
  delay(_delay);
  PORTD=0X03|0X20|0X1C;
  delay(_delay);
}
