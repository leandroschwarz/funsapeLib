/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>
#include "circularBuffer16.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef union systemFlags_f {
	struct {
		uint32_t	convReady				: 1;
		uint32_t	convCounter				: 5;
		uint32_t	unusedBits				: 26;

	};
	uint32_t allFlags;
} systemFlags_f;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ADC_CHANNELS_PER_SAMPLE			3
#define ADC_SAMPLES_PER_BUFFER			8
#define ADC_DMA_BUFFER_SIZE				(ADC_SAMPLES_PER_BUFFER * ADC_CHANNELS_PER_SAMPLE)
#define UART_TX_BUFFER_SIZE				50
#define UART_RX_BUFFER_SIZE				50

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

char txDmaBuffer[UART_TX_BUFFER_SIZE];
// char rxDmaBuffer[UART_RX_BUFFER_SIZE];
uint16_t adcDmaBuffer[ADC_DMA_BUFFER_SIZE];
volatile uint8_t adcDmaBufferIndex = 0;
volatile systemFlags_f systemFlags;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void systemError(uint8_t errorCode);

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

	systemFlags.allFlags = 0;
	CircularBuffer16 adcCircBuff(50 * ADC_CHANNELS_PER_SAMPLE);

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	while (1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(200);
	}

	// Syncronization trigger for debug
	HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_RESET);

	// Serial communication test
	strcpy(txDmaBuffer, "USART is working!\r");
	HAL_UART_Transmit_IT(&huart1, (uint8_t *)txDmaBuffer, strlen(txDmaBuffer));

	// TIMER3 start
	HAL_TIM_Base_Start_IT(&htim3);

	// ADC1 configuration
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcDmaBuffer, ADC_DMA_BUFFER_SIZE);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		if (systemFlags.convReady) {
			// if (++(systemFlags.convCounter) == 25) {
			// 	// Prepare tx buffer to send
			// 	sprintf(txDmaBuffer, "Values = %05u", adcDmaBuffer[adcDmaBufferIndex]);
			// 	for (uint8_t i = 1; i < (ADC_DMA_BUFFER_SIZE / 2); i++) {
			// 		sprintf(txDmaBuffer, "%s %05u", txDmaBuffer, adcDmaBuffer[adcDmaBufferIndex + i]);
			// 	}
			// 	strcat(txDmaBuffer, "\r");
			// 	// Send tx buffer
			// 	HAL_UART_Transmit_IT(&huart1, (uint8_t *)txDmaBuffer, strlen(txDmaBuffer));
			// 	systemFlags.convCounter = 0;
			// }
			//////////////////////// Using circular buffer class

			HAL_GPIO_WritePin(DEBUG_5_GPIO_Port, DEBUG_5_Pin, GPIO_PIN_SET);
			for (uint8_t i = 0; i < (ADC_DMA_BUFFER_SIZE / 2); i++) {
				adcCircBuff.push(adcDmaBuffer[adcDmaBufferIndex + i]);
			}
			systemFlags.convReady = 0;
			HAL_GPIO_WritePin(DEBUG_5_GPIO_Port, DEBUG_5_Pin, GPIO_PIN_RESET);
		}
		if (adcCircBuff.getOccupation() >= 90) {
			HAL_GPIO_WritePin(DEBUG_6_GPIO_Port, DEBUG_6_Pin, GPIO_PIN_SET);
			volatile uint16_t aux16 = 0;

			for (uint8_t i = 0; i < 90; i++) {
				adcCircBuff.pop((uint16_t *)&aux16);
			}
			HAL_GPIO_WritePin(DEBUG_6_GPIO_Port, DEBUG_6_Pin, GPIO_PIN_RESET);
			// Send 90 Bytes from circBuffer to SD card;
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
	// FIX: FIX something
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
								  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	adcDmaBufferIndex = 0;
	systemFlags.convReady = 1;
	HAL_GPIO_TogglePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	adcDmaBufferIndex = (ADC_DMA_BUFFER_SIZE / 2);
	systemFlags.convReady = 1;
	HAL_GPIO_TogglePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *uhart)
{
	HAL_GPIO_TogglePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uhart)
{
}

void systemError(uint8_t errorCode)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);	// Inverted logic (LED ON)
	HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_RESET);
	while (1) {
		for (uint8_t i = 0; i < errorCode; ++i) {
			HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_SET);
			HAL_Delay(10);
			HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_RESET);
			HAL_Delay(10);
		}
		HAL_Delay(100);
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
	while (1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(50);
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
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
