//Last tested Mar 7 11:20PM --> Both motors drive simultaneously, with different encoder readings.
//Pin Assignments 
const int wheel_d = 60;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 1920;   // (8 pairs N-S) * (120:1 gearbox) * (2 falling/rising edges)
const float count_to_dist = (wheel_c / counts_per_rev) ; // (1 tick = mm)
const float random_coeff= 2.31; 
//const float random_coeff= 2.4; 

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

volatile unsigned long encoder1Pos = 0;
volatile unsigned long encoder2Pos = 0;

volatile int speed1; 
volatile int speed2; 

int currentSpeed = 0;
long startTime = 0;

//const int offset = 3;


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
   
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), count1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), count2, RISING);
  
  startTime = millis();
  Serial.begin (115200);
}

void loop() {
  long count=0;
  
  //while(1){
  count = (millis()/1000)%2; //should alternate 0 and 1 because of the modulo 
  //if (count == 0){  
//    drive(200);
  //}
  //}
  Serial.print("  Distance1: ");
  Serial.print(dist1);
  Serial.print("\t");
  Serial.print("  Distance2: ");
  Serial.println(dist2);
  set_speed();
  }

void set_speed() { 
  //Constrain speed
  speed1 = constrain(speed1, 0, 255);
  speed2 = constrain(speed2, 0, 255);

  //Sample encoders 
  volatile unsigned long ticks1 = encoder1Pos; 
  volatile unsigned long ticks2 = encoder2Pos; 
  Serial.print("Ticks");
  Serial.print("\t");  
  Serial.print(ticks1);
  Serial.print("\t");
  Serial.println(ticks2);
  int diff = abs(ticks1-ticks2); 
  int offset = abs(ticks1-ticks2); 
//  int offset = abs(dist1-dist2); ;   
  if (diff != 0){
    
    if (ticks1>ticks2){
    
      speed1 = speed1 - offset; 
      speed2 = speed2 + offset; 
//      Serial.println("  Unveven: ");
  
    }
    if (ticks2>ticks1){
      speed1 = speed1 + offset; 
      speed2 = speed2 - offset; 
//      Serial.println("  Uneven ");
  
    }
  }
//  else if (ticks1==ticks1){
  if (diff == 0){
    speed1 = 255;
    speed2 = 255;
//    Serial.println("  wtf ");
  }
  Serial.print(speed1);
  Serial.print("\t");
  Serial.println(speed2);
//  if (speed1==0){
//    speed1 =0;
//  }
//  if (speed2==0){
//    speed2 =0;
//  }       
  if ((speed1<20) && (speed2<20)){
    speed1 = 255; 
    speed2 = 255; 
  }
  drive(speed1, speed2);

}

void drive(int speed1, int speed2){

  digitalWrite(mot_l_1,LOW) ;
  //Serial.println("high");
  
  digitalWrite(mot_l_2,HIGH) ;
  //Serial.println("low");
  
    digitalWrite(mot_r_1,LOW) ;
  //Serial.println("high");
  
  digitalWrite(mot_r_2,HIGH) ;
  //Serial.println("low");
  analogWrite(pwm_l,speed2) ;
  analogWrite(pwm_r,speed1) ;
  
}

//void brake(motor_in1, motor_in2, pwm){
//  digitalWrite(motor_in1,HIGH) ;
//  digitalWrite(motor_in2,HIGH) ;
//  analogWrite(pwm,0) ;
//}

void count1() {
  encoder1Pos++;
      dist1 = (encoder1Pos * count_to_dist * random_coeff); //distance in mm

//  dist1 = (encoder1Pos * count_to_dist ); //distance in mm
//  Serial.println(encoder0Pos);
//  Serial.print("  Distance: ");
//  Serial.println(dist);
}
void count2() {
  encoder2Pos++;
      dist2 = (encoder1Pos * count_to_dist * random_coeff); //distance in mm

//  dist2 = (encoder2Pos * count_to_dist ); //distance in mm
}
