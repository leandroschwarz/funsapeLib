/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			usart.h
 * Module:			USART interface
 * Author:			Leandro Schwarz
 * Version:			13.0
 * Last edition:	2017-06-17
 * -------------------------------------------------------------------------- */

#ifndef __USART_H
#define __USART_H 130

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBALDEFINES_H != 130
	#error Error 100 - globalDefines.h - wrong version (globalDefines must be version 13.0).
#endif
#include <stdio.h>

// -----------------------------------------------------------------------------
// Global variable declarations ------------------------------------------------

extern FILE usartStream;

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum usartError_t{
	USART_NO_ERRORS = 0,
	USART_FRAME_ERROR = 1,
	USART_PARITY_ERROR  = 2,
	USART_BUFFER_OVERFLOW_ERROR = 4
} usartError_t;

typedef enum usartDataBits_t{
	USART_DATA_BITS_5 = 0,
	USART_DATA_BITS_6,
	USART_DATA_BITS_7,
	USART_DATA_BITS_8,
	USART_DATA_BITS_9,
	USART_DATA_BITS_NO_CHANGE = 255
} usartDataBits_t;

typedef enum usartStopBits_t{
	USART_STOP_BIT_SINGLE = 0,
	USART_STOP_BIT_DOUBLE,
	USART_STOP_BIT_NO_CHANGE = 255
} usartStopBits_t;

typedef enum usartParity_t{
	USART_PARITY_NONE = 0,
	USART_PARITY_ODD,
	USART_PARITY_EVEN,
	USART_PARITY_NO_CHANGE = 255
} usartParity_t;

typedef enum usartMode_t{
	USART_MODE_ASYNCHRONOUS = 0,
	USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED,
	USART_MODE_SYNCHRONOUS_XCK_FALLING,
	USART_MODE_SYNCHRONOUS_XCK_RISING,
	USART_MODE_SYNCHRONOUS_SPI,
	USART_MODE_NO_CHANGE = 255
} usartMode_t;

typedef enum usartBaudRate_t{
	USART_BAUD_600 = 600UL,
	USART_BAUD_1200 = 1200UL,
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
	USART_BAUD_256000 = 256000UL,
	USART_BAUD_NO_CHANGE = 0xFFFFFFFFUL
} usartBaudRate_t;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

resultValue_t	usartConfig(usartMode_t mode, usartBaudRate_t baudRate, usartDataBits_t dataBits, usartParity_t parity, usartStopBits_t stopBits);
resultValue_t	usartEnableReceiver(void);
resultValue_t	usartDisableReceiver(void);
resultValue_t	usartEnableTransmitter(void);
resultValue_t	usartDisableTransmitter(void);
// resultValue_t	usartClearReceptionCompleteInterrupt(void);
resultValue_t	usartActivateReceptionCompleteInterrupt(void);
resultValue_t	usartDeactivateReceptionCompleteInterrupt(void);
// resultValue_t	usartClearTransmissionCompleteInterrupt(void);
resultValue_t	usartActivateTransmissionCompleteInterrupt(void);
resultValue_t	usartDeactivateTransmissionCompleteInterrupt(void);
// resultValue_t	usartClearBufferEmptyInterrupt(void);
resultValue_t	usartActivateBufferEmptyInterrupt(void);
resultValue_t	usartDeactivateBufferEmptyInterrupt(void);
bool_t			usartIsReceptionComplete(void);
bool_t			usartIsTransmissionComplete(void);
bool_t			usartIsBufferEmpty(void);
usartError_t	usartCheckError(void);
resultValue_t	usartTransmit(uint8 data);
resultValue_t	usartTransmit9bits(uint16 data);
uint8			usartReceive(void);
uint16			usartReceive9bits(void);
void			usartClearReceptionBuffer(void);
resultValue_t	usartStdio(void);
resultValue_t	usartTransmitStd(int8 data, FILE * stream);
int16			usartReceiveStd(FILE * stream);

#endif
