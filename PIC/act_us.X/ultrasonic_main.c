
#include "ultrasonic.h"
#include "actuators.h"


void __interrupt() ISR(){
	if(RBIF == 1){                       //Makes sure that it is PORTB On-Change Interrupt
		RBIE = 0;                         //Disab-le On-Change Interrupt
//		echoo = true;
//        trig();
        echo();
//        print_echo();
        RBIF = 0;                           //Clear PORTB On-Change Interrupt flag
        RBIE = 1;                           //Enable PORTB On-Change Interrupt
    }
//    else{
////        echoo = false; 
//    }
}
void echo(){
    if (b==1){
        if (RB7 == 1){                     //If ECHO is HIGH
            TMR1ON = 1; 
        }                      //Start Timer
        if (RB7 == 0){                     //If ECHO is LOW
            TMR1ON = 0;                     //Stop Timer
            a[0] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
            // a[0] = (TMR1L | (TMR1H<<8))*0.034/2; 
        }
                //Moving average 
        if (a[0]>=2 && a[0]<=200){
            if ((~sumf[0])&&(dist_final[0][0]!=0)&&(dist_final[0][1]!=0)&&(dist_final[0][2]!=0)){ //if haven't summed before 
                sum[0] = (dist_final[0][0]+dist_final[0][1]+dist_final[0][2]);
                sumf[0] = true;
            }
            if (sumf[0]){
                sum[0] = sum[0] - dist_final[0][meas] + a[0];
                dist_final[0][5] = (sum[0]/5);    //average         
            }
            dist_final[0][meas] = a[0];
        }
    }
    if (b==2){
        if (RB6 == 1){                     //If ECHO is HIGH
            TMR1ON = 1; 
        }                      //Start Timer
        if (RB6 == 0){                     //If ECHO is LOW        
            TMR1ON = 0;                     //Stop Timer
            a[1] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
            // a[1] = (TMR1L | (TMR1H<<8))*0.034/2; 
        }
                if (a[1]>=2 && a[1]<=200){

                if ((~sumf[1])&&(dist_final[1][0]!=0)&&(dist_final[1][1]!=0)&&(dist_final[1][2]!=0)){ //if haven't summed before 
            sum[1] = (dist_final[1][0]+dist_final[1][1]+dist_final[1][2]);
            sumf[1] = true;
        }
        if (sumf[1]){
            sum[1] = sum[1] - dist_final[1][meas] + a[1];
            dist_final[1][5] = (sum[1]/5);            
        }
        dist_final[1][meas] = a[1];
                }
    }
    if (b==3){
        if (RB5 == 1){                     //If ECHO is HIGH
            TMR1ON = 1; 
        }                      //Start Timer
        if (RB5 == 0){                     //If ECHO is LOW
            TMR1ON = 0;                     //Stop Timer
            a[2] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
            // a[2] = (TMR1L | (TMR1H<<8))*0.034/2; 
        }
                if (a[2]>=2 && a[2]<=200){

                if ((~sumf[2])&&(dist_final[2][0]!=0)&&(dist_final[2][1]!=0)&&(dist_final[2][2]!=0)){ //if haven't summed before 
            sum[2] = (dist_final[2][0]+dist_final[2][1]+dist_final[2][2]);
            sumf[2] = true;
        }
        if (sumf[2]){
            sum[2] = sum[2] - dist_final[2][meas] + a[2];
            dist_final[2][5] = (sum[2]/5);            
        }
        dist_final[2][meas] = a[2];
                }
    }
    if (b==4){
        if (RB4 == 1){                     //If ECHO is HIGH
            TMR1ON = 1; 
        }                      //Start Timer
        if (RB4 == 0){                     //If ECHO is LOW
            TMR1ON = 0;                     //Stop Timer
            a[3] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
            // a[3] = (TMR1L | (TMR1H<<8))*0.034/2; 
        }
                if (a[3]>=2 && a[3]<=200){

                if ((~sumf[3])&&(dist_final[3][0]!=0)&&(dist_final[3][1]!=0)&&(dist_final[3][2]!=0)){ //if haven't summed before 
            sum[3] = (dist_final[3][0]+dist_final[3][1]+dist_final[3][2]);
            sumf[3] = true;
        }
        if (sumf[3]){
            sum[3] = sum[3] - dist_final[3][meas] + a[3];
            dist_final[3][5] = (sum[3]/5);            
        }
        dist_final[3][meas] = a[3];
                }
    }
}
//void echo(){
//    if (b==1){
//        if (RB7 == 1){                     //If ECHO is HIGH
//            TMR1ON = 1; 
//        }                      //Start Timer
//        if (RB7 == 0){                     //If ECHO is LOW
//            TMR1ON = 0;                     //Stop Timer
//            a[0] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
//            // a[0] = (TMR1L | (TMR1H<<8))*0.034/2; 
//        }
//
//        if ((~sumf)&&(dist_final[0][0]!=0)&&(dist_final[0][1]!=0)&&(dist_final[0][2]!=0)&&(dist_final[0][3]!=0)&&(dist_final[0][4]!=0)){ //if haven't summed before 
//            sum[0] = (dist_final[0][0]+dist_final[0][1]+dist_final[0][2]+dist_final[0][3]+dist_final[0][4]);
//            sumf = true;
//        }
//        if (sumf){
//            sum[0] = sum[0] - dist_final[0][meas] + a[0];
//            dist_final[0][5] = (sum[0]/5);            
//        }
//        dist_final[0][meas] = a[0];
////        if (meas ==4){
////            meas =0;
////        }
////        if (meas==0){
////            meas=1;
////        }
////                if (meas==1){
////            meas=2;
////        }        if (meas==2){
////            meas=3;
////        }
////                if (meas==3){
////            meas=4;
////        }
//        /* keep an index for your measurement number that continues in a loop. use this to find out where to subtract and add the new measurement and recalculate the sum and avg 
//        if (dist_final[0][4]==0){   //if the last element of moving avg array is 0
//            for (int i=0; i<5; i++){    //runs through a loop to fill the first empty space 
//                if ((dist_final[0][i]==0)&&(set == 0)){ //if the ith element is 0 and nothing has been set yet
//                    dist_final[0][i] = a[0];    //set the element the distance just read 
//                    set = 1;    //set a flag 
//                }
//            } 
//        }
//
//        if ((dist_final[0][4]!=0)&&(avg == 0)){ //if the last element is not 0 (array is full)
//            
//            avg = ((dist_final[0][0]+dist_final[0][1]+dist_final[0][2]+dist_final[0][3]+dist_final[0][4])/5);
//            
//        }
//        if ((dist_final[0][4]!=0)&&(avg != 0)){
//            
//        }*/
//    }
//    if (b==2){
//        if (RB6 == 1){                     //If ECHO is HIGH
//            TMR1ON = 1; 
//        }                      //Start Timer
//        if (RB6 == 0){                     //If ECHO is LOW        
//            TMR1ON = 0;                     //Stop Timer
//            a[1] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
//            // a[1] = (TMR1L | (TMR1H<<8))*0.034/2; 
//        }
//    }
//    if (b==4){
//        if (RB4 == 1){                     //If ECHO is HIGH
//            TMR1ON = 1; 
//        }                      //Start Timer
//        if (RB4 == 0){                     //If ECHO is LOW
//            TMR1ON = 0;                     //Stop Timer
//            a[3] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
//            // a[3] = (TMR1L | (TMR1H<<8))*0.034/2; 
//        }
//    }
//   
//    if (b==3){
//        if (RB5 == 1){                     //If ECHO is HIGH
//            TMR1ON = 1; 
//        }                      //Start Timer
//        if (RB5 == 0){                     //If ECHO is LOW
//            TMR1ON = 0;                     //Stop Timer
//            a[2] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
//            // a[2] = (TMR1L | (TMR1H<<8))*0.034/2; 
//        }
//    }
//    if (b==4){
//        if (RB4 == 1){                     //If ECHO is HIGH
//            TMR1ON = 1; 
//        }                      //Start Timer
//        if (RB4 == 0){                     //If ECHO is LOW
//            TMR1ON = 0;                     //Stop Timer
//            a[3] = (TMR1L | (TMR1H<<8))/58.82; //Calculate Distance
//            // a[3] = (TMR1L | (TMR1H<<8))*0.034/2; 
//        }
//    }
////    print_echo();
//}

