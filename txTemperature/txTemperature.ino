
#include <VirtualWire.h>
#include "DHT.h"

#define DHTPIN 14 
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

const int led_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;

char tempHumidity[2]= {0,0};
void setup()
{
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);       // Bits per sec
    pinMode(led_pin, OUTPUT);
      dht.begin();
}

byte count = 1;
//25.6
void loop()
{
  //char msg[7] = {'h','e','l','l','o',' ','#'};
  //float h = dht.readHumidity();
  float t = dht.readTemperature();//get the temperature and save it in t
  float temp = t;// assing the temperature to temp
  t=t*10;//turn the temperature into a whole number
  int tWhole = (int)temp;// get the whole number part of temp
  int tDeci = (int)t%10;// get the fractional part of temp

  char twhole = (char)tWhole;// convert the results to char
  char tdeci = (char)tDeci;

  tempHumidity[0]=twhole;//save in char array
  tempHumidity[1]=tdeci;
  
  //msg[6] = count;
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)tempHumidity, 2);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(1000);
  //count = count + 1;
}
