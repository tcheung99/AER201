/**
 * @file
 * @author Tyler Gamvrelis
 * 
 * Created on August 29, 2017, 4:29 PM
 * 
 * @defgroup KeypadLCD_UART
 * @brief Demonstrates receiving key press data via UART bus
 * 
 * Preconditions:
 * @pre Co-processor running UART keypad encoder (key 2)
 * @pre Character LCD is in a PIC socket
 * @pre JP_TX and JP_RX in USB module are shorted
 */

#include <xc.h>
#include "configBits.h"
#include "lcd.h"

typedef enum{
    KPD_IDLE,    /**< Nothing has been received from the co-processor  */
    KPD_PRESSED, /**< A key on the keypad is currently being held down */
    KPD_RELEASED /**< A key on the keypad has stopped being pressed    */
}keypad_states_e;

const char keys[] = "123A456B789C*0#D"; 

/**
 * @brief Check/clear overrun error (next byte arrives before previous was
 *        read). We should check for overrun errors each time we receive a byte
 *        because if they happen, we need to clear them for the module to
 *        function correctly again
 */
inline void check_overrun(void){
    if(RCSTAbits.OERR){
        // Receive buffer flushed in hardware when receiver is reset 
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
}

void main(void){
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
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
    
    // Main loop
    unsigned char keyData;
    keypad_states_e current_state = KPD_IDLE;
    keypad_states_e next_state = KPD_IDLE;
    while(1){
        // Take action based on current state
        switch(current_state){
            case KPD_IDLE:
                // Wait to receive key press data from Co-processor via UART
                if(RCIF){
                    // Byte received via UART. First, read the receive register,
                    // RCREG, into memory. Here we are saving the key data
                    keyData = RCREG;
                    check_overrun();
                    next_state = KPD_PRESSED;
                }
                break;
            case KPD_PRESSED:
                // Wait for "end of key press" code
                if(RCIF){
                    unsigned char receivedByte = RCREG;
                    check_overrun();
                    
                    // The convention the Co-processor uses is that 0xF0
                    // signifies the end of a key press, so we check for this
                    // here
                    if(receivedByte == 0xF0){
                        next_state = KPD_RELEASED;
                    }
                }
                break;
            case KPD_RELEASED:
                // Take action now that the key has been released...in this
                // case, display the character corresponding to the key press on
                // the LCD
                putch(keys[keyData]);
                next_state = KPD_IDLE;
                break;
            default:
                next_state = KPD_IDLE;
                break;
        }
        
        // Update state
        current_state = next_state; 
    }
}