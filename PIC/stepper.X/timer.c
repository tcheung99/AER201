#include "timer.h"

int timer_count = 0;
int cnt =0;

//END DEFINE
//REVIEW DBL CHECK
//void CLK_INIT( void ){
//    OSCCONbits.IRCF = 0b100; //1 MHz
//}
void TIMER_INIT( void ){
    TRISD = 0x00;                //Define output or Input
    LATD = 0x00;
    T0CONbits.TMR0ON  = 0;      //stop TIMER0 counter
    T0CONbits.T08BIT  = 0;      //select 16-bit mode
    T0CONbits.T0CS    = 0;      //select the internal clock to drive timer0
    T0CONbits.PSA     = 0;      //use prescaler for TIMER0
    T0CONbits.T0PS    = 0b101;  //assign the 1:64 pre-scaler to TIMER0 
    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
    T0CONbits.TMR0ON  = 1;      //enable TIMER0
    INTCONbits.TMR0IE = 1;      //Enable the TIMER0 overflow interrupt
    //INTERUPT ENABLE
    INTCONbits.GIE    = 1;      //GLOBAL INTERUPT ENABLE
    INTCONbits.PEIE   = 1;      //PERIPHERAL INTERUPT ENABLE; TIMER0 = PERIPHERAL
}

void timer_main( void ){
    TRISD = 0x00;                //Define output or Input
    LATD = 0x00;
    initLCD(); 

//    CLK_INIT();
    TIMER_INIT();
    timer_count++;
//    while (1){
//    }
}
//INTERRUPT ISR

void __interrupt() SW_ISR( void ){    //WEIRD SYNTAX BECAUSE OF PIC
    INTCONbits.TMR0IF = 0;      //CLEAR FLAG SO IT CAN BE TRIGGERED AGAIN
    TMR0H = TIMER0_COUNTS_PER_HALF_SECOND >> 8;
    TMR0L = TIMER0_COUNTS_PER_HALF_SECOND;
    cnt++;
    LEDPin ^= 1u;           //Toggle LED
    lcd_clear();
    printf("time");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    if (cnt%2 == 0){
        printf("%d",cnt/2);
    }

}