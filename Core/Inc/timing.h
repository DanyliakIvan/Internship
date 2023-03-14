#ifndef __TIMING_H__
#define __TIMING_H__

#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <ctype.h>

#include "main.h"
#include "adxl.h"


//structure for funcs which are added to linked list
typedef struct timList
{
    uint16_t timeout; //period of time after which a function will be called one more time
    volatile uint16_t counter; //will be incresing, checking and compare with timeout
    uint32_t id; //memory address is used as an ID
    void (*funcPtr)(char **);//pointer to a function
    struct timList* next; //pointer to next element of the list
} timingList;


//public function prototypes
void timingInit(void);
void timingCheckPeriods(void);
void timingAddItem(timingList* newItem);
void timingDeleteItem(uint32_t id);
void timingDelay(uint32_t delay);



#endif /* __TIMING_H__ */