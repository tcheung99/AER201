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
//#define TIMER0_COUNTS_PER_HALF_SECOND (65536ul - (((_XTAL_FREQ / OSC_CLOCKS_PER_INSTRUCTION_CYCLE)/TIMER0_PRESCALE_COUNT)/2ul))
#define TIMER0_COUNTS_PER_HALF_SECOND (65536ul - 1)

int count = 0;
long cnt =0;
long main_cnt =0;

//END DEFINE
//REVIEW DBL CHECK
//void CLK_INIT( void ){
//    OSCCONbits.IRCF = 0b100; //1 MHz
//}
void TIMER_INIT( void ){
    T0CONbits.TMR0ON  = 0;      //stop TIMER0 counter
    T0CONbits.T08BIT  = 0;      //select 16-bit mode
    T0CONbits.T0CS    = 0;      //select the internal clock to drive timer0
    T0CONbits.PSA     = 0;      //use prescaler for TIMER0
    T0CONbits.T0PS    = 0b101;  //assign the 1:64 pre-scaler to TIMER0 
    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
    T0CONbits.TMR0ON  = 1;      //enable TIMER0
    INTCONbits.TMR0IE = 1;      //Enable the TIMER0 overflow interrupt
    //INTERUPT ENABLE
    INTCONbits.GIE    = 1;      //GLOBAL INTERUPT ENABLE
    INTCONbits.PEIE   = 1;      //PERIPHERAL INTERUPT ENABLE; TIMER0 = PERIPHERAL
}

void main( void ){
    TRISD = 0x00;                //Define output or Input
    LATD = 0x00;
    initLCD(); 

//    CLK_INIT();
    TIMER_INIT();
    count++;
    while (1){
        if (main_cnt%1000 ==0){ ///1 ms 
            printf("1 ms");
        }
    }
}
//INTERRUPT ISR
void __interrupt() SW_ISR( void ){    //WEIRD SYNTAX BECAUSE OF PIC
    INTCONbits.TMR0IF = 0;      //CLEAR FLAG SO IT CAN BE TRIGGERED AGAIN
    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
    
//    LEDPin ^= 1u;           //Toggle LED
    cnt++;
//    lcd_clear();
//    printf("time");
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    if (cnt%4 == 0){
        cnt = 0;
        main_cnt++;
//        printf("%d",cnt/2);
    }
}