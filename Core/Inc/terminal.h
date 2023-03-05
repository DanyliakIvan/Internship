#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

#include "usart.h"

//prototypes
void checkCommand(void);
void bufIntoTokens(char* buf);
void checkForCommand(char ** tokensArr);

void createListOfFuncs();
void listAddElement(char* name, void (*ptrToFunc)(int8_t, int8_t));
	

void add(int8_t a, int8_t b);
void sub(int8_t a, int8_t b);
void mul(int8_t a, int8_t b);
void divv(int8_t a, int8_t b);

typedef struct list
{
	char* name; //command name
	void (*test_ptr)(int8_t, int8_t);// pointer to a function
	struct list* next; // poinet to next element of the list
} list_t;


typedef enum {false, true} bool;



#endif /* __TERMINAL_H__ */