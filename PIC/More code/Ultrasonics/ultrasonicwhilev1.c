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
int a = 0; 
int dist_final[3]; // distance array 


void main()
{
  int a;

  TRISB = 0b00100000;           //RB4 as Input PIN (ECHO)
  TRISD = 0x00; // LCD Pins as Output


  T1CON = 0x10;                 //Initialize Timer Module

  while(1)
  {
    TMR1H = 0;                  //Sets the Initial Value of Timer
    TMR1L = 0;                  //Sets the Initial Value of Timer

    RB3 = 1;               //TRIGGER HIGH
    __delay_us(10);               //10uS Delay
    RB3 = 0;               //TRIGGER LOW

    while(!RB5);           //Waiting for Echo
    TMR1ON = 1;               //Timer Starts
    while(RB5);            //Waiting for Echo goes LOW
    TMR1ON = 0;               //Timer Stops

    a = (TMR1L | (TMR1H<<8));   //Reads Timer Value
    a = a/58.82;                //Converts Time to Distance
    a = a + 1;
    //Distance Calibration
    if(a>=2 && a<=400){         //Check whether the result is valid or not
			lcd_clear();
			printf(" Distance: %d", a);
			for (int i=0; i<3; i++){
				dist_final[i] = a%10 + 48; 
				a = a/10;
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
