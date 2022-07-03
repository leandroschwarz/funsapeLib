/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			usart.c
 * Module:			USART interface
 * Author:			Leandro Schwarz
 * Version:			13.0
 * Last edition:	2017-06-17
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "usart.h"
#if __USART_H != 130
	#error Error 101 - Version mismatch on header and source code files (usart).
#endif

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE usartStream = FDEV_SETUP_STREAM(usartTransmitStd, usartReceiveStd, _FDEV_SETUP_RW);

/* -----------------------------------------------------------------------------
 * Configures the USART controller
 * -------------------------------------------------------------------------- */

resultValue_t usartConfig(usartMode_t mode, usartBaudRate_t baudRate, usartDataBits_t dataBits, usartParity_t parity, usartStopBits_t stopBits)
{
	uint8 reg1 = UCSR0A;
	uint8 reg2 = UCSR0B;
	uint8 reg3 = UCSR0C;
	uint8 modeAux = 0;
	uint8 aux8 = 0;
	uint64 aux64 = 0;

	// Clear errors
	reg1 &= ~((1 << FE0) | (1 << DOR0) | (1 << UPE0));

	// USART stop bits
	if(stopBits != USART_STOP_BIT_NO_CHANGE){
		switch(stopBits){
			case USART_STOP_BIT_SINGLE:	clrBit(reg3, USBS0);				break;
			case USART_STOP_BIT_DOUBLE:	setBit(reg3, USBS0);				break;
			default:					return RESULT_UNSUPPORTED_USART_STOP_BITS;
		}
	}

	// USART parity bits
	if(parity != USART_PARITY_NO_CHANGE){
		reg3 &= ~(0x03 << UPM00);
		switch(parity){
			case USART_PARITY_NONE:		break;
			case USART_PARITY_EVEN:		reg3 |= (0x02 << UPM00);	break;
			case USART_PARITY_ODD:		reg3 |= (0x03 << UPM00);	break;
			default:					return RESULT_UNSUPPORTED_USART_PARITY;
		}
	}

	// USART data bits
	if(dataBits != USART_DATA_BITS_NO_CHANGE){
		clrBit(reg2, UCSZ02);
		reg3 &= ~(0x03 << UCSZ00);
		switch(dataBits){
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
			return RESULT_UNSUPPORTED_USART_DATA_BITS;
		}
	}

	// USART mode
	if(mode != USART_MODE_NO_CHANGE){
		modeAux = mode;
		reg3 &= ~(0x03 << UMSEL00);
		clrBit(reg1, U2X0);				// Single Speed
		clrBit(reg3, UCPOL0);			// Polarity
		switch(mode){
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
			return RESULT_UNSUPPORTED_USART_MODE;
		}
	}else{
		aux8 = (0x03 & (reg3 >> UMSEL00));
		switch(aux8){
		case 0:	// Asynchronous mode
			if(isBitClr(reg1, U2X0)){	// Single speed
				modeAux = USART_MODE_ASYNCHRONOUS;
			}else{						// Double speed
				modeAux = USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED;
			}
			break;
		case 3: // Synchronous SPI mode
			modeAux = USART_MODE_SYNCHRONOUS_SPI;
			break;
		}
	}

	if(baudRate != USART_BAUD_NO_CHANGE){
		switch(modeAux){
		case USART_MODE_ASYNCHRONOUS:
			aux64 = (F_CPU / 16 / baudRate) - 1;
			break;
		case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
			aux64 = (F_CPU / 8 / baudRate) - 1;
			break;
		case USART_MODE_SYNCHRONOUS_SPI:
			aux64 = (F_CPU / 2 / baudRate) - 1;
			break;
		}
//		if((aux64 > (1.05 * baudRate)) || (aux64 < (0.95 * baudRate)))
//			return RESULT_UNSUPPORTED_USART_BAUD_RATE;
	}

	UCSR0A = reg1;
	UCSR0B = reg2;
	UCSR0C = reg3;
	UBRR0H = (uint8)(0x0F & (aux64 >> 8));
	UBRR0L = (uint8)(0xFF & aux64);
	
	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Enables USART receiver module
 * -------------------------------------------------------------------------- */

resultValue_t usartEnableReceiver(void)
{
	setBit(UCSR0B, RXEN0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Disables USART receiver module
 * -------------------------------------------------------------------------- */

resultValue_t usartDisableReceiver(void)
{
	clrBit(UCSR0B, RXEN0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Enables USART transmitter module
 * -------------------------------------------------------------------------- */

resultValue_t usartEnableTransmitter(void)
{
	setBit(UCSR0B, TXEN0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Disables USART transmitter module
 * -------------------------------------------------------------------------- */

resultValue_t usartDisableTransmitter(void)
{
	clrBit(UCSR0B, TXEN0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Activates USART reception complete interrupt
 * -------------------------------------------------------------------------- */

resultValue_t usartActivateReceptionCompleteInterrupt(void)
{
	setBit(UCSR0B, RXCIE0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Deactivates USART reception complete interrupt
 * -------------------------------------------------------------------------- */

resultValue_t usartDeactivateReceptionCompleteInterrupt(void)
{
	clrBit(UCSR0B, RXCIE0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Activates USART transmission complete interrupt
 * -------------------------------------------------------------------------- */

resultValue_t usartActivateTransmissionCompleteInterrupt(void)
{
	setBit(UCSR0B, TXCIE0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Deactivates USART transmission complete interrupt
 * -------------------------------------------------------------------------- */

resultValue_t usartDeactivateTransmissionCompleteInterrupt(void)
{
	clrBit(UCSR0B, TXCIE0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Activates USART buffer empty interrupt
 * -------------------------------------------------------------------------- */

resultValue_t usartActivateBufferEmptyInterrupt(void)
{
	setBit(UCSR0B, UDRIE0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Deactivates USART buffer empty interrupt
 * -------------------------------------------------------------------------- */

resultValue_t usartDeactivateBufferEmptyInterrupt(void)
{
	clrBit(UCSR0B, UDRIE0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Checks if a reception has been completed
 * -------------------------------------------------------------------------- */

bool_t usartIsReceptionComplete(void)
{
	return isBitSet(UCSR0A, RXC0);
}

/* -----------------------------------------------------------------------------
 * Checks if a transmission has been completed
 * -------------------------------------------------------------------------- */

bool_t usartIsTransmissionComplete(void)
{
	return isBitSet(UCSR0A, TXC0);
}

/* -----------------------------------------------------------------------------
 * Checks if the buffer is empty
 * -------------------------------------------------------------------------- */

bool_t usartIsBufferEmpty(void)
{
	return isBitSet(UCSR0A, UDRE0);
}

/* -----------------------------------------------------------------------------
 * Checks if an error occurred during transmission or reception
 * -------------------------------------------------------------------------- */

usartError_t usartCheckError(void)
{
	usartError_t error = USART_NO_ERRORS;

	if(isBitSet(UCSR0A, FE0))
		error |= USART_FRAME_ERROR;
	if(isBitSet(UCSR0A, DOR0))
		error |= USART_PARITY_ERROR;
	if(isBitSet(UCSR0A, UPE0))
		error |= USART_BUFFER_OVERFLOW_ERROR;

	return error;
}

/* -----------------------------------------------------------------------------
 * Transmits data in 5, 6, 7 or 8 bits modes using the USART controller
 * -------------------------------------------------------------------------- */

resultValue_t usartTransmit(uint8 data)
{
	while(!usartIsBufferEmpty())
		;	// Waits until last transmission ends
	UDR0 = data;

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Transmits data in 9 bits mode using the USART controller
 * -------------------------------------------------------------------------- */

resultValue_t usartTransmit9bits(uint16 data)
{
	uint8 aux;

	while(!usartIsBufferEmpty())
		;	// Waits until last transmission ends
	aux = ((data & 0x100) >> 8);
	if(aux)
		setBit(UCSR0B, TXB80);
	else
		clrBit(UCSR0B, TXB80);
	UDR0 = (uint8)data;

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Receives data in 5, 6, 7 or 8 bits modes using the USART controller
 * -------------------------------------------------------------------------- */

uint8 usartReceive(void)
{
	uint8 status;

	while(!usartIsReceptionComplete())
		;	// Waits until last reception ends
	status = UCSR0A;

	return UDR0;
}

/* -----------------------------------------------------------------------------
 * Receives data in 9 bits modes using the USART controller
 * -------------------------------------------------------------------------- */

uint16 usartReceive9bits(void)
{
	uint8 status;
	uint8 byteh;
	uint8 bytel;
	uint16 byte;

	while(!usartIsReceptionComplete())
		;	// Waits until last reception ends
	status = UCSR0A;
	byteh = UCSR0B;
	bytel = UDR0;

	byte = (uint16)(byteh & 0x02) << 7;
	byte |= bytel;

	return  byte;
}

/* -----------------------------------------------------------------------------
 * Clears the receptor data buffer
 * -------------------------------------------------------------------------- */

void usartClearReceptionBuffer(void)
{
	uint8 aux;
	while(usartIsReceptionComplete())
		aux = UDR0;

	return;
}

/* -----------------------------------------------------------------------------
 * Changes the std handlers to usart module
 * -------------------------------------------------------------------------- */

resultValue_t usartStdio(void)
{
	stdin = stdout = stderr = &usartStream;

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Transmits data in 5, 6, 7 or 8 bits modes using the USART controller and
 * standard output heandler
 * -------------------------------------------------------------------------- */

resultValue_t usartTransmitStd(int8 data, FILE * stream)
{
	while(!usartIsBufferEmpty())
		;	// Waits until last transmission ends
	UDR0 = data;

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Receives data in 5, 6, 7 or 8 bits modes using the USART controller and
 * standard input heandler
 * -------------------------------------------------------------------------- */

int16 usartReceiveStd(FILE * stream)
{
	while(!usartIsReceptionComplete())
		;	// Waits until last reception ends

	return (int16)UDR0;
}
