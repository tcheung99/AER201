#include <Wire.h>

#define SENSORPIN2 12
#define SENSORPIN A3

///Motor setup
const int transferr = A2; // This is our input pin
const int led = A3; 

const int wheel_d = 65;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 1920;   // (8 pairs N-S) * (120:1 gearbox) * (2 falling/rising edges)
const float count_to_dist = (wheel_c / counts_per_rev) ; // (1 tick = mm)
const float random_coeff= 2.1; 

//const float random_coeff= 2.1375; //accurate at 75cm
//const float random_coeff= 2.1; //accurate at 50cm
//const float random_coeff= 2.175; //accurate at 100cm
//const float random_coeff= 2; //accurate at 400cm

//y = -(0.175/3000)*x+ 2.17583333;

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
volatile int ir_seen = 0; 
volatile int tire1_tick_beg =0;
volatile int tire2_tick_beg =0;
volatile int tire1_tick_end =0;
volatile int tire2_tick_end =0;
volatile int tire2_dist=0;
volatile int tire1_dist=0;

int LED = 13; // Use the onboard Uno LED
int isObstaclePin = A0; // This is our input pin
int isObstacle = HIGH; // HIGH MEANS NO OBSTACLE

int isObstaclePin2 = A1; // This is our input pin
int isObstacle2 = HIGH; // HIGH MEANS NO OBSTACLE

//Variables

volatile int cyl_seen = 0; 
volatile int cyl_seen2 = 0; 
 
volatile int sensorState = 0, lastState=0; // variable for reading the pushbutton status
volatile int sensorState2 = 0, lastState2=0; // variable for reading the pushbutton status

long dist1;
long dist2;
//volatile long avg_dist; 
volatile float avg_dist_float; 
volatile int avg_dist; 
long prev_avg_dist=0; 

volatile unsigned long encoder1Pos = 0;
volatile unsigned long encoder2Pos = 0;

volatile int speed1; 
volatile int speed2; 

volatile bool forward = true; 
volatile int stopp=0; 

bool sense = false;

long dist_total = 0; 

int currentSpeed = 0;
long startTime = 0;

//const int offset = 5;

//I2C setup 
volatile bool action = false ; 
volatile bool delaygo = false ; 
volatile bool send_to_pic = false;
volatile uint8_t incomingByte; //unsigned integer of length 8 bits (1 byte) 
volatile uint8_t prev_incomingByte;

long loop_cnt=0;

void setup(){
  Wire.begin(8); // Join I2C bus with address 8 (slave address)
  
  // Register callback functions
  Wire.onReceive(receiveEvent); // Called when this slave device receives a data transmission from master
  Wire.onRequest(requestEvent); // Called when master requests data from this slave device  

  //Motor pin assignments 
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

  pinMode(A2, OUTPUT);
  pinMode(led, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  pinMode(isObstaclePin2, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder1PinA), count1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), count2, RISING);
  
pinMode(SENSORPIN, INPUT);
pinMode(SENSORPIN2, INPUT);
digitalWrite(SENSORPIN, HIGH); // turn on the pullup
digitalWrite(SENSORPIN2, HIGH); // turn on the pullup

  startTime = millis();
  // Open serial port to PC (hardware UART)
  Serial.begin(9600);  
}

void loop(){
//  Serial.print("cylseen");
//  Serial.print(cyl_seen);
//  Serial.print("\t");
//  Serial.println(cyl_seen2);
sensorState = digitalRead(SENSORPIN);
sensorState2 = digitalRead(SENSORPIN2);
  if (!send_to_pic){
    digitalWrite(A2, LOW);
    if (action&&!delaygo) {
      Serial.println("WHAA");
      ir_sensor();
  ir_bb();
      if ((cyl_seen==0)&&(cyl_seen2==0)){
//          if ((cyl_seen2==0)){
        Serial.println("bruh");
        set_speed();
        drive(speed1, speed2);
      }
    }
    if (action&&delaygo){
  ir_bb();
      Serial.println("delaygo");
      if (loop_cnt==20){
        Serial.print("\t");
        Serial.print("\t");
        Serial.println("what the ");
        sense=true;
      }
      if (sense==true){
        Serial.print("\t");
        Serial.print("\t");
        Serial.println("normal ");

        ir_sensor();
        if ((cyl_seen==0)&&(cyl_seen2==0)){
          set_speed();
          drive(speed1, speed2);
        }
      }
      if (sense==false){
        Serial.print("\t");
        Serial.print("\t");
        Serial.println("wtfwtfwtf");
        
        set_speed();
        drive(speed1, speed2);
      }
    }
    if (action && !forward){

      avg_dist = 0; 
      while (avg_dist<dist_total){
        set_speed();
        drive(speed1, speed2);
      }
    }
  }
  if (send_to_pic) { //if sending to PIC, not moving, and the sending byte is not empty
    digitalWrite(A2, HIGH);
    Serial.println("keep send");
    if (!action && !incomingByte) {
      incomingByte = avg_dist; 
      Serial.println(incomingByte);
//        avg_dist = 0; //Reset the pole-to-pole distance 
    }
  }
  loop_cnt++;
}
  
