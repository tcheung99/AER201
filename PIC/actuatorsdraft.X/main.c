/* File:   main.c
 * Author: TrudieC
 *
 * Created on March 8, 2019, 12:39 AM
 */

/*March 10 12:10am keeps looping on main, but on the second time the stepper gets funky and the signals to the port are very slow, st the stepper itself isnt even moving 
 also the servos are twitching 
 March 10 8:14am -- right servo won't even move anymore?? the __delay_ms function seems to be delaying the program more than it should. running the servos seems to cause the steppers to stop working properly. 
 */ 
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
#include "timer.h"

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
#define speed1 10 // Speed Range 10 to 1  10 = lowest , 1 = highest 
#define speed2 4 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps1 14// how much step it will take (249 ~= 1 full rotation)
//#define steps2_adj 110 // how much step it will take (249 ~= 1 full rotation)
#define steps2 110 // how much step it will take (249 ~= 1 full rotation)
#define steps3 156 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro
#define steps_back 5
 
int act_cnt = 0;
//int oscold ;  

int steps2_adj = 0;

int timer_count = 0;
int cnt =0;

void system_init (void); // This function will initialise the ports.
void full_drive (char direction,int stepper_no); // This function will drive the motor in full drive mode
void stepper(void);
void stepper2(char direction, int steps2_adj);
void stepper2_back(char direction);
void servo(void);
void servoRotate0(int servo); //0 Degree
void servoRotate180(int servo); //0 Degree

//void PWM_End();

//void __delay_ms(unsigned int val);
//void delay(unsigned int val);

//INTERRUPT ISR
//void __interrupt() SW_ISR( void ){    //WEIRD SYNTAX BECAUSE OF PIC
//    INTCONbits.TMR0IF = 0;      //CLEAR FLAG SO IT CAN BE TRIGGERED AGAIN
//    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
//    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
//    cnt++;
//    LEDPin ^= 1u;           //Toggle LED
//    servoRotate0(1); 
////    lcd_clear();
////    printf("time");
////    lcd_set_ddram_addr(LCD_LINE2_ADDR);
////    if (cnt%2 == 0){
////        printf("%d",cnt/2);
////    }
//
//}

void main(){
    system_init();
    initLCD();
//    TIMER_INIT();
//    timer_main();
    
    lcd_clear();
    printf("cnt is %d",act_cnt); 
    __delay_ms(200);

    if (act_cnt==0){
        lcd_clear();
        printf("Stepper start");  
        
        stepper(); 
        steps2_adj = steps2 + 2; 
        stepper2(clockwise, steps2_adj);
        stepper2_back(anti_clockwise);
    //    __delay_ms(2000);
        lcd_clear();
        printf("Stepper done");    
        
        servo(); 
        //Servos return to resting position (not holding)
        lcd_clear();
        printf("Servo done"); 
        
//        system_init();
//        PWM_End(); 
        __delay_ms(200);
        
        stepper2(anti_clockwise, steps2_adj);
        act_cnt++; 
    }
//    else{
//        while(act_cnt); //this will prevent the main to loop over itself, because the counter doesnt do anything .-.
//    }
//    delay(2000);
//    stepper(); 
} 

