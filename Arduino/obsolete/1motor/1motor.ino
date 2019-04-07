// Parameters

//const int motor_power = 200;      // 0-255
//const int motor_offset = 5;       // Diff. when driving straight
const int wheel_d = 60;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 1920;   // (8 pairs N-S) * (120:1 gearbox) * (2 falling/rising edges)
const float count_to_dist = (wheel_c / counts_per_rev) ; // (1 tick = mm)

/*full rotation is 16 units*/ 
const int pwm = 6 ;  //initializing pin 2 as pwm
const int in_1 = 7 ;
const int in_2 = 8 ;
int val;
int dist;
int encoder0PinA = 2;
int encoder0PinB = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;


void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
  pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
  pinMode(in_2,OUTPUT) ;
  Serial.begin (9600);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), count, CHANGE);
  //delay(1000);
//  drive();
}

void loop() {
//  n = digitalRead(encoder0PinA);
//  if ((encoder0PinALast == LOW) && (n == HIGH)) {
//    if (digitalRead(encoder0PinB) == LOW) {
//      encoder0Pos--;
//    } else {
//      encoder0Pos++;
//    }
//    Serial.println (encoder0Pos);
//    Serial.print ("/");
//  }
//  encoder0PinALast = n;


//void drive(){
  //For Clock wise motion , in_1 = High , in_2 = Low
//
  //Serial.println("drive");
  digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,LOW) ;
  analogWrite(pwm,255) ;
  //Serial.println("hyuyu");

  /*setting pwm of the motor to 255
  we can change the speed of rotaion
  by chaning pwm input but we are only
  using arduino so we are using higest
  value to driver the motor  */

  if (((millis()/2000)%2)==0){
    Serial.print(encoder0Pos);
  Serial.print("  Distance: ");
  Serial.println(dist);
  }
  
  
//  //Clockwise for 3 secs
//  delay(3000) ;     
//  
////  //For brake
//  digitalWrite(in_1,HIGH) ;
//  digitalWrite(in_2,HIGH) ;
//  delay(1000) ;
//  Serial.println("faesdgqea");
//  //For Anti Clock-wise motion - IN_1 = LOW , IN_2 = HIGH
//  digitalWrite(in_1,LOW) ;
//  digitalWrite(in_2,HIGH) ;
//  delay(3000) ;
//  
//  //For brake
//  digitalWrite(in_1,HIGH) ;
//  digitalWrite(in_2,HIGH) ;
//  delay(1000) ;
}

void count() {
  encoder0Pos++;
  dist = (encoder0Pos * count_to_dist ); //distance in mm
//  Serial.print(encoder0Pos);
//    Serial.print("  numb  ");
//    Serial.print(num);
//
//  Serial.print("  coiunt  ");
//    Serial.print(count_to_dist);

//  Serial.print("  Distance: ");
//  Serial.println(dist);
}
