////////////////////////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
//////////////////////////////////////////////////////////////
double longi;
double lati;
double alti;
char longiDir;
char latiDir;


/////////////////////////////////////////////////////////////
float a;
float b;
float c;
float inf = 9999.99;
/////////////////////////////////////////////////////////////
 
double Datatransfer(char *data_buf,char num)//convert the data to the float type
{                                           //*data_buf£ºthe data array                                       
  double temp=0.0;                           //the number of the right of a decimal point
  unsigned char i,j;
 
  if(data_buf[0]=='-')
  {
    i=1;
    //process the data array
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    //convert the int type to the float type
    for(j=0;j<num;j++)
      temp=temp/10;
    //convert to the negative numbe
    temp=0-temp;
  }
  else//for the positive number
  {
    i=0;
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    for(j=0;j<num;j++)
      temp=temp/10 ;
  }
  return temp;
}
 
char ID()//Match the ID commands
{ 
  char i=0;
  char value[6]={
    '$','G','P','G','G','A'      };//match the gps protocol
  char val[6]={
    '0','0','0','0','0','0'      };
 
  while(1)
  {
    if(Serial.available())
    {
      val[i] = Serial.read();//get the data from the serial interface
      if(val[i]==value[i]) //Match the protocol
      {    
        i++;
        if(i==6)
        {
          i=0;
          return 1;//break out after get the command
        }
      }
      else
        i=0;
    }
  } 
}
 
void comma(char num)//get ','
{   
  char val;
  char count=0;//count the number of ','
 
  while(1)
  {
    if(Serial.available())
    {
      val = Serial.read();
      if(val==',')
        count++;
    }
    if(count==num)//if the command is right, run return
      return;
  }
 
}
void UTC()//get the UTC data -- the time
{
  char i;
  char time[9]={
    '0','0','0','0','0','0','0','0','0'
  };
  double t=0.0;
 
  if( ID())//check ID
  {
    comma(1);//remove 1 ','
    //get the datas after headers
    while(1)
    {
      if(Serial.available())
      {
        time[i] = Serial.read();
        i++;
      }
      if(i==9)
      {
        i=0;
        t=Datatransfer(time,2);//convert data
        t=t-30000.00;//convert to the chinese time GMT+8 Time zone
        long time=t;
        int h=time/10000;
        int m=(time/100)%100;
        int s=time%100;
        
//        if(h>=24)               //UTC+
//        {
//        h-=24;
//        }
 
         if (h<0)               //UTC-
        {
          h+=24;
        }
        Serial.print(h);
        Serial.print("h");
        Serial.print(m);
        Serial.print("m");
        Serial.print(s);
        Serial.println("s");
 
        //Serial.println(t);//Print data 
        return;
      }  
    }
  }
}

double latitude()//get latitude
{
  double la;
  char i;
  char lat[10] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
  };


  if ( ID())
  {
    comma(2);
    while (1)
    {
      if (Serial.available())
      {
        lat[i] = Serial.read();
        i++;
      }
      if (i == 10)
      {
        i = 0;
        la = Datatransfer(lat, 5) / 100.0; //,7;
        //Serial.println(la);//print latitude
        return la;
      }
    }
  }
}


char lat_dir()//get dimensions
{
  char i = 0, val;

  if ( ID())
  {
    comma(3);
    while (1)
    {
      if (Serial.available())
      {
        val = Serial.read();
        //Serial.write(val);
        Serial.println();
        i++;
      }
      if (i == 1)
      {
        i = 0;
        return val;
      }
    }
  }
}


double longitude()//get longitude
{
  double lo;
  char i;
  char lon[11] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
  };

  if ( ID())
  {
    comma(4);
    while (1)
    {
      if (Serial.available())
      {
        lon[i] = Serial.read();
        i++;
      }
      if (i == 11)
      {
        i = 0;
        lo = Datatransfer(lon, 5) / 100.0; //,7;
        //Serial.println(lo);
        return lo;
      }
    }
  }
}

char lon_dir()//get direction data
{
  char i = 0, val;

  if ( ID())
  {
    comma(5);
    while (1)
    {
      if (Serial.available())
      {
        val = Serial.read();
        //Serial.write(val); //Serial.println(val,BYTE);
        Serial.println();
        i++;
      }
      if (i == 1)
      {
        i = 0;
        return val;
      }
    }
  }
}


