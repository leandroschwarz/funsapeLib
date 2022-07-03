
#include "rtc.h"
#include <string.h>

RTC_HandleTypeDef hrtc;

void Error_Handler(uint16_t errorCode_p);

// -----------------------------------------------------------------------------
// RTC init function -----------------------------------------------------------

void MX_RTC_Init(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef DateToUpdate = {0};

	// Initialize RTC Only
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	if(HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler(ERROR_CODE_100E);
	}

	// Initialize RTC and set the Time and Date
	sTime.Hours = 0x12;
	sTime.Minutes = 0x34;
	sTime.Seconds = 0x56;
	if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler(ERROR_CODE_100F);
	}
	DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
	DateToUpdate.Month = RTC_MONTH_MARCH;
	DateToUpdate.Date = 0x22;
	DateToUpdate.Year = 0x21;
	if(HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler(ERROR_CODE_1010);
	} else {
		uint32_t dateMem;
		dateMem = BKP->DR3;
		dateMem |= BKP->DR2 << 16;
		memcpy(&DateToUpdate, &dateMem, sizeof(uint32_t));
		if(HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler(ERROR_CODE_1011);
		}
		if(HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler(ERROR_CODE_1012);
		}
	}
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle)
{

	if(rtcHandle->Instance == RTC) {
		HAL_PWR_EnableBkUpAccess();
		// Enable BKP CLK enable for backup registers
		__HAL_RCC_BKP_CLK_ENABLE();
		// RTC clock enable
		__HAL_RCC_RTC_ENABLE();

		// RTC interrupt Init
		HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(RTC_IRQn);
	}
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle)
{
	if(rtcHandle->Instance == RTC) {
		// Peripheral clock disable
		__HAL_RCC_RTC_DISABLE();

		// RTC interrupt Deinit
		HAL_NVIC_DisableIRQ(RTC_IRQn);
	}
}
