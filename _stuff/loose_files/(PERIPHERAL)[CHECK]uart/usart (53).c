/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			usart.c
 * Module:			USART module
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "usart.h"
#if __USART_H != 1
	#error Error 101 - Build mismatch on header and source code files (usart).
#endif

// -----------------------------------------------------------------------------
// Private function declarations -----------------------------------------------

int16_t usartTransmitStd(int8_t data, FILE * stream);
int16_t usartReceiveStd(FILE * stream);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE usartStream = FDEV_SETUP_STREAM(usartTransmitStd, NULL, _FDEV_SETUP_WRITE);

// -----------------------------------------------------------------------------
// Private functions -----------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	usartTransmitStd
 * Purpose:		Transmits data in 5, 6, 7 or 8 bits modes using the USART
 *				controller and the standard output handler
 * Arguments:	data		data to be sent
 *				stream		pointer to stdout handler
 * Returns:		none
 * -------------------------------------------------------------------------- */

int16_t  usartTransmitStd(int8_t data, FILE * stream)
{
	while(!usartIsBufferEmpty())
		;	// Waits until last transmission ends
	UDR0 = data;

	return 0;
}

/* -----------------------------------------------------------------------------
 * Function:	usartTransmitStd
 * Purpose:		Receives data in 5, 6, 7 or 8 bits modes using the USART
 *				controller and the standard output handler
 * Arguments:	stream		pointer to stdin handler
 * Returns:		int8_t		data read from stream
 * -------------------------------------------------------------------------- */

int16_t usartReceiveStd(FILE * stream)
{
	while(!usartIsReceptionComplete())
		;	// Waits until last reception ends

	return (int16_t)UDR0;
}

// -----------------------------------------------------------------------------
// Public functions ------------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	usartInit
 * Purpose:		Initializes the USART module
 * Arguments:	mode		usartMode_t enumeration
 *				baudRate	usartBaudRate_t enumeration
 *				dataBits	usartDataBits_t enumeration
 *				parity		usartParity_t enumeration
 *				stopBits	usartStopBits_t enumeration
 * Returns:		none
 * -------------------------------------------------------------------------- */

void usartInit(usartMode_t mode, usartBaudRate_t baudRate, usartDataBits_t dataBits, usartParity_t parity, usartStopBits_t stopBits)
{
	uint8_t reg1 = UCSR0A;
	uint8_t reg2 = UCSR0B;
	uint8_t reg3 = UCSR0C;
	uint8_t modeAux = 0;
	uint8_t aux8 = 0;
	uint64_t aux64 = (uint64_t)UBRR0;

	// Clear errors
	reg1 &= ~((1 << FE0) | (1 << DOR0) | (1 << UPE0));

	// USART stop bits
	if(stopBits != USART_STOP_BIT_NO_CHANGE) {
		switch(stopBits) {
		case USART_STOP_BIT_SINGLE:
			clrBit(reg3, USBS0);
			break;
		case USART_STOP_BIT_DOUBLE:
			setBit(reg3, USBS0);
			break;
		default:
			break;
		}
	}

	// USART parity bits
	if(parity != USART_PARITY_NO_CHANGE) {
		reg3 &= ~(0x03 << UPM00);
		switch(parity) {
		case USART_PARITY_NONE:
			break;
		case USART_PARITY_EVEN:
			reg3 |= (0x02 << UPM00);
			break;
		case USART_PARITY_ODD:
			reg3 |= (0x03 << UPM00);
			break;
		default:
			break;
		}
	}

	// USART data bits
	if(dataBits != USART_DATA_BITS_NO_CHANGE) {
		clrBit(reg2, UCSZ02);
		reg3 &= ~(0x03 << UCSZ00);
		switch(dataBits) {
		case USART_DATA_BITS_5:
			break;
		case USART_DATA_BITS_6:
			reg3 |= (0x01 << UCSZ00);
			break;
		case USART_DATA_BITS_7:
			reg3 |= (0x02 << UCSZ00);
			break;
		case USART_DATA_BITS_8:
			reg3 |= (0x03 << UCSZ00);
			break;
		case USART_DATA_BITS_9:
			setBit(reg2, UCSZ02);
			reg3 |= (0x03 << UCSZ00);
			break;
		default:
			break;
		}
	}

	// USART mode
	if(mode != USART_MODE_NO_CHANGE) {
		modeAux = mode;
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
			break;
		}
	} else {
		aux8 = (0x03 & (reg3 >> UMSEL00));
		switch(aux8) {
		case 0:	// Asynchronous mode
			if(isBitClr(reg1, U2X0)) {	// Single speed
				modeAux = USART_MODE_ASYNCHRONOUS;
			} else {						// Double speed
				modeAux = USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED;
			}
			break;
		case 3: // Synchronous SPI mode
			modeAux = USART_MODE_SYNCHRONOUS_SPI;
			break;
		}
	}

	// USART baud rate
	if(baudRate != USART_BAUD_NO_CHANGE) {
		switch(modeAux) {
		case USART_MODE_ASYNCHRONOUS:
			aux64 = (systemConfiguration.systemClock / 16 / baudRate) - 1;
			break;
		case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
			aux64 = (systemConfiguration.systemClock / 8 / baudRate) - 1;
			break;
		case USART_MODE_SYNCHRONOUS_SPI:
			aux64 = (systemConfiguration.systemClock / 2 / baudRate) - 1;
			break;
		}
	}

	// Configures USART
	UCSR0A = reg1;
	UCSR0B = reg2;
	UCSR0C = reg3;
	UBRR0H = (uint8_t)(0x0F & (aux64 >> 8));
	UBRR0L = (uint8_t)(0xFF & aux64);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	usartEnableReceiver
 * Purpose:		Enables the USART receiver
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline usartEnableReceiver(void)
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