//void trig(int number_of_sensors){
void trig(){
    
    for(b=1; b<(5); b++){
        TMR1H = 0;                        //Sets the Initial Value of Timer
        TMR1L = 0;                        //Sets the Initial Value of Timer
//        RBIE = 0;
        if(b==1){
            RBIE = 0;
            RD1 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RD1 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
            RBIE = 1;
            __delay_ms(us_delay);                  //Waiting for ECHO
//
//            if (echoo){
//                echo(); 
//            }
        }
        if(b==2){
            RBIE = 0;
            RD0 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RD0 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
                        RBIE = 1;
            __delay_ms(us_delay);                  //Waiting for ECHO
//            if (echoo){
//                echo(); 
//            }
        }
        if(b==3){
            RBIE = 0;
            RB3 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RB3 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
                        RBIE = 1;
            __delay_ms(us_delay);                  //Waiting for ECHO
//            if (echoo){
//                echo(); 
//            }
        }
        if(b==4){
            RBIE = 0;
            RB2 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RB2 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
                        RBIE = 1;
            __delay_ms(us_delay);                  //Waiting for ECHO
//            if (echoo){
//                echo(); 
//            }
        }
//    __delay_ms(100);                  //Waiting for ECHO
//    RBIE =1;
//    __delay_ms(100);                  //Waiting for ECHO    
    }

}