void system_init (void){
    TRISB = 0x00;     // PORT B as output port
    //PORTB = 0x0F;
    LATB = 0x0F;
    TRISA = 0x00;
    LATA = 0x00;


    LATD = 0x00;
    TRISD = 0x00; //set data direction as output 
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    TRISC = 0x00;
    LATC = 0x00;
    TRISE = 0x00;
    LATE = 0x00;
    
}
//void PWM_End(){
////    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
//    CCP1CON = 0x00;        /* Set PWM mode */
////    TRISCbits.TRISC1 = 0;  /* Set CCP1 pin as output for PWM out */
//    CCP2CON = 0x00;
//    OSCCON = 01111000; //OSCCON 0b00110010
//}
//
//
//int setPeriodTo(unsigned long FPWM){
//
//    int clockSelectBits=3; 
//    int TimerPrescaleBits=2;
//    int TimerPrescaleValue=Timer2Prescale_16;
//    float period;
//    unsigned long FOSC;
////    , _resolution = RESOLUTION;
//
////    lcd_clear();
////    printf("FOSC1,%ld", FOSC);
////    lcd_set_ddram_addr(LCD_LINE2_ADDR);
////    printf("freq1",_XTAL_FREQ);
////    __delay_ms(1000);
//    
////    if (FPWM < MINTHR)    {TimerPrescaleBits = 2; TimerPrescaleValue = Timer2Prescale_16;}
////    else                  {TimerPrescaleBits = 0; TimerPrescaleValue = Timer2Prescale_1;}
////
////    if (FPWM > _resolution)               {clockSelectBits = 7; FOSC = InternalOsc_8MHz;}
////    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 6; FOSC = InternalOsc_4MHz;}
////    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 5; FOSC = InternalOsc_2MHz;}
////    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 4; FOSC = InternalOsc_1MHz;}
////    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 3; FOSC = InternalOsc_500KHz;}
////    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 2; FOSC = InternalOsc_250KHz;}
////    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 1; FOSC = InternalOsc_125KHz;}
////    else                                  {clockSelectBits = 0; FOSC = InternalOsc_31KHz;}
//
//
//
////trying to modify servos     
//        
//    FOSC = 500000; //500kHz 
//    period = ((float)FOSC / (4.0 * (float)TimerPrescaleValue * (float)FPWM)) - 1.0;
//    period = round(period);
//    OSCCON = ((clockSelectBits & 0x07) << 4) | 0x02; //OSCCON 0b00110010
//    PR2 = (int)period; //TIMER2 Period Register 
//    T2CON = TimerPrescaleBits; 
//    TMR2 = 0;
//    T2CONbits.TMR2ON = 1;  // Turn ON Timer2 
//    
//    
////    lcd_clear();
////    printf("oscconold,%d", oscold);
////    lcd_set_ddram_addr(LCD_LINE2_ADDR);
////    printf("osccon",OSCCON);
////    __delay_ms(100);
//    
////    lcd_clear();
////    printf("FOSCf,%ld", FOSC);
////    lcd_set_ddram_addr(LCD_LINE2_ADDR);
////    printf("freqf",_XTAL_FREQ);
////    __delay_ms(100);
//
//    return (int)period;
//
//}
//
//void SetDutyCycleTo(float Duty_cycle, int Period, int serv){
//    int PWM10BitValue;
//
//    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
//    if (serv==1){
//        CCPR1L = (PWM10BitValue >> 2);
//        CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;    
//    }
//    if (serv==2){
//        CCPR2L = (PWM10BitValue >> 2);
//        CCP2CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
//    }    
////    LATAbits.LATA0 = PORTCbits.RC2;
//    
//}

//void rotateneg(){
//    int Period;
//    system_init();
//    Period = setPeriodTo(50);   /* 50Hz PWM frequency */
//
//    SetDutyCycleTo(3.0, Period,1);
//    SetDutyCycleTo(12.0, Period,2);
//        PR2 = 0b11111111; 
//    T2CON = 0b00000111	;
//    TMR2 = 0;
//    T2CONbits.TMR2ON = 1;
//    PR2 = 0b10011011 ;
//    T2CON = 0b00000111 ;
//    //3% duty cycle for RC1
//    CCPR2L = 0b00000111 ;
//    CCP2CON = 0b00101100 ;

//    //7% duty cycle for RC2
//    CCPR1L = 0b00011110 ; 
//    CCP1CON = 0b00101100 ;
//    //    return;
//}
//void rotatepos(){
//    int Period;
//    system_init();
//    Period = setPeriodTo(50);   /* 50Hz PWM frequency */
//
//    SetDutyCycleTo(12.0, Period,1); //neutral
//    SetDutyCycleTo(3.0, Period,2); //neutral
//    
////            PR2 = 0b11111111; 
////    T2CON = 0b00000111	;
////    TMR2 = 0;
////    T2CONbits.TMR2ON = 1;
////    PR2 = 0b10011011 ;
////    T2CON = 0b00000111 ;
////
////    //3% duty cycle for RC2
////    CCPR2L = 0b00000111 ;
////    CCP2CON = 0b00101100 ;
////
////    //7% duty cycle for RC1
////    CCPR2L = 0b00011110 ; 
////    CCP2CON = 0b00101100 ;
//}
//
//void servo() {
//    TRISCbits.TRISC1 = 0;  /* Set CCP2 pin as output for PWM out */
//    CCP2CON = 0x0C;        /* Set PWM mode */
//    
//    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
//    CCP1CON = 0x0C;        /* Set PWM mode */
//    
//    int count = 0;
////    while (1){
//        if (count<1){
////            rotatepos();
////            __delay_ms(100);
//            rotateneg();
//            __delay_ms(100);
//            rotatepos();
//            __delay_ms(100);
//            rotateneg();             
//            count++;
//        }
//    PWM_End(); 
////    }
//    return;
//}
//

//
//void servo(void) {
//    int count_servo = 0;
//    while (1){
//        if (count_servo<1){
//            rotateneg();
//            __delay_ms(200);
//            rotatepos();
//            __delay_ms(200);
//            rotateneg();  
//            __delay_ms(200);
//        }count_servo++;
//        else{
//            break;
//        }
//    }
//        lcd_clear();
//    printf("Servo loop");  
////    __delay_ms(2000);
//    PWM_End();
//    LATCbits.LATC2 = 0;
//    return;
//}

