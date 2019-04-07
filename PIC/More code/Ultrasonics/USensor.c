//Check Ultrasonic sensor for signal for pole or tire position. **Note one pin IN and one OUT for sensor. 
// Provide TRIGGER to ultrasonic module
// Listen for Echo
// Start Timer when ECHO HIGH is received
// Stop Timer when ECHO goes LOW
// Read Timer Value
// Convert it to Distance
// Display it 

#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "I2C.h"
#include "lcd.h"
int dist = 0; 
int dist_final[3]; // distance array 

void __interrupt() echo(void){
	if(RBIF == 1){                //if PORTB On-Change Interrupt Flag is up 
		RBIE = 0;                   //Disable On-Change Interrupt
		if(PORTBbits.RB4 == 1){				//If ECHO is HIGH
			TMR1ON = 1;               //Start Timer
		}
		if(PORTBbits.RB4 == 0){               //If ECHO is LOW
			TMR1ON = 0;               //Stop Timer
			dist = (TMR1L | (TMR1H<<8))/58.82;  //Calculate Distance
		}
	}
	RBIF = 0;                     //Clear PORTB On-Change Interrupt flag
	RBIE = 1;                     //Enable PORTB On-Change Interrupt
}

void main() {
	lcd_clear();
	printf(" hfdasf: ");
    TRISB = 0b00010000;           //RB4 as Input PIN (ECHO), rest are output 
	TRISD = 0x00; 				//LCD Pins as Output
	GIE = 1	;			//Global Interrupt Enable
	RBIF = 0;              //Clear PORTB On-Change Interrupt Flag
	RBIE = 1	;			//Enable PORTB On-Change Interrupt

	T1CON = 0x10;                 //Initialize Timer Module
    
	while(1){
		TMR1H = 0;                  //Sets the Initial Value of Timer
		TMR1L = 0;                  //Sets the Initial Value of Timer

		PORTBbits.RB0 = 1;					//TRIGGER HIGH
//        LATDbits.LATD0 = 1; 
		__delay_us(10);             //10uS Delay
		PORTBbits.RB0 = 0;               //TRIGGER LOW
        
		__delay_ms(100); //Waiting for ECHO
		dist = dist + 1; //Error Correction Constant

		if(dist>=2 && dist<=400){         //Check whether the result is valid or not
			lcd_clear();
			printf(" Distance: ");
			for (int i=0; i<3; i++){
				dist_final[i] = dist%10 + 48; 
				dist = dist/10;
			}
			lcd_set_ddram_addr(LCD_LINE2_ADDR);
			printf("%d%d%d", dist_final[2],dist_final[1],dist_final[0]);
		}
		else {
			lcd_clear();
			printf(" out of range: ");
		}
		__delay_ms(400);
	}
}

