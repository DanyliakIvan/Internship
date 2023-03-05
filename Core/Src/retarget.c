#include <stdio.h>
#include "usart.h"

/*! Standart output file stream */
FILE __stdout;
/*! Standart input file stream */
FILE __stdin; 

uint8_t CARRIAGE_RETURN = 0x0D;

int fputc(int ch, FILE *OStream)
{
	(void)OStream;
	
	while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY){};
	if (ch == '\n')
	{
		HAL_UART_Transmit(&huart2, &CARRIAGE_RETURN, 1, HAL_MAX_DELAY);
	}
	
	while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY){};
	
	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
			
	return ch;
}

int fgetc(FILE *OStream)
{
	(void)OStream;
	
	int ch = EOF;
	
  //__HAL_UART_CLEAR_OREFLAG(&huart2);
	
	while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY){};
	
	HAL_UART_Receive(&huart2, (uint8_t*) &ch, 1, 100);
		
	return ch;
}