void print_echo(){
    lcd_clear();
    if(a[0]>=2 && a[0]<=400){                //Check whether the result is valid or not
        printf("Dist1=%d,Avg=%d ",a[0],dist_final[0][5]);
    }
    if(!(a[0]>=2 && a[0]<=400)){
        printf("Dist1 = X");
    }
    if(a[1]>=2 && a[1]<=400){                //Check whether the result is valid or not
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("Dist2=%d,Avg=%d",a[1],dist_final[1][5]);
    }		
    if(!(a[1]>=2 && a[1]<=400)){
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("Dist2 = X");
    }
    if(a[2]>=2 && a[2]<=400){                //Check whether the result is valid or not
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
        printf("Dist3=%d,Avg=%d",a[2],dist_final[2][5]);
    }
    if(!(a[2]>=2 && a[2]<=400)){
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
        printf("Dist3 = X");
    }
    if(a[3]>=2 && a[3]<=400){                //Check whether the result is valid or not
        lcd_set_ddram_addr(LCD_LINE4_ADDR);
        printf("Dist4=%d,Avg=%d ",a[3],dist_final[3][5]);        
//        printf("Dist 4= %d",a[3]);
    }
    if(!(a[3]>=2 && a[3]<=400)){
        lcd_set_ddram_addr(LCD_LINE4_ADDR);
        printf("Dist4 = X");
    }
}
//void print_echo(){
//    lcd_clear();
//    if(a[0]>=2 && a[0]<=400){                //Check whether the result is valid or not
//        printf("%d,%d,%d,%d,%d",a[0],dist_final[0][0],dist_final[0][5],sum[0],meas);
//    }
//    if(!(a[0]>=2 && a[0]<=400)){
//        printf("Distance 1 = X");
//    }
//    if(a[1]>=2 && a[1]<=400){                //Check whether the result is valid or not
//        lcd_set_ddram_addr(LCD_LINE2_ADDR);
//        printf("Distance 2 = %d",a[1]);
//    }		
//    if(!(a[1]>=2 && a[1]<=400)){
//        lcd_set_ddram_addr(LCD_LINE2_ADDR);
//        printf("Distance 2 = X");
//    }
//    if(a[2]>=2 && a[2]<=400){                //Check whether the result is valid or not
//        lcd_set_ddram_addr(LCD_LINE3_ADDR);
//        printf("Distance 3= %d",a[2]);
//    }
//    if(!(a[2]>=2 && a[2]<=400)){
//        lcd_set_ddram_addr(LCD_LINE3_ADDR);
//        printf("Distance 3 = X");
//    }
//    if(a[3]>=2 && a[3]<=400){                //Check whether the result is valid or not
//        lcd_set_ddram_addr(LCD_LINE4_ADDR);
//        printf("Distance 4= %d",a[3]);
//    }
//    if(!(a[3]>=2 && a[3]<=400)){
//        lcd_set_ddram_addr(LCD_LINE4_ADDR);
//        printf("Distance 4 = X");
//    }
//}
    
