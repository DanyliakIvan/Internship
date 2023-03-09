#ifndef __TIMING_H__
#define __TIMING_H__

//#include <string.h>
//#include <stdlib.h>
//#include <ctype.h>

#include "main.h"



//structure for funcs which are added to linked list
typedef struct list
{
    uint8_t timeout; //period of time after which a function will be called one more time
    uint8_t counter; //
    void (*test_ptr)(char **);//pointer to a function
    struct list* next; //pointer to next element of the list
} timingList;


#endif /* __TIMING_H__ */