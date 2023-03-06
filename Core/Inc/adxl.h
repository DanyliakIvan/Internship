/*
	There're functions to work with accelerometer adxl345
*/

#ifndef __ADXL_H__
#define __ADXL_H__

#include "spi.h"
#include "oled.h"
#include <stdio.h>

/* Prototypes */
void adxlInit(void);
void adxlWrite(uint8_t address, uint8_t value);
void adxlRead(uint8_t* adxlData);
void adxlReadAndOutToPC(char ** tokensArr);
void adxlReadAndOutOnDisplay(char ** tokensArr);

#endif /* __ADXL_H__ */