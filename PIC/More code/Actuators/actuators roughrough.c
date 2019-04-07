/*
 * File:   main.c
 * Author: TrudieC
 *
 * Created on March 8, 2019, 12:39 AM
 */

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
 

#include <stdio.h>
#include <math.h>

#define MINTHR              8000
#define RESOLUTION          488

#define InternalOsc_8MHz    8000000
#define InternalOsc_4MHz    4000000
#define InternalOsc_2MHz    2000000
#define InternalOsc_1MHz    1000000
#define InternalOsc_500KHz  500000
#define InternalOsc_250KHz  250000
#define InternalOsc_125KHz  125000
#define InternalOsc_31KHz   31000

#define Timer2Prescale_1    1
#define Timer2Prescale_4    4
#define Timer2Prescale_16   16
 
//define constants
//#define _XTAL_FREQ 200000000 //Crystal Frequency, used in delay
#define speed 20 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps 120 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro

char stepper1;
char stepper2;

 
/*
 *Application related function and definition
 */
 
void system_init (void); // This function will initialise the ports.
void full_drive (char direction, char stepper_no); // This function will drive the motor in full drive mode
//void __delay_ms(unsigned int val);
void stepper(char stepper_no);
void servo(void);

//void delay(unsigned int val);

void main(){
    system_init();
    initLCD();
    
    stepper(1); 
    __delay_ms(2000);
    
    stepper(2); 
    __delay_ms(2000);
    
    lcd_clear();
    printf("Servo start");  
    __delay_ms(2000);
    
    servo(); 
    lcd_clear();
    printf("Servo done");    

//    delay(2000);
//    stepper(); 
} 


void system_init (void){
    TRISB = 0x00;     // PORT B as output port
    //PORTB = 0x0F;
    LATB = 0x0F;
    TRISA = 0x00;
    LATA = 0x0F;
    
    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x0C;        /* Set PWM mode */
    
    LATD = 0x00;
    TRISD = 0x00; //set data direction as output 
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
}
void PWM_End()            
{
    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x00;        /* Set PWM mode */
}

int setPeriodTo(unsigned long FPWM){
    int clockSelectBits, TimerPrescaleBits;
    int TimerPrescaleValue;
    float period;
    unsigned long FOSC, _resolution = RESOLUTION;

    if (FPWM < MINTHR)    {TimerPrescaleBits = 2; TimerPrescaleValue = Timer2Prescale_16;}
    else                  {TimerPrescaleBits = 0; TimerPrescaleValue = Timer2Prescale_1;}

    if (FPWM > _resolution)               {clockSelectBits = 7; FOSC = InternalOsc_8MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 6; FOSC = InternalOsc_4MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 5; FOSC = InternalOsc_2MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 4; FOSC = InternalOsc_1MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 3; FOSC = InternalOsc_500KHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 2; FOSC = InternalOsc_250KHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 1; FOSC = InternalOsc_125KHz;}
    else                                  {clockSelectBits = 0; FOSC = InternalOsc_31KHz;}

    period = ((float)FOSC / (4.0 * (float)TimerPrescaleValue * (float)FPWM)) - 1.0;
    period = round(period);

    OSCCON = ((clockSelectBits & 0x07) << 4) | 0x02;
    PR2 = (int)period;
    T2CON = TimerPrescaleBits;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;  /* Turn ON Timer2 */
    return (int)period;
}

void SetDutyCycleTo(float Duty_cycle, int Period)
{
    int PWM10BitValue;

    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR1L = (PWM10BitValue >> 2);
    CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}


//void delay(unsigned int val)
//{
//     unsigned int i,j;
//        for(i=0;i<val;i++)
//            for(j=0;j<10;j++);
//}

void rotateneg(){
    int Period;
    system_init();
        Period = setPeriodTo(50);   /* 50Hz PWM frequency */

    SetDutyCycleTo(3.0, Period);
    return;
//    while(1){
//        continue;}
}
void rotatepos(){
    int Period;
    system_init();
        Period = setPeriodTo(50);   /* 50Hz PWM frequency */

    SetDutyCycleTo(7.0, Period);
//    while(1){
//        continue;}
}

void servo(void) {
    int count_servo = 0;
    while (1){
        if (count_servo<1){
            rotateneg();
            __delay_ms(200);
            rotatepos();
            __delay_ms(200);
            rotateneg();  
            __delay_ms(200);
            count_servo++;
        }
        else{
            break;
        }
    }
    PWM_End();
    LATCbits.LATC2 = 0;
    return;
}

void stepper(int stepper_no){
    int count_stepper = 0;
    while (1){
        if (count_stepper<2){
            for(int i=0;i<steps;i++){
                full_drive(anti_clockwise, stepper_no);
                count_stepper++;
            }
        }
        else{
            break;
        }
    }
    //return;
}
 
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
    if (stepper_no == 2){ //RC0,2 and RE1,2
        LATCbits.LATC1 = 1;
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 0;
                __delay_ms(speed);
                LATCbits.LATC1 = 0;
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 0;
                __delay_ms(speed);
                LATCbits.LATC1 = 0;
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 1;
                __delay_ms(speed);
                LATCbits.LATC1 = 1;
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 1;                
                __delay_ms(speed);
                LATCbits.LATC1 = 1;
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 0;
                __delay_ms(speed); 
    }

}
  

