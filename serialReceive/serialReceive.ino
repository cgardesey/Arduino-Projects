void setup() {
 Serial.begin(9600);
 for(int i=0; i<4; i++){
    pinMode(14+i,OUTPUT);
  }

}

void loop() {
  if(Serial.available())
  { char Char= Serial.read();
    switch(Char)
    {
      case 'a':
      digitalWrite(14, HIGH);
       digitalWrite(15, LOW); 
       digitalWrite(16, LOW);
       digitalWrite(17, LOW);
      break;

      case 'b':
      digitalWrite(15, HIGH);
       digitalWrite(14, LOW); 
       digitalWrite(16, LOW);
       digitalWrite(17, LOW);
      break;

      case 'c':
      digitalWrite(16, HIGH);
       digitalWrite(14, LOW); 
       digitalWrite(15, LOW);
       digitalWrite(17, LOW);
      break;

        case 'd':
      digitalWrite(17, HIGH);
       digitalWrite(14, LOW); 
       digitalWrite(15, LOW);
       digitalWrite(16, LOW);
      break;

      default:

       digitalWrite(17, LOW);
       digitalWrite(14, LOW); 
       digitalWrite(15, LOW);
       digitalWrite(16, LOW);
      break;
      
      
      }
    
    
    }
  

}
