#include <NewPing.h>
#include <LiquidCrystal.h>
LiquidCrystal l(2,3,4,5,6,7);
#define TRIGGER_PIN  16  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     17  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200);
   l.begin(20,4);// Open serial monitor at 115200 baud to see ping results.
   l.clear();
   l.print("DISTANCE:");
}

void loop() {
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  l.setCursor(9,0);
  l.print(uS / US_ROUNDTRIP_CM);
}
