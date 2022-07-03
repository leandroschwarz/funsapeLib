/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			usart.h
 * Module:			USART module
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __USART_H
#define __USART_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdio.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum usartError_e {
	USART_NO_ERRORS = 0,
	USART_FRAME_ERROR = 1,
	USART_PARITY_ERROR  = 2,
	USART_BUFFER_OVERFLOW_ERROR = 4
} usartError_e;

typedef enum usartFormat_e {
	//			= 0bDDDDPPSS
	USART_FORMAT_8_N_1		= 0b10000001,
	USART_FORMAT_8_O_1		= 0b10000101,
	USART_FORMAT_8_E_1		= 0b10001001,
	USART_FORMAT_8_N_2		= 0b10000010,
	USART_FORMAT_8_O_2		= 0b10000110,
	USART_FORMAT_8_E_2		= 0b10001010,
	USART_FORMAT_9_N_1		= 0b10010001,
	USART_FORMAT_9_O_1		= 0b10010101,
	USART_FORMAT_9_E_1		= 0b10011001,
	USART_FORMAT_9_N_2		= 0b10010010,
	USART_FORMAT_9_O_2		= 0b10010110,
	USART_FORMAT_9_E_2		= 0b10011010,
	USART_FORMAT_5_N_1		= 0b01010001,
	USART_FORMAT_5_O_1		= 0b01010101,
	USART_FORMAT_5_E_1		= 0b01011001,
	USART_FORMAT_5_N_2		= 0b01010010,
	USART_FORMAT_5_O_2		= 0b01010110,
	USART_FORMAT_5_E_2		= 0b01011010,
	USART_FORMAT_6_N_1		= 0b01100001,
	USART_FORMAT_6_O_1		= 0b01100101,
	USART_FORMAT_6_E_1		= 0b01101001,
	USART_FORMAT_6_N_2		= 0b01100010,
	USART_FORMAT_6_O_2		= 0b01100110,
	USART_FORMAT_6_E_2		= 0b01101010,
	USART_FORMAT_7_N_1		= 0b01110001,
	USART_FORMAT_7_O_1		= 0b01110101,
	USART_FORMAT_7_E_1		= 0b01111001,
	USART_FORMAT_7_N_2		= 0b01110010,
	USART_FORMAT_7_O_2		= 0b01110110,
	USART_FORMAT_7_E_2		= 0b01111010,
	USART_FORMAT_NO_CHANGE	= 0b00000000
} usartFormat_e;

typedef enum usartMode_e {
	USART_MODE_ASYNCHRONOUS = 0,
	USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED,
	USART_MODE_SYNCHRONOUS_XCK_FALLING,
	USART_MODE_SYNCHRONOUS_XCK_RISING,
	USART_MODE_SYNCHRONOUS_SPI,
	USART_MODE_NO_CHANGE = 0xFF
} usartMode_e;

typedef enum usartBaudRate_e {
	USART_BAUD_600 = 600UL,
	USART_BAUD_1200 = 1200UL,
	USART_BAUD_1800 = 1800UL,
	USART_BAUD_2400 = 2400UL,
	USART_BAUD_4800 = 4800UL,
	USART_BAUD_9600 = 9600UL,
	USART_BAUD_14400 = 14400UL,
	USART_BAUD_19200 = 19200UL,
	USART_BAUD_28800 = 28800UL,
	USART_BAUD_38400 = 38400UL,
	USART_BAUD_56000 = 56000UL,
	USART_BAUD_57600 = 57600UL,
	USART_BAUD_115200 = 115200UL,
	USART_BAUD_128000 = 128000UL,
	USART_BAUD_230400 = 230400UL,
	USART_BAUD_256000 = 256000UL,
	USART_BAUD_NO_CHANGE = 0xFFFFFFFFUL
} usartBaudRate_e;
#define usartBaudRateListAll() {USART_BAUD_600, USART_BAUD_1200, USART_BAUD_1800, USART_BAUD_2400, USART_BAUD_4800, USART_BAUD_9600, USART_BAUD_14400, USART_BAUD_19200, USART_BAUD_28800, USART_BAUD_38400, USART_BAUD_56000, USART_BAUD_57600, USART_BAUD_115200, USART_BAUD_128000, USART_BAUD_230400, USART_BAUD_256000}

// -----------------------------------------------------------------------------
// Global variable declarations ------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void			usartInit(usartMode_e mode, usartBaudRate_e baudRate, usartFormat_e format);
void			usartEnableReceiver(void);
void			usartDisableReceiver(void);
void			usartEnableTransmitter(void);
void			usartDisableTransmitter(void);
void			usartActivateReceptionCompleteInterrupt(void);
void			usartDeactivateReceptionCompleteInterrupt(void);
void			usartActivateTransmissionCompleteInterrupt(void);
void			usartDeactivateTransmissionCompleteInterrupt(void);
void			usartActivateBufferEmptyInterrupt(void);
void			usartDeactivateBufferEmptyInterrupt(void);
bool_t			usartIsReceptionComplete(void);
bool_t			usartIsTransmissionComplete(void);
bool_t			usartIsBufferEmpty(void);
usartError_e	usartCheckErrors(void);
void			usartTransmit(uint8_t data);
void			usartTransmit9bits(uint16_t data);
uint8_t			usartReceive(void);
uint16_t		usartReceive9bits(void);
void			usartFlushReceptionBuffer(void);
void			usartStdio(void);

#endif
