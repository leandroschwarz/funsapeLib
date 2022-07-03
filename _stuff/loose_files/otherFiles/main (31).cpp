
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
  * Copyright (c) 2018 STMicroelectronics International N.V.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

#include "mainDefs.h"
#include "globalDefines.hpp"
#include "packageApi.hpp"
#include "circularBuffer8.hpp"


#define ADDRESS_PORTABLE		0x10
#define ADDRESS_BASE			0x80

#define RTC_DEFAULT_HOURS		0
#define RTC_DEFAULT_MINUTES		50
#define RTC_DEFAULT_SECONDS		56
#define RTC_DEFAULT_WEEKDAY		RTC_WEEKDAY_FRIDAY
#define RTC_DEFAULT_DATE		1
#define RTC_DEFAULT_MONTH		RTC_MONTH_JUNE
#define RTC_DEFAULT_YEAR		18
#define RTC_ALARM_HOURS			1
#define RTC_ALARM_MINUTES		13
#define RTC_ALARM_SECONDS		5

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

volatile systemFlags_f systemFlags;
vuint8_t txBuffer[50];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void myErrorHandler(void);
void uart2SafeSend(uint8_t * buffer, uint16_t size);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	/* USER CODE BEGIN 1 */

	// -------------------------------------------------------------------------
	// Local variables
	// -------------------------------------------------------------------------

	uint16_t aux16 = 0;					// Auxiliary variables
	bool_t auxBool = FALSE;				// ...
//	packageApiError_e error;			// Package variables
	char auxString[50];					// Buffers
	uint8_t rxBuffer[50];				// ...
	uint8_t auxBuffer[10];				// ...
	RTC_TimeTypeDef sTime;				// RTC  variables
	RTC_DateTypeDef sDate;				// ...
	RTC_AlarmTypeDef sAlarm;			// ...

	// -------------------------------------------------------------------------
	// Variable initialization
	// -------------------------------------------------------------------------

	systemFlags.allFlags = 0;



	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

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
	MX_USART2_UART_Init();
	MX_RTC_Init();
	MX_ADC_Init();
	MX_DAC_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_FATFS_Init();
	MX_TIM2_Init();
	MX_TIM6_Init();
	MX_TIM21_Init();
	MX_TIM22_Init();
	MX_USART1_UART_Init();

	/* USER CODE BEGIN 2 */

	// -------------------------------------------------------------------------
	// Trigger for logic analyser
	// -------------------------------------------------------------------------

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	// -------------------------------------------------------------------------
	// Send UART welcome message
	// -------------------------------------------------------------------------

	aux16 = sprintf(auxString, "UART Test Message\r\r");
	uart2SafeSend((uint8_t *)auxString, aux16);
	HAL_UART_Receive_DMA(&huart2, rxBuffer, 10);

	// -------------------------------------------------------------------------
	// API packages
	// -------------------------------------------------------------------------

	// Create transmitter package object
	PackageApi txPackage(100);
	if (txPackage.getLastError() != ERROR_NONE) {
		myErrorHandler();
	}
	// Set escape characters
	auxBool = txPackage.setEscapeCharacters(TRUE);
	if (!auxBool) {
		myErrorHandler();
	}

	// Create receiver package object
	PackageApi rxPackage(100);
	if (rxPackage.getLastError() != ERROR_NONE) {
		myErrorHandler();
	}
	// Set escape characters
	auxBool = rxPackage.setEscapeCharacters(TRUE);
	if (!auxBool) {
		myErrorHandler();
	}


	// -------------------------------------------------------------------------
	// Sample packages
	// -------------------------------------------------------------------------

	// Configure payload
	aux16 = 0;
	auxBuffer[aux16++] = 0x20;
	auxBuffer[aux16++] = 0x21;
	auxBuffer[aux16++] = 0x7D;
	auxBuffer[aux16++] = 0x23;
	auxBuffer[aux16++] = 0x7E;
	auxBuffer[aux16++] = 0x25;
	auxBuffer[aux16++] = 0x26;
	auxBuffer[aux16++] = 0x27;
	// Create package
	auxBool = txPackage.createNewPackage(0x0A, ADDRESS_BASE, ADDRESS_PORTABLE, auxBuffer, aux16);
	if (!auxBool) {
		myErrorHandler();
	}

	// Transmit package
	uart2SafeSend(txPackage.getPackageData(), txPackage.getPackageSize());

	// -------------------------------------------------------------------------
	// RTC configuration
	// -------------------------------------------------------------------------

	// RTC initialization
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 37;
	hrtc.Init.SynchPrediv = 1000;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// RTC date and time configuration   --   Must be moved to a function later
//	sTime.Hours = RTC_DEFAULT_HOURS;
//	sTime.Minutes = RTC_DEFAULT_MINUTES;
//	sTime.Seconds = RTC_DEFAULT_SECONDS;
//	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}
//	sDate.WeekDay = RTC_DEFAULT_WEEKDAY;
//	sDate.Month = RTC_DEFAULT_MONTH;
//	sDate.Date = RTC_DEFAULT_DATE;
//	sDate.Year = RTC_DEFAULT_YEAR;
//	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}

	// RTC alarm configuration
//	sAlarm.AlarmTime.Hours = RTC_ALARM_HOURS;
//	sAlarm.AlarmTime.Minutes = RTC_ALARM_MINUTES;
//	sAlarm.AlarmTime.Seconds = RTC_ALARM_SECONDS;
//	sAlarm.AlarmTime.SubSeconds = 0;
//	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//	sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
//	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_SS14_6;
//	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
//	sAlarm.AlarmDateWeekDay = RTC_DEFAULT_DATE;
//	sAlarm.Alarm = RTC_ALARM_A;
//	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
//		_Error_Handler(__FILE__, __LINE__);
//	}
	// RTC_ALARMSUBSECONDMASK_SS14_10	= 1000ms
	// RTC_ALARMSUBSECONDMASK_SS14_9	= 500ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_7	= 125ms
	// RTC_ALARMSUBSECONDMASK_SS14_6	= 62.5ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms





	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

		sprintf(auxString, "%02d/%02d/%02d %d %02d:%02d:%02d.%03lu\r", sDate.Date, sDate.Month, sDate.Year, sDate.WeekDay, sTime.Hours, sTime.Minutes, sTime.Seconds, sTime.SubSeconds);
		uart2SafeSend((uint8_t *)auxString, strlen(auxString));
		sprintf(auxString, "%02d/XX/XX X %02d:%02d:%02d.%03lu\r\r", sAlarm.AlarmDateWeekDay, sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds, sAlarm.AlarmTime.SubSeconds);
		uart2SafeSend((uint8_t *)auxString, strlen(auxString));

		HAL_Delay(500);

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

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Configure the main internal regulator output voltage
	*/
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_6;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
								  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2
										 | RCC_PERIPHCLK_RTC;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_RTC_AlarmAEventCallback (RTC_HandleTypeDef * hrtc)
{
}

void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
{
}

void HAL_UART_RxHalfCpltCallback (UART_HandleTypeDef * huart)
{
}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef * huart)
{
	systemFlags.usart2TxBusy = FALSE;
}

void myErrorHandler(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	systemHalt();
}
void uart2SafeSend(uint8_t * buffer, uint16_t size)
{
	// Wait last transfer to finish
	do {
	} while (systemFlags.usart2TxBusy);
	// Transfer data between buffers
	memcpy((void *)txBuffer, (void *)buffer, size);
	systemFlags.usart2TxBusy = TRUE;
	HAL_UART_Transmit_IT(&huart2, (uint8_t *)txBuffer, size);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
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

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
