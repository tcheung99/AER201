/**
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 * 
 * Created on July 18, 2016, 12:11 PM
 * 
 * @defgroup RTC
 * @brief Sets the real-time clock (RTC) and reads back the time once per
 *        second
 * 
 * Preconditions:
 * @pre Jumpers JP6, and JP7 in the RTC module are shorted
 * @pre RTC chip is properly situated in its socket
 * @pre CR2032 20 mm coin battery is properly situated in its socket and is not
 *      low on charge
 */

#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "I2C.h"
#include "lcd.h"

const char keys[] = "123A456B789C*0#D";
const char happynewyear[7] = {
    0x00, // 45 Seconds 
    0x25, // 59 Minutes
    0x15, // 24 hour mode, set to 23:00
    0x00, // Sunday
    0x05, // 31st
    0x02, // December
    0x19  // 2018
};

void rtc_set_time(void);

void RTC_main(void) {
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    initLCD();

    // Initialize I2C Master with 100 kHz clock
    I2C_Master_Init(100000);
    
    // Set the time in the RTC. To see the RTC keep time, comment this line out
    // after programming the PIC directly before with this line included
    rtc_set_time();
    
    // Main loop
    unsigned char time[7]; // Create a byte array to hold time read from RTC
    while(1){
        // Reset RTC memory pointer
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
        lcd_home();
        printf("%02x/%02x/%02x", time[6],time[5],time[4]); // Print date in YY/MM/DD
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS
        __delay_ms(1000);
    }
}

/** @brief Writes the happynewyear array to the RTC memory */
void rtc_set_time(void){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    // Write array
    for(char i=0; i < 7; i++){
        I2C_Master_Write(happynewyear[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}