/*
 * File:   main.c
 * Author: TrudieC
 *
 * Created on February 24, 2019, 9:08 PM
 */

// 3/10 8:13am -- LCD print statements aren't doing anything 
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
#include "timer.h"
//#include "timer.c"

 
//define constants
//#define _XTAL_FREQ 200000000 //Crystal Frequency, used in delay
#define speed 6 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps 249 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro
 
//~7 steps for 1 cm 

int stepper_no; 

int timer_count = 0;
int cnt =0;

//Old code 

/*
 *Application related function and definition
 */
 
void system_init (void); // This function will initialise the ports.
void full_drive (char direction, int stepper_no); // This function will drive the motor in full drive mode
void half_drive (char direction); // This function will drive the motor in full drive mode
void wave_drive (char direction); // This function will drive the motor in full drive mode
//void __delay_ms(unsigned int val);
 
/*
 * main function starts here
 */
int count = 0;

/*from the front, right = anti_clockwise and left = clockwise */

void main(void){
    system_init();
    initLCD();
//    lcd_clear();
//    printf("starting");
//    TIMER_INIT();
//    timer_main();
    
    //while(1){
    /* Drive the motor in full drive mode clockwise */
    while (1){
//        lcd_set_ddram_addr(LCD_LINE4_ADDR);
//        printf("count is: %d", cnt);
        if (count<1){
            lcd_clear();
            printf("step1");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            for(int i=0;i<(7);i++){ // is about half the ramp 
                full_drive(clockwise, 2); //ramp stepper , fwd  //110 steps from right stepper to start of door, speed 6; 156 from left stepper (aligning right side to right side)  
//                full_drive(clockwise, 1); //right stepper forwards A's
//                full_drive(anti_clockwise, 3); //left stepper forwards, 14 steps is good, speed 10
            }
//                        for(int i=0;i<(steps+50)/2;i++){
//                full_drive(anti_clockwise, 1); //ramp stepper , fwd 
////                full_drive(anti_clockwise, 2);
//            }
//            printf("cnt is %d", count);
            count++;
                __delay_ms(3000);

        }
//        if ((count>=1)&&(count<3)){
//            lcd_clear();
//            printf("step2");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            for(int i=0;i<steps;i++){
//                full_drive(anti_clockwise, 1);
//            }
//            printf("cnt is %d", count);
//            count++;
//        }
        else{
            lcd_clear();
            printf("steps done");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("cnt is %d", count);
            break;
        }
    }
    return;
    __delay_ms(3000);

    /* Drive the motor in wave drive mode anti-clockwise */
    //for(int i=0;i<steps;i++)
    //{
    //            wave_drive(anti_clockwise);
    //            //full_drive(anti_clockwise);
    //}
    //__delay_ms(1000);
    //while(1){continue;}
//    }
}
 
/*System Initialising function to set the pin direction Input or Output*/
 
void system_init (void){
    
    TRISB = 0x00;     // PORT B as output port
    //PORTB = 0x0F;
    LATB = 0x00;
    TRISA = 0x00;
    LATA = 0x00;
    TRISC = 0x00;
    LATC = 0x00;
    TRISE = 0x00;
    LATE = 0x00;

}

 
/*This will drive the motor in full drive mode depending on the direction*/
 
void full_drive (char direction, int stepper_no){
    if (stepper_no == 1){
        if (direction == anti_clockwise){
            LATA = 0b00000011;
            __delay_ms(speed); 
            LATA = 0b00000110;
            __delay_ms(speed);
            LATA = 0b00001100;
            __delay_ms(speed);
            LATA = 0b00001001;
            __delay_ms(speed);
            LATA = 0b00000011;
            __delay_ms(speed);        
        }
        if (direction == clockwise){
            LATA = 0b00001001;
            __delay_ms(speed);
            LATA = 0b00001100;
            __delay_ms(speed);
            LATA = 0b00000110;
            __delay_ms(speed);
            LATA = 0b00000011;
            __delay_ms(speed);
            LATA = 0b00001001;
            __delay_ms(speed);
        }
    }
    if (stepper_no == 2){ //RC0,RE0,1,2, 
        if (direction == anti_clockwise){
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 0;
            __delay_ms(speed);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 0;
            __delay_ms(speed);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 1;
            __delay_ms(speed);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 1;                
            __delay_ms(speed);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 0;
            __delay_ms(speed);
        }
        if (direction == clockwise){
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 1;                
            __delay_ms(speed);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 1;
            __delay_ms(speed);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 0;
            __delay_ms(speed);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 0;
            __delay_ms(speed);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 1;                
            __delay_ms(speed);
        }
    }
    if (stepper_no == 3){
        printf("what");
        if (direction == clockwise){ //RC1,5,6,7
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 1;                
            __delay_ms(speed);
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 1;
            LATAbits.LATA5 = 1;
            __delay_ms(speed);
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 1;
            LATAbits.LATA5 = 0;
            __delay_ms(speed);
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 0;
            __delay_ms(speed);  
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 1;              
            __delay_ms(speed);
        }
        if (direction == anti_clockwise){
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 0;
                LATAbits.LATA5 = 0;
                __delay_ms(speed);  
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 1;
                LATAbits.LATA5 = 0;
                __delay_ms(speed);
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 1;
                LATAbits.LATA5 = 1;
                __delay_ms(speed);
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 0;
                LATAbits.LATA5 = 1;              
                __delay_ms(speed);
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 0;
                LATAbits.LATA5 = 0;
                __delay_ms(speed); 
        }
    }
}



