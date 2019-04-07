/**
 * @file
 * @author Trudie Cheung 
 * 
 * Created on February 2, 2019, 1:43 PM
 * 
 * 
 * @defgroup KeypadLCD_IO_Interrupt
 * @brief Demonstrates of interrupt on change feature of RB1. The main loop
 *        changes the characters displayed on the top line of the LCD, while the
 *        interrupt handler displays key press data on the bottom line
 * 
 * Preconditions:
 * @pre Character LCD in a PIC socket
 * @pre Co-processor is running default keypad encoder program
 */
//int poles[10]
//struct Pole{
//int t_deployed
//int t_final 
//int distance;
//}

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
#include "i2c_mine.h"

//const char keys[] = "123A456B789C*0#D";

unsigned char temp;
short int temp_int, tick;
volatile bool key_was_pressed = false;
int Pole[10]; //declare pole array 
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

typedef struct Poles{
	int tires_deployed; //check var
	int tires_final;//check var
	int dist_from_start;//check var
	} Poles;
	
void main(void) {
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
    if(pressed == 0){
        lcd_clear();           
        printf("1 - Start");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("2 - Summary");
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
        printf("3 - Date&Time ");
        }
    while(1){
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
                
                //Start I2C, writes 1 to the Arduino's serial monitor
                I2C_Master_Start(); // Start condition
                I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
                I2C_Master_Write(temp); // Write key press data which shows up on Arduino's serial monitor
                I2C_Master_Stop();
                //Turn PIC into receiver mode 
//                send = false;
                }
            
                if ((temp_int == 2)||(state == 2)){
                    if ((temp_int == 2)){	 //this loop is never traversed.. 
                        state = 2; 
                        disp = 0;
                        lcd_clear();
                        printf("Op Time:");
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("2min5sec");
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("    0-Menu   #>");
//                I2C_Master_Start(); // Start condition
//                I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
//                I2C_Master_Write(temp); // Write key press data which shows up on Arduino's serial monitor
//                I2C_Master_Stop();
                        //__delay_ms(1500);
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
                        printf("2min5sec");
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("     0-Menu   #>");
                        //__delay_ms(1500);
                    }
                    if (disp == 1){
                        lcd_clear();
                        printf("No. Tires: 3");
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("No. Poles: 2");
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("<*   0-Menu   #>");                            
                        //__delay_ms(1500);
                        }
                    if (disp > 1 && disp <11){
                        i = disp - 2;
                        lcd_clear();
                        printf("P%d Dep: %d", i+1 , Pole[i].tires_deployed);
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("P%d Fin: %d",i+1 , Pole[i].tires_final);
                        lcd_set_ddram_addr(LCD_LINE3_ADDR);
                        printf("P%d Dist: %d",i+1 , Pole[i].dist_from_start);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("<*   0-Menu   #>");      
                    }
                    if (disp == 11){
                        i = disp - 2;
                        lcd_clear();
                        printf("P%d Dep: %d", i+1 , Pole[i].tires_deployed);
                        lcd_set_ddram_addr(LCD_LINE2_ADDR);
                        printf("P%d Fin: %d",i+1 , Pole[i].tires_final);
                        lcd_set_ddram_addr(LCD_LINE3_ADDR);
                        printf("P%d Dist: %d",i+1 , Pole[i].dist_from_start);
                        lcd_set_ddram_addr(LCD_LINE4_ADDR);
                        printf("<*   0-Menu     "); 
                            //__delay_ms(1500);
//                            if (i == 10){ //cant scroll to screens with no pole info 
//                                state = 0;
//                            }
					//}
				}
				
			}
		
	

//            if (temp_int == 3){
//                lcd_clear();
//                printf("Machine In Use");
//                __delay_ms(1000);
//                lcd_clear();
//                }

// time should tick for 5 seconds 
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

/**
 * @brief Any time an interrupt is generated, the microcontroller will execute
 *        this function (as long as interrupts are enabled). Any interrupts
 *        which are enabled need to be handled in this function, otherwise
 *        unexpected behavior will arise, perhaps even causing the PIC to reset
 *        (you AT LEAST need to clear the flag for each interrupt which is
 *        enabled!)
 */
void __interrupt () interruptHandler(void){
    // Interrupt on change handler for RB1
    if(INT1IF){
        // Notice how we keep the interrupt processing very short by simply
        // setting a "flag" which the main program loop checks
        key_was_pressed = true;
        INT1IF = 0; // Clear interrupt flag bit to signify it's been handled
    }
}