void requestEvent(void){
//    prev_incomingByte = incomingByte;
//  if (action){
    incomingByte = avg_dist; 
    digitalWrite(A2, LOW);
    Wire.write(incomingByte); // Respond with message of 1 byte for sensor distance
    incomingByte = 0; // Clear output buffer
    avg_dist = 0; 
    send_to_pic = false;
//  } 
}

void receiveEvent(int){
  static uint8_t counter = 0;
  
  uint8_t x = Wire.read(); // Receive byte
  Serial.println((char)x); // Print to serial output as char (ASCII representation)
  loop_cnt=0;
  if (x == '1'){
    action = true;
    digitalWrite(A2, LOW);
    forward = true; 
    delaygo = false; 
    prev_incomingByte =0;
    send_to_pic = false;
    stopp = 0;
//    ir_seen = false;    
  }    
  if (x == '2'){
    loop_cnt=0;  
    action = true;
    digitalWrite(A2, LOW);
    stopp = 0;
    forward = true; 
    delaygo = true; 
    prev_incomingByte =0;
    sense = false;
    send_to_pic = false;
//    cyl_seen = 0; 
    
    speed1 = 200; 
    speed2 = 200; 
    
//    cyl_seen2 = 0; 
//    ir_seen = false;
  }
  
  if (x == '5'){
    action = true;
    digitalWrite(A2, LOW);
    send_to_pic = false;

    forward = false;      
    prev_incomingByte =0;
  }  
  
  if (x == '9'){
    action = false; //brake
    digitalWrite(A2, HIGH);  
    send_to_pic = true;
  }
}

void set_speed() { 
  if (stopp == 0){
    //Constrain speed
    speed1 = constrain(speed1, 0, 255);
    speed2 = constrain(speed2, 0, 255);
    
    //Sample encoders 
    volatile unsigned long ticks1 = encoder1Pos; 
    volatile unsigned long ticks2 = encoder2Pos; 
    Serial.print("Ticks: ");
    Serial.print(ticks1);
    Serial.print("\t");
    Serial.println(ticks2);
    
    Serial.print("Dist: ");
    Serial.print(dist1);
    Serial.print("\t");
    Serial.println(dist2);
  
    avg_dist_float = float(((float((dist1 + dist2 ))/float(2)))/10); //avg distance in cm 
    avg_dist = (int)(avg_dist_float + 0.5); //casting avg distance into an int and rounding 
  
    Serial.println("Avgdist: ");
    Serial.println(avg_dist);
    
//    float diff = ticks1/ticks2; /////////////////
//    if (diff >1.2){ //ticks 1 is much greater than 2 ///////////////
//       speed1 = speed1 - offset; 
//       speed2 = speed2 + offset;  
//    }
//    if (diff <1.2){ //ticks 2 is much greater than 1 
//        speed1 = speed1 + offset; 
//        speed2 = speed2 - offset;  
//    }
//    if (diff == 1.2){
//      speed1 = 255;
//      speed2 = 255;  
//    }    
    
    int diff = abs(ticks1-ticks2); 
  //  int offset = abs(dist1-dist2); ;    
    int offset = 2;   
    
    if (diff != 0){
      if (ticks1>ticks2){
        speed1 = speed1 - offset; 
        speed2 = speed2 + offset;   
      }
      if (ticks2>ticks1){
        speed1 = speed1 + offset; 
        speed2 = speed2 - offset;   
      }
    }
    if (diff == 0){
      speed1 = 255;
      speed2 = 255;
  //    speed1 = 245;
  //    speed2 = 245;    
  //    speed1 = 180;
  //    speed2 = 180;
  //    Serial.println("  wtf ");
    }
    if (((speed1)<25)&&((speed2)<25)){
      speed1 = 255;
      speed2 = 255;
  //    speed1 = 245; 
  //    speed2 = 245; 
  //    speed1 = 180;
  //    speed2 = 180;
    }
  Serial.print("Speed: ");
  Serial.print(speed1);
  Serial.print("\t");
  Serial.println(speed2);
//  drive(speed1, speed2);
  }
  digitalWrite(A2, LOW);
}
void drive(int speed1, int speed2){
  if (stopp==0){
    if ((action)&&(forward==true)){ 
      digitalWrite(A2, LOW);
      
      digitalWrite(mot_l_1,HIGH) ;    
      digitalWrite(mot_l_2,LOW) ;
      
      digitalWrite(mot_r_1,HIGH) ;    
      digitalWrite(mot_r_2,LOW) ;
      
      analogWrite(pwm_l,speed2) ;
      analogWrite(pwm_r,speed1) ;
    }
    if ((action)&&(forward==false)){
      digitalWrite(A2, LOW);
  
      digitalWrite(mot_l_1,LOW) ;    
      digitalWrite(mot_l_2,HIGH) ;
      
      digitalWrite(mot_r_1,LOW) ;    
      digitalWrite(mot_r_2,HIGH) ;
  
      analogWrite(pwm_l,speed2) ;
      analogWrite(pwm_r,speed1) ;
    }
    if (!action){ //If action is false, brake
      brake();
    }
  }
}

