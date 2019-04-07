/*
 * File:   main.c
 * Author: TrudieC
 *
 * Created on March 8, 2019, 12:39 AM
 */

/*March 10 12:10am keeps looping on main, but on the second time the stepper gets funky and the signals to the port are very slow, st the stepper itself isnt even moving 
 also the servos are twitching 
 March 10 8:14am -- right servo won't even move anymore?? the __delay_ms function seems to be delaying the program more than it should. running the servos seems to cause the steppers to stop working properly. 
 */
/*
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

#define speed1 10 // Speed Range 10 to 1  10 = lowest , 1 = highest 
#define speed2 6 // Speed Range 10 to 1  10 = lowest , 1 = highest 
//1 is when it stops turning, 4 is when lights stop flickering, 5 10 is ok, 20 25 30 is finnicky and slow 
#define steps1 14// how much step it will take (249 ~= 1 full rotation)
#define steps2 110 // how much step it will take (249 ~= 1 full rotation)
#define steps3 156 // how much step it will take (249 ~= 1 full rotation)
#define clockwise 0 // clockwise direction macro
#define anti_clockwise 1 // anti clockwise direction macro 
int act_cnt = 0;
 
void system_init (void); // This function will initialise the ports.
void full_drive (char direction,int stepper_no); // This function will drive the motor in full drive mode
void stepper(void);
void servo(void);
void PWM_End();

//void __delay_ms(unsigned int val);
//void delay(unsigned int val);
*/

#include "actuators.h"

void actuators_main(int stack, int steps2_adj){
    int act_cnt = 0;
    system_init();
    initLCD();
    lcd_clear();
    printf("cnt is %d",act_cnt); 
    __delay_ms(200);
    while (1){
    if (act_cnt==0){
//        lcd_clear();
//        printf("Stepper start");  
        
        stepper(stack);
        stepper2(clockwise, steps2_adj);
        stepper2_back(anti_clockwise, steps2_adj);
        LATA = 0b00000000;
        LATCbits.LATC0 = 0;
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;
        LATEbits.LATE2 = 0;
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATAbits.LATA5 = 0;   
    //    __delay_ms(2000);
        lcd_clear();
        printf("Stepper done");    
        
        servo(); 
        lcd_clear();
        printf("Servo done"); 
                LATA = 0b00000000;
        LATCbits.LATC0 = 0;
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;
        LATEbits.LATE2 = 0;
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATAbits.LATA5 = 0;   
//        system_init();
//        PWM_End(); 
        __delay_ms(200);
        
        stepper2(anti_clockwise, steps2_adj);
                        LATA = 0b00000000;
        LATCbits.LATC0 = 0;
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;
        LATEbits.LATE2 = 0;
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATAbits.LATA5 = 0;   
        act_cnt++; 
    }
//    printf("fk");
    if(act_cnt){ //this will prevent the main to loop over itself, because the counter doesnt do anything .-.
             
        break;
    }
    
//    delay(2000);
//    stepper(); 
    }
} 

void system_init (void){
    TRISB = 0x00;     // PORT B as output port
    //PORTB = 0x0F;
    LATB = 0x0F;
    TRISA = 0x00;
    LATA = 0x0F;

    TRISCbits.TRISC1 = 0;  /* Set CCP2 pin as output for PWM out */
//    CCP2CON = 0x0C;
    
    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
//    CCP1CON = 0x0C;        /* Set PWM mode */
    
    LATD = 0x00;
    TRISD = 0x00; //set data direction as output 
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    TRISC = 0x00;
    LATC = 0x00;
    TRISE = 0x00;
    LATE = 0x00;
    
}

void servoRotate0() //0 Degree
{
    unsigned int i;
//    if (servo==1){
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
          
//            RC1 = 1;
//            RC2 = 1;       
//            __delay_us(1400);
//            RC1 = 0;
////            __delay_us(1150);
//            __delay_us(800);
//            RC2 = 0;   
//            __delay_us(18600);
          }
