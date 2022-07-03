/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "myHeader.hpp"
#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "ff.h"

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

/* USER CODE BEGIN PV */

uint16_t ecgAdcDmaBuffer[ECG_ADC_BUFFER_SIZE];
CircularBuffer<uint16_t> ecgCircBuffer(300, true);
CircularBuffer<uint8_t> sdCardCircBuffer(5000, true);
char uart3TxData[500];
char auxBuff[4096];
volatile unsigned int Timer;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

	HAL_StatusTypeDef halStatus;

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
	MX_SPI2_Init();
	MX_USART3_UART_Init();
	MX_ADC1_Init();
	MX_ADC3_Init();
	MX_TIM1_Init();
	MX_TIM8_Init();
	/* USER CODE BEGIN 2 */

	sprintf(uart3TxData, "UART3 is Working!\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);

	FATFS	sdCardDriver;
	FIL		sdCardFile;
	FRESULT	res;
	uint16_t auxUint16;

	setBit(hspi2.Instance->CR1, 6);

	debug7TurnOn();
	HAL_Delay(10);
	debug7TurnOff();


	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim8);

	// Sd Card - Mounting Volume
	res = f_mount(&sdCardDriver, "", 1);
	if(res) {
		sprintf(uart3TxData, "SD card mounting error = %d!\r", res);
		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		systemHalt();
	}
	sprintf(uart3TxData, "SD card mounted successfully!\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);

	// Sd Card - Create new file
	res = f_open(&sdCardFile, "teste.txt", (BYTE)(FA_OPEN_ALWAYS | FA_WRITE));
	if(res) {
		sprintf(uart3TxData, "File creation error = %d!\r", res);
		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		systemHalt();
	}
	sprintf(uart3TxData, "File created successfully!\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	debug7TurnOn();
	HAL_Delay(10);
	debug7TurnOff();
	res = f_write(&sdCardFile, auxBuff, 512, (UINT *)&auxUint16);
	debug7TurnOn();
	HAL_Delay(10);
	debug7TurnOff();
	if(res) {
		sprintf(uart3TxData, "File writing error = %d!\r", res);
		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		systemHalt();
	}
	f_sync(&sdCardFile);
	sprintf(uart3TxData, "File writing sucessfully! %d bytes written!\r", auxUint16);
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	systemHalt();


//	f_sync(&sdCardFile);

	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ecgAdcDmaBuffer, ECG_ADC_BUFFER_SIZE);
	if(halStatus != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while(1) {
		if(ecgCircBuffer.getOccupation() >= 64) {
			ecgCircBuffer.popBuffer((uint16_t *)auxBuff, 64);
			sdCardCircBuffer.pushBuffer((uint8_t *)auxBuff, 128);
		}
		if(sdCardCircBuffer.getOccupation() >= 512) {
			sdCardCircBuffer.popBuffer((uint8_t *)auxBuff, 512);
			res = f_write(&sdCardFile, auxBuff, 512, (UINT *)&auxUint16);
			if(res) {
				sprintf(uart3TxData, "File writing error = %d!\r", res);
				HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
				// systemHalt();
			}
			f_sync(&sdCardFile);
			sprintf(uart3TxData, "File writing sucessfully! %d bytes written!\r", auxUint16);
			HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		}
		// ledToggle();
		// HAL_Delay(500);

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

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
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

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static volatile uint16_t auxTimer = 0;

	if(htim->Instance == TIM1) {
		debug0Toggle();
		auxTimer++;
		if(auxTimer == 4000) {
			ledToggle();
			auxTimer = 0;
		}
	} else if(htim->Instance == TIM8) {
		debug1Toggle();
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	debug2Toggle();
	debug6TurnOn();
	for(uint8_t i = 0; i < 12; i++) {
		ecgCircBuffer.push(ecgAdcDmaBuffer[i]);
	}
	debug6TurnOff();
	return;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	debug3Toggle();
	debug6TurnOn();
	for(uint8_t i = 0; i < 12; i++) {
		ecgCircBuffer.push(ecgAdcDmaBuffer[12 + i]);
	}
	debug6TurnOff();
	return;
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

	while(1) {
		ledToggle();
		HAL_Delay(200);
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
