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
    sensorState = PORTDbits.RD0; 
    if (!sensorState){
        ir_break = true; 
    }
    else{
        ir_break = false; 
    }
    if (sensorState && !lastState){
        ir = true; 
    }
    if (!sensorState && lastState){
        ir = false; 
    }    
    lcd_clear();
    printf("sensorState, %d", sensorState);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);    
    printf("lastState, %d", lastState);
    lastState = sensorState; 
    return;
}