//void servoRotate90() //90 Degree
//{
//  unsigned int i;
//  for(i=0;i<50;i++)
//  {
//    RC1 = 1;
////    RC2 = 1;
//    __delay_us(1500);
//    RC1 = 0;
////    RC2 = 0;    
//    __delay_us(18500);
//  }
//}

void servoRotate0(int servo){ //0 Degree
    unsigned int i;
    if (servo==1){
      for(i=0;i<40;i++)
          {
            RC1 = 1;
            RC2 = 1;
            __delay_us(1100);
            RC2 = 0;
            __delay_us(300);
            RC1 = 0;
//            __delay_us(1150);
//            __delay_us(300);
              
            __delay_us(18600);
          }
    }
    if (servo==2){
      for(i=0;i<40;i++)
        {
//        RB7 = 1;
        RD1 = 1;
//        RD0 = 1;
          __delay_us(1350);

//          __delay_us(1000);
//        RB7 = 0;
        RD1 = 0;
//        RD0 = 0;
          __delay_us(16850);

//          __delay_us(19000);
        }
    }
    if (servo==3){
      for(i=0;i<40;i++)
        {
        RD0 = 1;
        __delay_us(2000);
        //    RB7 = 0;
        //    RD1 = 0;
        RD0 = 0;
        __delay_us(11000);
        }
    }
}
void servoRotate180(int servo) //180 Degree
{
  unsigned int i;
    if (servo==1){
    for(i=0;i<40;i++)
        {
          RC1 = 1;
            RC2 = 1;
          __delay_us(450); //goes down to go lower old
//          __delay_us(420); //goes down to go lower 
          RC2 =0;
          __delay_us(2100); //old 
//          __delay_us(2130); //old
          RC1 = 0;
          __delay_us(13550);
        }
  }
  if (servo==2){
    for(i=0;i<40;i++)
          {
//    RB7 = 1; 
    RD1 = 1;
//    RD0 = 1;

    __delay_us(950); //left 
//    RB7 = 0;
    RD1 = 0;
//        RD0 = 0;

    __delay_us(26650); //left
          }
  }
    if (servo==3){
      for(i=0;i<40;i++)
        {
        RD0 = 1;

        __delay_us(2500); //left 
    //    RB7 = 0;
    //    RD1 = 0;
            RD0 = 0;

        __delay_us(10500); //left
        }
    }
}
void servo()
{
  TRISC = 0; // PORTB as Ouput Port
  {
//    servoRotate0(1); //0 Degree
    servoRotate0(1); //0 Degree
//    __delay_ms(2000);
//    servoRotate90(); //90 Degree
//    servoRotate180(1); //180 Degree
    servoRotate180(1); //180 Degree
    __delay_ms(450);        
    servoRotate0(1); //0 Degree
    
    servoRotate180(2); //180 Degree
    servoRotate0(2); //0 Degree
    servoRotate180(2); //180 Degree
    
    __delay_ms(50);
    servoRotate180(3); //180 Degree
    servoRotate0(3); //0 Degree
    servoRotate180(3); //180 Degree

  }
}

void stepper(void){
    int count_stepper = 0;
    
        while (1){
        if (count_stepper<1){
            lcd_clear();
            printf("step1");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("Step speed %d",speed1); 
            
            for(int i=0;i<steps1+7;i++){
//                full_drive(anti_clockwise, 2);
                full_drive(clockwise, 1);
//                full_drive(anti_clockwise, 2);
            }
            printf("cnt is %d", count_stepper);
            count_stepper++;
        }
//        if ((count_stepper>=1)&&(count_stepper<3)){
//            lcd_clear();
//            printf("step2");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            for(int i=0;i<0;i++){
//                full_drive(clockwise, 2);
//            }
//            printf("cnt is %d", count_stepper);
//            count_stepper++;
//        }
        else{
            lcd_clear();
            printf("steps done");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("cnt is %d", count_stepper);
            break;
        }
    }
}

