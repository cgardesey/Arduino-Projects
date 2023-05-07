#include <SevenSeg.h>
SevenSeg disp(3, 4, 5, 6, 7, 8, 9);
const int numOfDigits =4;
int digitPins[numOfDigits]={10, 11, 12, 13};
void setup()
{
disp.setDigitPins(numOfDigits ,digitPins);
disp.setTimer(1);
disp.startTimer();
}
void loop () 
{
disp.write("0000");
}
ISR(TIMER1_COMPA_vect)
{
disp.interruptAction();
}
