/**
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 *
 * Created on July 18, 2016, 12:11 PM
 * 
 * @defgroup PIC_I2C_Arduino
 * @brief Demonstrates communication between the primary PIC and the Arduino
 *        Nano, via I2C. To see the results on the Arduino Nano side, open the 
 *        Arduino sample program Arduino_I2C_PIC. If the PIC receives a triple-A
 *        sequence from the keypad, it changes from being a transmitter to being
 *        a receiver that displays data on the character LCD. To change it back,
 *        reset the PIC
 * 
 * Preconditions:
 * @pre PIC-Arduino link switches are enabled (ON) for A4 and A5 of the Arduino
 *      Nano (RC4 and RC3 for the primary PIC, respectively)
 * @pre PIC and Arduino Nano agree on the Arduino Nano's slave address (i.e. the
 *      same slave address is used in software)
 * @pre A PC is connected if the Arduino Nano serial monitor is to be used. Note
 *      that the serial monitor baud rate must be 9600 for this program
 */

#include <xc.h>
#include <stdbool.h>
#include <configBits.h>
#include "lcd.h"
#include "I2C.h"

const char keys[] = "123A456B789C*0#D";

void main(void) {
    // RB1, RB4, RB5, RB6, RB7 as inputs (for keypad)
    LATB = 0x00;
    TRISB = 0b11110010;
    
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    initLCD();  

    // Write the address of the slave device, that is, the Arduino Nano. Note
    // that the Arduino Nano must be configured to be running as a slave with
    // the same address given here. Note that other addresses can be used if
    // desired, as long as the change is reflected on both the PIC and Arduino
    // ends
    I2C_Master_Init(100000); 
    I2C_Master_Start();
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Stop();
    
    // Main loop
    unsigned char mem[3]; // Initialize array to check for triple-A sequence
    unsigned char counter = 0; // Increments each time a byte is sent
    unsigned char keypress; // Stores the data corresponding to the last key press
    unsigned char data; // Holds the data to be sent/received
    bool send = true;
    while(1) {
        if(send){
            while (PORTBbits.RB1 == 0){
                continue;
            }
            
            keypress = (PORTB & 0xF0) >> 4;
            
            while (PORTBbits.RB1 == 1){
                continue;
            }
            
            data = keys[keypress];
            
            I2C_Master_Start(); // Start condition
            I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
            I2C_Master_Write(data); // Write key press data
            I2C_Master_Stop();

            // Check for a triple-A sequence. If this sequence occurs, switch 
            // the PIC to receiver mode. To switch back to transmitter mode,
            // reset the PIC
            mem[counter] = data;
            counter++;
            counter = (counter == 3) ? 0 : counter;
            if((mem[0] == 'A') && (mem[1] == 'A') && (mem[2] == 'A')){
                send = false;
            }
        }
        else{
            // Receive data from Arduino and display it on the LCD
            I2C_Master_Start();
            I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
            data = I2C_Master_Read(NACK); // Read one char only
            I2C_Master_Stop();
            if(data){
                putch(data);
            }
        }
    }
}