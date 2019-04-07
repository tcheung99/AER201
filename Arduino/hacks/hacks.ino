#include <NewPing.h>

#include <NewPing.h>

#define PING_PIN A2 // Arduino pin for both trig and echo

NewPing sonar(PING_PIN, PING_PIN );

void setup() {

Serial.begin(115200);

}

void loop() {

delay(100); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay

unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
int dist = ((uS/2)/29.1);
Serial.print("Ping: ");
Serial.print(uS / US_ROUNDTRIP_CM); // convert time into distance

Serial.println("cm"); 
//Serial.print(dist);
//Serial.println("fdsajk"); 
}
