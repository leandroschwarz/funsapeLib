/* =============================================================================
 * Project:		demiFatFs example application
 * File name:	main.cpp
 * Authors:		Leandro Schwarz
 * Date:		August 24, 2018
 * ========================================================================== */

// =============================================================================
// Include files
// =============================================================================

#include <avr/interrupt.h>
#include <demiFatFs.hpp>
#include <globalDefines.hpp>
#include <stdio.h>
#include <string.h>
#include <usart0.hpp>
#include <timer.hpp>
#include <mmc_avr.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// Data Stored in Flash
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Function declarations
// =============================================================================

// NONE

// =============================================================================
// Main function
// =============================================================================

// =============================================================================
// Main function ==============================================================-

int main(void)
{
	// =========================================================================
	// General purpose variables
	// =========================================================================

	char auxStr[40];
//	uint16_t totalBytes;
//	uint8_t byte;

	// =========================================================================
	// Variable initialization
	// =========================================================================

	// NONE

	// =========================================================================
	// Enable Global Interrupts
	// =========================================================================

	sei();

	// =========================================================================
	// USART configuration
	// =========================================================================

	Usart0 usart0;
	usart0.setBaudRate(USART_BAUD_57600);
	usart0.initialize();
	usart0.enableTransmitter();
	usart0.stdio();
	printf("Teste da USART\r");

	// =========================================================================
	// TIMER configuration
	// =========================================================================

	timer0.init(TIMER0_MODE_NORMAL, TIMER0_CLOCK_PRESCALER_1024);
	timer0.activateOverflowInterrupt();

	// =========================================================================
	// SD Card configuration
	// =========================================================================

	DemiFatFs sdCard;
	printf_P(PSTR("Object Creation             = %d\r"), sdCard.getLastError());
	printf_P(PSTR("	Object FATFS            = %x\r"), (unsigned int)(sdCard.card_));
	printf_P(PSTR("	Object FIL              = %x\r"), (unsigned int)(sdCard.file_));
	printf_P(PSTR("	Object FILEINFO         = %x\r"), (unsigned int)(sdCard.fno_));
	printf_P(PSTR("	Object DIR              = %x\r"), (unsigned int)(sdCard.directory_));
	sdCard.mount();
	printf_P(PSTR("Card Mounting               = %d\r"), sdCard.getLastError());
	sdCard.opendir("");
	printf_P(PSTR("Card Openning Directory     = %d\r"), sdCard.getLastError());
	while (1) {
		sdCard.readdir();
		printf_P(PSTR("Card Reading Directory      = %d\r"), sdCard.getLastError());

		// 	res = f_readdir(&directory, &fno);
		// printf("f_opendir = %d\r", res);
		// 	if (res != FR_OK || fno.fname[0] == 0) {
		// 		break;
		// 	}
		// 	printf("%s\r", fno.fname);

	}
	sdCard.closedir();
	printf_P(PSTR("Card Closing Directory      = %d\r"), sdCard.getLastError());



	// Read file
	// byte = 0;
	// res = f_open(&outputFile, "file2.txt", FA_READ | FA_OPEN_EXISTING);
	// if (res) {
	// 	printf("f_open = %d => File does not exists\r", res);
	// } else {
	// 	printf("f_open = %d => File do exists\r", res);
	// 	totalBytes = 1;
	// 	while (totalBytes) {
	// 		res = f_read(&outputFile, &byte, 1, (UINT *)&totalBytes);
	// 	}
	// 	printf("%d\r", byte);
	// 	byte++;
	// 	f_close(&outputFile);
	// }

	// // Write file
	// res = f_open(&outputFile, "file2.txt", FA_WRITE | FA_OPEN_APPEND);
	// printf("f_open = %d => File openned\r", res);

	// f_write(&outputFile, &byte, 1, (UINT *)&totalBytes);
	// f_sync(&outputFile);
	// f_close(&outputFile);

	while (1) {
	}

	return 0;
}

// =============================================================================
// Interruption handlers ======================================================-

ISR(TIMER0_OVF_vect)
{
	mmc_disk_timerproc();
}

// =============================================================================
// Function definitions ========================================================
