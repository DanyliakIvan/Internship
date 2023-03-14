#include <stdio.h>
#include "usart.h"

/*! Standart output file stream */
FILE __stdout;
/*! Standart input file stream */
FILE __stdin; 

#define RETARGET_DELAY 10

uint8_t CARRIAGE_RETURN = 0x0D;

int fputc(int ch, FILE *OStream)
{
	(void)OStream;
	
    //wait while line is busy
	while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX){};
        
    //'\n' = '\r' + '\n'
	if (ch == '\n')
	{
		HAL_UART_Transmit(&huart2, &CARRIAGE_RETURN, 1, RETARGET_DELAY);
	}
    
	//wait while line is busy
	while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX){};
	
	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, RETARGET_DELAY);
			
	return ch;
}

int fgetc(FILE *OStream)
{
	(void)OStream;
	
	int ch = EOF;
	
    __HAL_UART_CLEAR_OREFLAG(&huart2);

    //wait while line is busy
	while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_RX){};

    //check the availability of data in receive register
    if(huart2.Instance->ISR & UART_FLAG_RXNE)
    {
        HAL_UART_Receive(&huart2, (uint8_t*) &ch, 1, RETARGET_DELAY);
    }
          
	return ch;
}