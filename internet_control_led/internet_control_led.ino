
// Required libraries
#include <ESP8266WiFi.h>

// WiFi parameters
const char* ssid = "Cyril";
const char* password = "";

// Create an instance of the server
WiFiServer server(80);

// Pin
int LAMP = 5;
int FAN = 4;
int FRIDGE = 12;
int HEATER = 13;

void setup() {

  // Start Serial
  Serial.begin(115200);
  delay(10);

  // Prepare GPIO5
  pinMode(LAMP, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(FRIDGE, OUTPUT);
  pinMode(HEATER, OUTPUT);
  digitalWrite(LAMP, 0);
  digitalWrite(FAN, 0);
  digitalWrite(FRIDGE, 0);
  digitalWrite(HEATER, 0);

  // Connect to WiFi network
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

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  Serial.println(req.length());
  client.flush();

if(req=="GET /LAMP HTTP/1.1"){
    digitalWrite(LAMP, !digitalRead(LAMP));
  }

if(req=="GET /FAN HTTP/1.1"){
    digitalWrite(FAN, !digitalRead(FAN));
  }

if(req=="GET /FRIDGE HTTP/1.1"){
    digitalWrite(FRIDGE, !digitalRead(FRIDGE));
  }

if(req=="GET /HEATER HTTP/1.1"){
    digitalWrite(HEATER, !digitalRead(HEATER));
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "</head>";
  s += "<div class=\"container\">";
  s += "<h1>Lamp Control</h1>";
  s += "<div class=\"row\">";
  
  s += "<div class=\"col-md-2\"><input id=\"btn1\" class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"LAMP\" onclick=\"colorchange('btn1')\"></div>";
  s += "<div class=\"col-md-2\"><input id=\"btn2\" class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"FAN\" onclick=\"colorchange('btn2')\"></div>";
  //////
  s += "<div class=\"col-md-2\"><input id=\"btn3\" class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"FRIDGE\" onclick=\"colorchange('btn3')\"></div>";
  s += "<div class=\"col-md-2\"><input id=\"btn4\" class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"HEATER\" onclick=\"colorchange('btn4')\"></div>";
  //////
  s += "</div></div>";

  /////
  s += "<script>function colorchange(id) {var background = document.getElementById(id).style.backgroundColor;if (background == \"rgb(255, 0, 0)\") {document.getElementById(id).style.background = \"rgb(0,255,0)\";} else { document.getElementById(id).style.background = \"rgb(255,0,0)\";}$.get(\"/\" + document.getElementById(id).value);}</script>";
  //////
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
