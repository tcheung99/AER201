#ifndef I2C_MINE_H
#define	I2C_MINE_H

#include <xc.h> 
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"
#include "I2C.h"

const char keys[] = "123A456B789C*0#D";

void i2c_mine(void);

#endif	

