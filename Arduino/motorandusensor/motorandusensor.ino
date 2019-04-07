const int wheel_d = 60;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 1920;   // (8 pairs N-S) * (120:1 gearbox) * (2 falling/rising edges)
const float count_to_dist = (wheel_c / counts_per_rev) ; // (1 tick = mm)

const int pwm = 2 ;  //initializing pin 2 as pwm
const int in_1 = 8 ;
const int in_2 = 9 ;

int dist; //motor dist 
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;

const int trigPin = A0;
const int echoPin = A1;
// defines variables
long duration;
int distance; //usensor dist 

void setup() {
  // put your setup code here, to run once:
  pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
  pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
  pinMode(in_2,OUTPUT) ;

  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);  

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin (9600);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), count, CHANGE);

}

void loop() {
  // put your main code here, to run repeatedly:
  //sens();
  drive();
  //sens();
}

void drive(){
//Serial.println("drive");
  //sens(); 
  digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,LOW) ;
  analogWrite(pwm,160) ;
  delay(1000) ;
    
//For brake
//  digitalWrite(in_1,HIGH) ;
//  digitalWrite(in_2,HIGH) ;
//delay(500) ;
}

void count() {
  encoder0Pos++;
  dist = (encoder0Pos * count_to_dist ); //distance in mm
  Serial.print(encoder0Pos);
  Serial.print("  Distance: ");
  Serial.println(dist);
  sens();
}

void sens(){
    digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH); //returns length of pulse going from low to high
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Sensor Distance: ");
  Serial.println(distance);
}
