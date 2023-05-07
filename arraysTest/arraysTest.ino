int cap[] = {2,45,67,90,3};
char chara[7] = {'b','r','a','y','o','m','i'};

uint8_t count=0;

void setup()
{
   Serial.begin(9600);
   
}

void loop()
{
  while(count<7)
   {
    Serial.print(chara[count]);
    count++;
    delay(1000);
   }
   
}
