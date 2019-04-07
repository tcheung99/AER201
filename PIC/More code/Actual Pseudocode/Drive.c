//(Arduino) Driving DC motors straight using PWM 

//Set parameters, pins, interrupts 
//Encoder is 16 counts per revolution (CPR)

void drivestraight(){
	set initial motor power 
	reset encoder counts (left and right encoders)
	calculate/set encoder target count (based on robot deployment range) 	
	while (enc_l < target_count && enc_r < target_count) {
		//set temporary variable 
		temp_l = enc_l;
		temp_r = enc_r;
		//compute difference 
		diff_l = temp_l - enc_l_prev;
		diff_r = temp_r - enc_r_prev;
		//Set the next values for enc_prev
		enc_l_prev = temp_l;
		enc_r_prev =temp_r ;
		if ( diff_l > diff_r ) {
			decrease left motor speed
			increase right motor speed
		}
		if ( diff_l > diff_r ) {
			decrease right motor speed
			increase left motor speed
		}
	}
}
	
void drive(){
	//Set DC motor direction 
	digitalWrite() 
	//Set DC motor speed
	analogWrite() //Writes analog values (PWM wave) to a pin 
} 

//distance function can be implemented to determine pole-to-pole distance by setting variables for each START to POLE distance (dist1, dist2...) 
void distance(){
	ticks = number of times sensor is triggerred
	if (digitalRead(HallEffectSensorpin) == HIGH){
		ticks++	
	}
	calculate distance using wheel circumference, # of ticks per revolution, # motor rev/wheel 
	if (dist_total >= employment range){
		stop powering motors (brake) 
	}
}

void brake(){
	analogWrite(pins, 0) //no power 
}


