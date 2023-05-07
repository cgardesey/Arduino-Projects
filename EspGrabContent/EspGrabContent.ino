
#include <ESP8266WiFi.h>

const char* ssid = "ayatullah";
const char* password = "";

const char* host = "www.example.com";
void setup() {

Serial.begin(115200);
delay(10);

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
} 

Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
} 

int value = 0;


void loop() {
Serial.print("Connecting to ");
Serial.println(host);

WiFiClient client;
const int httpPort = 80;
if (!client.connect(host, httpPort)) {
Serial.println("connection failed");
return;
}

client.print(String("GET /") + " HTTP/1.1\r\n" +
"Host: " + host + "\r\n" +
"Connection: close\r\n\r\n");
delay(10);

while(client.available()){
String line = client.readStringUntil('\R');
Serial.print(line);
} 
Serial.println();
Serial.println("closing connection");
delay(5000);
}
