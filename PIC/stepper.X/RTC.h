/* 
 * File: RTC.h     
 * Author: Trudie Cheung
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RTC_H
#define	RTC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <string.h>
#include "configBits.h"

unsigned char time[7] = {0}; //sec, min, hour, weekday, weekday(#), month, year

void initTime(unsigned char s, unsigned char m, unsigned char h,unsigned char w,
              unsigned char DD, unsigned char MM, unsigned char YY);
void loadRTC();
void getRTC(void);
void printRTC(void);

#endif	/* RTC_H */

