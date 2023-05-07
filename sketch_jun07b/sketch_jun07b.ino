uint8_t i=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

while(i < 101)
{
  Serial.println(i);
  i++;
}

}
