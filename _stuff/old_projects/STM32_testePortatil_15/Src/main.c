/* -----------------------------------------------------------------------------
 * File:			main.c
 * Author:			Leandro Schwarz
 * Last edition:	May 1, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include "fatfs.h"
#include <string.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Local variables
	char txBufferData[300];
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	GPIO_InitTypeDef GPIO_InitStruct;
	RTC_AlarmTypeDef sAlarm;
	HAL_StatusTypeDef status;

	// Variable initialization

	// MCU Configuration
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
//	MX_SPI1_Init();
	MX_I2C2_Init();
	MX_DAC_Init();
	MX_USART1_UART_Init();
	MX_TIM22_Init();
	MX_TIM21_Init();
	MX_TIM6_Init();
	MX_TIM2_Init();
	MX_FATFS_Init();
	MX_ADC_Init();
	MX_RTC_Init();

	// Configures GPIOA outputs
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	// Show UART message
	sprintf(txBufferData, "Teste da UART\r");
	HAL_UART_Transmit(&huart2, (uint8_t *)txBufferData, strlen(txBufferData), 500);

	// Sets default time and date
	sTime.Hours = RTC_DEFAULT_HOURS;
	sTime.Minutes = RTC_DEFAULT_MINUTES;
	sTime.Seconds = RTC_DEFAULT_SECONDS;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sDate.WeekDay = RTC_DEFAULT_WEEKDAY;
	sDate.Month = RTC_DEFAULT_MONTH;
	sDate.Date = RTC_DEFAULT_DATE;
	sDate.Year = RTC_DEFAULT_YEAR;
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

		// Enables the alarm A
		sAlarm.AlarmTime.Hours = RTC_DEFAULT_HOURS;
		sAlarm.AlarmTime.Minutes = RTC_DEFAULT_MINUTES;
		sAlarm.AlarmTime.Seconds = RTC_DEFAULT_SECONDS + 10;
//		sAlarm.AlarmTime.SubSeconds = 255;
		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
		sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
//		sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_SS14_6;
		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
		sAlarm.AlarmDateWeekDay = RTC_DEFAULT_DATE;
		sAlarm.Alarm = RTC_ALARM_A;
		if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}
	// -------------------------------------------------------------------------
	// Infinite loop -----------------------------------------------------------
	while (1) {

/*		do {
			status = HAL_RTC_PollForAlarmAEvent(&hrtc, 1000000);
		} while(status == HAL_BUSY);
		txBufferData[0] = status;
		txBufferData[1] = '\r';
		txBufferData[2] = '\r';
		HAL_UART_Transmit(&huart2, (uint8_t *)txBufferData, 3, 500);
*/

		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		sprintf(txBufferData, "%02d/%02d/20%02d %02d %02d:%02d:%02d\r", sDate.Date,
					sDate.Month, sDate.Year, sDate.WeekDay, sTime.Hours, sTime.Minutes,
					sTime.Seconds);
		HAL_UART_Transmit(&huart2, (uint8_t *)txBufferData, strlen(txBufferData), 500);

		HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

		sprintf(txBufferData, "%02u/--/---- -- %02u:%02u:%02u.%x %u %u %u %u %u %x %x\r\r",
			sAlarm.AlarmDateWeekDay,
			sAlarm.AlarmTime.Hours,
			sAlarm.AlarmTime.Minutes,
			sAlarm.AlarmTime.Seconds,
			sAlarm.AlarmTime.SubSeconds,

			sAlarm.AlarmTime.DayLightSaving,
			sAlarm.AlarmTime.SecondFraction,
			sAlarm.AlarmTime.StoreOperation,
			sAlarm.AlarmTime.TimeFormat,
			sAlarm.AlarmDateWeekDaySel,
			sAlarm.AlarmMask,
			sAlarm.AlarmSubSecondMask);
		HAL_UART_Transmit(&huart2, (uint8_t *)txBufferData, strlen(txBufferData), 500);
		HAL_Delay(500);
	}
}

// -----------------------------------------------------------------------------
// Interrupt callback functions ------------------------------------------------

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef * hrtc)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

// -----------------------------------------------------------------------------
// Function definitions ---------------------------------------------------------
