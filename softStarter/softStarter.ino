#define T1Pulse 5
#define T2Pulse 6
#define T3Pulse 7
#define T4Pulse 8
#define T5Pulse 9
#define T6Pulse 10


void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  pinMode(T1Pulse, OUTPUT);
  pinMode(T2Pulse, OUTPUT);
  pinMode(T3Pulse, OUTPUT);
  pinMode(T4Pulse, OUTPUT);
  pinMode(T5Pulse, OUTPUT);
  pinMode(T6Pulse, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  pulse(T6Pulse, T1Pulse);
  delayMicroseconds(3133.333333333333333333);
  pulse(T1Pulse, T2Pulse);
  delayMicroseconds(3133.333333333333333333);
  pulse(T2Pulse, T3Pulse);
  delayMicroseconds(3133.333333333333333333);
  pulse(T3Pulse, T4Pulse);
  delayMicroseconds(3133.333333333333333333);
  pulse(T4Pulse, T5Pulse);
  delayMicroseconds(3133.333333333333333333);
  pulse(T5Pulse, T6Pulse);
  delayMicroseconds(3133.333333333333333333);
}


void pulse(int P1, int P2)
{
  digitalWrite(P1, HIGH);
  digitalWrite(P2, HIGH);
  delayMicroseconds(200);
  digitalWrite(P1, LOW);
  digitalWrite(P2, LOW);

}
