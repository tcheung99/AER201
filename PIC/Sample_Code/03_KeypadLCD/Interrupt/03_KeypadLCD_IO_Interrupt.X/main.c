/**
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 * 
 * Created on July 18, 2016, 12:11 PM
 * 
 * Edited by Tyler Gamvrelis, summer 2017
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

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"

const char keys[] = "123A456B789C*0#D";
const char msg1[] = "Chocolate?";
const char msg2[] = "CHOCOLATE?!";

volatile bool key_was_pressed = false;

void main(void) {
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    // Enable RB1 (keypad data available) interrupt
    INT1IE = 1;
    
    // Initialize LCD
    initLCD();
    
    // Enable interrupts
    ei();
    
    // Main loop
    unsigned long tick = 0;
    const char* msg = msg1;
    while(1){
        if(key_was_pressed){
            key_was_pressed = false; // Clear the flag
            
            // Write key press data to bottom line of LCD
            unsigned char keypress = (PORTB & 0xF0) >> 4;
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            putch(keys[keypress]);
        }
        
        if(tick % 1000 == 0){
            lcd_home();
            printf("%s", msg);
            msg = (msg == msg1) ? msg2 : msg1; // Alternate the message
        }
        
        tick++;
        __delay_ms(1);
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
void __interrupt() interruptHandler(void){
    // Interrupt on change handler for RB1
    if(INT1IF){
        // Notice how we keep the interrupt processing very short by simply
        // setting a "flag" which the main program loop checks
        key_was_pressed = true;
        INT1IF = 0; // Clear interrupt flag bit to signify it's been handled
    }
}