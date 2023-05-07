void setup() {
  pinMode(14, OUTPUT);
   pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);
     pinMode(17, OUTPUT);
pinMode(0, INPUT);
}

void loop() {
  if(digitalRead(0))
  {
    digitalWrite(14, HIGH);
    delay(250);
      if(digitalRead(0))
       {
          digitalWrite(15, HIGH);
           delay(250);


           if(digitalRead(0))
           {
             digitalWrite(16, HIGH);
             delay(250);

               if(digitalRead(0))
                 {
                   digitalWrite(17, HIGH);
                   delay(250);
        
                  }
           }
        }
    
    
    }

}
