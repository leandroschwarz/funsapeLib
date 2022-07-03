/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			usart.h
 * Module:			USART module
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			2
 * Last edition:	April 22, 2018
 * -------------------------------------------------------------------------- */

#ifndef __USART_H
#define __USART_H 2

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif
#include <stdio.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum usartError_f {
	USART_ERROR_OK					= 0,
	USART_ERROR_FRAME				= 1,
	USART_ERROR_PARITY				= 2,
	USART_ERROR_BUFFER_OVERFLOW		= 4
} usartError_f;

typedef enum usartFormat_e {
	//						= 0bDDDDPPSS
	// DDDD -> Number of data bits (5-8)
	// PP   -> Parity (NONE / ODD / EVEN)
	// SS   -> Number of stop bits (1-2)
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
	USART_MODE_ASYNCHRONOUS					= 0,
	USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED	= 1,
	USART_MODE_SYNCHRONOUS_XCK_FALLING		= 2,
	USART_MODE_SYNCHRONOUS_XCK_RISING		= 3,
	USART_MODE_SYNCHRONOUS_SPI				= 4,
	USART_MODE_NO_CHANGE					= 5
} usartMode_e;

typedef enum usartBaudRate_e {
	USART_BAUD_NO_CHANGE	= 0UL,
	USART_BAUD_600			= 600UL,
	USART_BAUD_1200			= 1200UL,
	USART_BAUD_1800			= 1800UL,
	USART_BAUD_2400			= 2400UL,
	USART_BAUD_4800			= 4800UL,
	USART_BAUD_9600			= 9600UL,
	USART_BAUD_14400		= 14400UL,
	USART_BAUD_19200		= 19200UL,
	USART_BAUD_28800		= 28800UL,
	USART_BAUD_38400		= 38400UL,
	USART_BAUD_56000		= 56000UL,
	USART_BAUD_57600		= 57600UL,
	USART_BAUD_115200		= 115200UL,
	USART_BAUD_128000		= 128000UL,
	USART_BAUD_230400		= 230400UL,
	USART_BAUD_256000		= 256000UL
} usartBaudRate_e;

// -----------------------------------------------------------------------------
// Extern variable declarations ------------------------------------------------

extern FILE usartStream;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define usartBaudRateListAll() {USART_BAUD_600, USART_BAUD_1200, USART_BAUD_1800, USART_BAUD_2400, USART_BAUD_4800, USART_BAUD_9600, USART_BAUD_14400, USART_BAUD_19200, USART_BAUD_28800, USART_BAUD_38400, USART_BAUD_56000, USART_BAUD_57600, USART_BAUD_115200, USART_BAUD_128000, USART_BAUD_230400, USART_BAUD_256000}

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

error_e			usartConfig(usartMode_e mode, usartBaudRate_e baudRate, usartFormat_e format);
usartError_f	usartCheckErrors(void);
void			usartTransmit(uint8_t data);
void			usartTransmit9bits(uint16_t data);
uint8_t			usartReceive(void);
uint16_t		usartReceive9bits(void);
void			usartFlushReceptionBuffer(void);

// -----------------------------------------------------------------------------
// Static functions definitions ------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	usartEnableReceiver
 * Purpose:		Enables the USART receiver
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartEnableReceiver(void)
{
	setBit(UCSR0B, RXEN0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartDisableReceiver
 * Purpose:		Disables the USART receiver
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartDisableReceiver(void)
{
	clrBit(UCSR0B, RXEN0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartEnableTransmitter
 * Purpose:		Enables the USART transmitter
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartEnableTransmitter(void)
{
	setBit(UCSR0B, TXEN0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartDisableTransmitter
 * Purpose:		Disables the USART transmitter
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartDisableTransmitter(void)
{
	clrBit(UCSR0B, TXEN0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartActivateReceptionCompleteInterrupt
 * Purpose:		Activates the USART reception complete interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartActivateReceptionCompleteInterrupt(void)
{
	setBit(UCSR0B, RXCIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartDeactivateReceptionCompleteInterrupt
 * Purpose:		Deactivates the USART reception complete interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartDeactivateReceptionCompleteInterrupt(void)
{
	clrBit(UCSR0B, RXCIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartActivateTransmissionCompleteInterrupt
 * Purpose:		Activates the USART transmission complete interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartActivateTransmissionCompleteInterrupt(void)
{
	setBit(UCSR0B, TXCIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartDeactivateTransmissionCompleteInterrupt
 * Purpose:		Deactivates the USART transmission complete interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartDeactivateTransmissionCompleteInterrupt(void)
{
	clrBit(UCSR0B, TXCIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartActivateBufferEmptyInterrupt
 * Purpose:		Activates the USART buffer empty interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartActivateBufferEmptyInterrupt(void)
{
	setBit(UCSR0B, UDRIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartDeactivateBufferEmptyInterrupt
 * Purpose:		Deactivates the USART buffer empty interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartDeactivateBufferEmptyInterrupt(void)
{
	clrBit(UCSR0B, UDRIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartIsReceptionComplete
 * Purpose:		Checks if a reception has been completed
 * Arguments:	none
 * Returns:		bool_t		TRUE / FALSE
 * -------------------------------------------------------------------------- */

bool_t inline __attribute__((always_inline)) usartIsReceptionComplete(void)
{
	return isBitSet(UCSR0A, RXC0);
}

/* -----------------------------------------------------------------------------
 * Function:	usartIsTransmissionComplete
 * Purpose:		Checks if a transmission has been completed
 * Arguments:	none
 * Returns:		bool_t		TRUE / FALSE
 * -------------------------------------------------------------------------- */

bool_t inline __attribute__((always_inline)) usartIsTransmissionComplete(void)
{
	return isBitSet(UCSR0A, TXC0);
}

/* -----------------------------------------------------------------------------
 * Function:	usartIsBufferEmpty
 * Purpose:		Checks if the buffer is empty
 * Arguments:	none
 * Returns:		bool_t		TRUE / FALSE
 * -------------------------------------------------------------------------- */

bool_t inline __attribute__((always_inline)) usartIsBufferEmpty(void)
{
	return isBitSet(UCSR0A, UDRE0);
}

/* -----------------------------------------------------------------------------
 * Function:	usartStdio
 * Purpose:		Changes the standard handlers to USART controller
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) usartStdio(void)
{
	stdin = stdout = stderr = &usartStream;

	return;
}

#endif
