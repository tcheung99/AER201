/**
 * @file
 * @author Tyler Gamvrelis
 *
 * Created on July 10, 2017, 10:27 AM
 * 
 * @defgroup PC_Demo
 * @brief Sends key press data to a PC connected to the co-processor, while
 *        also displaying data received from the PC on the character LCD
 * 
 * Preconditions:
 * @pre Co-processor running the virtual COM port program
 * @pre The character LCD is in a PIC socket
 * @pre JP_TX and JP_RX in the USB module are shorted
 * @pre USB port in co-processor module is plugged into a PC
 * @pre Co-processor has enumerated correctly to host PC (i.e. is recognized
 *      as a virtual COM port by the host PC)
 */

#include <xc.h>
#include <configBits.h>
#include "lcd.h"

const char keys[] = "123A456B789C*0#D";

void main(void) {
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;

    // <editor-fold defaultstate="collapsed" desc="Initialize EUSART module for asynchronous 9600/8N1">
    // Configure the baud rate generator for 9600 bits per second
    long baudRate = 9600;
    SPBRG = (unsigned char)((_XTAL_FREQ / (64 * baudRate)) - 1);
    
    // Configure transmit control register
    TXSTAbits.TX9 = 0; // Use 8-bit transmission (8 data bits, no parity bit)
    TXSTAbits.SYNC = 0; // Asynchronous communication
    TXSTAbits.TXEN = 1; // Enable transmitter
    __delay_ms(5); // Enabling the transmitter requires a few CPU cycles for stability
    
    // Configure receive control register
    RCSTAbits.RX9 = 0; // Use 8-bit reception (8 data bits, no parity bit)
    RCSTAbits.CREN = 1; // Enable receiver
    
    // Enforce correct pin configuration for relevant TRISCx
    TRISCbits.TRISC6 = 0; // TX = output
    TRISCbits.TRISC7 = 1; // RX = input
   
    // Enable serial peripheral
    RCSTAbits.SPEN = 1;
    // </editor-fold>
    
    initLCD();

    unsigned char receivedByte; // For receiving characters from the PC
    while(1){
        // RB1 is the interrupt pin, so if there is no key pressed, RB1 will be
        // 0. RCIF is the interrupt flag for UART reception. Even without UART
        // receive interrupts enabled, this flag will still be set when a byte
        // is received and the UART module is running. The loop below forces the
        // PIC to do nothing until either a key has been pressed, or information
        // has been received from the PC (i.e. there is a UART reception)
        while(!(PORTBbits.RB1 || RCIF)){
            continue;
        }

        /*************** CASE 1: CHARACTER RECEIVED FROM PC *******************/
        // Check we exited the while loop because of a UART reception
        if(RCIF){
            // Byte received via UART. First, read the receive register, RCREG, 
            // into memory
            receivedByte = RCREG;

            // Check/clear overrun error (next byte arrives before previous
            // was read). This error sometimes arises for the first reception,
            // but usually doesn't happen otherwise
            if(RCSTAbits.OERR){
                // Receive buffer flushed in hardware when receiver is reset 
                RCSTAbits.CREN = 0;
                RCSTAbits.CREN = 1;
            }
            
            // Print the character received to the character LCD
            putch(receivedByte);
        }
        
        /**************** CASE 2: KEY PRESS DETECTED ON KEYPAD ****************/
        // If we did not receive a byte via the UART, then we exited the while
        // loop due to a key press. In this case, read the 4-bit character 
        // code
        if(PORTBbits.RB1){
            unsigned char keypress = (PORTB & 0xF0) >> 4;

            // Wait until the key has been released
            while(PORTBbits.RB1 == 1){
                continue;
            }

            // Wait until the previous TXREG data has completed its transfer 
            // into the TSR and the TSR has finished transmitting all bits 
            // before loading the TXREG register
            while(!TXIF | !TRMT){
                continue;
            }

            // Load data into the transmit register, TXREG
            TXREG =  keys[keypress];
        }
    }
}