void brake(){
  stopp = 1; 
  action = false;
  digitalWrite(mot_l_1,LOW) ;    
  digitalWrite(mot_l_2,LOW) ;
  
  digitalWrite(mot_r_1,LOW) ;    
  digitalWrite(mot_r_2,LOW) ;
  
  analogWrite(pwm_l,0) ;
  analogWrite(pwm_r,0) ;
  
  digitalWrite(A2, HIGH);
  Serial.println("braking");

  dist_total = dist_total+avg_dist; 
  
  Serial.print("  AVG_DIST: ");
  Serial.print(avg_dist);
  Serial.print("\t");
  Serial.print("  DIST_TOT ");
  Serial.println(dist_total);
  send_to_pic = true;

//  avg_dist = 0; 
}
void ir_sensor(){
  isObstacle = digitalRead(isObstaclePin);
  isObstacle2 = digitalRead(isObstaclePin2);  
  if ((isObstacle == LOW)&&(cyl_seen==0)) { 
    cyl_seen = 1; 
    Serial.println("OBSTACLE!!, OBSTACLE!!");
  }
  if (isObstacle2 == LOW) { 
    analogWrite(led , 255); 
    cyl_seen2 = 1; 
    Serial.println("OBSTACLE2");
  }  
  if (isObstacle2 == HIGH) { 
    cyl_seen2 = 0; 
  }
  if ((cyl_seen==1)&&(cyl_seen2==0)){
    speed1 = 80;
    speed2 = 80;   
  ir_bb();
    drive(speed1,(speed2)); //drive slower  
  }
  if ((cyl_seen==1)&&(cyl_seen2==1)){
    brake();
    Serial.println("break");
  }
  if ((cyl_seen==1)&&(cyl_seen2==1)&&(isObstacle2 == HIGH)){
    cyl_seen = 0; 
    cyl_seen2 = 0; 
  tire1_tick_beg = 0;
  tire2_tick_beg = 0; 
  tire1_tick_end = 0;
  tire2_tick_end = 0;
  tire1_dist = tire1_tick_end - tire1_tick_beg;
  tire2_dist = tire2_tick_end - tire2_tick_beg;
  
    Serial.println("REST");    
  Serial.println(tire1_dist);
    Serial.println(tire2_dist);
  }
}

void ir_bb(){
  sensorState = digitalRead(SENSORPIN);
  sensorState2 = digitalRead(SENSORPIN2);
  if (sensorState && !lastState&&(tire1_tick_beg==0)) { //first start 
    Serial.println("Unbroken1");
    tire1_tick_beg = encoder1Pos;
  }
  if (sensorState2 && !lastState2&&(tire2_tick_beg==0)) {
    Serial.println("Unbroken2");
    tire2_tick_beg = encoder1Pos;

  }
  if (!sensorState && lastState&&(tire1_tick_end==0)) { //first end 
  Serial.println("Broken");
      tire1_tick_end = encoder1Pos;       
  }
  if (!sensorState2 && lastState2&&(tire2_tick_end==0)) {
    Serial.println("Broken2");
    tire2_tick_end = encoder1Pos;
  }
  lastState = sensorState;
  lastState2 = sensorState2;
}

void count1() {
  if (action){
    encoder1Pos++;
    dist1 = (encoder1Pos * count_to_dist * random_coeff); //distance in mm
  }
}
void count2() {
  if (action){
    encoder2Pos++;
    dist2 = (encoder2Pos * count_to_dist * random_coeff); //distance in mm
  }
}
