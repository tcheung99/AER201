///*
// * File:   main.c
// * Author: TrudieC
// *
// * Created on January 11, 2019, 3:17 PM
// */
//
//// PIC18F4620 Configuration Bit Settings
//
//// 'C' source line config statements
//
//// CONFIG1H
//#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
//#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
//
//// CONFIG2L
//#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
//#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
//#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)
//
//// CONFIG2H
//#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
//#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
//
//// CONFIG3H
//#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
//#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
//#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
//#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
//
//// CONFIG4L
//#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
//#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
//#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
//
//// CONFIG5L
//#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
//#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
//#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
//#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)
//
//// CONFIG5H
//#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
//#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)
//
//// CONFIG6L
//#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
//#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
//#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
//#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)
//
//// CONFIG6H
//#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
//#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
//#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
//
//// CONFIG7L
//#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
//#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
//#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
//#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
//
//// CONFIG7H
//#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
//
//// #pragma config statements should precede project file includes.
//// Use project enums instead of #define for ON and OFF.
//
//#define _XTAL_FREQ 40000000

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
//#include "RTC.h"
//#include "I2C.h"
//#include "timer.h"
//int min(int *array, int size){
//
//    int minimum  = array[0];
//    for (int i = 0; i<size; i++){
//        if (minimum>=array[i]){
//            lcd_clear(); 
//            printf("minimum1 %d", minimum);
//            __delay_ms(1000);
//            minimum = array[i]; 
//            lcd_clear(); 
//            printf("minimum2 %d", minimum);
//            __delay_ms(1000);
//        }
//    }
//    return minimum;
//}



void main(void) {
        LATD = 0x00;
    TRISD = 0x00;
            initLCD();

//                lcd_clear(); 
//        printf("garfsa");
//        __delay_ms(1000);
//    int a[6] = {23, 55, 22, 3, 45, 18};
////    int a[2] = {23, 55};
//    int minimum = min(a, 6);
//        lcd_clear(); 
//        printf("minimum %d", minimum);
//        __delay_ms(1000);
//        return;
    
//    TRISBbits.RB1 = 0;
//    LATBbits.LATB1 = 1;
//    TRISEbits.RE0 = 0;
//    LATEbits.LATE0 = 1; //DISABLES KPD
//    TRISC = 0;
//    LATC = 0;
//    TRISC = 0x00;
//    LATC = 0x00;
//    TRISB = 0x00;
//    LATB = 0b10000000;
//    TRISA = 0x00;
//    LATA = 0x00;    
////    TRISCbits.RC7 = 0;
////    LATAbits.LATA5 = 1; 
////    TRISCbits.RC4 = 0;
////    LATCbits.LATC4 = 0;
//    TRISD = 0x00;
//    LATD = 0b00000010;
    
//
//    initLCD();
//    lcd_clear(); 
//printf("fhiad");    
//            __delay_ms(1000);
//    TRISAbits.RA5 = 0;

    TRISAbits.RA4 = 0;
//        PORTAbits.RA4 = LATAbits.LATA4;
//        PORTAbits.RA4 = LATAbits.LATA5;
//                printf(PORTAbits.RA4)
                    PORTAbits.RA4 = LATAbits.LATA4;

    if (!PORTAbits.RA4){
    lcd_clear(); 
        printf("fuck");
    }
    if (PORTAbits.RA4){
        lcd_clear(); 
        printf("hep");
//        __delay_ms(200);
           } 

            
//        if (PORTAbits.RA4 == 1){
//        lcd_clear(); 
//        printf("hep");
//        __delay_ms(1000);
//            }
//    TRISBbits.RB1 = 1; //by default all pins are inputs anyways though 
//    TRISCbits.RC0 = 0;  
//    while(1){
//        while(!PORTBbits.RB1){continue;}
//        LATCbits.LATC0 = ~LATCbits.LATC0;
//        while(PORTBbits.RB1){continue;}  
//    }
//    return; 
}
//interrupt or no interrupt -- interrupt allows your program to do other stuff, polling is simple but blocks program from doing anything else
//RB1 will go high on keypad presses -- kind of counts up in binary 
//
//    TRISC = 0b00000010; //set RC0 to output 
//    LATC = 0b11111101;
//    while(1){
//        __delay_ms(500);
//        LATC = ~LATC; 
//    }
//}
//    INTCONbits.GIE = 1;  //interrupts no longer masked 
//    INTCON3bits.INT1IE = 1; //enable specific interrupt for INT1 (for keypad)
//    TRISBbits.RB1 = 1; //by default all pins are inputs anyways though 
//    TRISCbits.RC0 = 0; 
//    while(1);
//}
//void __interrupt() interrupthandler(void){ 
//        if (INT1IE && INT1IF){ //technically only need IF for flags 
//            INT1IF = 0; //clear interrupt, to make sure you catch interrupts if they occur during the running of this part
//            LATCbits.LATC0 = ~LATCbits.LATC0; //compiler generates its own return for interrupts 
//        }
//    }    
    
    