#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "I2C.h"
#include "lcd.h"

//#define RS RD2
//#define EN RD3
//#define D4 RD4
//#define D5 RD5
//#define D6 RD6
//#define D7 RD7

int dist_final[3]; // distance array 
volatile int a[4];
volatile int b;

void ultrasonic_main();

#endif