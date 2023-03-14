/*
	There're functions to work with accelerometer adxl345
*/

#ifndef __ADXL_H__
#define __ADXL_H__

#include <stdio.h>

#include "spi.h"
#include "oled.h"
#include "terminal.h"

//struct for x y z
typedef struct 
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
} positionOfAdxl;

/* Prototypes */
void adxlInit(void);
void adxlWrite(uint8_t address, uint8_t value);
void adxlRead(uint8_t* adxlData);
void adxlReadAndOutToPC(char ** tokensArr);
void adxlReadAndOutOnDisplay(char ** tokensArr);
void adxlTerminalInit(void);

#endif /* __ADXL_H__ */