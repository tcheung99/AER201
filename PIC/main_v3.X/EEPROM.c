/*
 * File:   EEPROM.c
 * Author: trudi
 *
 * Created on April 8, 2019, 1:19 PM
 */

//#include <xc.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include "configBits.h"
//#include "lcd.h"
//
//#pragma stack 0x300 0xff // set 64 byte stack at 0x300, needed by sdcc

#include "EEPROM.h"

void ee_write_byte(unsigned char address, unsigned char *_data){
    EEDATA = *_data;
    EEADR = address;
    // start write sequence as described in datasheet, page 91
    EECON1bits.EEPGD = 0; //point to data memory--access data EEPROM memory 
    EECON1bits.CFGS = 0; //access data EEPROM memory 
    EECON1bits.WREN = 1; // enable writes to data EEPROM
    INTCONbits.GIE = 0;  // disable interrupts
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    EECON1bits.WR = 1;   // start writing
//    while(EECON1bits.WR){
//        #asm 
//        __asm
//                
//        nop 
//        __endasm
//        #endasm} //does nothing (like a delay?)
//    if(EECON1bits.WRERR){
//        printf("ERROR");
//    }
    do {
      ClrWdt();
      } while(EECON1bits.WR);   
    EECON1bits.WREN = 0; //disable writing 
    INTCONbits.GIE = 1;  // enable interrupts
}

void ee_read_byte(unsigned char address, unsigned char *_data){
    EEADR = address;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    *_data = EEDATA;
}
//
//void initUsart()
//{
//    usart_open(    // Use USART library to initialise the hardware
//            USART_TX_INT_OFF
//            & USART_RX_INT_OFF
//            & USART_BRGH_HIGH
//            & USART_ASYNCH_MODE
//            & USART_EIGHT_BIT,
//            10                      // '10' = 115200 Baud with 20 MHz oscillator and BRGH=1
//            );
//    stdout = STREAM_USART;
//}

//void main(){
//    char save_me = 'x';
//    char from_eeprom;
//    int help = 65; 
//    initLCD();
//    TRISD = 0x00;
//
////    initUsart();
//    lcd_clear();
//    printf("EEPROM");
//    ee_read_byte(0x00, &from_eeprom);
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    printf("Char read from 0x00: %c", from_eeprom);
//    __delay_ms(1000);
//
//    lcd_clear();
//    printf("EEPROM");    
//    ee_write_byte(0x00, &save_me);
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    printf("Char written to 0x00: %c", save_me);
////    __delay_ms(500);
//    __delay_ms(1000);
//
//    lcd_clear();
//    printf("EEPROM");    
//    ee_read_byte(0x00, &from_eeprom);
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    printf("Char read from 0x00: %c", from_eeprom);
////    __delay_ms(500);
//    __delay_ms(1000);
//    
//    
//    lcd_clear();
//    printf("EEPROM");
//    ee_read_byte(0x01, &from_eeprom);
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    printf("Char read from 0x01: %c", from_eeprom);
////    __delay_ms(500);
//    __delay_ms(1000);
//    
//    lcd_clear();
//    printf("EEPROM");    
//    ee_write_byte(0x01, &help);
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    printf("int written to 0x01: %d", help);
////    __delay_ms(500);
//    __delay_ms(1000);
//
//    lcd_clear();
//    printf("EEPROM");    
//    ee_read_byte(0x01, &from_eeprom);
//    lcd_set_ddram_addr(LCD_LINE2_ADDR);
//    printf("int read from 0x01: %d", from_eeprom);
////    __delay_ms(500);
//    __delay_ms(1000);
//}
