 /* Read Quadrature Encoder
   Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.

   Sketch by max wolf / www.meso.net
   v. 0.1 - very basic functions - mw 20061220

*/

/*full rotation is 7 units*/ 
const int pwm = 6 ;  //initializing pin 2 as pwm
const int in_1 = 7 ;
const int in_2 = 8 ;
int val;
int encoder0PinA = 3;
int encoder0PinB = 5;
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
}

void loop() {
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    Serial.println (encoder0Pos);
    Serial.print ("/");
  }
    Serial.println (encoder0Pos);
    Serial.print ("/");  
  encoder0PinALast = n;

  //For Clock wise motion , in_1 = High , in_2 = Low
//
//  digitalWrite(in_1,HIGH) ;
//  digitalWrite(in_2,LOW) ;
//  analogWrite(pwm,55) ;
  
  /*setting pwm of the motor to 255
  we can change the speed of rotaion
  by chaning pwm input but we are only
  using arduino so we are using higest
  value to driver the motor  */
  
//  //Clockwise for 3 secs
//  delay(3000) ;     
//  
//  //For brake
//  digitalWrite(in_1,HIGH) ;
//  digitalWrite(in_2,HIGH) ;
//  delay(1000) ;
//  
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
