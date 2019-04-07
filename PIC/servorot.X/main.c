#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
 

#include <stdio.h>
#include <math.h>

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

void PWM_Init()            /* Initialize PWM */
{
    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x0C;        /* Set PWM mode */
}

void PWM_End()            
{
    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x00;        /* Set PWM mode */
}

/*
int setPeriodTo(unsigned long FPWM) // Set period 
{
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
    T2CONbits.TMR2ON = 1;  // Turn ON Timer2 
    return (int)period;
}

void SetDutyCycleTo(float Duty_cycle, int Period)
{
    int PWM10BitValue;

    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR1L = (PWM10BitValue >> 2);
    CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}

*/

//void delay(unsigned int val)
//{
//     unsigned int i,j;
//        for(i=0;i<val;i++)
//            for(j=0;j<10;j++);
//}


void rotateneg(){
//    int Period;
//    system_init();
//    Period = setPeriodTo(50);   /* 50Hz PWM frequency */
//    SetDutyCycleTo(3.0, Period,1);
//    SetDutyCycleTo(8.0, Period,2);
        CCP2CON = 0b00101100 ;

    PR2 = 0b10011011 ;
    T2CON = 0b00000111 ;
    //3% duty cycle for RC1
    CCPR2L = 0b00000100 ;
    __delay_ms(1000);
//
//    //7% duty cycle for RC2
//    CCPR1L = 0b00001010 ; 
//    CCP1CON = 0b00111100 ;
//    //    return;
}
void rotatepos(){
//    int Period;
//    system_init();
//    Period = setPeriodTo(50);   /* 50Hz PWM frequency */
//
//    SetDutyCycleTo(8.0, Period,1); //neutral
//    SetDutyCycleTo(3.0, Period,2); //neutral
        CCP2CON = 0b00111100 ;

    PR2 = 0b10011011 ;
    T2CON = 0b00000111 ;
//
//    //3% duty cycle for RC2
//    CCPR1L = 0b00000100 ;
//    CCP1CON = 0b00101100 ;
//
    //7% duty cycle for RC1
    CCPR2L = 0b00001010 ; 
        __delay_ms(1000);

}

void main() 
{
//    int Period;
//    PWM_Init();                 /* Initialize PWM */
//    Period = setPeriodTo(50);   /* 50Hz PWM frequency */
    /* Note that period step size will gradually increase with PWM frequency */
//    while(1)
//    {
//    for (int i=0;i<2;i++){
//        Period = setPeriodTo(50);
//        SetDutyCycleTo(3.0, Period);    /* 3% duty cycle */
//        delay(3000);
////        SetDutyCycleTo(7.0, Period);    /* 7% duty cycle */
////        delay(1000);
//        SetDutyCycleTo(12.0, Period);    /* 12% duty cycle */
//        delay(3000);
//        
        
        //PWM_End();
//        LATCbits.LATC2 = 0;
   
    //the below works, which means that it originally wasn't looping through the main?? 
    int count = 0;
    while (1){
//        if (count<2){
            rotatepos();
            __delay_ms(200);
            rotateneg();
            __delay_ms(200);
            rotatepos();  
//            count++;
//        }
    }
    
    return;
//    while(1){
//        continue;}
    
//    }
}

