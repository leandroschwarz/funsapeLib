



/* Include files ------------------------------------------------------------------*/


#include "functions.hpp"
#include "peripheral.hpp"

#include "mainDefs.h"


#include "stm32l0xx_hal.h"


#include "globalDefines.hpp"
#include "circularBuffer8.hpp"
#include "dateTime.hpp"
#include "packageApi.hpp"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// #if !defined (GPDSE_IGNORE_STUBS)
// #	include "..\gpdse\stubs\stubs.h"
// #endif

#define ADDRESS_PORTABLE		0x10
#define ADDRESS_BASE			0x80

volatile systemFlags_f systemFlags;
vuint8_t rxBuffer[2];
CircularBuffer8 serialRxBuffer(200);
vuint16_t ticksCounter = 0;
error_e errorCode;

int main(void)
{
	// =========================================================================
	// Peripheral configuration
	// =========================================================================

	do {
		DateTime dateTime;
		dateTime.setTime(18, 2, 2, 0);

		initHalLibrary();										// Peripheral init
		HAL_UART_Receive_DMA(&huart2, (uint8_t *)rxBuffer, 2);	// UART RX init
		rtcInit();												// RTC init
		rtcSetAlarm(&dateTime, ALARM_A);						// RTC alarm
	} while (0);

	// =========================================================================
	// Trigger signal for logic analyser
	// =========================================================================

	ledTurnOff();
	_delay_ms(200);
	ledTurnOn();
	_delay_ms(200);
	ledTurnOff();

	// =========================================================================
	// Configures circular buffer
	// =========================================================================

	errorCode = serialRxBuffer.getLastError();
	if (errorCode) {
		myErrorHandler(errorCode);
	}

	// =========================================================================
	// API package
	// =========================================================================

	//  Creates package object
	PackageApi package(255, true);

	do {
		// Configures package object
		errorCode = package.getLastError();
		if (errorCode) {	// ERROR
			myErrorHandler(errorCode);
		}
		if (!package.configCharSet(0x7E, 0x7D, 0x20)) {
			myErrorHandler(package.getLastError());
		}
		// Configures transmitter package data
		if (!package.txSetId(1, true)) {
			myErrorHandler(package.getLastError());
		}
		if (!package.txSetSourceAddress(ADDRESS_PORTABLE)) {
			myErrorHandler(package.getLastError());
		}
		if (!package.txSetDestinationAddress(ADDRESS_BASE)) {
			myErrorHandler(package.getLastError());
		}
	} while (0);

	// =========================================================================
	// Reset system flags
	// =========================================================================

	systemFlags.allFlags = 0;

	// =========================================================================
	// Main loop
	// =========================================================================

	while (1) {

		// =====================================================================
		// Retrieve package from buffer
		// =====================================================================
		while ((!serialRxBuffer.isEmpty()) && (!package.rxIsReady())) {
			uint8_t data;
			if (!serialRxBuffer.pop(&data)) {
				// ERROR
			}
			if (!package.rxAddData(data)) {
				// ERROR
			}
		}

		// =====================================================================
		// Process received package
		// =====================================================================
		if (package.rxIsReady()) {
			uint8_t packageId;
			uint8_t packageDestinationAddress;
			uint8_t packageSourceAddress;
			uint8_t packagePayloadSize;
			uint8_t packagePayloadData[255];

			if (!package.rxGetId(&packageId)) {
				myErrorHandler(package.getLastError());
			}
			if (!package.rxGetDestinationAddress(&packageDestinationAddress)) {
				myErrorHandler(package.getLastError());
			}
			if (!package.rxGetSourceAddress(&packageSourceAddress)) {
				myErrorHandler(package.getLastError());
			}
			if (!package.rxGetPayload(packagePayloadData, &packagePayloadSize, 255)) {
				myErrorHandler(package.getLastError());
			}
			package.rxFreePackage();

			// Sends response back
			uint8_t auxBuff[200];
			uint16_t aux16;

			if (!package.txSetPayload(packagePayloadData, packagePayloadSize)) {
				myErrorHandler(package.getLastError());
			}
			if (!package.txCreatePackage(auxBuff, &aux16, 200)) {
				ledTurnOn();
				while (1);
				myErrorHandler(package.getLastError());
			}
			uartSafeSend(&huart2, auxBuff, aux16);
		}


		// ---------------------------------------------------------------------
		// ALARM A TRIGGER
		// ---------------------------------------------------------------------
		if (systemFlags.triggerAlarmA) {
			// RTC_TimeTypeDef sTime;
			// RTC_DateTypeDef sDate;

			// HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			// HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			// sprintf(auxString, "%02d/%02d/%02d %d %02d:%02d:%02d.%03lu\r", sDate.Date,
			// 		sDate.Month, sDate.Year, sDate.WeekDay, sTime.Hours, sTime.Minutes,
			// 		sTime.Seconds, sTime.SubSeconds);
			// uart2SafeSend((uint8_t *)auxString, strlen(auxString));
			systemFlags.triggerAlarmA = false;
		}

		// ---------------------------------------------------------------------
		// ALARM B TRIGGER
		// ---------------------------------------------------------------------
		if (systemFlags.triggerAlarmB) {
			systemFlags.triggerAlarmB = false;
		}

		// ---------------------------------------------------------------------
		// 200 Hz TRIGGER
		// ---------------------------------------------------------------------
		if (systemFlags.trigger200Hz) {
			systemFlags.trigger200Hz = false;
		}

	}
}

void HAL_RTC_AlarmAEventCallback (RTC_HandleTypeDef * hrtc)
{
	systemFlags.triggerAlarmA = true;
}

void HAL_RTC_AlarmBEventCallback (RTC_HandleTypeDef * hrtc)
{
	systemFlags.triggerAlarmB = true;
}

void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
{
	serialRxBuffer.push(rxBuffer[1]);

}

void HAL_UART_RxHalfCpltCallback (UART_HandleTypeDef * huart)
{
	serialRxBuffer.push(rxBuffer[0]);
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
void HAL_SYSTICK_Callback(void)
{
	ticksCounter++;
	if (ticksCounter == 50) {
		ticksCounter = 0;
		systemFlags.trigger200Hz = true;
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}

#endif /* USE_FULL_ASSERT */