//    }
//    if (servo==2){
//      for(i=0;i<40;i++)
//        {
//          RC2 = 1;
//          __delay_us(449); //goes down to go lower 
//          RC2 = 0;
//          __delay_us(27300); //goes up to go lower 
//        }
//    }
}
void servoRotate180() //180 Degree
{
  unsigned int i;
//    if (servo==1){
    for(i=0;i<40;i++)
        {
          RC1 = 1;
          RC2 = 1;
          __delay_us(450); //goes down to go lower 
//          __delay_us(445); //goes down to go lower 
          RC2 =0;
          __delay_us(2100);
          RC1 = 0;
          __delay_us(13550);
        }
//  }
//  if (servo==2){
//    for(i=0;i<40;i++)
//          {
//            RC2 = 1;
//            __delay_us(2200);
//            RC2 = 0;
//            __delay_us(17800);
//          }
//  }
}
void servo()
{
  TRISC = 0; // PORTB as Ouput Port
  {
//    servoRotate0(1); //0 Degree
    servoRotate0(); //0 Degree,l,L
//    __delay_ms(2000);
//    servoRotate90(); //90 Degree
//    servoRotate180(1); //180 Degree
    servoRotate180(); //180 Degree down 

    __delay_ms(500);        
    servoRotate0(); //0 Degree

  }
}

void stepper(int stack){
    int count_stepper = 0;
 
//        while (1){
        if (count_stepper<1){
//            lcd_clear();
//            printf("step1");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            printf("Step speed %d",speed); 
            
            if (stack == 1){
                for(int i=0;i<steps1;i++){
    //                full_drive(clockwise, 2); //ramp stepper , fwd 
                    full_drive(clockwise, 1); //right stepper forwards A's
    //                full_drive(anti_clockwise, 3); //left stepper forwards 
                }
            }
            if (stack == 2){
                for(int i=0;i<steps1;i++){
                    full_drive(clockwise, 3); //ramp stepper , fwd 
//                    full_drive(clockwise, 1); //right stepper forwards A's
//                    full_drive(anti_clockwise, 3); //left stepper forwards 
                }
            }
//            printf("cnt is %d", count_stepper);
            count_stepper++;
        }
//        if ((count_stepper>=1)&&(count_stepper<3)){
//            lcd_clear();
//            printf("step2");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            for(int i=0;i<steps2;i++){
//                full_drive(clockwise, 2); //forwards 
//            }
//            printf("cnt2 is %d", count_stepper);
//            count_stepper++;
//        }
        else{
//            lcd_clear();
////            printf("steps done");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            printf("cnt3 is %d", count_stepper);
//            break;
            return;
        }
//    }
}

void stepper2(char direction, int steps2_adj){
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
    if (direction == clockwise){
//            lcd_clear();
//            printf("step1");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            printf("Step speed %d",speed2); 
            for(int i=0;i<steps2_adj;i++){
//            for(int i=0;i<110+adjfinal;i++){
                full_drive(direction, 2);
//                full_drive(anti_clockwise, 2);
            }
            printf("cnt is %d", count_stepper);
            count_stepper++;
    }
    if (direction == anti_clockwise){
        lcd_clear();
                        printf("steppp %d", steps2_adj-(steps_back));

                for(int i=0;i<(steps2_adj-(steps_back));i++){
//                for(int i=0;i<30;i++){
                    full_drive(direction, 2);
    //                full_drive(anti_clockwise, 2);
                }
                count_stepper++;
            }
        
    }
} 
void stepper2_back(char direction, int steps2_adj){
//    /*
//    int adjust[4]={0,0,0,0};
//    int base_val = 12; 
//    if ((dist_final[0][5]>=2) && (dist_final[0][5] <= thrdist)){
//        adjust[0]= dist_final[0][5] - base_val;
//    }  
//    if ((dist_final[1][5]>=2) && (dist_final[1][5] <= thrdist)){
//        adjust[1]= dist_final[1][5] - base_val;
//    }
//    if ((dist_final[2][5]>=2) && (dist_final[2][5] <= thrdist)){
//        adjust[2]= dist_final[2][5] - base_val;
//    }        
//    if ((dist_final[3][5]>=2) && (dist_final[3][5] <= thrdist)){
//        adjust[3]= dist_final[2][5] - base_val;
//    }    
//    int adjfinal = adjust[1];
//    */
    int count_stepper = 0;
////        while (1){
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
//        
////    }
}
////    while (1){
////        if (count_stepper<2){
////            for(int i=0;i<steps;i++){
////                full_drive(anti_clockwise);
////                count_stepper++;
////            }
////        }
////        else{
////            break;
////        }
////    }
//    //return;
////}
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
    if (stepper_no == 2){ //RC1,5, 6, 7 
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
        printf("what");
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
  