void stepper2(char direction, int steps2_adj){
    int count_stepper = 0;
        while (1){
        if (count_stepper<1){
            lcd_clear();
            printf("step1");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("Step speed %d",speed2); 
            if (direction == clockwise){
                for(int i=0;i<steps2_adj;i++){
//            for(int i=0;i<steps2-(steps_back);i++){
                full_drive(direction, 2);
//                full_drive(anti_clockwise, 2);
            }
            printf("cnt is %d", count_stepper);
            count_stepper++;
            }
            if (direction == anti_clockwise){
                for(int i=0;i<steps2_adj-(steps_back);i++){
    //            for(int i=0;i<steps2-(steps_back);i++){
                    full_drive(direction, 2);
    //                full_drive(anti_clockwise, 2);
                }
                printf("cnt is %d", count_stepper);
                count_stepper++;
            }
        }
        else{
            break;
        }
        
    }
}
//    while (1){
//        if (count_stepper<2){
//            for(int i=0;i<steps;i++){
//                full_drive(anti_clockwise);
//                count_stepper++;
//            }
//        }
//        else{
//            break;
//        }
//    }
    //return;
//}

void stepper2_back(char direction){
    /*
    int adjust[4]={0,0,0,0};
    int base_val = 12; 
    if ((dist_final[0][5]>=2) && (dist_final[0][5] <= thrdist)){
        adjust[0]= dist_final[0][5] - base_val;
    }  
    if ((dist_final[1][5]>=2) && (dist_final[1][5] <= thrdist)){
        adjust[1]= dist_final[1][5] - base_val;
    }
    if ((dist_final[2][5]>=2) && (dist_final[2][5] <= thrdist)){
        adjust[2]= dist_final[2][5] - base_val;
    }        
    if ((dist_final[3][5]>=2) && (dist_final[3][5] <= thrdist)){
        adjust[3]= dist_final[2][5] - base_val;
    }    
    int adjfinal = adjust[1];
    */
    int count_stepper = 0;
//        while (1){
        if (count_stepper<1){
            lcd_clear();
            printf("step1");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("Step speed %d",speed2); 
            for(int i=0;i<steps_back;i++){
//            for(int i=0;i<110+adjfinal;i++){
                full_drive(direction, 2);
//                full_drive(anti_clockwise, 2);
            }
            printf("cnt is %d", count_stepper);
            count_stepper++;
        }
        
//    }
}
//    while (1){
//        if (count_stepper<2){
//            for(int i=0;i<steps;i++){
//                full_drive(anti_clockwise);
//                count_stepper++;
//            }
//        }
//        else{
//            break;
//        }
//    }
    //return;
//}
void full_drive (char direction, int stepper_no){
    if (stepper_no == 1){ 
        if (direction == anti_clockwise){
            LATA = 0b00000011;
            __delay_ms(speed1); 
            LATA = 0b00000110;
            __delay_ms(speed1);
            LATA = 0b00001100;
            __delay_ms(speed1);
            LATA = 0b00001001;
            __delay_ms(speed1);
            LATA = 0b00000011;
            __delay_ms(speed1);        
        }
        if (direction == clockwise){
            LATA = 0b00001001;
            __delay_ms(speed1);
            LATA = 0b00001100;
            __delay_ms(speed1);
            LATA = 0b00000110;
            __delay_ms(speed1);
            LATA = 0b00000011;
            __delay_ms(speed1);
            LATA = 0b00001001;
            __delay_ms(speed1);
        }
    }
    if (stepper_no == 2){ //RC0,RE0,1,2
        if (direction == anti_clockwise){
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 0;
            __delay_ms(speed2);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 0;
            __delay_ms(speed2);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 1;
            __delay_ms(speed2);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 1;                
            __delay_ms(speed2);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 0;
            __delay_ms(speed2);
        }
        if (direction == clockwise){
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 1;                
            __delay_ms(speed2);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 1;
            __delay_ms(speed2);
            LATCbits.LATC0 = 0;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 1;
            LATEbits.LATE2 = 0;
            __delay_ms(speed2);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 0;
            __delay_ms(speed2);
            LATCbits.LATC0 = 1;
            LATEbits.LATE0 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE2 = 1;                
            __delay_ms(speed2);
        }      
    }
    if (stepper_no == 3){
        if (direction == clockwise){ //RC1,5,6,7
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 1;                
            __delay_ms(speed1);
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 0;
            __delay_ms(speed1);  
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 1;
            LATAbits.LATA5 = 0;
            __delay_ms(speed1);
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 1;
            LATAbits.LATA5 = 1;
            __delay_ms(speed1);
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 1;              
            __delay_ms(speed1);
        }
        if (direction == anti_clockwise){
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 0;
            __delay_ms(speed1);  
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 1;
            LATAbits.LATA5 = 0;
            __delay_ms(speed1);
            LATCbits.LATC5 = 0;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 1;
            LATAbits.LATA5 = 1;
            __delay_ms(speed1);
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 1;              
            __delay_ms(speed1);
            LATCbits.LATC5 = 1;
            LATCbits.LATC6 = 1;
            LATCbits.LATC7 = 0;
            LATAbits.LATA5 = 0;
            __delay_ms(speed1); 
        }
    }
}
  