int min(int *array, int size){
    int minimum  = array[0];
//            lcd_clear(); 
//        printf("minimum %d", minimum);
//        __delay_ms(1000);
    for (int i = 0; i<size; i++){
//        lcd_clear(); 
//        printf("fieas");
//        __delay_ms(1000);
        if ((minimum==0)||(minimum>=array[i])&&(array[i]!=0)&&(array[i]>=14)&&(array[i]<=23)){
//            lcd_clear(); 
//            printf("minimum1 %d", minimum);
//            __delay_ms(1000);
            minimum = array[i]; 
//            lcd_clear(); 
//            printf("minimum2 %d", minimum);
//            __delay_ms(1000);
        }
    }
    return minimum;
}

int number_deploy(int avg_dist, poles_detected){
    int tires_t=0;
    int tires_detected=0;
    int t_count = 0; 
    
//	while (DC motors off){
    while (t_count<3){
		if (poles_detected != 0){				
			if ((avg_dist/1000)<30){
				//Total number of tires tires_t
				tires_t = 1; 
			}
			if ((avg_dist/1000)>30){
				tires_t = 2; 
			}
		}
        
		if (poles_detected == 0){
			tires_t = 2;
		}    
            
		if ((a[0]>=2 && a[0] <= 15)){
//            sensl_1 = true;  //lower sensor is high 
            if ((a[1]>=2 && a[1] <= 15)){
//                sensl_2 = true; // 
                tires_detected = 1;
            }
            else{
                tires_detected = 0;
            }
        }
        if ((tires_detected==1)){
            if((a[2]>=2 && a[2] <= 15)&&(a[3] >=2 && a[3]<= 15)){
                tires_detected++;
            }
        }

    t_count = tires_t - tires_detected; //tires need to be deployed         
//    Pole[poles_detected].tires_deployed = t_count; 
//    Pole[poles_detected].tires_final = tires_t;
    lcd_clear();
    printf("det,%d, tcnt %d",tires_detected,t_count);
//    __delay_ms(1000);
    
    if (t_count<=2){
//            lcd_clear();
//    printf("dfads");
//    __delay_ms(1000);
        return (int) t_count;
            break;

    }
    }
}

int ultrasonic_main(){
	TRISB = 0b11110000;                 //RB5 6 as Input PIN (ECHO)
	TRISD = 0x00;                       // LCD Pins as Output
	GIE = 1;                            //Global Interrupt Enable
	RBIF = 0;                           //Clear PORTB On-Change Interrupt Flag
	RBIE = 1;                           //Enable PORTB On-Change Interrupt
    ADCON1=0x0F;
    
    TRISBbits.RB0 = 0;
    LATBbits.LATB0 = 1; //DISABLES KPD
    
	initLCD();
	lcd_clear();

    int poles_detected=0; 
    
	T1CON = 0x10;                       //Initialize Timer Module
    
//	while(1){  
                for (int i=0; i<2;i++){

        for (meas=0; meas<5;meas++){
//        trig(4);
        trig();
//        __delay_ms(100);
        RBIE = 0;
//        __delay_ms(100);
        print_echo();
//        __delay_ms(100);
        RBIE = 1;
//        __delay_ms(100);
//        number_deploy(20,0);
        }
   
//        }        
//    lcd_clear();
//        printf("reset");
//        __delay_ms(1000);
//            for (int m=0;m<4;m++){
//                sum[m] = 0 ;
//                a[m] = 0 ;
//                sumf[m] = 0 ;
//                
//                for (int k=0;k<6;k++){
//                    dist_final[m][k] = 0; 
//                }
//            } 
    }
    int min_us_dist = min(a, 4);
    lcd_clear(); 
    printf("minimum %d", min_us_dist);
//    __delay_ms(1000);
        int step2_offset =0;

        if (min_us_dist<14){
        step2_offset = 0;
    }
    else{
    step2_offset = abs(min_us_dist - 14);
    }
//    int step2_offset = abs(min_us_dist - 14);
    int steps2_adj = steps2 + step2_offset;
        lcd_clear(); 
    printf("stepsadj %d", steps2_adj);
//    __delay_ms(1000);
    return steps2_adj;
}



