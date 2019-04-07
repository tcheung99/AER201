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

int LED = 13; // Use the onboard Uno LED
int isObstaclePin = A0; // This is our input pin
int isObstacle = HIGH; // HIGH MEANS NO OBSTACLE

//Variables 
long dist1;
long dist2;
long sum_dist;

volatile float avg_dist_float; 
volatile int avg_dist; 

volatile unsigned long encoder1Pos = 0;
volatile unsigned long encoder2Pos = 0;

volatile int speed1; 
volatile int speed2; 
volatile int stopp=0; 

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
//  Serial.begin (115200);
  Serial.begin (9600);
      pinMode(LED, OUTPUT);
    pinMode(isObstaclePin, INPUT);
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

  sum_dist = dist1+dist2;
  avg_dist_float = float(((float((dist1 + dist2 ))/float(2)))/10); //avg distance in cm 
  avg_dist = (int)(avg_dist_float + 0.5); //casting avg distance into an int and rounding 
  Serial.println("Avgdist: ");
  Serial.print(sum_dist);
  Serial.print("\t");
  Serial.print(avg_dist_float);
  Serial.print("\t");
  Serial.println(avg_dist);
    set_speed();
  }

void set_speed() { 
  //Constrain speed
  ir_sensor();
  if (stopp == 0){
  speed1 = constrain(speed1, 0, 255);
  speed2 = constrain(speed2, 0, 255);

  //Sample encoders 
  volatile unsigned long ticks1 = encoder1Pos; 
  volatile unsigned long ticks2 = encoder2Pos; 
  
  Serial.print(ticks1);
  Serial.print("\t");
  Serial.println(ticks2);
  int diff = abs(ticks1-ticks2); 
  
  //int offset = abs(dist1-dist2); ;
  int offset = 2;   
  if (diff != 0){
    
    if (ticks1>ticks2){
    
      speed1 = speed1 - offset; 
      speed2 = speed2 + offset; 
      Serial.println("  Unveven: ");
  
    }
    if (ticks2>ticks1){
      speed1 = speed1 + offset; 
      speed2 = speed2 - offset; 
      Serial.println("  Uneven ");
  
    }
  }
//  else if (ticks1==ticks1){
  if (diff == 0){
    speed1 = 255;
    speed2 = 255;
    Serial.println("  wtf ");
  }
  Serial.print(speed1);
  Serial.print("\t");
  Serial.println(speed2);
//     avg_dist_float = (((dist1 + dist2 )/2)/1000); //avg distance is cm 
//     avg_dist_float = (((dist1 + dist2 )/2)); //avg distance is mm 


//  if (speed1==0){
//    speed1 =0;
//  }
//  if (speed2==0){
//    speed2 =0;
//  }
//  if ((speed1 && speed2)<25){
  if (((speed1)<25)&&((speed2)<25)){
    speed1 = 255; 
    speed2 = 255; 
  }           
  drive(speed1, speed2);
  }
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
void ir_sensor(){
    isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW) {
//    Serial.println("OBSTACLE!!, OBSTACLE!!");
    brake();
    digitalWrite(LED, HIGH);
  } 
  else {
//    Serial.println("clear");
    digitalWrite(LED, LOW);
    stopp=0;
  }
}
void brake(){
  stopp = 1; 
     digitalWrite(mot_l_1,LOW) ;    
    digitalWrite(mot_l_2,LOW) ;
    
    digitalWrite(mot_r_1,LOW) ;    
    digitalWrite(mot_r_2,LOW) ;
    
      analogWrite(pwm_l,0) ;
  analogWrite(pwm_r,0) ;
  
}



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
