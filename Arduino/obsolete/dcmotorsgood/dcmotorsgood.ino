const int pwm = 6 ;  //initializing pin 2 as pwm
const int in_1 = 7 ;
const int in_2 = 8;
const int pwm1 = 11 ;  //initializing pin 2 as pwm
const int in1_1 = 9 ;
const int in1_2 = 10;

//For providing logic to L298 IC to choose the direction of the DC motor 

void setup()
{
pinMode(pwm,OUTPUT) ;   //we have to set PWM pin as output
pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
pinMode(in_2,OUTPUT) ;
pinMode(pwm1,OUTPUT) ;   //we have to set PWM pin as output
pinMode(in1_1,OUTPUT) ;  //Logic pins are also set as output
pinMode(in1_2,OUTPUT) ;
}

void loop()
{
/*setting pwm of the motor to 255
we can change the speed of rotaion
by chaning pwm input but we are only
using arduino so we are using higest
value to driver the motor  */
//For Clock wise motion , in_1 = High , in_2 = Low

digitalWrite(in_1,LOW) ;
digitalWrite(in_2,HIGH) ;
analogWrite(pwm,255) ;
digitalWrite(in1_1,LOW) ;
digitalWrite(in1_2,HIGH) ;
analogWrite(pwm1,255) ;

//Clockwise for 3 secs
//delay(2000) ;     

//For brake
//digitalWrite(in_1,HIGH) ;
//digitalWrite(in_2,HIGH) ;
////digitalWrite(in1_1,HIGH) ;
////digitalWrite(in1_2,HIGH) ;
//delay(2000) ;

//lower speed
//digitalWrite(in_1,HIGH) ;
//digitalWrite(in_2,LOW) ;
//analogWrite(pwm,170) ;
////digitalWrite(in1_1,HIGH) ;
////digitalWrite(in1_2,LOW) ;
////analogWrite(pwm1,170) ;
//delay(3000);
/*setting pwm of the motor to 255
we can change the speed of rotaion
by chaning pwm input but we are only
using arduino so we are using higest
value to driver the motor  */

//Clockwise for 3 secs
//delay(3000) ; 

////For Anti Clock-wise motion - IN_1 = LOW , IN_2 = HIGH
//digitalWrite(in_1,LOW) ;
//digitalWrite(in_2,HIGH) ;
//digitalWrite(in1_1,LOW) ;
//digitalWrite(in1_2,HIGH) ;
//delay(2000) ;
////
//////For brake
//digitalWrite(in_1,HIGH) ;
//digitalWrite(in_2,HIGH) ;
//digitalWrite(in1_1,HIGH) ;
//digitalWrite(in1_2,HIGH) ;
//delay(1000) ;
 }
