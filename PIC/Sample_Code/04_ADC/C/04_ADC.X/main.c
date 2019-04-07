/**
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 *
 * Created on July 18, 2016, 12:11 PM
 *
 * @defgroup ADC
 * @brief Displays ADC readings, which can be varied using the potentiometers
 *        in the ADC module, on the character LCD
 * 
 * Preconditions:
 * @pre Jumpers JP5 and JP4 are shorted
 * @pre Character LCD is in a PIC socket
 */

#include <xc.h>
#include "configBits.h"
#include "lcd.h"

/**
 * @brief Samples the analog input from the specified analog channel
 * @param channel The byte corresponding to the channel to read
 * @return The 10-bit value corresponding to the voltage sampled from the
 *         specified channel
 */
unsigned short readADC(char channel){
    ADCON0 = (channel & 0x0F) << 2; // Select ADC channel (i.e. pin)
    ADON = 1; // Enable module
    ADCON0bits.GO = 1; // Initiate sampling
    while(ADCON0bits.GO_NOT_DONE){
        continue; // Poll for acquisition completion
    }
    return (ADRESH << 8) | ADRESL; // Return result as a 16-bit value
}

void main(void) {
    // Use 8 MHz internal oscillator block
    OSCCON = 0xF2;
    
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // RA2 and RA3 are connected to the potentiometers in the A2D module. We
    // want these to be inputs so we can read the voltage on them (all pins are
    // inputs by default, but we show this step explicitly for learning)
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 1;
    
    // Set RA0-3 to analog mode (pg. 222)
    ADCON1 = 0x0B;
    
    // Right justify A/D result
    ADCON2bits.ADFM = 1;
    
    initLCD();

    // Print results to character LCD. Since ADC results are 10 bits, they are
    // split up into two registers ADRESH and ADRESL. The results of the ADC 
    // are displayed in hexadecimal so that the values in each of these
    // registers will be easily visible; ADRESL corresponds to the two least
    // significant digits, while ADRESH corresponds to the most significant 
    // bits
    while(1) {
        lcd_clear();
        printf("RA2: %.3x", readADC(2));
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("RA3: %.3x", readADC(3));

        __delay_ms(100);
    }
}