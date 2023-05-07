#include <OneButton.h>

const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPin3 = 5;

OneButton button(2, true);


void setup() {

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  //button.attachClick(led1);
  button.attachDoubleClick(led2);
button.attachLongPressStart(led3);
//  button.attachLongPressStop(led4);
//   button.attachDuringLongPress(led4);
}

void loop() {
 button.tick();

}

void led1()
{
 digitalWrite(ledPin1, 1);
 digitalWrite(ledPin2, 0); 
 digitalWrite(ledPin3, 0);
}

void led2()
{
 digitalWrite(ledPin1, 0);
 digitalWrite(ledPin2, 1); 
 digitalWrite(ledPin3, 0);
}

void led3()
{
 digitalWrite(ledPin1, 0);
 digitalWrite(ledPin2, 0); 
 digitalWrite(ledPin3, 1);
}

void led4()
{
 digitalWrite(ledPin1, 1);
 digitalWrite(ledPin2, 0); 
 digitalWrite(ledPin3, 1);
}

void led5()
{
 digitalWrite(ledPin1, 1);
 digitalWrite(ledPin2, 1); 
 digitalWrite(ledPin3, 1);
}