void inline usartDisableReceiver(void)
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

void inline usartEnableTransmitter(void)
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

void inline usartDisableTransmitter(void)
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

void inline usartActivateReceptionCompleteInterrupt(void)
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

void inline usartDeactivateReceptionCompleteInterrupt(void)
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

void inline usartActivateTransmissionCompleteInterrupt(void)
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

void inline usartDeactivateTransmissionCompleteInterrupt(void)
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

void inline usartActivateBufferEmptyInterrupt(void)
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

void inline usartDeactivateBufferEmptyInterrupt(void)
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

bool_t inline usartIsReceptionComplete(void)
{
	return isBitSet(UCSR0A, RXC0);
}

/* -----------------------------------------------------------------------------
 * Function:	usartIsTransmissionComplete
 * Purpose:		Checks if a transmission has been completed
 * Arguments:	none
 * Returns:		bool_t		TRUE / FALSE
 * -------------------------------------------------------------------------- */

bool_t inline usartIsTransmissionComplete(void)
{
	return isBitSet(UCSR0A, TXC0);
}

/* -----------------------------------------------------------------------------
 * Function:	usartIsBufferEmpty
 * Purpose:		Checks if the buffer is empty
 * Arguments:	none
 * Returns:		bool_t		TRUE / FALSE
 * -------------------------------------------------------------------------- */

bool_t inline usartIsBufferEmpty(void)
{
	return isBitSet(UCSR0A, UDRE0);
}

/* -----------------------------------------------------------------------------
 * Function:	usartCheckErrors
 * Purpose:		Checks if an error occurred during last operation
 * Arguments:	none
 * Returns:		usartError_t	error flags
 * -------------------------------------------------------------------------- */

usartError_t usartCheckErrors(void)
{
	usartError_t error = USART_NO_ERRORS;

	if(isBitSet(UCSR0A, FE0)) {
		error |= USART_FRAME_ERROR;
	}
	if(isBitSet(UCSR0A, DOR0)) {
		error |= USART_PARITY_ERROR;
	}
	if(isBitSet(UCSR0A, UPE0)) {
		error |= USART_BUFFER_OVERFLOW_ERROR;
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
	while(!usartIsBufferEmpty())
		;	// Waits until last transmission ends
	UDR0 = data;

	return;
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

	while(!usartIsBufferEmpty())
		;	// Waits until last transmission ends
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
 * Function:	usartTransmit9bits
 * Purpose:		Receives data in 5, 6, 7 or 8 bits using the USART controller
 * Arguments:	none
 * Returns:		uint8_t	data received
 * -------------------------------------------------------------------------- */

uint8_t usartReceive(void)
{
	__attribute__((unused)) vuint8_t status;

	while(!usartIsReceptionComplete())
		;	// Waits until last reception ends
	status = UCSR0A;

	return UDR0;
}

/* -----------------------------------------------------------------------------
 * Function:	usartTransmit9bits
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

	while(!usartIsReceptionComplete())
		;	// Waits until last reception ends
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

/* -----------------------------------------------------------------------------
 * Function:	usartStdio
 * Purpose:		Changes the standard handlers to USART controller
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline usartStdio(void)
{
	stdin = stdout = stderr = &usartStream;

	return;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

/* NONE */
