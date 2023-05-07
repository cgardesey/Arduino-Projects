//const int led1 = 7;
//const int buzz = 9;
//char c;
//String mystring="slow";
////int rate1=10;
//String string1 = "";
//
//void setup() {
//  Serial.begin(9600);
//  pinMode(led1,OUTPUT);
//  pinMode(buzz,OUTPUT);
//
//  digitalWrite(led1,1);
//  delay(100);
//  digitalWrite(led1,0);
//}
//
//void loop() {
//  if(Serial.available())
//  {
//    mystring = Serial.readString();
//     Serial.print(mystring);
//  }
//
//  if(mystring.equals("fast"))
//  {
//     flashFast();
//  }
//  if(mystring.equals("slow"))
//  {
//     flashSlow();
//  }
//}
//
//void flashFast()
//{
//  digitalWrite(led1,1);
//  delay(100);
//  digitalWrite(led1,0);
//  delay(100);  
//}
//
//void flashSlow()
//{
//  digitalWrite(led1,1);
//  delay(2000);
//  digitalWrite(led1,0);
//  delay(2000);  
//}

//////////////////////////////////////////////////////////
//const int led1 = 7;
//const int buzz = 9;
////char c;
//int rate=100;
//int rate1=10;
//
//void setup() {
//  Serial.begin(9600);
//  pinMode(led1,OUTPUT);
// pinMode(buzz,OUTPUT);
//}
//
//void loop() {
//  if(Serial.available())
//  {
//    rate = Serial.parseInt();
//    Serial.println(rate);
//  }
//
//  flash(rate);
//}
//
//void flash(int x)
//{
//  digitalWrite(led1,1);
//  digitalWrite(buzz,1);
//  delay(x);
//  digitalWrite(led1,0);
//  digitalWrite(buzz,0);
//  delay(x);  
//}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
const int led1 = 5;
const int buzz = 9;
//char c;
int rate=100;
//int rate1=10;

void setup() {
  Serial.begin(9600);
  pinMode(led1,OUTPUT);
 pinMode(buzz,OUTPUT);
}

void loop() {
  if(Serial.available())
  {
    rate = Serial.parseInt();
    Serial.println(rate);
  }

  sound(rate);
}

void sound(int x)
{
  analogWrite(led1,x);
}
