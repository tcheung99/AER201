#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "RTC.h"
#include "I2C.h"
#include "lcd.h"
#include "actuators.h"
//#include "timer.h"
//#include "ultrasonic.h"


#define us_delay 135 
#define thrdist 30

int timer_count = 0;
int cnt =0;

int a[4];
int prev_dist_final[4];
int b;
void echo();
volatile int sens;
void print_echo();
void sense_tires(int sensed);    
int ultrasonic_main();    

const char keys[] = "123A456B789C*0#D";

unsigned char temp;
short int temp_int, tick;
volatile bool key_was_pressed = false;
int state = 0;
int disp = 0;
int i = 0;
int pressed = 0;

//I2C
unsigned char counter = 0; // Increments each time a byte is sent
unsigned char keypress; // Stores the data corresponding to the last key press
unsigned char data; // Holds the data to be sent/received
int dists[2] = {0,-1};
bool send = true; //PIC is sending 

int tires_deployed; 
int poles_detected; 

volatile long avg_dist; 
//void start_dc();
void start();
void brake();

typedef struct Poles{
	int tires_deployed; //check var
	int tires_final;//check var
	int dist_from_start;//check var
    int dist_from_cl;
	} Poles;
struct Poles Pole[10] = {0,0,0,0,0,0,0,0,0,0,}; //declare pole array 

int dist_final[4][6] = { 
    {0, 0, 0, 0, 0,0}, // dist_final[0] 
    {0, 0, 0, 0, 0,0}, // dist_final[1] 
    {0, 0, 0, 0, 0,0}, // dist_final[2] 
    {0, 0, 0, 0, 0,0}, // dist_final[3] 
};
int sum[4];
bool sumf[4] = {false,false,false,false};
volatile int meas =0;

