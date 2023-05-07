float firstNumber=0;
float secondNumber=0;
float result=0;
byte operation;

void setup() {
  Serial.begin(9600);

}

void loop() {

  while(result==0)
  {
    Serial.println("please enter the first number");

    while(Serial.available()==0)
      {}
      firstNumber= Serial.parseInt();
      Serial.println("firstNumber= "+ String(firstNumber));
      
      

     Serial.println("please enter the second number");
         while(Serial.available()==0)
         {}
     secondNumber = Serial.parseInt();
     Serial.println("secondNumber = "+ String(secondNumber));

     Serial.println("enter the operation: >");
         while(Serial.available()==0)
         {}
     operation = Serial.read();

     switch(operation)
     {
      case '+':
      result = secondNumber + firstNumber;
      Serial.print("answer: ");
      Serial.println(result);
      Serial.println();
      result = 0;
      break;

      case '*':
      result = secondNumber * firstNumber;
      Serial.print("answer: ");
      Serial.println(result);
      Serial.println();
      result = 0;
      break;

      case '/':
      result = firstNumber / secondNumber;
      Serial.print("answer: ");
      Serial.println(result);
      Serial.println();
      result = 0;
      break;

      case '-':
      result = firstNumber - secondNumber;
      Serial.print("answer: ");
      Serial.println(result);
      Serial.println();
      result = 0;
      break;

      default:
      Serial.print("Please Enter a valid operation");
      Serial.println();
      break;
    }

  }
}


