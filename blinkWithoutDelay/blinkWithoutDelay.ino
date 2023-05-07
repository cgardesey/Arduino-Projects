const byte ledPin1 = 3;
const byte ledPin2 = 4;
unsigned long x =0;

void setup() {
     pinMode(ledPin1,OUTPUT);
     pinMode(ledPin2,OUTPUT);

}

void loop() {
  if(millis()-x==1000)
  {
     digitalWrite(ledPin1,!digitalRead(ledPin1)); 
     x+=1000;
  }
//  digitalWrite(ledPin2,1);
//   for(int i=0; i<20; i++)
//      delayMicroseconds(1000);
//  digitalWrite(ledPin2,0);
//   for(int i=0; i<20; i++)
//      delayMicroseconds(1000);
    

}
