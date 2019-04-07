#include <Wire.h>

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
volatile uint8_t distance; //usensor dist 

void setup(){
    Wire.begin(8); // Join I2C bus with address 8 (slave address)
  
    // Register callback functions
    Wire.onReceive(receiveEvent); // Called when this slave device receives a data transmission from master
    Wire.onRequest(requestEvent); // Called when master requests data from this slave device
  
    // Open serial port to PC (hardware UART)
    Serial.begin(9600);    

    pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
    pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
    pinMode(in_2,OUTPUT) ;
    
    pinMode (encoder0PinA, INPUT);
    pinMode (encoder0PinB, INPUT);  
    
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    
    attachInterrupt(digitalPinToInterrupt(encoder0PinA), count, CHANGE);  
}

volatile bool action = false ; 
volatile bool send_to_pic = false;
volatile uint8_t incomingByte; //unsigned integer of length 8 bits (1 byte) 
volatile uint8_t prev_incomingByte;
void loop(){
    // If we should send to the PIC, then we wait to receive a byte from the PC
    if (action) {
        drive();
    }
    if (send_to_pic && !action && !incomingByte) {
        //Serial.println(incomingByte);  
        if (prev_incomingByte == distance){
          incomingByte = dist;

        }
        else{
        incomingByte = distance;
        }
        Serial.println(incomingByte);
               
    }
    
}
void requestEvent(void){
    prev_incomingByte = incomingByte;
    Wire.write(incomingByte); // Respond with message of 1 byte for sensor distance
    incomingByte = 0; // Clear output buffer

}
/** @brief Callback for when the master transmits data */
void receiveEvent(int){
    static uint8_t buf[3] = {0};
    static uint8_t counter = 0;
    
    uint8_t x = Wire.read(); // Receive byte
    Serial.println((char)x); // Print to serial output as char (ASCII representation)

    if (x == '1'){
      action = true;
      prev_incomingByte =0;
      
    }
}

void drive(){
//Serial.println("drive");
  //sens(); 
  if (action){
  digitalWrite(in_1,HIGH) ;
  digitalWrite(in_2,LOW) ;
  analogWrite(pwm,160) ;
  delay(1000) ;
    
//For brake
//  digitalWrite(in_1,HIGH) ;
//  digitalWrite(in_2,HIGH) ;
//delay(500) ;
  }
}

void count() {
  if (action){
  encoder0Pos++;
  dist = (encoder0Pos * count_to_dist ); //distance in mm
  Serial.print(encoder0Pos);
  Serial.print("  Distance: ");
  Serial.println(dist);
  sens();
  }
}

void sens(){ 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Sensor Distance: ");
  Serial.println(distance);
  if (distance<25){
    action = false;
    send_to_pic = true;
  }
}
