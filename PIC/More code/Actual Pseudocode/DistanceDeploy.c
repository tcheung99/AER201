// This program determines number of tires to deploy based on existing number of tires and distance travelled.

//Check Ultrasonic sensor for signal for pole or tire position. **Note one pin IN and one OUT for sensor. 
// Provide TRIGGER to ultrasonic module
// Listen for Echo
// Start Timer when ECHO HIGH is received
// Stop Timer when ECHO goes LOW
// Read Timer Value
// Convert it to Distance
// Display it 

//initialize Timer Module 
T2CON = 0b10000000; 

void main_deploy{
	while(1){
		// Set the Initial Value of Timer0
		// Trigger ultrasonic sensor (set port high) 
		if (motors stopped){
			
			t_deployed_tot = t_deployed + t_deployed_tot; 
			t_deployed = 0;
			number_deploy() //determine number to deploy 
			dispense() //dispense tires 
			pole_tot++
		}
	}
}

//Interrupt when ECHO change (INTERRUPT ON CHANGE)  
void __interrupt () ECHO(void){
	disable interrupt 
	if ECHO high{
		start TIMER0
		keep going some dstance to see if both sensors are trigerred 
		stop motors (brake) //communicate with Arduino. 
	}
	if ECHO low{
		stop TIMER0
		calculate distance // d = (speed of sound*Time)/2
	}
	clear interrupt flag 
	set (lower/upper)sensor(1/2) flag
	enable interrupt 
}

void number_deploy(int distance, poles_detected){
	while (DC motors off){
		if (poles_detected != 0){				
			if (distance<30){
				//Total number of tires tires_t
				tires_t = 1; 
			}
			else{
				tires_t = 2; 
			}
		}
		if (poles_detected == 0){
			tires_t = 2;
		}
		if (lowersensors){
			if (uppersensors){
				if (>2 sensors high){
					tires_detected = 2;
				}
				else{
					tires_detected = 0;
				}
			}
			else{
				tires_detected = 1;
			}
		}
		t_count = tires_t - tires_detected;
	}
}
//include a sensor 
void dispense{
	int stepCount = 0;  // number of steps the motor has taken
	for (int i=0; i<t_count){
		//set steps_init1 and steps_init2 for initial trap door position (below each stack) 
		//set steps_push (determine number of duty cycles through testing # required to push a tire out of stack) 
		if(t_deployed<7){ //tires in stack 1 
			setting = steps_init1;
			stack=1; //stack 1 
		}
		if (t_deployed>=7 ){ //no tires in stack 1 & tires in stack 2 
			setting = steps_init2;
			stack = 2; 
		}
		if (t_deployed=15){ //no tires in either stack
			stop operation (brake) 
			branch to UI design (display menu on LCD)
		}
		//Set trapdoor placement
		stepper_loop(stepper, setting);
		//Push tires
		if (stack==1){
		servo(servo1,steps_push1);
		__delay_ms(5000);
		}
		if (stack==2){
		servo(servo2, steps_push2);
		__delay_ms(5000);
		}
		stepper_loop(steps_move); //moves based on ultrasonic sensor distance calculation 
		servo() //rotate microservo to open trapdoor 
		reverse trapdoor stepper direction (return to setting) //counterclockwise rotation 
		i++;
		t_deployed++
	}

//Controlling the stepper motor. Stepper: 4 coils 
// (360 ° / 5.625 °) * 64 = 4096 steps 
void stepper_loop(int stepper, int steps) {
	//set period
	//Rotate Stepper Motor clockwise with Full step sequence
	//stepper controls which stepper to use (1,2 are stack 1, 2 respectively, 3 is for trapdoor)
	for(int i=0;i<steps;i++){
		LATports = 0x09;			//Write to ports
		__delay_ms(period);
		LATports = 0x0C;
		__delay_ms(period);
		LATports = 0x06;
		__delay_ms(period);
		LATports = 0x03;
		__delay_ms(period);
	}
	LATports = 0x09; //Return to original position
	__delay_ms(period);	
}

 
void servo(){
	//Controlling servo-use PWM (p. 7-154) 
	//Use TIMER2, CCP1CON (PWM module)
	
	//set PR2 register to match frequency (PR2 register sets the PWM period) 
	//Set PWM duty cycle on CCPRxL:CCPxCON<5:4>
	//Configure CCPxCON
	//Set TIMER2 prescale values and enable TIMER2 
	//Set internal oscillator
	//Infinite loop with CCPx pin as output, and connect this to servo input 
} 	

	
	