void __interrupt() ISR(){
	if(RBIF == 1){                       //Makes sure that it is PORTB On-Change Interrupt
        if (sens){
            RBIE = 0;                         //Disable On-Change Interrupt
            echo();
            RBIF = 0;                           //Clear PORTB On-Change Interrupt flag
            RBIE = 1;                           //Enable PORTB On-Change Interrupt
        }
        if (~sens){
            key_was_pressed = true;
            INT1IF = 0; // Clear interrupt flag bit to signify it's been handled
        }
    }
//    if (TMR0IF){
//        INTCONbits.TMR0IF = 0;      //CLEAR FLAG SO IT CAN BE TRIGGERED AGAIN
//        TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
//        TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
//        cnt++;
//        LEDPin ^= 1u;           //Toggle LED
//    //    lcd_clear();
//    //    printf("time");
//    //    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    //    if (cnt%2 == 0){
//    //        printf("%d",cnt/2);
//    //    }
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

void trig(){
    for(b=1; b<5; b++){
        TMR1H = 0;                        //Sets the Initial Value of Timer
        TMR1L = 0;                        //Sets the Initial Value of Timer
        if(b==1){
            RBIE = 0;
            RD1 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RD1 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
            RBIE = 1;
            __delay_ms(us_delay);   
        }
        if(b==2){
            RBIE = 0;
            RD0 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RD0 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
            RBIE = 1;
            __delay_ms(us_delay);   
        }
        if(b==3){
            RBIE = 0;
            RB3 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RB3 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
            RBIE = 1;
            __delay_ms(us_delay);   
        }
        if(b==4){
            RBIE = 0;
            RB2 = 1;                          //TRIGGER HIGH
            __delay_us(10);                   //10uS Delay
            RB2 = 0;                          //TRIGGER LOW 
//            __delay_ms(100);                  //Waiting for ECHO
            RBIE = 1;
            __delay_ms(us_delay);   
        } 
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
int min(int *array, int size){
    int minimum =0 ;
    if (array[0]<=23){
         minimum  = array[0];}
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

int ultrasonic_main(){ 
    TRISBbits.RB0 = 0;
    LATBbits.LATB0 = 1; //DISABLES KPD	

        lcd_clear(); 
    printf("or here"); 	
	TRISB = 0b11110000;                 //RB5,6,7 as Input PIN (ECHO)
    lcd_clear(); 
    printf("break heree?");    
    RBIF = 0;                           //Clear PORTB On-Change Interrupt Flag
	RBIE = 1;                           //Enable PORTB On-Change Interrupt
 	
    int sensed = 0;
    int min_us_dist=0;
    int steps2_adj=0;
    int step2_offset=0;
    while(send){
        for (meas=0; meas<3;meas++){ //3 measurements in the moving average array 
        trig();
        RBIE = 0;
        print_echo();
        RBIE = 1;
        int min_us_dist = min(a, 4);
        lcd_clear(); 
        printf("minimum %d", min_us_dist);
//        __delay_ms(1000);
        if (min_us_dist<14){
            step2_offset = 0;
        }
        if (min_us_dist>=14){
        step2_offset = abs(min_us_dist - 14);
        }
        steps2_adj = steps2 + step2_offset;
        sensed++;

//        sense_tires(sensed);
        }        
        if (sensed>2){
            break;
            send = false; 
        }
//            int min_us_dist = min(a, 4);
//    lcd_clear(); 
//    printf("minimum %d", min_us_dist);
//    __delay_ms(1000);
//    
//    int step2_offset = abs(min_us_dist - 14);
//    int steps_adj = steps2 + step2_offset;

    }
//    while (~send){ //gets data from arduino 
        //Read Arduino's encoder data
//                lcd_clear(); 
//        printf("wtf");
//        I2C_Master_Start();
//        I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
//        avg_dist = I2C_Master_Read(NACK); // Read one char only
//        I2C_Master_Stop();
//        if(avg_dist){
//            
////            while(1){
//            lcd_clear();
////                if (dists[0] != 0 && dists[1] == -1){
////                    dists[1] = data; 
////                }
////                if (dists[0] == 0){
////                    dists[0] = data;
////                }
//
//            printf("%d",avg_dist); //puts character on LCD 
////                lcd_set_ddram_addr(LCD_LINE2_ADDR);
////                printf("Motor dist1 %d",dists[0]); 
////                lcd_set_ddram_addr(LCD_LINE3_ADDR);
////                printf("Motor dist2 %d",dists[1]);
//            __delay_ms(1000);  
////            }
//        send = true; //return to PIC as sender
//        }
//
//        else{
//            send = false;
//            break;
//        }
//    }
         lcd_clear(); 
    printf("stepsadj %d", steps2_adj);
    __delay_ms(1000);
    return steps2_adj;   
}

void sense_tires(int sensed){    
        //        __delay_ms(1000);
//        if ((a[0]>=2 && a[0] <= 15) ||(a[1]>=2 && a[1] <= 15)||(a[2]>=2 && a[2] <= 15)||(a[3] >=2 && a[3]<= 15)){ 
//        __delay_ms(300);  //delay to allow sensors to read more than just one reading    
            if (((sensed>2)&&(poles_detected>0))||(poles_detected==0)){    //this algorithm may need to be changed////////////////////////////////
                if ((dist_final[0][5]>=2 && dist_final[0][5] <= thrdist) ||(dist_final[1][5]>=2 && dist_final[1][5] <= thrdist)||(dist_final[2][5]>=2 && dist_final[2][5] <= thrdist)||(dist_final[3][5] >=2 && dist_final[3][5]<= thrdist)){ 
                    //Tell Arduino to stop motors by writing 9 
                    brake();
                    //Turn PIC into receiver mode 
                    send = false; 
    //                start_dc();
    //                lcd_clear();
    //                printf("tiemr %d",cnt);
    //                __delay_ms(1000);
                }
            }
//        }
//			__delay_ms(400);   
//    }
        //        __delay_ms(500);
        
        /*if ((a[0]>=2 && a[0] <= 15) ||(a[1]>=2 && a[1] <= 15)||(a[2]>=2 && a[2] <= 15)||(a[3] >=2 && a[3]<= 15)){ 
            //Tell Arduino to stop motors by writing 9 
            I2C_Master_Start(); // Start condition
            I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
            I2C_Master_Write('9'); // Write key press data which shows up on Arduino's serial monitor
            I2C_Master_Stop(); 
            //Turn PIC into receiver mode 
            send = false; 
		}    
//			__delay_ms(400);   
    }*/

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
        Pole[poles_detected].tires_deployed = t_count; 
        Pole[poles_detected].tires_final = tires_t;
        lcd_clear();
        printf("det,%d, tcnt %d",tires_detected,t_count);
        __delay_ms(1000);

        if (t_count<=2){
        //            lcd_clear();
        //    printf("dfads");
        //    __delay_ms(1000);
        //        return (int) t_count;
        break;
        }
    }
    return (int) t_count;
}
//void start_dc(){
//    send=true;
//    lcd_clear();
//    printf("I NEED HELP");
//    __delay_ms(500);
//    //Start I2C, writes 1 to the Arduino's serial monitor
//    I2C_Master_Start(); // Start condition
//    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
//    I2C_Master_Write('1'); // Write key press data which shows up on Arduino's serial monitor
//    I2C_Master_Stop();
//}

void UI_main(int t_dep, int poles_detected){
    sens = 0; 
    //    i2c_mine(); 
    // RD2 is the character LCD RS (Register Select pin)
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00; //set data direction as output 
    
    // Set all A/D ports to digital (pg. 222)
    TRISD = 0x00; //set data direction as output 
    
    // Enable RB1 (keypad data available) interrupt
    INT1IE = 1;
    
    // Initialize LCD
    initLCD();
    
    // Enable interrupts
    ei();
    
    I2C_Master_Init(100000); 
    I2C_Master_Start();
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Stop();
    
    // Main loop
	Poles Pole[10]; //is this allowed

//    I2C_Master_Start(); // Start condition
//    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
//    I2C_Master_Write(temp); // Write key press data which shows up on Arduino's serial monitor
//    I2C_Master_Stop();
	unsigned long tick = 0;
    if((pressed == 0)&&(sens==0)){
        lcd_clear();           
        printf("1 - Start");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("2 - Summary");
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
        printf("3 - Date&Time ");
        }
    while(sens==0){
//        if(pressed == 0){
//            lcd_clear();           
//            printf("1 - Start");
//            lcd_set_ddram_addr(LCD_LINE2_ADDR);
//            printf("2 - Summary");
//            lcd_set_ddram_addr(LCD_LINE3_ADDR);
//            printf("3 - Date&Time ");
//            }
        if (send){
        if(key_was_pressed){
            pressed = 1;
            key_was_pressed = false; // Clear the flag
            unsigned char keypress = (PORTB & 0xF0) >> 4; //right shift
            temp = keys[keypress];
            temp_int = (temp-'0');
            
            if (temp_int == 0){
                state = 0; 
                lcd_clear();           
                printf("1 - Start");
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
                printf("2 - Summary");
                lcd_set_ddram_addr(LCD_LINE3_ADDR);
                printf("3 - Date&Time ");

            }
            if ((temp_int == 1)&&(temp!= '*')&&(temp!= '#')){
                state = 0;
                lcd_clear();
                printf("Machine In Use");
                lcd_set_ddram_addr(LCD_LINE4_ADDR);
                printf("     0-Menu     ");
                //__delay_ms(2000);
                //lcd_clear();
//                start();
                I2C_Master_Start(); // Start condition
                I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
                I2C_Master_Write('1'); // Write key press data which shows up on Arduino's serial monitor
                I2C_Master_Stop();
//                timer_main(); //Start operation timer 
                sens = 1;
                }
            
                if ((temp_int == 2)||(state == 2)){
                    if ((temp_int == 2)){	 //this loop is never traversed.. 
                        state = 2; 
                        disp = 0;
                        lcd_clear();
                        printf("Op Time:");
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("%d", cnt);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("    0-Menu   #>");
                    }
                    if ((temp == '*') && (disp != 0)){
                        disp = disp - 1; 
                        }
                    if ((temp == '#')&& (disp <12)){
                        disp++; 
                        }
                    if (disp == 0){
                        lcd_clear();
                        printf("Op Time:");
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("%d", cnt);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("     0-Menu   #>");
                        //__delay_ms(1500);
                    }
                    if (disp == 1){
                        lcd_clear();
                        printf("No. Tires: %d",t_dep);
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("No. Poles: %d", poles_detected);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("<*   0-Menu   #>");                            
                        //__delay_ms(1500);
                        }
                    if (disp > 1 && disp <11){
                        i = disp - 2;
                        lcd_clear();
                        printf("P%d Dep:%d,Tot:%d", i+1 , Pole[i].tires_deployed,Pole[i].tires_final);
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("DistToStart:%d",i+1 , Pole[i].dist_from_start);
                        lcd_set_ddram_addr(LCD_LINE3_ADDR);
                        printf("DistToCL:%d",i+1 , Pole[i].dist_from_cl);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("<*   0-Menu   #>");      
                    }
                    if (disp == 11){
                        i = disp - 2;
                        lcd_clear();
                        printf("P%d Dep:%d,Tot:%d", i+1 , Pole[i].tires_deployed,Pole[i].tires_final);
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("DistToStart:%d",i+1 , Pole[i].dist_from_start);
                        lcd_set_ddram_addr(LCD_LINE3_ADDR);
                        printf("DistToCL:%d",i+1 , Pole[i].dist_from_cl);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("<*   0-Menu     "); 
				}
			}
            if (temp_int == 3){
                state = 0;
                tick = 0; 
                while(~key_was_pressed){                    
                    if(tick % 1000 == 0){
                        lcd_clear();
                        printf("DATE & TIME"); 
                        I2C_Master_Init(100000);
                        I2C_Master_Start(); // Start condition
                        I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
                        I2C_Master_Write(0x00); // Set memory pointer to seconds
                        I2C_Master_Stop(); // Stop condition
                        // Read current time
                        I2C_Master_Start(); // Start condition
                        I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
                        for(unsigned char i = 0; i < 6; i++){
                            time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
                        }
                        time[6] = I2C_Master_Read(NACK); // Final Read with NACK
                        I2C_Master_Stop(); // Stop condition

                        // Print received data on LCD
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("%02x/%02x/%02x", time[6],time[5],time[4]); // Print date in YY/MM/DD
                        lcd_set_ddram_addr(LCD_LINE3_ADDR);
                        printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("     0-Menu     ");
                        }
                    //__delay_ms(1000); 
                    tick++;
                    //lcd_clear();                       
                }        
            //lcd_clear();    
            }
        }
        }
    }
}

void brake(){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write('9'); // Write key press data which shows up on Arduino's serial monitor
    I2C_Master_Stop(); 
    I2C_Master_Start();
    I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
    avg_dist = I2C_Master_Read(NACK); // Read one char only
    I2C_Master_Stop();
    if(avg_dist){

//            while(1){
        lcd_clear();
//                if (dists[0] != 0 && dists[1] == -1){
//                    dists[1] = data; 
//                }
//                if (dists[0] == 0){
//                    dists[0] = data;
//                }

        printf("%d",avg_dist); //puts character on LCD 
//                lcd_set_ddram_addr(LCD_LINE2_ADDR);
//                printf("Motor dist1 %d",dists[0]); 
//                lcd_set_ddram_addr(LCD_LINE3_ADDR);
//                printf("Motor dist2 %d",dists[1]);
        __delay_ms(1000);  
//            }
    send = true; //return to PIC as sender
    }
}
    
void start(){
    I2C_Master_Init(100000); 
    I2C_Master_Start();
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Stop();
    //Start I2C, writes 1 to the Arduino's serial monitor
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write('2'); // Write key press data which shows up on Arduino's serial monitor
    I2C_Master_Stop();
}
void backwards(){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write('5'); // Write key press data which shows up on Arduino's serial monitor
    I2C_Master_Stop(); 
}

void main(){
    int stack=1; 
    int t_dep=0;
    int t_count; 
	TRISD = 0x00;                       // LCD Pins as Output
	GIE = 1;                            //Global Interrupt Enable
	    int steps2_adj =0;
    ADCON1=0x0F;                        // Set all A/D ports to digital (pg. 222)

	initLCD();

	T1CON = 0x10;                       //Initialize Timer Module
    int poles_detected=0;
    int read=0;
    volatile long prev_avg_dist=0; 

    TRISAbits.RA4 = 0;    
    PORTAbits.RA4 = LATAbits.LATA4;

    bool act_done = false; 
    bool arduino_stopped = false; 
    
    Poles Pole[10];
//    int Pole[10];
//    for (i=0; i<10; i++){
//        Poles Pole[i];
//    }
    
//            actuators_main(1);
    
    if (~sens){
        UI_main( t_dep, poles_detected);
    }
    while (1){
        PORTAbits.RA4 = LATAbits.LATA4;

//        lcd_clear();
//        printf("happen");
//        __delay_ms(1000);
        if ((poles_detected <10)&&((Pole[poles_detected].dist_from_start )<4000)){ //while there are less than 10 poles detected and the robot has travelled less than 4m 
//            lcd_clear();
//            printf("help");
//            __delay_ms(1000);
//    while (1){
//        lcd_clear();
//        printf("sensing ");
//        __delay_ms(2000);
            while (sens){
//            if (sens){
//                    __delay_ms(2000);
                t_count = 5;  ///something absurd 
                    lcd_clear(); 
                    printf("waiting"); //stops here 
                    //                    while (!PORTAbits.RA4); 
                    if (!PORTAbits.RA4){
                        lcd_clear(); 
                        printf("more waiting"); //stops here 
                    } 
                    if (PORTAbits.RA4){
//                        brake();
                        lcd_clear(); 
                        printf("done wait");
                        arduino_stopped = true; 
                    }
                    if (arduino_stopped){
                        lcd_clear(); 
                        printf("dun break");
                        steps2_adj=ultrasonic_main();
                        lcd_clear(); 
                        printf("stepsadj %d", steps2_adj);
                        t_count = number_deploy(avg_dist, poles_detected); 
                        sens = 0;
                    }
            }
            int pole_cl_dist = (avg_dist)-(prev_avg_dist); //centerline to centerline distance 
//            t_count = number_deploy(avg_dist, poles_detected); 
//            lcd_clear();
//            printf("tcnt %d", t_count);
//            printf("tdep %d", t_dep);
//            __delay_ms(500);
            act_done = false; 
            if (t_count<=2){ //check 
                if (t_dep <8){
//                    lcd_clear();
//                    printf("bruh");
                    stack = 1;
                                    brake();        

                    for(int i=0; i<(t_count); i++){
                        actuators_main(stack, steps2_adj, t_dep); 
//                        actuators_main(stack, 110); 
                        t_dep++;
                    }
                }
                if (t_dep >= 8){
                    stack = 2;
                    for(int i=0; i<(t_count); i++){
                        actuators_main(stack, steps2_adj, t_dep); 
//                        actuators_main(stack, 110); 
                        t_dep++;
                    }
                }
                act_done = true; 
            }
            if (act_done){
            Pole[poles_detected].dist_from_cl = avg_dist;
            Pole[poles_detected].dist_from_start = avg_dist+prev_avg_dist;
            lcd_clear();
            printf("dist p[%d]:%d,%d", poles_detected,Pole[poles_detected].dist_from_cl, avg_dist);
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("p[%d] fs : %d",poles_detected, Pole[poles_detected].dist_from_start);
            __delay_ms(3000);
            
            prev_avg_dist = avg_dist ; 
            poles_detected++;
            start();
            arduino_stopped = false; 
            
            lcd_clear();
            printf("avg dist %d", avg_dist);
            printf("poles d %d", poles_detected);
            for (int i=0;i<4;i++){ //reset ultrasonic sensors 
                sum[i] = 0 ;
                a[i] =0 ;
                sumf[i] = 0 ;
                for (int k=0;k<6;k++){
                    dist_final[i][k] = 0; 
                    }
                }   
                __delay_ms(1000);
                sens = 1;
            }
        }
        else{
            brake();
            lcd_clear();
            printf("poles done");
            __delay_ms(1000);
            UI_main(t_dep, poles_detected);
            backwards();
            lcd_clear();
            printf("backwards");
            __delay_ms(1000);
        }
        t_count = 5;
    }
//    UI_main(t_dep, poles_detected);
}
