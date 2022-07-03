/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			usart.c
 * Module:			USART module
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			2
 * Last edition:	April 22, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "usart.h"
#if __USART_H != 2
	#error Error 101 - Build mismatch on header and source code files (usart).
#endif

// -----------------------------------------------------------------------------
// Private function declarations -----------------------------------------------

int16_t usartTransmitStd(char data, FILE * stream);
char usartReceiveStd(FILE * stream);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE usartStream = FDEV_SETUP_STREAM(usartTransmitStd, NULL, _FDEV_SETUP_WRITE);

// -----------------------------------------------------------------------------
// Static functions definitions ------------------------------------------------

// -----------------------------------------------------------------------------
// Public functions definitions ------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	usartConfig
 * Purpose:		Configures the USART module
 * Arguments:	mode		usartMode_e enumeration
 *				baudRate	usartBaudRate_e enumeration
 *				format		usartFormat_e enumeration
 * Returns:		error_e		error description enumeration
 * -------------------------------------------------------------------------- */

error_e usartConfig(usartMode_e mode, usartBaudRate_e baudRate, usartFormat_e format)
{
	uint8_t reg1 = UCSR0A;
	uint8_t reg2 = UCSR0B;
	uint8_t reg3 = UCSR0C;
	uint8_t aux8 = 0;
	uint64_t aux64 = (uint64_t)UBRR0;

	// Clear errors
	reg1 &= ~((1 << FE0) | (1 << DOR0) | (1 << UPE0));

	if(format != USART_FORMAT_NO_CHANGE) {
		// USART stop bits
		aux8 = format & 0x03;
		switch(aux8) {
		case 1:						// Single stop bit
			clrBit(reg3, USBS0);
			break;
		case 2:						// Double stop bit
			setBit(reg3, USBS0);
			break;
		default:					// Error
			return ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT;
		}

		// USART parity bits
		aux8 = (format >> 2) & 0x03;
		reg3 &= ~(0x03 << UPM00);
		switch(aux8) {
		case 0:						// Parity NONE
			break;
		case 1:						// Parity ODD
			reg3 |= (0x03 << UPM00);
			break;
		case 2:						// Parity EVEN
			reg3 |= (0x02 << UPM00);
			break;
		default:					// Error
			return ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY;
		}

		// USART data bits
		aux8 = (format >> 4) & 0x0F;
		clrBit(reg2, UCSZ02);
		reg3 &= ~(0x03 << UCSZ00);
		switch(aux8) {
		case 5:						// 5 data bits
			break;
		case 6:						// 6 data bits
			reg3 |= (0x01 << UCSZ00);
			break;
		case 7:						// 7 data bits
			reg3 |= (0x02 << UCSZ00);
			break;
		case 8:						// 8 data bits
			reg3 |= (0x03 << UCSZ00);
			break;
		case 9:						// 9 data bits
			setBit(reg2, UCSZ02);
			reg3 |= (0x03 << UCSZ00);
			break;
		default:						// 9 data bits
			return ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS;
		}
	}

	// USART mode
	if(mode != USART_MODE_NO_CHANGE) {
		reg3 &= ~(0x03 << UMSEL00);
		clrBit(reg1, U2X0);				// Single Speed
		clrBit(reg3, UCPOL0);			// Polarity
		switch(mode) {
		case USART_MODE_ASYNCHRONOUS:
			break;
		case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
			setBit(reg1, U2X0);			// Double Speed
			break;
		case USART_MODE_SYNCHRONOUS_XCK_FALLING:
			reg3 |= (1 << UMSEL00);		// Synchronous Mode
			break;
		case USART_MODE_SYNCHRONOUS_XCK_RISING:
			reg3 |= (1 << UMSEL00);		// Synchronous Mode
			break;
		case USART_MODE_SYNCHRONOUS_SPI:
			reg3 |= (3 << UMSEL00);		// Synchronous SPI Mode
			setBit(reg3, UCPOL0);		// Polarity
			break;
		default:
			return ERROR_USART_UNSUPPORTED_CONFIGURATION_MODE;
		}
	} else {	// Retrieve current mode
		aux8 = (0x03 & (reg3 >> UMSEL00));
		switch(aux8) {
		case 0:	// Asynchronous mode
			if(isBitClr(reg1, U2X0)) {	// Single speed
				mode = USART_MODE_ASYNCHRONOUS;
			} else {						// Double speed
				mode = USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED;
			}
			break;
		case 3: // Synchronous SPI mode
			mode = USART_MODE_SYNCHRONOUS_SPI;
			break;
		}
	}

	// USART baud rate
	if(baudRate > USART_BAUD_256000) {
		return ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE;
	}
	if(baudRate != USART_BAUD_NO_CHANGE) {
		switch(mode) {
		case USART_MODE_ASYNCHRONOUS:
			aux64 = (systemConfiguration.systemClock / 16 / baudRate) - 1;
			break;
		case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
			aux64 = (systemConfiguration.systemClock / 8 / baudRate) - 1;
			break;
		case USART_MODE_SYNCHRONOUS_SPI:
			aux64 = (systemConfiguration.systemClock / 2 / baudRate) - 1;
			break;
		default:
			return ERROR_USART_UNSUPPORTED_CONFIGURATION_MODE;
		}
	}

	// Configures USART registers
	UCSR0A = reg1;
	UCSR0B = reg2;
	UCSR0C = reg3;
	UBRR0H = (uint8_t)(0x0F & (aux64 >> 8));
	UBRR0L = (uint8_t)(0xFF & aux64);

	return ERROR_NONE;
}

