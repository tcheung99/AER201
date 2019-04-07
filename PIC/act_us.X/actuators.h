#ifndef ACTUATORS_H
#define	ACTUATORS_H

//#include <xc.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include "configBits.h"
//#include "lcd.h"
//#include "RTC.h"
//#include "I2C.h"
//
//#include <stdio.h>
//#include <math.h>
//
//#define MINTHR              8000
//#define RESOLUTION          488
//
//#define InternalOsc_8MHz    8000000
//#define InternalOsc_4MHz    4000000
//#define InternalOsc_2MHz    2000000
//#define InternalOsc_1MHz    1000000
//#define InternalOsc_500KHz  500000
//#define InternalOsc_250KHz  250000
//#define InternalOsc_125KHz  125000
//#define InternalOsc_31KHz   31000
//
//#define Timer2Prescale_1    1
//#define Timer2Prescale_4    4
//#define Timer2Prescale_16   16
// 
////define constants
////#define _XTAL_FREQ 200000000 //Crystal Frequency, used in delay
//
#define speed1 9 // Speed Range 10 to 1  10 = lowest , 1 = highest 
#define speed2 4 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps1 14// how much step it will take (249 ~= 1 full rotation)
#define steps2 105 // how much step it will take (249 ~= 1 full rotation)
//#define steps2 110 // how much step it will take (249 ~= 1 full rotation)
//#define steps2 60 // how much step it will take (249 ~= 1 full rotation)
#define steps3 156 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro 
//#define thrdist 25


//int act_cnt = 0;
//
////#define speed 4 // Speed Range 10 to 1  10 = lowest , 1 = highesdirection macro
// 
//int act_cnt = 0;
// 
//void system_init (void); // This function wilt 
////1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
////#define steps1 60// how much step it will take (249 ~= 1 full rotation)
////#define steps2 100 // how much step it will take (249 ~= 1 full rotation)
//#define clockwise 0 // clockwise direction macro
//#define anti_clockwise 1 // anti clockwise l initialise the ports.
//void full_drive (char direction,int stepper_no); // This function will drive the motor in full drive mode
//void stepper(int stack);
//void stepper2(void);
//void servo(void);
//void PWM_End();
//#endif	/* XC_HEADER_TEMPLATE_H */

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"

#include <stdio.h>
#include <math.h>
 
//define constants
//#define _XTAL_FREQ 200000000 //Crystal Frequency, used in delay
#define speed1 10 // Speed Range 10 to 1  10 = lowest , 1 = highest 
#define speed2 6 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps1 14// how much step it will take (249 ~= 1 full rotation)
#define steps2 110 // how much step it will take (249 ~= 1 full rotation)
#define steps3 156 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro
 
#define steps_back 5

//int act_cnt = 0;
//int oscold ;  

void system_init (void); // This function will initialise the ports.
void full_drive (char direction,int stepper_no); // This function will drive the motor in full drive mode
void stepper(int stack);
void stepper2(char direction, int steps2_adj);
void servo(void);
void actuators_main (int stack, int step2_offset);
void stepper2_back(char direction, int steps2_adj);
#endif