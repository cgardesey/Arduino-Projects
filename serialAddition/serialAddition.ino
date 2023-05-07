float firstNumber=0;
float secondNumber=0;
float result=0;
unsigned long a;

void setup() {
  Serial.begin(9600);

}

void loop() {

  while(result==0)
  {
   
    Serial.println("please enter the first number");

    while(Serial.available()==0)
      {}
       a = millis();
      firstNumber= Serial.parseInt();
      Serial.println("firstNumber= "+ String(firstNumber));
      
      

     Serial.println("please enter the second number");
         while(Serial.available()==0)
         {}

        secondNumber = Serial.parseInt();
         Serial.println("secondNumber = "+ String(secondNumber));
     result = secondNumber + firstNumber;
    Serial.print("answer: ");
     Serial.println(result);
     Serial.print("time used: "); Serial.println(millis()-a);
    result=0;
  }
}