double altitude()//get altitude data
{
  double alti;
  char i, flag = 0;
  char alt[8] = {
    '0', '0', '0', '0', '0', '0', '0', '0'
  };

  if ( ID())
  {
    comma(9);
    while (1)
    {
      if (Serial.available())
      {
        alt[i] = Serial.read();
        if (alt[i] == ',')
          flag = 1;
        else
          i++;
      }
      if (flag)
      {
        i = 0;
        alti = Datatransfer(alt, 1); //,1;
        //Serial.println(alt);//print altitude data
        return alti;
      }
    }
  }
}


void setup()
{
  delay(5000);//wait for gsm
  ///////////////////////////////////////////////////////////
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
    accel.setRange(ADXL345_RANGE_16_G);
  /* Display some basic information on this sensor */
//  displaySensorDetails();
//  /* Display additional settings (outside the scope of sensor_t) */
//  displayDataRate();
//  displayRange();
//  Serial.println("");
  ///////////////////////////////////////////////////////////
  pinMode(2,OUTPUT);//The default digital driver pins for the GSM and GPS mode
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  delay(1500);
  digitalWrite(5,LOW);
 
  digitalWrite(2,LOW);//Enable GSM mode
  digitalWrite(3,HIGH);//Disable GPS mode
  delay(2000);
  Serial.begin(115200); 
  delay(5000);//GPS ready
 
  Serial.println("AT");   
  delay(2000);
  //turn on GPS power supply
  Serial.println("AT+CGPSPWR=1");
  delay(1000);
  //reset GPS in autonomy mode
  Serial.println("AT+CGPSRST=1");
  delay(1000);
 
  digitalWrite(3,LOW);//Enable GPS mode
  digitalWrite(2,HIGH);//Disable GSM mode
  delay(2000);
 
  //Serial.println("$GPGGA statement information: ");
}
void loop()
{
//  while(1)
//  { 
//////////////////////////////////////////////////////////////
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
//  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
//  delay(500);
  a=event.acceleration.x;
  b=event.acceleration.y;
  c=event.acceleration.z;
//////////////////////////////////////////////////////////////

      digitalWrite(3,LOW);//Enable GPS mode
      digitalWrite(2,HIGH);//Disable GSM mode
      delay(2000);
  
//    Serial.print("UTC:");
//    UTC();
   ////////////////////////////////////////////////////////////
   //Serial.print("X: "); Serial.println(a);
   //Serial.print("Y: "); Serial.println(b);
   //Serial.print("Z: "); Serial.println(c);
   ////////////////////////////////////////////////////////////
    //Serial.print("Lat:");
    lati = latitude();
    //Serial.println(lati, 7);

    //Serial.print("Dir:");
    latiDir = lat_dir();
    //Serial.println(latiDir);

    //Serial.print("Lon:");
    longi = longitude();
    //Serial.println(longi, 7);

    //Serial.print("Dir:");
    longiDir = lon_dir();
    //Serial.println(longiDir);

    //Serial.print("Alt:");
    alti = altitude();
    //Serial.println(alti);

    //Serial.println(' ');
//    if (lati==inf)
//    {
//      Serial.println("problem");
//    }
  
  sendToThingspeak();
    
}


void sendToThingspeak()
{
  digitalWrite(2, LOW); // ENABLE GSM SERIAL
  digitalWrite(3, HIGH); // DISABLE GPS SERIAL

  delay(2000);
  

  Serial.println("AT+CPIN?");
  delay(1000);
  


  Serial.println("AT+CREG?");
  delay(1000);
  

  Serial.println("AT+CGATT?");
  delay(1000);


  Serial.println("AT+CIPSHUT");
  delay(1000);
  

  Serial.println("AT+CIPSTATUS");
  delay(2000);
  

  Serial.println("AT+CIPMUX=0");
  delay(2000);
 

  Serial.println("AT+CSTT=\"MTN\"");
  delay(1000);
 

  Serial.println("AT+CIICR");
  delay(3000);


  Serial.println("AT+CIFSR");
  delay(2000);
 

  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\", \"80\"");
  delay(6000);


  Serial.println("AT+CIPSEND");
  delay(4000);
 

  //Serial.println("GET http://api.thingspeak.com/update?key=W68QUVX7MAZT3NXY&field1=" + String(lati));
  Serial.println("GET http://api.thingspeak.com/update?key=W68QUVX7MAZT3NXY&field1=" + String(lati,7) +"&field2=" + String(longi,7) + "&field3=" +String(alti)+ "&field4=" +String(a)+ "&field5=" +String(b)+ "&field6=" +String(c));
  delay(4000);

  Serial.println((char)26);
  delay(5000);
 

  Serial.println("AT+CIPSHUT");
  delay(100);
}

