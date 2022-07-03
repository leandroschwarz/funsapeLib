


#include "mainDefs.h"
#include "peripheral.hpp"
#include "functions.hpp"
#include <string.h>


vuint8_t txBuffer[200];


// System functions

void initHalLibrary(void)
{
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
}

void myErrorHandler(error_e errorCode)
{
	uint16_t error = (uint16_t)errorCode;
	while (1) {
		ledTurnOff();
		_delay_ms(500);
		for (uint8_t i = 0; i < error; i++) {
			ledTurnOn();
			_delay_ms(100);
			ledTurnOff();
			_delay_ms(100);
		}
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
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 10000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// RTC functions

void rtcInit(void)
{
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
}

void rtcSetDateTime(DateTime * timestamp)
{
	// Local variables
	uint8_t aux8_1;
	uint8_t aux8_2;
	uint8_t aux8_3;
	uint16_t aux16;
	weekDay_t weekDay;
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	// Get time
	timestamp->getTime(&aux8_1, &aux8_2, &aux8_3);
	sTime.Hours = aux8_1;
	sTime.Minutes = aux8_2;
	sTime.Seconds = aux8_3;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	// Get date
	timestamp->getDate(&aux8_1, (month_t *)&aux8_2, &aux16);
	sDate.Date = aux8_1;
	sDate.Month = aux8_2;
	sDate.Year = (uint8_t)(aux16 - 2000);

	// Get weekday
	timestamp->getWeekDay(&weekDay);
	sDate.WeekDay = (weekDay == WEEKDAY_SUNDAY) ? 7 : ((uint8_t)weekDay - 1);

	// Update values
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
}

void rtcSetAlarm(DateTime * timestamp, rtcAlarmSel alarm)
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t millisecond;
	RTC_AlarmTypeDef sAlarm;

	timestamp->getTime(&hour, &minute, &second, &millisecond);

	sAlarm.AlarmTime.Hours = hour;
	sAlarm.AlarmTime.Minutes = minute;
	sAlarm.AlarmTime.Seconds = second;
	sAlarm.AlarmTime.SubSeconds = millisecond;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_SS14_10;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;

	if ((alarm == ALARM_A) ||  (alarm == ALARM_ALL)) {
		sAlarm.Alarm = RTC_ALARM_A;
		if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}
	}
	if ((alarm == ALARM_B) ||  (alarm == ALARM_ALL)) {
		sAlarm.Alarm = RTC_ALARM_B;
		if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}
	}
	// RTC_ALARMSUBSECONDMASK_SS14_10	= 1000ms
	// RTC_ALARMSUBSECONDMASK_SS14_9	= 500ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_7	= 125ms
	// RTC_ALARMSUBSECONDMASK_SS14_6	= 62.5ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms
	// RTC_ALARMSUBSECONDMASK_SS14_8	= 250ms

}


// UART functions

void uartSafeSend(UART_HandleTypeDef * uartHandle, uint8_t * buffer, uint16_t size)
{
	// Wait last transfer to finish
	do {
	} while (systemFlags.usart2TxBusy);
	// Transfer data between buffers
	memcpy((void *)txBuffer, (void *)buffer, size);
	systemFlags.usart2TxBusy = true;
	HAL_UART_Transmit_IT(uartHandle, (uint8_t *)txBuffer, size);
}



