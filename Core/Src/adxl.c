#include "adxl.h"

//struct for x y z
positionOfAdxl position;

//private prototypes
void adxlCheckForTiming(char ** tokensArr);


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

void adxlReadAndOutToPC(char ** tokensArr)
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
    
    adxlCheckForTiming(tokensArr);
}

void adxlReadAndOutOnDisplay(char ** tokensArr)
{
    uint8_t adxlData[6];
    
    char templateForPrintOnScreen[10];
   
    //read data from accelerometer
    adxlRead(adxlData);
    
    //left-shift data
    position.x = ((adxlData[1] << 8) | adxlData[0]);
    position.y = ((adxlData[3] << 8) | adxlData[2]);
    position.z = ((adxlData[5] << 8) | adxlData[4]);

    
    //turn off all of the pixels
    oledFill(black);
    
    //concatenate in format (x = ***) and print
    //x
    sprintf(templateForPrintOnScreen, "%c = %d\n", 'x', position.x);
    oledWriteString(templateForPrintOnScreen, white);
    
    //y
    sprintf(templateForPrintOnScreen, "%c = %d\n", 'y', position.y);
    oledWriteString(templateForPrintOnScreen, white);
    
    //z
    sprintf(templateForPrintOnScreen, "%c = %d\n", 'z', position.z);
    oledWriteString(templateForPrintOnScreen, white);
    
    //refresh the screen
    oledUpdateScreen();
    
    adxlCheckForTiming(tokensArr);
}

void adxlTerminalInit(void)
{
    static terminalList dataFromADXL345PCItem =
    {
        .name = "adxlPC",
        .desc = "reads data from ADXL345 and sends this data to the terminal",
        .funcPtr = adxlReadAndOutToPC,
        .next = NULL
    };
    terminalListAddItem(&dataFromADXL345PCItem);

    static terminalList dataFromADXL345DisplayItem =
    {
        .name = "adxlDisplay",
        .desc = "reads data from ADXL345 and sends this data to the oled",
        .funcPtr = adxlReadAndOutOnDisplay,
        .next = NULL
    };
    terminalListAddItem(&dataFromADXL345DisplayItem);
}

void adxlCheckForTiming(char ** tokensArr)
{
    //creat new element of the structure
    static timingList dataFromADXLToPCItem;
    static timingList dataFromADXLToDisplayItem;
    
    //check whether there's number in second element
    if (atoi(tokensArr[1]))
    {
        if (!strcmp(tokensArr[0], "adxlPC"))
        {
            dataFromADXLToPCItem.timeout = atoi(tokensArr[1]);
            dataFromADXLToPCItem.counter = 0;
            dataFromADXLToPCItem.next = NULL;
            
            dataFromADXLToPCItem.funcPtr = adxlReadAndOutToPC;
            
            timingAddItem(&dataFromADXLToPCItem);
        }
        else if (!strcmp(tokensArr[0], "adxlDisplay"))
        {
            dataFromADXLToDisplayItem.timeout = atoi(tokensArr[1]);
            dataFromADXLToDisplayItem.counter = 0;
            dataFromADXLToDisplayItem.next = NULL;
            
            dataFromADXLToDisplayItem.funcPtr = adxlReadAndOutOnDisplay;
            
            timingAddItem(&dataFromADXLToDisplayItem);
        }
    }
    else if (!strcmp(tokensArr[1], "stop"))
    {
        if (!strcmp(tokensArr[0], "adxlPC"))
        {
            printf("adxlPC stop\n");
            timingDeleteItem(dataFromADXLToPCItem.id);            
        }
        else if (!strcmp(tokensArr[0], "adxlDisplay"))
        {
            printf("adxlDisplay stop\n");
            timingDeleteItem(dataFromADXLToDisplayItem.id); 
        }
    }
}


