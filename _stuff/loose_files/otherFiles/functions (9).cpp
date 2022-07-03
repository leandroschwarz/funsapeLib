

#include "functions.hpp"
#include <string.h>


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
	systemFlags.usart2TxBusy = true;
	HAL_UART_Transmit_IT(&huart2, (uint8_t *)txBuffer, size);
}
