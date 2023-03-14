#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <string.h> //strtok()
#include <stdlib.h> //atoi()
#include <ctype.h>  //isprint()

#include "usart.h"
#include "adxl.h"
#include "oled.h"
#include "main.h"
#include "timing.h"


//structure for funcs which are added to linked list
typedef struct termList
{
    char* name; //command name
    char* desc; //description of what a function do
    void (*funcPtr)(char **); //pointer to a function
    struct termList* next; //pointer to next element of the list
} terminalList;


//public function prototypes
void terminalCheckCommand(void);
void terminalListAddItem(terminalList* newItem);


#endif /* __TERMINAL_H__ */