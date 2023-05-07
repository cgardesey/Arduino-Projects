//// read a digital pin and print the state to the serial monitor
//// D3 as input
//const int upButton = 14;
//const int downButton = 15;
//const int menuButton = 2;
//const int ledPin1 = 3;
//const int ledPin2 = 4;
//const int ledPin3 = 5;
//const int buzzerPin = 9;
//const int motorPin = 11;
//
//
void setup()
{ 
Serial.begin(9600);
//  pinMode(2, INPUT);
//  pinMode(7,  INPUT);
  pinMode(A3, OUTPUT);
  //pinMode(13, OUTPUT);


}

void loop()
{

//  int input1 = digitalRead(2);
//  int input2 = digitalRead(7);

//  digitalWrite(13, digitalRead(2));
digitalWrite(A3, 1);
Serial.println("Nano 3 test");
delay(1000);
digitalWrite(A3, 0);
delay(1000);

}
//
//
//
//void beep()
//{
//       digitalWrite(buzzerPin, HIGH);
//       delay(50);
//       digitalWrite(buzzerPin, LOW);
//       delay(50);
//}
//
//void ledON()
//{
//  digitalWrite(ledPin1, HIGH);
//  
//}
//
//
//
//  void ledOFF()
//{
//  digitalWrite(ledPin1, LOW);
//  
//}

//const int tonePin = 11;
//unsigned long  toneTime = 50;
//unsigned int pause = 5;
//uint8_t no_of_freqs = 10; 
//unsigned int freq[10]={100, 300, 500, 1000, 2000, 3000,  2500, 1500, 800, 5000};
//unsigned int freq1[10]={1000, 1200, 1700, 2000, 2200, 300,  500, 100, 800, 3500};
//
//
//void setup()
//{
//   pinMode(tonePin, OUTPUT);
//}
//
//void loop()
//{
//   for(uint8_t i=0; i<no_of_freqs; i++)
//      {
//         tone(tonePin, freq[i]);
//         //noTone(tonePin);
//         delay(toneTime);
//         noTone(tonePin);
//         delay(pause);
//         
//      }
//
//   for(uint8_t i=no_of_freqs; i>0; i--)
//      {
//         tone(tonePin, freq1[i]);
//         //noTone(tonePin);
//         delay(toneTime+100);
//         noTone(tonePin);
//         delay(pause+20);
//         
//      }
//
//      
//      for(uint8_t i=0; i<no_of_freqs; i++)
//      {
//         tone(tonePin, freq[i]);
//         //noTone(tonePin);
//         delay(toneTime+100);
//         noTone(tonePin);
//         delay(pause+20);
//         
//      }
//
//      
//   for(uint8_t i=no_of_freqs; i>0; i--)
//      {
//         tone(tonePin, freq1[i]);
//         //noTone(tonePin);
//         delay(toneTime);
//         noTone(tonePin);
//         delay(pause);
//         
//      }
//
//   
//}
//const byte intPin = 2;
//const byte ledPin1 = 3;
//const byte ledPin2 = 4;
//void setup()
//{
//  Serial.begin(9600);
//  pinMode(intPin,INPUT_PULLUP);
//   pinMode(ledPin1,OUTPUT);
//   pinMode(ledPin2,OUTPUT);
//   attachInterrupt(digitalPinToInterrupt(2),blinck,CHANGE);
//}
//
//void loop()
//{
//  digitalWrite(ledPin1,1); 
//  delay(500);
//  digitalWrite(ledPin1,0);
//  delay(500);
//}
//
//void blinck()
//{
//  Serial.println("ISR");
//  digitalWrite(ledPin2,1);
//  for(int i=0; i<500; i++)
//     delayMicroseconds(1000);
//  digitalWrite(ledPin2,0);
//  for(int i=0; i<500; i++)
//     delayMicroseconds(1000);
//}
