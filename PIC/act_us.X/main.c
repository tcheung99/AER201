#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
//#include "RTC.h"
#include "I2C.h"
#include "lcd.h"
#include "actuators.h"
#include "ultrasonic.h"
//#include "timer.h"

int poles_detected=0;

void main(){
    int t_count = 0;
    int t_dep = 0;
    
    int steps2_adj=ultrasonic_main();
    
    __delay_ms(1000);
        int stack=1; 

    t_count = number_deploy(20, poles_detected); 

            if (t_count<=2){ //check 
                if (t_dep <8){
//                    lcd_clear();
//                    printf("bruh");
                    stack = 1;
//                                    brake();        

                    for(int i=0; i<(t_count); i++){
                        actuators_main(stack, steps2_adj); 
                        t_dep++;
                    }
                }
                if (t_dep >= 8){
                    stack = 2;
                    for(int i=0; i<(t_count); i++){
                        actuators_main(stack, steps2_adj); 
                        t_dep++;
                    }
                }
            }
}
