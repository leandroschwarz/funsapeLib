/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ff.h"
#include <string.h>
#include <stdio.h>
#include "globalDefines.hpp"
#include "circularBuffer8.hpp"
#include "circularBuffer16.hpp"
#include "circularBuffer32.hpp"
#include "circularBuffer64.hpp"
#include "dateTime.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_CHANNELS_PER_SAMPLE				4
#define ADC_BYTES_PER_CHANNEL				2
#define ADC_BYTES_PER_SAMPLE				(ADC_BYTES_PER_CHANNEL * ADC_CHANNELS_PER_SAMPLE)
#define ADC_SAMPLES_PER_BUFFER				2
#define ADC_BYTES_PER_BUFFER				(ADC_BYTES_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER)
#define ADC_WORDS_PER_BUFFER				(ADC_BYTES_PER_BUFFER / 4)
#define ADC_HWORDS_PER_BUFFER				(ADC_WORDS_PER_BUFFER * 2)

#define DEBUG_PORT						GPIOB
#define DEBUG_CHANNEL_1					GPIO_PIN_15
#define DEBUG_CHANNEL_2					GPIO_PIN_14
#define DEBUG_CHANNEL_3					GPIO_PIN_13
#define DEBUG_CHANNEL_4					GPIO_PIN_12
#define DEBUG_CHANNEL_5					GPIO_PIN_11
#define DEBUG_CHANNEL_6					GPIO_PIN_9
#define DEBUG_CHANNEL_7					GPIO_PIN_8
#define DEBUG_CHANNEL_8					GPIO_PIN_6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

CircularBuffer8 circBuff1(30, false);
CircularBuffer16 circBuff2(30, false);
CircularBuffer32 circBuff3(30, false);
CircularBuffer64 circBuff4(30, false);
DateTime dateTest;
FATFS SDFatFs;							/* File system object for SD card logical drive */
FIL MyFile;								/* File object */
char USERPath[4];						/* SD card logical drive path */
volatile uint16_t dmaAdcBuff[ADC_HWORDS_PER_BUFFER];
volatile uint16_t dmaAdcBuff2[ADC_HWORDS_PER_BUFFER];
volatile uint16_t *dmaAdcBuffStart;
volatile uint16_t *dmaAdcBuffHalf;
char uartTxBuff[100];
volatile uint8_t send = 0;
uint32_t samplesCounter = 0;
FRESULT res;										// FatFs function common result code
uint32_t sdBytesWritten;							// File write counts
uint32_t sdBytesRead;								// File read counts
char sdBuffWrite[100];								// File write buffer
char sdBuffRead[100];								// File read buffer
volatile uint16_t counter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void Error_Handler2(uint8_t errorCode);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void sendMessage(void)
{
	HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_4, GPIO_PIN_RESET);
	if (send == 1) {
		memcpy(sdBuffWrite, (void *)dmaAdcBuffStart, (ADC_BYTES_PER_BUFFER / 2));
	} else {
		memcpy(sdBuffWrite, (void *)dmaAdcBuffHalf, (ADC_BYTES_PER_BUFFER / 2));
	}
	res = f_write(&MyFile, (uint8_t *)sdBuffWrite, (ADC_BYTES_PER_BUFFER / 2), (UINT *)&sdBytesWritten);
	if ((sdBytesWritten == 0) || (res != FR_OK)) {
		Error_Handler2(5);
	}
	if (f_sync(&MyFile) != FR_OK) {
		Error_Handler2(6);
	}

	// HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_CHANNEL_4);
	// HAL_UART_Transmit(&huart2, (uint8_t *)uartTxBuff, strlen(uartTxBuff), 50);
	HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_4, GPIO_PIN_SET);
	// HAL_UART_Transmit(&huart2, (uint8_t *)dmaAdcBuff, ADC_BYTES_PER_BUFFER, 50);
	// HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_CHANNEL_4);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */
	char fileName[13];
	uint16_t fileNumber = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	dmaAdcBuffStart = &dmaAdcBuff[0];
	dmaAdcBuffHalf = &dmaAdcBuff[ADC_HWORDS_PER_BUFFER / 2];

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_SPI1_Init();
	MX_TIM2_Init();
	MX_SPI2_Init();
	MX_I2C1_Init();
	MX_ADC_Init();
	/* USER CODE BEGIN 2 */

	HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_1 | DEBUG_CHANNEL_2 | DEBUG_CHANNEL_3 | DEBUG_CHANNEL_4 | DEBUG_CHANNEL_5 |
					  DEBUG_CHANNEL_6 | DEBUG_CHANNEL_7 | DEBUG_CHANNEL_8, GPIO_PIN_RESET);
	HAL_TIM_Base_MspInit(&htim2);

	HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_8, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_8, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_8, GPIO_PIN_SET);
	HAL_Delay(200);

	if (f_mount(&SDFatFs, "", 0) != FR_OK) {
		Error_Handler2(1);
	} else {
		// Finding first uncreated fileNumber
		do {
			sprintf(fileName, "file%04d.txt", ++fileNumber);
			res = f_open(&MyFile, fileName, FA_OPEN_EXISTING | FA_READ);
		} while (res == FR_OK);
		res = f_open(&MyFile, fileName, FA_OPEN_ALWAYS | FA_WRITE);
		if (res != FR_OK) {
			Error_Handler2(2);
		}
	}

	HAL_ADC_Start_DMA(&hadc, (uint32_t *)dmaAdcBuff, ADC_HWORDS_PER_BUFFER);
	HAL_TIM_Base_Start_IT(&htim2);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	while (1) {
		HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_CHANNEL_4, GPIO_PIN_RESET);
		if (send) {
			sendMessage();
			send = 0;
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI14;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
								  | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

static void Error_Handler2(uint8_t errorCode)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(2000);
		/* USER CODE END Error_Handler_Debug */
		for (uint8_t i = 0; i < errorCode; i++) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			HAL_Delay(200);
		}
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_CHANNEL_1);
	if ((samplesCounter++ % 4) == 0) {
		HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_CHANNEL_5);
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_CHANNEL_2);
	for (uint8_t i = 0; i < 4; i++) {
		dmaAdcBuffStart[i] = counter++;
		counter %= 4096;
	}
	send = 1;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_CHANNEL_3);
	for (uint8_t i = 0; i < 4; i++) {
		dmaAdcBuffHalf[i] = counter++;
		counter %= 4096;
	}
	send = 2;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
// 	/* User can add his own implementation to report the HAL error return state */
// 	while (1) {
// 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
// 		HAL_Delay(2000);
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
void assert_failed(char *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