//old code 


/* This method will drive the motor in half-drive mode using direction input */
/*
void half_drive (char direction){
    if (direction == anti_clockwise){
        PORTB = 0b00000001;
        __delay_ms(speed);
        PORTB = 0b00000011;
        __delay_ms(speed);
        PORTB = 0b00000010;
        __delay_ms(speed);
        PORTB = 0b00000110;
        __delay_ms(speed);
        PORTB = 0b00000100;
        __delay_ms(speed);
        PORTB = 0b00001100;
        __delay_ms(speed);
        PORTB = 0b00001000;
        __delay_ms(speed);
        PORTB = 0b00001001;
        __delay_ms(speed);
    }
    if (direction == clockwise){
       PORTB = 0b00001001;
       __delay_ms(speed);
       PORTB = 0b00001000;
       __delay_ms(speed);
       PORTB = 0b00001100;
       __delay_ms(speed); 
       PORTB = 0b00000100;
       __delay_ms(speed);
       PORTB = 0b00000110;
       __delay_ms(speed);
       PORTB = 0b00000010;
       __delay_ms(speed);
       PORTB = 0b00000011;
       __delay_ms(speed);
       PORTB = 0b00000001;
       __delay_ms(speed);
    }
}
 
/* This function will drive the the motor in wave drive mode with direction input*/
/* 
void wave_drive (char direction){
    if (direction == anti_clockwise){
        PORTB = 0b00000001;
        __delay_ms(speed);
        PORTB = 0b00000010;
        __delay_ms(speed);
        PORTB = 0b00000100;
        __delay_ms(speed);
        PORTB = 0b00001000;
        __delay_ms(speed);
    }
     if (direction == clockwise){
        PORTB = 0b00001000;
        __delay_ms(speed);
        PORTB = 0b00000100;
        __delay_ms(speed);
        PORTB = 0b00000010;
        __delay_ms(speed);
        PORTB = 0b00000001;
        __delay_ms(speed);
    }
    
} 
*/
/*This method will create required delay*/
 
//void ms_delay(unsigned int val)
//{
//     unsigned int i,j;
//        for(i=0;i<val;i++)
//            for(j=0;j<1650;j++);
//}

//void TIMER_INIT( void ){
//    TRISD = 0x00;                //Define output or Input
//    LATD = 0x00;
//    T0CONbits.TMR0ON  = 0;      //stop TIMER0 counter
//    T0CONbits.T08BIT  = 0;      //select 16-bit mode
//    T0CONbits.T0CS    = 0;      //select the internal clock to drive timer0
//    T0CONbits.PSA     = 0;      //use prescaler for TIMER0
//    T0CONbits.T0PS    = 0b101;  //assign the 1:64 pre-scaler to TIMER0 
//    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
//    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
//    T0CONbits.TMR0ON  = 1;      //enable TIMER0
//    INTCONbits.TMR0IE = 1;      //Enable the TIMER0 overflow interrupt
//    //INTERUPT ENABLE
//    INTCONbits.GIE    = 1;      //GLOBAL INTERUPT ENABLE
//    INTCONbits.PEIE   = 1;      //PERIPHERAL INTERUPT ENABLE; TIMER0 = PERIPHERAL
//}
//
//void __interrupt() SW_ISR( void ){    //WEIRD SYNTAX BECAUSE OF PIC
//    INTCONbits.TMR0IF = 0;      //CLEAR FLAG SO IT CAN BE TRIGGERED AGAIN
//    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
//    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
//    cnt++;
//    LEDPin ^= 1u;           //Toggle LED
//    lcd_clear();
//    printf("time");
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    if (cnt%2 == 0){
//        printf("%d",cnt/2);
//    }
//
//}