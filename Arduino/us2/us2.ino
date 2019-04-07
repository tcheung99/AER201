const int trigPin = A0;
const int echo1Pin = A1;
const int trigPin2 = A2;
const int echo2Pin = A3;


// defines variables
long duration1;
long duration2;
int distance1;
int distance2;


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(trigPin2, OUTPUT);
  pinMode(echo1Pin, INPUT); // Sets the echoPin as an Input
  pinMode(echo2Pin, INPUT);
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin2, LOW);

  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
    digitalWrite(trigPin2, HIGH);

  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
    digitalWrite(trigPin2, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echo1Pin, HIGH);
  duration2 = pulseIn(echo2Pin, HIGH);
  // Calculating the distance
  distance1= duration1*0.034/2;
  distance2= duration2*0.034/2;

  // Prints the distance on the Serial Monitor
//  int cnt = 0;
//  if (cnt%2){
     Serial.print("Distance1: ");
    Serial.println(distance1);
    Serial.print("Distance2: ");
    Serial.println(distance2);
//  }
//  cnt++; 
}
