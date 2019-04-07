/* March 11 2:25am : This program runs 4 ultrasonic sensors round robin. tested with 4 but RB4 has no signal. The trigger for RB4 (RB2) is blinking as expected.
 *  (tested with 3 and confirmed in multisensor.c. The actual file to run only sensors is ultrasonicsens.X */
#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "I2C.h"
#include "lcd.h"

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

int dist_final[3]; // distance array 
int a[4];
int b;

void __interrupt() echo(){
	if(RBIF == 1){                       //Makes sure that it is PORTB On-Change Interrupt
		RBIE = 0;                         //Disable On-Change Interrupt
		if (b==1){
			if (RB7 == 1){                     //If ECHO is HIGH
				TMR1ON = 1; 
			}                      //Start Timer
			if (RB7 == 0){                     //If ECHO is LOW
				TMR1ON = 0;                     //Stop Timer
				a[0] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
				// a[0] = (TMR1L | (TMR1H<<8))*0.034/2; 
			}
		}
		if (b==2){
			if (RB6 == 1){                     //If ECHO is HIGH
				TMR1ON = 1; 
			}                      //Start Timer
			if (RB6 == 0){                     //If ECHO is LOW        
				TMR1ON = 0;                     //Stop Timer
				a[1] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
				// a[1] = (TMR1L | (TMR1H<<8))*0.034/2; 
			}
		}
		if (b==3){
			if (RB5 == 1){                     //If ECHO is HIGH
				TMR1ON = 1; 
			}                      //Start Timer
			if (RB5 == 0){                     //If ECHO is LOW
				TMR1ON = 0;                     //Stop Timer
				a[2] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
				// a[2] = (TMR1L | (TMR1H<<8))*0.034/2; 
			}
		}
        if (b==4){
			if (RB4 == 1){                     //If ECHO is HIGH
				TMR1ON = 1; 
			}                      //Start Timer
			if (RB4 == 0){                     //If ECHO is LOW
				TMR1ON = 0;                     //Stop Timer
				a[3] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
				// a[3] = (TMR1L | (TMR1H<<8))*0.034/2; 
			}
		}
	}
	RBIF = 0;                           //Clear PORTB On-Change Interrupt flag
	RBIE = 1;                           //Enable PORTB On-Change Interrupt
}

void us_main(){
	TRISB = 0b11100000;                 //RB5 6 as Input PIN (ECHO)
	TRISD = 0x00;                       // LCD Pins as Output
	GIE = 1;                            //Global Interrupt Enable
	RBIF = 0;                           //Clear PORTB On-Change Interrupt Flag
	RBIE = 1;                           //Enable PORTB On-Change Interrupt

	initLCD();
	//  int b;

	__delay_ms(3000);
	lcd_clear();

	T1CON = 0x10;                       //Initialize Timer Module

	while(1){
		for(b=1; b<5; b++){
			TMR1H = 0;                        //Sets the Initial Value of Timer
			TMR1L = 0;                        //Sets the Initial Value of Timer
			if(b==1){
				RD1 = 1;                          //TRIGGER HIGH
				__delay_us(10);                   //10uS Delay
				RD1 = 0;                          //TRIGGER LOW 
				__delay_ms(100);                  //Waiting for ECHO
			}
			if(b==2){
				RD0 = 1;                          //TRIGGER HIGH
				__delay_us(10);                   //10uS Delay
				RD0 = 0;                          //TRIGGER LOW 
				__delay_ms(100);                  //Waiting for ECHO
			}
			if(b==3){
				RB3 = 1;                          //TRIGGER HIGH
				__delay_us(10);                   //10uS Delay
				RB3 = 0;                          //TRIGGER LOW 
				__delay_ms(100);                  //Waiting for ECHO
			}
            if(b==4){
				RB2 = 1;                          //TRIGGER HIGH
				__delay_us(10);                   //10uS Delay
				RB2 = 0;                          //TRIGGER LOW 
				__delay_ms(100);                  //Waiting for ECHO
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
            if(a[3]>=2 && a[3]<=400){                //Check whether the result is valid or not
				lcd_set_ddram_addr(LCD_LINE4_ADDR);
				printf("Distance 4= %d",a[3]);
			}
            if(!(a[3]>=2 && a[3]<=400)){
                lcd_set_ddram_addr(LCD_LINE4_ADDR);
                printf("Distance 4 = X");
            }
			__delay_ms(400);   
		}
	}
}

