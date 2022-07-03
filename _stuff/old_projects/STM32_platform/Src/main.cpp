
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

#include "..\gpdse\globalDefines.hpp"
#include "..\gpdse\util\circularBuffer8.hpp"
#include "..\gpdse\util\dateTime.hpp"
#include "..\gpdse\util\packageApi.hpp"
#include <string.h>
#include <stdio.h>
#include "functions.hpp"

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

volatile systemFlags_f systemFlags;
vuint8_t txBuffer[50];

void SystemClock_Config(void);

int main(void)
{
	// -------------------------------------------------------------------------
	// Local variables
	// -------------------------------------------------------------------------

	uint16_t aux16 = 0;					// Auxiliary variables
	bool auxBool = false;				// ...
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

	HAL_Init();
	SystemClock_Config();
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
	auxBool = txPackage.setEscapeCharacters(true);
	if (!auxBool) {
		myErrorHandler();
	}

	// Create receiver package object
	PackageApi rxPackage(100);
	if (rxPackage.getLastError() != ERROR_NONE) {
		myErrorHandler();
	}
	// Set escape characters
	auxBool = rxPackage.setEscapeCharacters(true);
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

	/*
	// RTC alarm configuration
	sAlarm.AlarmTime.Hours = RTC_ALARM_HOURS;
	sAlarm.AlarmTime.Minutes = RTC_ALARM_MINUTES;
	sAlarm.AlarmTime.Seconds = RTC_ALARM_SECONDS;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_SS14_6;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = RTC_DEFAULT_DATE;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	// RTC_ALARMSUBSECONDMASK_SS14_10	= 1000ms
	// RTC_ALARMSUBSECONDMASK_SS14_9	= 500ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_7	= 125ms
	// RTC_ALARMSUBSECONDMASK_SS14_6	= 62.5ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	*/

	while (1) {

		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

		sprintf(auxString, "%02d/%02d/%02d %d %02d:%02d:%02d.%03lu\r", sDate.Date, sDate.Month, sDate.Year, sDate.WeekDay, sTime.Hours, sTime.Minutes, sTime.Seconds, sTime.SubSeconds);
		uart2SafeSend((uint8_t *)auxString, strlen(auxString));
		sprintf(auxString, "%02d/XX/XX X %02d:%02d:%02d.%03lu\r\r", sAlarm.AlarmDateWeekDay, sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds, sAlarm.AlarmTime.SubSeconds);
		uart2SafeSend((uint8_t *)auxString, strlen(auxString));

		HAL_Delay(500);
	}

}

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
	systemFlags.usart2TxBusy = false;
}

void _Error_Handler(char *file, int line)
{
	while (1) {
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}

#endif /* USE_FULL_ASSERT */
