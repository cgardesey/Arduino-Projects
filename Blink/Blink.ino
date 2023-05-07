// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() 
{
	if (Serial.available() > 0)
	{
		if (Serial.read() == '1')
		{
			digitalWrite(13, HIGH);
		}
		if (Serial.read() == '0')
		{
			digitalWrite(13, LOW);
		}
	}
}
