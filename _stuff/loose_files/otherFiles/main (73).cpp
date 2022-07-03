



/* Include files ------------------------------------------------------------------*/


#include "functions.hpp"
#include "peripheral.hpp"
#include "mainDefs.h"
#include "stm32l0xx_hal.h"


#include "globalDefines.hpp"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

volatile systemFlags_f systemFlags;

int main(void)
{
	// =========================================================================
	// Peripheral configuration
	// =========================================================================

	do {
		initHalLibrary();								// Peripheral init
	} while (0);

	// =========================================================================
	// Trigger signal for logic analyser
	// =========================================================================

	// ledTurnOff();
	// _delay_ms(200);
	// ledTurnOn();
	// _delay_ms(200);
	// ledTurnOff();

	// =========================================================================
	// Configures circular buffer
	// =========================================================================

	// errorCode = serialRxBuffer.getLastError();
	// if (errorCode) {
	// 	myErrorHandler(errorCode);
	// }

	// =========================================================================
	// API package
	// =========================================================================

	//  Creates package object
	// PackageApi package(255, true);

	// do {
	// 	// Configures package object
	// 	errorCode = package.getLastError();
	// 	if (errorCode) {	// ERROR
	// 		myErrorHandler(errorCode);
	// 	}
	// 	if (!package.configCharSet(0x7E, 0x7D, 0x20)) {
	// 		myErrorHandler(package.getLastError());
	// 	}
	// 	// Configures transmitter package data
	// 	if (!package.txSetId(1, true)) {
	// 		myErrorHandler(package.getLastError());
	// 	}
	// 	if (!package.txSetSourceAddress(ADDRESS_PORTABLE)) {
	// 		myErrorHandler(package.getLastError());
	// 	}
	// 	if (!package.txSetDestinationAddress(ADDRESS_BASE)) {
	// 		myErrorHandler(package.getLastError());
	// 	}
	// } while (0);

	// =========================================================================
	// Reset system flags
	// =========================================================================

	systemFlags.allFlags = 0;

	// =========================================================================
	// Main loop
	// =========================================================================

	while (1) {
		// ---------------------------------------------------------------------
		// 100 Hz TRIGGER
		// ---------------------------------------------------------------------
		if (systemFlags.trigger100Hz) {
			ledToggle();
			systemFlags.trigger100Hz = false;
		}

	}
}

void _Error_Handler(char *file, int line)
{
	while (1) {
	}
}

void HAL_SYSTICK_Callback(void)
{
	systemFlags.trigger100Hz = true;
}

// void HAL_RTC_AlarmAEventCallback (RTC_HandleTypeDef * hrtc)
// {
// 	systemFlags.triggerAlarmA = true;
// }

// void HAL_RTC_AlarmBEventCallback (RTC_HandleTypeDef * hrtc)
// {
// 	systemFlags.triggerAlarmB = true;
// }

// void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
// {
// 	serialRxBuffer.push(rxBuffer[1]);

// }

// void HAL_UART_RxHalfCpltCallback (UART_HandleTypeDef * huart)
// {
// 	serialRxBuffer.push(rxBuffer[0]);
// }

// void HAL_UART_TxCpltCallback (UART_HandleTypeDef * huart)
// {
// 	systemFlags.usart2TxBusy = false;
// }


// #ifdef  USE_FULL_ASSERT

// void assert_failed(uint8_t *file, uint32_t line)
// {
// }

// #endif /* USE_FULL_ASSERT */