/* -----------------------------------------------------------------------------
 * Function:	usartCheckErrors
 * Purpose:		Checks if an error occurred during last operation
 * Arguments:	none
 * Returns:		usartError_f	error flags
 * -------------------------------------------------------------------------- */

usartError_f usartCheckErrors(void)
{
	usartError_f error = USART_ERROR_OK;

	if(isBitSet(UCSR0A, FE0)) {
		error |= USART_ERROR_FRAME;
	}
	if(isBitSet(UCSR0A, DOR0)) {
		error |= USART_ERROR_PARITY;
	}
	if(isBitSet(UCSR0A, UPE0)) {
		error |= USART_ERROR_BUFFER_OVERFLOW;
	}

	return error;
}

/* -----------------------------------------------------------------------------
 * Function:	usartTransmit
 * Purpose:		Transmits data in 5, 6, 7 or 8 bits using the USART controller
 * Arguments:	data		data to be transmitted
 * Returns:		none
 * -------------------------------------------------------------------------- */

void usartTransmit(uint8_t data)
{
	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	UDR0 = data;
	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartTransmit9bits
 * Purpose:		Receives data in 5, 6, 7 or 8 bits using the USART controller
 * Arguments:	none
 * Returns:		uint8_t	data received
 * -------------------------------------------------------------------------- */

uint8_t usartReceive(void)
{
	__attribute__((unused)) vuint8_t status;

	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends
	status = UCSR0A;
	return UDR0;
}

/* -----------------------------------------------------------------------------
 * Function:	usartTransmit9bits
 * Purpose:		Transmits data in 9 bits using the USART controller
 * Arguments:	data		data to be transmitted
 * Returns:		none
 * -------------------------------------------------------------------------- */

void usartTransmit9bits(uint16_t data)
{
	__attribute__((unused)) vuint8_t aux;

	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	aux = ((data & 0x100) >> 8);
	if(aux) {
		setBit(UCSR0B, TXB80);
	} else {
		clrBit(UCSR0B, TXB80);
	}
	UDR0 = (uint8_t)data;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartReceive9bits
 * Purpose:		Receives data in 9 bits using the USART controller
 * Arguments:	none
 * Returns:		uint16_t	data received
 * -------------------------------------------------------------------------- */

uint16_t usartReceive9bits(void)
{
	__attribute__((unused)) vuint8_t status;
	uint8_t byteh;
	uint8_t bytel;
	uint16_t byte;

	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends
	status = UCSR0A;
	byteh = UCSR0B;
	bytel = UDR0;

	byte = (uint16_t)(byteh & 0x02) << 7;
	byte |= bytel;

	return  byte;
}

/* -----------------------------------------------------------------------------
 * Function:	usartFlushReceptionBuffer
 * Purpose:		Flushes the reception buffer
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void usartFlushReceptionBuffer(void)
{
	__attribute__((unused)) vuint8_t aux8;

	while(usartIsReceptionComplete()) {
		aux8 = UDR0;
	}

	return;
}

// -----------------------------------------------------------------------------
// Hidden functions definitions ------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	usartReceiveStd
 * Purpose:		Receives data in 5, 6, 7 or 8 bits modes using the USART
 *				controller and the standard output handler
 * Arguments:	stream		pointer to stdin handler
 * Returns:		char		data read from stream
 * -------------------------------------------------------------------------- */

char usartReceiveStd(FILE * stream)
{

	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends

	return (int16_t)UDR0;
}

/* -----------------------------------------------------------------------------
 * Function:	usartTransmitStd
 * Purpose:		Transmits data in 5, 6, 7 or 8 bits modes using the USART
 *				controller and the standard output handler
 * Arguments:	data		data to be sent
 *				stream		pointer to stdout handler
 * Returns:		none
 * -------------------------------------------------------------------------- */

int16_t usartTransmitStd(char data, FILE * stream)
{
	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	UDR0 = data;

	return 0;
}
