#include "actuators.h"

void actuators_main(int stack, int steps2_adj, int t_dep){
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
        
        stepper(stack, t_dep);
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
//        __delay_ms(200);
        if (t_dep%2!=0){ //odd numbers 
            steps2_adj = steps2_adj+1;
        }
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
void servoRotate0(int servo){ //0 Degree
    unsigned int i;
    if (servo==1){
      for(i=0;i<30;i++)
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
      for(i=0;i<30;i++)
        {
//        RB7 = 1;
        RD1 = 1;
//        RD0 = 1;

          __delay_us(1250);
//        RB7 = 0;
        RD1 = 0;
//        RD0 = 0;

          __delay_us(16950);
        }
    }
    if (servo==3){
      for(i=0;i<30;i++)
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
    for(i=0;i<30;i++)
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
    for(i=0;i<30;i++)
          {
//    RB7 = 1; 
    RD1 = 1;
//    RD0 = 1;

//    __delay_us(525); //left 
    __delay_us(950); //left 
//    RB7 = 0;
    RD1 = 0;
//        RD0 = 0;

    __delay_us(26650); //left
//    __delay_us(27000); //left
          }
  }
    if (servo==3){
      for(i=0;i<30;i++)
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
  TRISB = 0; // PORTB as Ouput Port
    TRISC = 0; // PORTB as Ouput Port
    TRISD = 0; // PORTB as Ouput Port

      TRISBbits.RB0 = 0;
    LATBbits.LATB0 = 1; //DISABLES KPD	
    {
//    servoRotate0(1); //0 Degree
    servoRotate0(1); //0 Degree
//    __delay_ms(2000);
//    servoRotate90(); //90 Degree
//    servoRotate180(1); //180 Degree
    servoRotate180(1); //180 Degree
    __delay_ms(400);        
    servoRotate0(1); //0 Degree
    
//    servoRotate180(2); //180 Degree
//    servoRotate0(2); //0 Degree
//    servoRotate180(2); //180 Degree
    
    __delay_ms(50);
    servoRotate180(3); //180 Degree
    servoRotate0(3); //0 Degree
    servoRotate180(3); //180 Degree


  }
}

//void servoRotate0(int servo){ //0 Degree
//    unsigned int i;
//    if (servo==1){
//      for(i=0;i<30;i++)
//          {
//            RC1 = 1;
//            RC2 = 1;
//            __delay_us(1100);
//            RC2 = 0;
//            __delay_us(300);
//            RC1 = 0;
////            __delay_us(1150);
////            __delay_us(300);
//              
//            __delay_us(18600);
//          }
//    }
//    if (servo==2){
//      for(i=0;i<30;i++)
//        {
////        RB7 = 1;
//        RD1 = 1;
////        RD0 = 1;
//
//          __delay_us(1000);
////        RB7 = 0;
//        RD1 = 0;
////        RD0 = 0;
//
//          __delay_us(19000);
//        }
//    }
//    if (servo==3){
//      for(i=0;i<30;i++)
//        {
//        RD0 = 1;
//        __delay_us(2000);
//        //    RB7 = 0;
//        //    RD1 = 0;
//        RD0 = 0;
//        __delay_us(11000);
//        }
//    }
//}
//void servoRotate180(int servo) //180 Degree
//{
//  unsigned int i;
//    if (servo==1){
//    for(i=0;i<30;i++)
//        {
//          RC1 = 1;
//            RC2 = 1;
//          __delay_us(450); //goes down to go lower old
////          __delay_us(420); //goes down to go lower 
//          RC2 =0;
//          __delay_us(2100); //old 
////          __delay_us(2130); //old
//          RC1 = 0;
//          __delay_us(13550);
//        }
//  }
//  if (servo==2){
//    for(i=0;i<30;i++)
//          {
////    RB7 = 1; 
//    RD1 = 1;
////    RD0 = 1;
//
//    __delay_us(525); //left 
////    RB7 = 0;
//    RD1 = 0;
////        RD0 = 0;
//
//    __delay_us(27075); //left
//          }
//  }
//    if (servo==3){
//      for(i=0;i<30;i++)
//        {
//        RD0 = 1;
//
//        __delay_us(2500); //left 
//    //    RB7 = 0;
//    //    RD1 = 0;
//            RD0 = 0;
//
//        __delay_us(10500); //left
//        }
//    }
//}
//void servo()
//{
//  TRISB = 0; // PORTB as Ouput Port
//    TRISC = 0; // PORTB as Ouput Port
//    TRISD = 0; // PORTB as Ouput Port
//
//      TRISBbits.RB0 = 0;
//    LATBbits.LATB0 = 1; //DISABLES KPD	
//    {
////    servoRotate0(1); //0 Degree
//    servoRotate0(1); //0 Degree
////    __delay_ms(2000);
////    servoRotate90(); //90 Degree
////    servoRotate180(1); //180 Degree
//    servoRotate180(1); //180 Degree
//    __delay_ms(400);        
//    servoRotate0(1); //0 Degree
//    
//    servoRotate180(2); //180 Degree
//    servoRotate0(2); //0 Degree
//    servoRotate180(2); //180 Degree
//    
//    __delay_ms(50);
//        servoRotate0(3); //180 Degree
//    servoRotate180(3); //0 Degree
//    servoRotate0(3); //180 Degree
//
//
//  }
//}
//void servoRotate0() //0 Degree
//{
//    unsigned int i;
////    if (servo==1){
////      for(i=0;i<40;i++)
//      for(i=0;i<15;i++)
//          {
//                    
//            RC1 = 1;
//            RC2 = 1;
//            __delay_us(1100);
//            RC2 = 0;
//            __delay_us(300);
//            RC1 = 0;
////            __delay_us(1150);
////            __delay_us(300);
//              
//            __delay_us(18600);
//          
////            RC1 = 1;
////            RC2 = 1;       
////            __delay_us(1400);
////            RC1 = 0;
//////            __delay_us(1150);
////            __delay_us(800);
////            RC2 = 0;   
////            __delay_us(18600);
//          }
////    }
////    if (servo==2){
////      for(i=0;i<40;i++)
////        {
////          RC2 = 1;
////          __delay_us(449); //goes down to go lower 
////          RC2 = 0;
////          __delay_us(27300); //goes up to go lower 
////        }
////    }
//}
//void servoRotate180() //180 Degree
//{
//  unsigned int i;
////    if (servo==1){
////    for(i=0;i<40;i++)
//for(i=0;i<15;i++){
//
//        {
//          RC1 = 1;
//          RC2 = 1;
////          __delay_us(450); //goes down to go lower 
//          __delay_us(475); //goes down to go lower 
//          RC2 =0;
//          __delay_us(2100);
//          RC1 = 0;
//          __delay_us(13525);
//        }
////  }
////  if (servo==2){
////    for(i=0;i<40;i++)
////          {
////            RC2 = 1;
////            __delay_us(2200);
////            RC2 = 0;
////            __delay_us(17800);
////          }
////  }
//}
//}
//void servo()
//{
//  TRISC = 0; // PORTB as Ouput Port
//  {
////    servoRotate0(1); //0 Degree
//    servoRotate0(); //0 Degree,l,L
////    __delay_ms(2000);
////    servoRotate90(); //90 Degree
////    servoRotate180(1); //180 Degree
//    servoRotate180(); //180 Degree down 
//
//    __delay_ms(500);        
//    servoRotate0(); //0 Degree
//
//  }
//}

void stepper(int stack, int t_dep){
    int count_stepper = 0;
 
//        while (1){
        if (count_stepper<1){
//            lcd_clear();
//            printf("step1");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            printf("Step speed %d",speed); 
            
            if (stack == 1){
                if (t_dep<=3){
                    for(int i=0;i<steps1;i++){
        //                full_drive(clockwise, 2); //ramp stepper , fwd 
                        full_drive(clockwise, 1); //right stepper forwards A's
        //                full_drive(anti_clockwise, 3); //left stepper forwards 
                    }
                }
                if (t_dep>3){
                    for(int i=0;i<steps1-1;i++){
        //                full_drive(clockwise, 2); //ramp stepper , fwd 
                        full_drive(clockwise, 1); //right stepper forwards A's
        //                full_drive(anti_clockwise, 3); //left stepper forwards 
                    }
                }
            }
            if (stack == 2){
                if ((t_dep-8)<=3){
                    for(int i=0;i<steps1;i++){
                        full_drive(clockwise, 3); //ramp stepper , fwd 
    //                    full_drive(clockwise, 1); //right stepper forwards A's
    //                    full_drive(anti_clockwise, 3); //left stepper forwards 
                    }
                }
                if ((t_dep-8)>3){
                    for(int i=0;i<steps1-1;i++){
                        full_drive(clockwise, 3); //ramp stepper , fwd 
    //                    full_drive(clockwise, 1); //right stepper forwards A's
    //                    full_drive(anti_clockwise, 3); //left stepper forwards 
                    }
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
//void stepper2(char direction, int steps2_adj){
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
//    int count_stepper = 0;
////        while (1){
//        if (count_stepper<1){
////            lcd_clear();
////            printf("step1");
////            lcd_set_ddram_addr(LCD_LINE2_ADDR);
////            printf("Step speed %d",speed2); 
//            for(int i=0;i<steps2_adj;i++){
////            for(int i=0;i<110+adjfinal;i++){
//                full_drive(direction, 2);
////                full_drive(anti_clockwise, 2);
//            }
//            printf("cnt is %d", count_stepper);
//            count_stepper++;
//        }
////9
//        
////    }
//} 
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
                    LATA = 0b00000000;
        LATCbits.LATC0 = 0;
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;
        LATEbits.LATE2 = 0;
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATAbits.LATA5 = 0; 
}
  

