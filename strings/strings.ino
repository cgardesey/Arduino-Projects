String a = "ayatullah ";
String b = "enjoys coding so much ";
String c ="he does ";
String d = "arduino";
String byteRead;
String f = "ARDUINO";
String ab = "250";
String ba = "150";

void setup() {
  Serial.begin(9600);
  Serial.print(a);
  Serial.println(b);

  //finding the length with length()
  Serial.println(a.length());
  Serial.println(b.length());
  
  //finding the char at a position with charAt()
  Serial.println(a.charAt(3));
  Serial.println(b.charAt(7));

  //finding the index of a char in a string
  Serial.println(a.indexOf('h'));
  Serial.println(b.indexOf('o'));
  Serial.println(b.lastIndexOf('o'));
  Serial.println(b.lastIndexOf('o',14));

  //combining strings with concat()
  a.concat(b);
  //Serial.println(c);
  Serial.println(a);
  b.concat(c);
  Serial.println(b);

  //gettin portions of a string with substring()
  Serial.println(a.substring(13));
  Serial.println(b.substring(6));
  //removing portions of a string with remove()
  a.remove(10);
  Serial.println(a);
  d.remove(2,2);
  Serial.println(d);
  d.concat("duino");

  //replacing portions of a string
  Serial.println(d);
  d.replace("duino","gamei");
  Serial.println(d);

  //upper case
  c.toUpperCase();
  Serial.println(c);

  //lower case
  Serial.println(f);
  f.toLowerCase();
  Serial.println(f);

  //String to int with toInt();
  Serial.println(ab+ba);
  int x = ab.toInt();
  int y = ba.toInt();
  Serial.println(x+y);

  //String to float with toFloat();
  float xy = ab.toFloat();
  float yx = ba.toFloat();
  Serial.println(xy+yx);
  
 }

void loop() {
  if(Serial.available())
  {
    //Serial.write(Serial.read());
    byteRead =Serial.readString();
  }

  if(byteRead!="")
  {
    Serial.print(byteRead);
  }
}
