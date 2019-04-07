/**
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 * 
 * Created on July 18, 2016, 12:11 PM
 * 
 * @defgroup KeypadLCD_IO_Polling
 * @brief Receives keypress data from co-processor and displays the
 *        corresponding characters on the character LCD
 * 
 * Preconditions:
 * @pre Co-processor running default program
 * @pre Character LCD in PIC socket
 */

#include <xc.h>
#include "configBits.h"
#include "lcd.h"

const char keys[] = "123A456B789C*0#D"; 

void main(void){
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    // Initialize LCD
    initLCD();

    // Main loop
    while(1){
        // RB1 is the interrupt pin, so if there is no key pressed, RB1 will be
        // 0. The PIC will wait and do nothing until a key press is signaled
        while(PORTBbits.RB1 == 0){  continue;   }
        
        // Read the 4-bit character code
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        
        // Wait until the key has been released
        while(PORTBbits.RB1 == 1){  continue;   }
        
        Nop(); // Apply breakpoint here to prevent compiler optimizations
        
        unsigned char temp = keys[keypress];
        putch(temp); // Push the character to be displayed on the LCD
    }
}
