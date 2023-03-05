#include "adxl.h"


void adxlInit(void)
{
	adxlWrite(0x2E, 0x80);  // reset all bits
	adxlWrite(0x2d, 0x08);  // power_cntl measure and wake up 8hz
	adxlWrite(0x31, 0x0B);  // data_format range= +- 4g
}

void adxlWrite(uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address | 0x40;  // multibyte write
	data[1] = value;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  // pull the cs pin low
	HAL_SPI_Transmit(&hspi1, data, 2, 1000);  // write data to register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  // pull the cs pin high
}

void adxlRead(uint8_t* adxlData)
{
	uint8_t address = 0x32;
	address |= 0x80;  // read operation
	address |= 0x40;  // multibyte read
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  // pull the pin low
	HAL_SPI_Transmit(&hspi1, &address, 1, 1000);  // send address
	HAL_SPI_Receive(&hspi1, adxlData, 6, 1000);  // receive 6 bytes data
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  // pull the pin high
}

void adxlReadAndOutInTerm(void)
{
	uint8_t x, y, z;
	uint8_t adxlData[6];
	adxlRead(adxlData);
	
	//read data from accelerometer
	x = ((adxlData[1] << 8) | adxlData[0]);
	y = ((adxlData[3] << 8) | adxlData[2]);
	z = ((adxlData[5] << 8) | adxlData[4]);
	
	//print x, y and z in terminal
	printf("Tx = %d, y = %d, z = %d\n", x, y, z);
	
	//HAL_Delay(500);
}

void adxlReadAndOutOnScreen(void)
{
	uint8_t x, y, z;
	uint8_t adxlData[6];
	char arr[] =  "x = 000\0 y = 000\0 z = 000";
	
	//read data from accelerometer
	adxlRead(adxlData);
	
	//left-shift data
	x = ((adxlData[1] << 8) | adxlData[0]);
	y = ((adxlData[3] << 8) | adxlData[2]);
	z = ((adxlData[5] << 8) | adxlData[4]);

	//replace 000 in string to figures from data
	arr[4] = (x / 100) + 48; 
	x -= (x / 100)*100;
	arr[5] = (x / 10) + 48;  
	x -= (x / 10)*10;
	arr[6] = x + 48;  
	
	arr[13] = (y / 100 + 48); 
	y -= (y / 100)*100;
	arr[14] = (y / 10) + 48; 
	y -= (y / 10)*10;
	arr[15] = y + 48; 
	
	arr[22] = (z / 100) + 48; 
	z -= (z / 100)*100;
	arr[23] = (z / 10) + 48; 
	z -= (z / 10)*10;
	arr[24] = z + 48; 
	
	//print x, y and z  on screen
	oledSetCursor(0, 0);
	oledWriteString(arr, white);
	oledSetCursor(0, 10);
	oledWriteString(arr + 9, white);
	oledSetCursor(0, 20);
	oledWriteString(arr + 18, white);
	oledUpdateScreen();
		
	//HAL_Delay(500);
}