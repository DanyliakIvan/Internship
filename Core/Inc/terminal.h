#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <string.h> //strtok()
#include <stdlib.h> //atoi()
#include <ctype.h>  //isprint()

#include "usart.h"
#include "adxl.h"
#include "main.h"



//structure for funcs which are added to linked list
typedef struct list
{
    char* name; //command name
    char* desc; //description of what a function do
    void (*test_ptr)(char **); //pointer to a function
    struct list* next; //pointer to next element of the list
} terminalList;


//public function prototypes
void terminalCheckCommand(void);
void terminalListAddItem(terminalList* newItem);


#endif /* __TERMINAL_H__ */