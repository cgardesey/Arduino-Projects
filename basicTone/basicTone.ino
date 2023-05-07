void setup() {
  // put your setup code here, to run once:
  pinMode(8,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<500; i++)
  {
    tone(8,1000);
    }
    for(int i=0; i<500; i++)
  {
    tone(8,2000);
   }
     for(int i=0; i<1000; i++)
  {
    tone(8,700);
    }

    for(int i=0; i<10; i++)
  {
      for(int i=0; i<500; i++)
  {
    tone(8,1000);
    }
    for(int i=0; i<500; i++)
  {
    tone(8,2000);
   }
   
    }  
  
  
  
//    tone(8,1000);
//  tone(8,2000);
//   delay(1000);
//    tone(8,1000);
//  tone(8,2000);
//   delay(1000);
//  
//    tone(8,1000);
//  tone(8,2000);
//   delay(1000);
//    tone(8,1000);
//  tone(8,2000);
// delay(1000);
}
