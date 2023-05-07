const int MOTORPIN = 3
const int SENSORPIN = A0;

const int MAXTIME = 10000

int time;

void setup() {
  // put your setup code here, to run once:
pinMode(MOTORPIN, OUTPUT);

}

void loop() {
  sensor_reading
   time = map(analogRead(SENSORPIN), 0 1023, 0, MAXTIME);
  if(sensor_reading)
  { 
    time = map(analogRead(SENSORPIN), 0 1023, 0, MAXTIME);   
     dititalWrite(MOTORPIN, HIGH);
     delay(time);
     dititalWrite(MOTORPIN, LOW);
  }
}
