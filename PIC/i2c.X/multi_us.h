#ifndef MULTI_US_H
#define	MULTI_US_H

#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "I2C.h"
#include "lcd.h"

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

int dist_final[3]; // distance array 
int a[4];
int b;

void __interrupt() echo();
void us_main();

#endif	/* XC_HEADER_TEMPLATE_H */

