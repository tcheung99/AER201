/*
 * File:   main.c
 * Author: trudi
 *
 * Created on April 6, 2019, 10:49 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "RTC.h"
#include "I2C.h"
//#include "i2c_mine.h"

void main(void) {
    TRISD = 0b00000001; //set data direction as input 
    LATD = 0b00000000; 
    int sensorState = 0;
    int lastState=0;
    bool ir_break = false;
    bool ir = false;
    initLCD();
    while(1){
        sensorState = PORTDbits.RD0; 
        if (!sensorState){
            ir_break = true;  
        }
        else{
            ir_break = false; 
        }
        if (sensorState && !lastState){ //ir starting (beg)
            ir = true; 
            I2C_Master_Start(); // Start condition
            I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
            I2C_Master_Write('3'); // Write key press data which shows up on Arduino's serial monitor
            I2C_Master_Stop();
        if (!sensorState && lastState){ //ir ending 
            ir = false; 
            I2C_Master_Start(); // Start condition
            I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
            I2C_Master_Write('4'); // Write key press data which shows up on Arduino's serial monitor
            I2C_Master_Stop(); 
        }    
        lcd_clear();
        printf("sensorState, %d", sensorState);
        lcd_set_ddram_addr(LCD_LINE2_ADDR);    
        printf("lastState, %d", lastState);
        lastState = sensorState; 
    }
    return;
}
