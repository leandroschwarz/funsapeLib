/* =============================================================================
 * Project:			STM32
 * File name:		main.cpp
 * Module:			Main source code file
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "myMain.hpp"

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Import external variables
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Inline function definitions
// =============================================================================

// NONE

// =============================================================================
// Function declarations
// =============================================================================

// NONE

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	//     ////////////////////     SYSTEM FLAGS    /////////////////////     //
	systemFlags.allFlags = 0;

	//     ///////////////////     INITIALIZATION    ////////////////////     //
	setupHal();
	setupUart3();
	setupTimer1();
	setupDebug();
	setupUart1();
	setupUart2();
	setupSpi1();
	setupSpi2();
	setupTwi1();
	setupTwi2();
	setupAdc1();
	setupAdc3();
	setupTimer3();
	setupTimer7();
	setupTimer8();
	setupRtc();

	// -------------------------------------------------------------------------
	// SD card configuration ---------------------------------------------------

	sdCardMount();
/*

	// SD card - OPEN FILE
	startChronometer();
	sdCard.result = f_open(&fileName, "file.txt", FA_WRITE | FA_CREATE_ALWAYS);
	aux64 = stopChronometer();
	if(res != FR_OK) {
		Error_Handler(2);
	}
	sprintf(uartTxBuffer, "File created! t = %lu\r", (uint32_t)aux64);
	HAL_UART_Transmit(&huart1, (uint8_t *)uartTxBuffer, strlen(uartTxBuffer), 100);
	debugStatus(1002);




	char sdCardBuffer[1000];

#define SD_BUFFER_MAX_SIZE 256
	uint8_t index = 0;
	for(uint16_t i = 0; i < 1000; i++) {
		sdCardBuffer[i] = 'a' + index;
		index = (index == 25) ? 0 : index + 1;
	}
	sdCardBuffer[SD_BUFFER_MAX_SIZE - 1] = '\n';

	while(1) {
		// SD card - WRITE DATA
		UINT auxUint = 0;
		led.low();
		startChronometer();
		res = f_write(&fileName, sdCardBuffer, SD_BUFFER_MAX_SIZE, &auxUint);
		led.high();
		aux64 = stopChronometer();
		if(res != FR_OK) {
			Error_Handler(3);
		}
		sprintf(uartTxBuffer, "Data written! t = %u\r", (uint32_t)aux64);
		HAL_UART_Transmit(&huart1, (uint8_t *)uartTxBuffer, strlen(uartTxBuffer), 100);

		// SD card - SYNC DATA
		res = f_sync(&fileName);
		if(res != FR_OK) {
			Error_Handler(4);
		}
		sprintf(uartTxBuffer, "counter = %u\r", htim7.Instance->CNT);
		HAL_UART_Transmit(&huart1, (uint8_t *)uartTxBuffer, strlen(uartTxBuffer), 100);

		delayMs(100);
	}
	*/

	while(1) {

		// ---------------------------------------------------------------------
		// Update RTC date and time --------------------------------------------

		if(systemFlags.rtcUpdated) {
			systemFlags.rtcUpdated = false;
			if(HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN) != HAL_OK) {
				Error_Handler(ERROR_CODE_1039);
			}
			if(HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN) != HAL_OK) {
				Error_Handler(ERROR_CODE_103A);
			}
		}




	}

	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) {
	}
	if(htim->Instance == TIM4) {
	}
	if(htim->Instance == TIM7) {
		chronometer++;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_8) { // If The INT Source Is EXTI Line9 (A9 Pin)
//		led.cpl();
		// updateDateTime = true;
	}
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	systemFlags.rtcUpdated = true;
}

// =============================================================================
// END OF FILE
// =============================================================================
