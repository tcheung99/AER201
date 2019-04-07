const int wheel_d = 60;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 1920;   // (8 pairs N-S) * (120:1 gearbox) * (2 falling/rising edges)
const float count_to_dist = (wheel_c / counts_per_rev) ; // (1 tick = mm)

const int pwm = 6 ;  //initializing pin 2 as pwm
const int in_1 = 8 ;
const int in_2 = 9 ;

int dist;
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;

int currentSpeed = 0;
long startTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
  pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
  pinMode(in_2,OUTPUT) ;

  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);  
  Serial.begin (115200);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), count, CHANGE);
  //drive();
}


void loop() {
  int count;
  
  //while(1){
  
  count = ((millis())%2);
  // put your main code here, to run repeatedly:
  Serial.print("count");

       Serial.println(count);
  if (count == 0){
    currentSpeed = 200;
       Serial.print("dist");

       Serial.println(dist);

//    Serial.println(millis());
//    Serial.println(startTime);

  } 
//  else {
//    currentSpeed = 0;
//
//  }
  drive(currentSpeed);
   Serial.println(dist);
      

 
  }
  /*
  while(1){
    if(count%2 == 0){
      drive(200);
    } else {
      drive(0);
    }
    delay(1000);
    count++;
  }*/


void drive(int speed){

  digitalWrite(in_1,HIGH) ;
  //Serial.println("high");
  
  digitalWrite(in_2,LOW) ;
  //Serial.println("low");
  analogWrite(pwm, speed) ;

//  delay(2000);
//  digitalWrite(in_1,LOW) ;
//  digitalWrite(in_2,LOW) ;
//  delay(1000) ;
  
  /*Serial.println("drive");
  digitalWrite(in_1,HIGH) ;
  Serial.println("high");
  
  digitalWrite(in_2,LOW) ;
  Serial.println("low");
  analogWrite(pwm,255) ;
  Serial.println("PWERJGRAK");
  delay(3000) ;     
    Serial.println("delay");
analogWrite(pwm,0) ;
//For brake
  Serial.println("fasjrdikglie");

  digitalWrite(in_1,LOW) ;
  digitalWrite(in_2,LOW) ;
  delay(1000) ;*/
  return;
}

void count() {
  encoder0Pos++;
  dist = (encoder0Pos * count_to_dist ); //distance in mm
//  Serial.println(encoder0Pos);
//  Serial.print("  Distance: ");
//  Serial.println(dist);
} 
