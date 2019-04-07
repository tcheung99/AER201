
//#ifndef XC_HEADER_TEMPLATE_H
//#define	XC_HEADER_TEMPLATE_H

#ifndef TIMER_H
#define TIMER_H

#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
//BEGIN CONFIG
//#pragma config OSC = INTIO67
#pragma config WDT = OFF
//END CONFIG
//#define _XTAL_FREQ 1000000ul        //Set clock FRQ to 1 MHz
#define LEDPin LATDbits.LATD7       //Define LEDPin as Port D PIN 0
#define LEDTRIS TRISDbits.TRISD7    //Define LEDTris as TRISD Pin 0
#define OSC_CLOCKS_PER_INSTRUCTION_CYCLE (4ul)
#define TIMER0_PRESCALE_COUNT (64ul)
#define TIMER0_COUNTS_PER_HALF_SECOND (65536ul - (((_XTAL_FREQ / OSC_CLOCKS_PER_INSTRUCTION_CYCLE)/TIMER0_PRESCALE_COUNT)/2ul))

void TIMER_INIT( void );
void timer_main( void );

#endif