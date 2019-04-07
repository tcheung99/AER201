#include <NewPing.h>

#define trigPin1 A0
#define echoPin1 A1
#define trigPin2 A2
#define echoPin2 A3
#define trigPin3 A4 //low l
#define echoPin3 A5
#define trigPin4 7 // up l 
#define echoPin4 8



long duration, distance, RightSensor,BackSensor,FrontSensor,LeftSensor,UpLSensor;

void setup()
{
Serial.begin (9600);
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(trigPin3, OUTPUT);
pinMode(echoPin3, INPUT);
pinMode(trigPin4, OUTPUT);
pinMode(echoPin4, INPUT);
}

void loop() {
  SonarSensor(trigPin1, echoPin1);
  RightSensor = distance;
  SonarSensor(trigPin2, echoPin2);
  LeftSensor = distance;
  SonarSensor(trigPin3, echoPin3);
  FrontSensor = distance;
  SonarSensor(trigPin4, echoPin4);
  UpLSensor = distance;
  Serial.print("Low Right:");
  Serial.print(RightSensor); //low r
  Serial.print("     Up Right:");
  Serial.print(LeftSensor); //up r 
  Serial.print("     Low Left:");
  Serial.print(FrontSensor); //low l 
  Serial.print("     Up Left:");
  Serial.println(UpLSensor); //up l
}

void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;

}
