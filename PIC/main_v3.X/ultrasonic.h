#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "I2C.h"
#include "lcd.h"

#define us_delay 150 

int dist_final[4][6] = { 
    {0, 0, 0, 0, 0,0}, // dist_final[0] 
    {0, 0, 0, 0, 0,0}, // dist_final[1] 
    {0, 0, 0, 0, 0,0}, // dist_final[2] 
    {0, 0, 0, 0, 0,0}, // dist_final[3] 
};

int a[4];
int sum[4];
int b;
bool sumf[4] = {false,false,false,false};
volatile int meas =0;

volatile bool echoo;
void echo();
void trig();
void print_echo();
int number_deploy(int avg_dist, poles_detected);
int ultrasonic_main();
#endif