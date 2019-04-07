/*
 * File:   main.c
 * Author: TrudieC
 *
 * Created on February 24, 2019, 9:08 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
 
//define constants
//#define _XTAL_FREQ 200000000 //Crystal Frequency, used in delay
#define speed 5 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps 249 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro
 
int stepper_no; 


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
 
 
void main(void){
    system_init();
    //while(1){
    /* Drive the motor in full drive mode clockwise */
    int count = 0;
    LATDbits.LATD0 = 1;
    while (1){
        if (count<1){
            for(int i=0;i<steps;i++){
                full_drive(anti_clockwise, 1);
//                full_drive(anti_clockwise, 2);
                count++;
            }
//        }
        if ((count>1)&&(count<3)){
            for(int i=0;i<steps;i++){
                full_drive(anti_clockwise, 2);
                count++;
            }
        }
    }
    return;
    //__delay_ms(1000);

    /* Drive the motor in wave drive mode anti-clockwise */
    //for(int i=0;i<steps;i++)
    //{
    //            wave_drive(anti_clockwise);
    //            //full_drive(anti_clockwise);
    //}
    //__delay_ms(1000);
    //while(1){continue;}
    }
}
 
/*System Initialising function to set the pin direction Input or Output*/
 
void system_init (void){
    TRISB = 0x00;     // PORT B as output port
    //PORTB = 0x0F;
    LATB = 0x0F;
    TRISA = 0x00;
    LATA = 0x0F;
}

 
/*This will drive the motor in full drive mode depending on the direction*/
 
void full_drive (char direction, int stepper_no){
    if (stepper_no == 1){
        if (direction == anti_clockwise){
    //        LATB = 0b00000011;
    //        __delay_ms(speed);
    //        LATB = 0b00000110;
    //        __delay_ms(speed);
    //        LATB = 0b00001100;
    //        __delay_ms(speed);
    //        LATB = 0b00001001;
    //        __delay_ms(speed);
    //        LATB = 0b00000011;
    //        __delay_ms(speed);

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
            PORTB = 0b00001001;
            __delay_ms(speed);
            PORTB = 0b00001100;
            __delay_ms(speed);
            PORTB = 0b00000110;
            __delay_ms(speed);
            PORTB = 0b00000011;
            __delay_ms(speed);
            PORTB = 0b00001001;
            __delay_ms(speed);
        }
    }
    if (stepper_no == 2){ //RC1,5, 6, 7 
        if (direction == anti_clockwise){
                LATCbits.LATC0 = 1;
                LATCbits.LATC2 = 1;
                LATEbits.LATE1 = 0;
                LATEbits.LATE2 = 0;
                __delay_ms(speed);
                LATCbits.LATC0 = 0;
                LATCbits.LATC2 = 1;
                LATEbits.LATE1 = 1;
                LATEbits.LATE2 = 0;
                __delay_ms(speed);
                LATCbits.LATC0 = 0;
                LATCbits.LATC2 = 0;
                LATEbits.LATE1 = 1;
                LATEbits.LATE2 = 1;
                __delay_ms(speed);
                LATCbits.LATC0 = 1;
                LATCbits.LATC2 = 0;
                LATEbits.LATE1 = 0;
                LATEbits.LATE2 = 1;                
                __delay_ms(speed);
                LATCbits.LATC0 = 1;
                LATCbits.LATC2 = 1;
                LATEbits.LATE1 = 0;
                LATEbits.LATE2 = 0;
                __delay_ms(speed);
//            LATCbits.LATC1 = 1;
//            LATCbits.LATC5 = 1;
//            LATCbits.LATC6 = 0;
//            LATCbits.LATC7 = 0;
//                __delay_ms(speed);
//            LATCbits.LATC1 = 0;
//            LATCbits.LATC5 = 1;
//            LATCbits.LATC6 = 1;
//            LATCbits.LATC7 = 0;
//                __delay_ms(speed);
//            LATCbits.LATC1 = 0;
//            LATCbits.LATC5 = 0;
//            LATCbits.LATC6 = 1;
//            LATCbits.LATC7 = 1;
//                __delay_ms(speed);
//            LATCbits.LATC1 = 1;
//            LATCbits.LATC5 = 0;
//            LATCbits.LATC6 = 0;
//            LATCbits.LATC7 = 1;                
//                __delay_ms(speed);
//            LATCbits.LATC1 = 1;
//            LATCbits.LATC5 = 1;
//            LATCbits.LATC6 = 0;
//            LATCbits.LATC7 = 0;
//                __delay_ms(speed);
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