#include <SevenSeg.h>
SevenSeg disp(2,3,4,5,6,7,8);
const int digits = 4;
int digitPins[digits]={A0,A1,A2,A3};

void setup() {
  disp.setDigitPins(digits,digitPins);
  disp.setCommonCathode();

}

void loop() {
  disp.write(2067);

}
