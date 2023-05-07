void setup() {
 Serial.begin(9600);
  for(int i=0; i<4; i++){
    pinMode(14+i,INPUT);
  }
}

void loop() {
   for(int i=0; i<4; i++){
     if(digitalRead(14+i))
     {
     switch(i)
     {
      case 0:
       Serial.print('a');
       break;

       case 1:
       Serial.print('b');
       break;

       case 2:
       Serial.print('c');
       break;

       case 3:
       Serial.print('d');
       break;
        default:
        break;
      }
   }
    }

}
