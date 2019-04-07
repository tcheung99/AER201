/**
 * @file
 * @author Tyler Gamvrelis
 * 
 * Created on August 12, 2017, 5:40 PM
 * 
 * @defgroup CharacterLCD_2
 * @brief Demonstrates more character LCD capabilities such as display shifting
 *        and moving the cursor to a specific DDRAM address
 * 
 * Precondition:
 * @pre Character LCD is in a PIC socket
 */

#include <xc.h>
#include <math.h>
#include "configBits.h"
#include "lcd.h"

void main(void) {
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Initialize LCD
    initLCD();

    // Demonstrate printing a string literal
    printf("Hello world!");
    __delay_ms(2000);
    
    // Main loop
    while(1){
        // Demonstrate writing on different lines
        lcd_clear();
        printf("Text can be on");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("multiple lines");
        __delay_ms(2000);
        
        // Demonstrate placeholders
        lcd_clear();
        printf("Integer: %d", 10);
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("Float: %f", M_PI);
        __delay_ms(2000);
        
        lcd_clear();
        printf("Long: %lu", 4291234567);
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        char arr[] = "AER201";
        printf("Char[]: %s", arr);
        __delay_ms(2000);
        
        // Demonstrate cursor blink on/off
        lcd_clear();
        printf("Cursor blink can");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("be toggled");
        __delay_ms(2000);
        
        lcd_display_control(true, true, false);
        __delay_ms(2000);
        
        // Demonstrate cursor on/off
        lcd_clear();
        printf("As can the");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("cursor!");
        __delay_ms(2000);

        lcd_display_control(true, false, false);
        __delay_ms(2000);
        
        // Demonstrate display on/off
        lcd_clear();
        printf("As can the");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("display");
        __delay_ms(2000);
        
        lcd_display_control(false, false, false);
        __delay_ms(2000);
        
        lcd_clear();
        printf("But that will");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("likely stay on!");
        lcd_display_control(true, false, false);
        __delay_ms(2000);

        // Demonstrate set cursor position function
        lcd_clear();
        printf("Cursor position");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("can be specified");
        __delay_ms(2000);
        
        lcd_clear();
        for(unsigned char x = 0; x < LCD_SIZE_HORZ; x++){
            if(x % 2){
                lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_SIZE_HORZ - x);
                printf("%d", x % 10);
            }
            else{
                lcd_set_ddram_addr(LCD_LINE2_ADDR + x);
                printf("%d", x % 10);
            }
            __delay_ms(100);
        }
        
        // Demonstrate display shifting
        lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_SIZE_HORZ + 1);
        printf("Display can be");
        lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_SIZE_HORZ + 4);
        printf("shifted");
        __delay_ms(2000);

        for(unsigned char i = 0; i < LCD_SIZE_HORZ; i++){
            lcd_shift_display(1, LCD_SHIFT_LEFT);
            __delay_ms(150);
        }
        __delay_ms(2000);
        
        // Clear bottom row of display characters
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        for(unsigned char i = 0; i < LCD_SIZE_HORZ; i++){
            putch(' ');
        }
        
        // Write text offscreen (area we shifted away from) and shift back from
        // it
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        printf("Hello world!    ");
        for(unsigned char i = 0; i < LCD_SIZE_HORZ; i++){
            lcd_shift_display(1, LCD_SHIFT_RIGHT);
            __delay_ms(150);
        }
        lcd_display_control(true, true, true); // Turn on cursor, blink, and display
        __delay_ms(2000);
    }
}