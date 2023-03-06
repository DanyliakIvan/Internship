/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "oled.h"
#include "adxl.h"
#include "terminal.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
/* USER CODE BEGIN PV */
uint8_t brightnessPercentage; //LED brightness (0-100)
uint16_t previosState = 1600;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
uint16_t map(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
uint16_t charIntoNumber(uint8_t* receiveBuf);


/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
	
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

	
  /* USER CODE BEGIN Init */
	adxlInit();	
	oledInit();
  /* USER CODE END Init */
	
	
  /* USER CODE BEGIN 2 */
	printf("%s", "Hello!\n\n");
  /* USER CODE END 2 */


  /* Infinite loop */
	
  /* USER CODE BEGIN WHILE */
	

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
				
		checkCommand();
  }
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//convertion a value from one range into a proportional value of another value
uint16_t map(uint16_t data, uint16_t intMin, uint16_t intMax, uint16_t outMin, uint16_t outMax)
{
	return (data - intMin) * (outMax - outMin) / (intMax - intMin) + outMin; 
}

//{ '1', '0', '0' } -> 100
uint16_t charIntoNumber(uint8_t* receiveBuf)
{
	uint16_t number = 0;
	
	number = (receiveBuf[0] - '0') * 100 + (receiveBuf[1] - '0') * 10 + (receiveBuf[2] - '0');
	
	receiveBuf[0] = receiveBuf[1] = receiveBuf[2] = receiveBuf[3] = '\0';
	
	return number;
}

void receiveBrightnessFromPotentiometer(char ** tokensArr)
{
	uint16_t dataFromPotentiometer = 0; //(0-4096)
	//receiving data from PC using UART
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	dataFromPotentiometer = HAL_ADC_GetValue(&hadc1);

	// converting (0-4096) -> (0-100) 
	brightnessPercentage = map(dataFromPotentiometer, 0, 4096, 0, 100); 

	if(previosState != brightnessPercentage) //print only when there're changes
	{
		//transmitting data to PC using UART
		printf("Duty cycle = %d\n", previosState = brightnessPercentage);
	}
}

void receiveBrightnessFromPC(char ** tokensArr)
{
	int8_t x;
	uint8_t receiveBuf[4];
	
	//receiving data from PC using UART (3 "digits")
	for (uint8_t i = 0; i < 3; i++)
	{
		memcpy(receiveBuf, tokensArr[1], 3);
		//receiveBuf[i] = tokensArr[1][i];
	}
	
	//checking for data availability
	if(receiveBuf[0] + receiveBuf[1] + receiveBuf[2] > 1)
	{
		//{ '1', '0', '0' } -> 100
		printf("Duty cycle = %d\n", brightnessPercentage = charIntoNumber(receiveBuf));
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
