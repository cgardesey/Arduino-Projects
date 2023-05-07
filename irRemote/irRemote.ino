#include <IRremote.h>
#include <IRremoteInt.h>

//#include <IRemote.h>
int TKD2=11;
int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  if (irrecv.decode(&results))
    {
     Serial.println(results.value, DEC);
     irrecv.resume(); // Receive the next value
    }
}
NEC
0 16738455
1 16724175
2 16718055
3 16743045
4 16716015
5 16726215
6 16734885
7 16728765
8 16730805
9 16732845
- 16769055
+ 16754775
< 16720605
> 16712445
E 16748655



