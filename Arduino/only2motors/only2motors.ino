//Last tested Mar 7 11:20PM --> Both motors drive simultaneously, with different encoder readings.
//Pin Assignments 
const int wheel_d = 65;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 1920;   // (8 pairs N-S) * (120:1 gearbox) * (2 falling/rising edges)
const float count_to_dist = (wheel_c / counts_per_rev) ; // (1 tick = mm)

const int pwm_l = 11 ;  
const int mot_l_1 = 9 ;
const int mot_l_2 = 10 ;

const int pwm_r = 6 ;
const int mot_r_1 = 7 ; 
const int mot_r_2 = 8 ;

const int encoder1PinA = 2;
const int encoder1PinB = 4;

const int encoder2PinA = 3; 
const int encoder2PinB = 5;

//Variables 
long dist1;
long dist2;

long encoder1Pos = 0;
long encoder2Pos = 0;

int currentSpeed = 0;
long startTime = 0;

void setup() {
  pinMode(pwm_l,OUTPUT) ;   
  pinMode(pwm_r,OUTPUT) ;
  pinMode(mot_l_1,OUTPUT) ;  
  pinMode(mot_l_2,OUTPUT) ;
  pinMode(mot_r_1,OUTPUT) ;  
  pinMode(mot_r_2,OUTPUT) ;

  pinMode (encoder1PinA, INPUT);
  pinMode (encoder1PinB, INPUT);
  pinMode (encoder2PinA, INPUT);
  pinMode (encoder2PinB, INPUT);   
   
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), count1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), count2, CHANGE);
  
  startTime = millis();
  Serial.begin (115200);
}

void loop() {
  long count=0;
  
  //while(1){
  count = (millis()/1000)%2; //should alternate 0 and 1 because of the modulo 
  //if (count == 0){  
    drive(200);
  //}
  //}
  Serial.print("  Distance1: ");
  Serial.print(dist1);
  Serial.print("\t");
  Serial.print("  Distance2: ");
  Serial.println(dist2);
  
  }

void drive(int speed){

  digitalWrite(mot_l_1,HIGH) ;
  //Serial.println("high");
  
  digitalWrite(mot_l_2,LOW) ;
  //Serial.println("low");
  
    digitalWrite(mot_r_1,HIGH) ;
  //Serial.println("high");
  
  digitalWrite(mot_r_2,LOW) ;
  //Serial.println("low");
  analogWrite(pwm_l,speed) ;
  analogWrite(pwm_r,speed) ;
  
}

//void brake(motor_in1, motor_in2, pwm){
//  digitalWrite(motor_in1,HIGH) ;
//  digitalWrite(motor_in2,HIGH) ;
//  analogWrite(pwm,0) ;
//}

void count1() {
  encoder1Pos++;
  dist1 = (encoder1Pos * count_to_dist ); //distance in mm
//  Serial.println(encoder0Pos);
//  Serial.print("  Distance: ");
//  Serial.println(dist);
}
void count2() {
  encoder2Pos++;
  dist2 = (encoder2Pos * count_to_dist ); //distance in mm
}
