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
int a[4];
int dist_final[3]; // distance array 


void main(){
	int b;
	TRISB = 0b11110000;           //RB4,5,6,7 as Input PIN (ECHO)
	TRISD = 0x00; 				// LCD Pins as Output

	T1CON = 0x10;                 //Initialize Timer Module

	LATBbits.LATB0 = 1; //DISABLES KPD
    
	initLCD();

	while(1){
		TMR1H = 0;                  //Sets the Initial Value of Timer
		TMR1L = 0;                  //Sets the Initial Value of Timer

		if ((a[0]>=2 && a[0] <= 15) ||(a[1]>=2 && a[1] <= 15)||(a[2]>=2 && a[2] <= 15)||(a[3] >=2 && a[3]<= 15)){ 
			break;
		}

		for(b=1; b<5; b++){
			if(b==1){
				RB2 = 1;               //TRIGGER HIGH
				__delay_us(10);               //10uS Delay
				RB2 = 0;               //TRIGGER LOW

				while(!RB4);           //Waiting for Echo
				TMR1ON = 1;               //Timer Starts
				while(RB4);            //Waiting for Echo goes LOW
				TMR1ON = 0;               //Timer Stops
				a[0] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance

			}
			if(b==2){
				RB3 = 1;               //TRIGGER HIGH
				__delay_us(10);               //10uS Delay
				RB3 = 0;               //TRIGGER LOW

				while(!RB5);           //Waiting for Echo
				TMR1ON = 1;               //Timer Starts
				while(RB5);            //Waiting for Echo goes LOW
				TMR1ON = 0;               //Timer Stops
				a[1] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance

			}        
			if(b==3){
				RD0 = 1;               //TRIGGER HIGH
				__delay_us(10);               //10uS Delay
				RD0 = 0;               //TRIGGER LOW

				while(!RB6);           //Waiting for Echo
				TMR1ON = 1;               //Timer Starts
				while(RB6);            //Waiting for Echo goes LOW
				TMR1ON = 0;               //Timer Stops
				a[2] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance

			}        
			if(b==4){
				RD1 = 1;               //TRIGGER HIGH
				__delay_us(10);               //10uS Delay
				RD1 = 0;               //TRIGGER LOW

				while(!RB7);           //Waiting for Echo
				TMR1ON = 1;               //Timer Starts
				while(RB7);            //Waiting for Echo goes LOW
				TMR1ON = 0;               //Timer Stops
				a[3] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
			}
		}
		lcd_clear();
		if(a[0]>=2 && a[0]<=400){                //Check whether the result is valid or not
			printf("Distance 1 = %d",a[0]);
		}
		if(!(a[0]>=2 && a[0]<=400)){
			printf("Distance 1 = X");
		}
		if(a[1]>=2 && a[1]<=400){                //Check whether the result is valid or not
			lcd_set_ddram_addr(LCD_LINE2_ADDR);
			printf("Distance 2 = %d",a[1]);
		}		
		if(!(a[1]>=2 && a[1]<=400)){
			lcd_set_ddram_addr(LCD_LINE2_ADDR);
			printf("Distance 2 = X");
		}
		if(a[2]>=2 && a[2]<=400){                //Check whether the result is valid or not
			lcd_set_ddram_addr(LCD_LINE3_ADDR);
			printf("Distance 3= %d",a[2]);
		}
		if(!(a[2]>=2 && a[2]<=400)){
			lcd_set_ddram_addr(LCD_LINE3_ADDR);
			printf("Distance 3 = X");
		}
		if(a[3]>=2 && a[3]<=800){                //Check whether the result is valid or not
			lcd_set_ddram_addr(LCD_LINE4_ADDR);
			printf("Distance 4= %d",a[3]);
		}
		if(!(a[3]>=2 && a[3]<=800)){
			lcd_set_ddram_addr(LCD_LINE4_ADDR);
			printf("Distance 4 = X");
		}
    //__delay_ms(400);
	}
}
