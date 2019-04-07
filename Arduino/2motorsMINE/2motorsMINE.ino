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

int encoder2PinA = 2;
int encoder2PinB = 4;

int encoder1PinA = 3; 
int encoder1PinB = 5;

int encoder1Pos = 0;
int encoder2Pos = 0;
const int offset = 10;
int dist1;
int dist2;

//int currentSpeed = 0;
//long startTime = 0;
//int dir; 
int speed1 ;
int speed2 ;
int changed ; 

void count1();
void count2();
void set_speed(int dir, int enc1,int enc2,int speed1,int speed2);

void setup() {
  // put your setup code here, to run once:
  pinMode(pwm_l,OUTPUT) ;   //we have to set PWM pin as output
  pinMode(pwm_r,OUTPUT) ;
  pinMode(mot_l_1,OUTPUT) ;  //Logic pins are also set as output
  pinMode(mot_l_2,OUTPUT) ;
  pinMode(mot_r_1,OUTPUT) ;  //Logic pins are also set as output
  pinMode(mot_r_2,OUTPUT) ;

  pinMode (encoder1PinA, INPUT_PULLUP);
  pinMode (encoder1PinB, INPUT);
  pinMode (encoder2PinA, INPUT_PULLUP);
  pinMode (encoder2PinB, INPUT);    
  Serial.begin (115200);
 
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), count1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), count2, CHANGE);
}

void loop() {
  int count=0;
  
  //while(1){
  
  count = (millis()/1000)%2; 
  if (count==0){
//    Serial.println(dist1);  
    Serial.print("enc1:");
  Serial.print(encoder1Pos);
  Serial.print("\t");
  Serial.print("enc2:");
  Serial.println(encoder2Pos);
    }
  set_speed(1,encoder1Pos,encoder2Pos,speed1,speed2);

//  drive(1,speed1,speed2);
////  if (ultrasonic sensors triggered/pole detected){
////    brake();
////  }
//}
}

void set_speed(int dir, int enc1,int enc2,int speed1,int speed2){ 
//  //sample encoder counts 
//  if (changed!=1){
//    speed1 = 200; 
//    speed2 = 200; 
//  }
////  prev_speed1 = speed1; 
////  prev_speed2 = speed2; 
//  
//  long ticks1 = enc1; 
//  long ticks2 = enc2; 
//  Serial.print("ticks1:");
//  Serial.print(ticks2);
//  Serial.print("\t");
//  Serial.print("ticks2:");
//  Serial.println(ticks1);
//
//  Serial.print("Init Speed1:");
//  Serial.print(speed1);
//  Serial.print("\t");
//  Serial.print("Init Speed2:");
//  Serial.println(speed2);
//
////  *speed1 = constrain(*speed1, 0, 255);
////  *speed2 = constrain(*speed2, 0, 255);
//
//
//  if (ticks1!=ticks2){
//  speed1 = constrain(speed1, 0, 255);
//  speed2 = constrain(speed2, 0, 255);    
//  if (ticks1>ticks2){
//      speed1=speed1-offset; 
//      speed2=speed2+offset; 
//      changed = 1; 
//      Serial.print("Speed1:");
//      Serial.print(speed1);
//      Serial.print("\t");
//      Serial.print("Speed2:");
//      Serial.println(speed2);
//    }
//    if (ticks2>ticks1){
//      speed2=speed2-offset; 
//      speed1=speed1+offset; 
//      changed = 1; 
//      Serial.print("Speed1:");
//      Serial.print(speed1);
//      Serial.print("\t");
//      Serial.print("Speed2:");
//      Serial.println(speed2);    
//    }
//  }    
      speed1 = 100; 
    speed2 = 100; 
  drive(1,speed1,speed2);

}
//
void drive(int dir,int speed1,int speed2){
  if (dir==1){
    digitalWrite(mot_l_1,HIGH) ;
    //Serial.println("high");
    digitalWrite(mot_l_2,LOW) ;
    //Serial.println("low");
    digitalWrite(mot_r_1,HIGH) ;
    //Serial.println("high");
    digitalWrite(mot_r_2,LOW) ;
    //Serial.println("low");
    analogWrite(pwm_l,speed1) ;
    analogWrite(pwm_r,speed2) ;
  }
  else{
    digitalWrite(mot_l_1,LOW) ;
    //Serial.println("low");
    digitalWrite(mot_l_2,HIGH) ;
    //Serial.println("high");
    digitalWrite(mot_r_1,LOW) ;
    //Serial.println("low");
    digitalWrite(mot_r_2,HIGH) ;
    //Serial.println("high");
    analogWrite(pwm_l,speed1) ;
    analogWrite(pwm_r,speed2) ;
  }
}


void brake() {
  digitalWrite(mot_l_1, LOW);
  digitalWrite(mot_l_2, LOW);
  digitalWrite(mot_r_1, LOW);
  digitalWrite(mot_r_2, LOW);
  analogWrite(pwm_l, 0);
  analogWrite(pwm_r, 0);
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
//  Serial.println(encoder0Pos);
//  Serial.print("  Distance: ");
//  Serial.println(dist);
}
