/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	usart.hpp
 * Module:		USART Module Interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 16, 2018
 * ========================================================================== */

#ifndef __USART_HPP
#define __USART_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[usart.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include "system.hpp"
#include <stdio.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

typedef enum usartBaudRate_e {
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

typedef enum usartMode_e {
	USART_MODE_ASYNCHRONOUS					= 0,
	USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED	= 1,
	USART_MODE_SYNCHRONOUS_XCK_FALLING		= 2,
	USART_MODE_SYNCHRONOUS_XCK_RISING		= 3,
	USART_MODE_SYNCHRONOUS_SPI				= 4,
} usartMode_e;

typedef enum usartParity_e {
	USART_PARITY_NONE	= 0,
	USART_PARITY_ODD	= 1,
	USART_PARITY_EVEN	= 2
} usartParity_e;

typedef enum usartStopBit_e {
	USART_STOP_BIT_SINGLE	= 0,
	USART_STOP_BIT_DOUBLE	= 1
} usartStopBit_e;

typedef enum usartDataBits_e {
	USART_DATA_BITS_5	= 0,
	USART_DATA_BITS_6	= 1,
	USART_DATA_BITS_7	= 2,
	USART_DATA_BITS_8	= 3,
	USART_DATA_BITS_9	= 4
} usartDataBits_e;

typedef enum usartFrameFormat_e {
	//						= 0bDDDDPPSS
	// DDDD -> Number of data bits (5-8)
	// PP   -> Parity (NONE / ODD / EVEN)
	// SS   -> Number of stop bits (1-2)
	USART_FRAME_FORMAT_8_N_1		= 0b10000001,
	USART_FRAME_FORMAT_8_O_1		= 0b10000101,
	USART_FRAME_FORMAT_8_E_1		= 0b10001001,
	USART_FRAME_FORMAT_8_N_2		= 0b10000010,
	USART_FRAME_FORMAT_8_O_2		= 0b10000110,
	USART_FRAME_FORMAT_8_E_2		= 0b10001010,
	USART_FRAME_FORMAT_9_N_1		= 0b10010001,
	USART_FRAME_FORMAT_9_O_1		= 0b10010101,
	USART_FRAME_FORMAT_9_E_1		= 0b10011001,
	USART_FRAME_FORMAT_9_N_2		= 0b10010010,
	USART_FRAME_FORMAT_9_O_2		= 0b10010110,
	USART_FRAME_FORMAT_9_E_2		= 0b10011010,
	USART_FRAME_FORMAT_5_N_1		= 0b01010001,
	USART_FRAME_FORMAT_5_O_1		= 0b01010101,
	USART_FRAME_FORMAT_5_E_1		= 0b01011001,
	USART_FRAME_FORMAT_5_N_2		= 0b01010010,
	USART_FRAME_FORMAT_5_O_2		= 0b01010110,
	USART_FRAME_FORMAT_5_E_2		= 0b01011010,
	USART_FRAME_FORMAT_6_N_1		= 0b01100001,
	USART_FRAME_FORMAT_6_O_1		= 0b01100101,
	USART_FRAME_FORMAT_6_E_1		= 0b01101001,
	USART_FRAME_FORMAT_6_N_2		= 0b01100010,
	USART_FRAME_FORMAT_6_O_2		= 0b01100110,
	USART_FRAME_FORMAT_6_E_2		= 0b01101010,
	USART_FRAME_FORMAT_7_N_1		= 0b01110001,
	USART_FRAME_FORMAT_7_O_1		= 0b01110101,
	USART_FRAME_FORMAT_7_E_1		= 0b01111001,
	USART_FRAME_FORMAT_7_N_2		= 0b01110010,
	USART_FRAME_FORMAT_7_O_2		= 0b01110110,
	USART_FRAME_FORMAT_7_E_2		= 0b01111010
} usartFrameFormat_e;

typedef enum usartConfigFlags_f {
	USART_CONFIG_FLAGS_NONE			= 0x00,
	USART_CONFIG_FLAGS_BAUD_RATE	= 0x01,
	USART_CONFIG_FLAGS_DATA_BITS	= 0x02,
	USART_CONFIG_FLAGS_MODE			= 0x04,
	USART_CONFIG_FLAGS_PARITY		= 0x08,
	USART_CONFIG_FLAGS_STOP_BIT		= 0x10,
	USART_CONFIG_FLAGS_ALL			= 0x1F
} usartConfigFlags_f;

inline __attribute__((always_inline)) usartConfigFlags_f operator ~(usartConfigFlags_f a)
{
	return usartConfigFlags_f(~int(a));
}

inline __attribute__((always_inline)) usartConfigFlags_f operator&(usartConfigFlags_f a, usartConfigFlags_f b)
{
	return usartConfigFlags_f(int(a) & int(b));
}

inline __attribute__((always_inline)) usartConfigFlags_f operator|(usartConfigFlags_f a, usartConfigFlags_f b)
{
	return usartConfigFlags_f(int(a) | int(b));
}

inline __attribute__((always_inline)) usartConfigFlags_f operator^(usartConfigFlags_f a, usartConfigFlags_f b)
{
	return usartConfigFlags_f(int(a) ^ int(b));
}

inline __attribute__((always_inline)) usartConfigFlags_f operator>>(usartConfigFlags_f a, uint8_t b)
{
	return usartConfigFlags_f(int(a) >> b);
}

inline __attribute__((always_inline)) usartConfigFlags_f operator<<(usartConfigFlags_f a, uint8_t b)
{
	return usartConfigFlags_f(int(a) << b);
}

typedef enum usartReceptionError_f {
	USART_RECEPTION_ERROR_NONE				= 0,
	USART_RECEPTION_ERROR_FRAME				= 1,
	USART_RECEPTION_ERROR_PARITY			= 2,
	USART_RECEPTION_ERROR_BUFFER_OVERFLOW	= 4
} usartReceptionError_f;

inline __attribute__((always_inline)) usartReceptionError_f operator|(usartReceptionError_f a, usartReceptionError_f b)
{
	return usartReceptionError_f(int(a) | int(b));
}

// =============================================================================
// Usart - Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Usart0 ----------------------------------------------------------------------

#if USART0_EXISTS
class Usart0
{
	// Constructors
public:
	Usart0(void);

	// Methods - Public
public:
	// Configuration
	bool flush(void);
	bool init(usartBaudRate_e baudRate = USART_BAUD_9600, usartMode_e mode = USART_MODE_ASYNCHRONOUS, usartFrameFormat_e format = USART_FRAME_FORMAT_8_N_1);
	bool setDefault(void);
	bool setReceiver(bool activate);
	bool setTransmitter(bool activate);
	error_e getLastError(void);
	usartReceptionError_f getReceptionErrors(void);
	// Data manipulation
	bool receiveData(uint8_t *data);
	bool receiveData9bits(uint16_t *data);
	bool sendData(uint8_t data);
	bool sendData9Bits(uint16_t data);
	// Buffer empty interrupt
	bool activateBufferEmptyInterrupt(void);
	bool deactivateBufferEmptyInterrupt(void);
	bool isBufferEmpty(void);
	// Reception complete interrupt
	bool activateReceptionCompleteInterrupt(void);
	bool deactivateReceptionCompleteInterrupt(void);
	bool isReceptionComplete(void);
	// Transmission complete interrupt
	bool activateTransmissionCompleteInterrupt(void);
	bool deactivateTransmissionCompleteInterrupt(void);
	bool isTransmissionComplete(void);
	int sendDataStd(char data, FILE *stream);

	// Methods - Private
private:
	char receiveDataStd(FILE *stream);
	void checkReceptionErrors(void);

	// Data members - Private
private:
	usartMode_e				mode_				: 3;
	usartBaudRate_e			baudRate_			: 18;
	usartParity_e			parity_				: 2;
	usartStopBit_e			stopBit_			: 1;
	usartDataBits_e			dataBits_			: 3;
	bool					initialized_		: 1;
	usartConfigFlags_f		configFlags_		: 5;
	usartReceptionError_f	receptionErrors_	: 3;
	error_e					lastError_;
}; // class Usart0
#endif

// -----------------------------------------------------------------------------
// Usart1 ----------------------------------------------------------------------

#if USART1_EXISTS

#endif

// =============================================================================
// Extern global variables
// *INDENT-OFF*
// =============================================================================

#if USART0_EXISTS
	extern FILE usartStream;
	extern Usart0 usart0;
	extern Usart0 *usartDefaultHandler;
#endif

#if USART1_EXISTS
	extern FILE usart1Stream;
	extern Usart1 usart1;
	extern Usart1 *usart1DefaultHandler;
#endif
// *INDENT-ON*

// =============================================================================
// Usart - Class inline function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Usart0 ----------------------------------------------------------------------

#if USART0_EXISTS
bool inline __attribute__((always_inline)) Usart0::setReceiver(bool activate)
{
	if (activate) {
		setBit(USART0_CONTROL_STATUS_B_REG, USART0_RECEIVER_ENABLE_BIT);
	} else {
		clrBit(USART0_CONTROL_STATUS_B_REG, USART0_RECEIVER_ENABLE_BIT);
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::setTransmitter(bool activate)
{
	if (activate) {
		setBit(USART0_CONTROL_STATUS_B_REG, USART0_TRANSMITTER_ENABLE_BIT);
	} else {
		clrBit(USART0_CONTROL_STATUS_B_REG, USART0_TRANSMITTER_ENABLE_BIT);
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::activateReceptionCompleteInterrupt(void)
{
	setBit(USART0_CONTROL_STATUS_B_REG, USART0_RECEP_COMP_INT_ENABLE_BIT);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::deactivateReceptionCompleteInterrupt(void)
{
	clrBit(USART0_CONTROL_STATUS_B_REG, USART0_RECEP_COMP_INT_ENABLE_BIT);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::activateTransmissionCompleteInterrupt(void)
{
	setBit(USART0_CONTROL_STATUS_B_REG, USART0_TRANS_COMP_INT_ENABLE_BIT);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::deactivateTransmissionCompleteInterrupt(void)
{
	clrBit(USART0_CONTROL_STATUS_B_REG, USART0_TRANS_COMP_INT_ENABLE_BIT);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::activateBufferEmptyInterrupt(void)
{
	setBit(USART0_CONTROL_STATUS_B_REG, USART0_DATA_READY_INT_ENABLE_BIT);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::deactivateBufferEmptyInterrupt(void)
{
	clrBit(USART0_CONTROL_STATUS_B_REG, USART0_DATA_READY_INT_ENABLE_BIT);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Usart0::isReceptionComplete(void)
{
	return isBitSet(USART0_CONTROL_STATUS_A_REG, USART0_RECEP_COMP_BIT);
}

bool inline __attribute__((always_inline)) Usart0::isTransmissionComplete(void)
{
	return isBitSet(USART0_CONTROL_STATUS_A_REG, USART0_TRANS_COMP_BIT);
}

bool inline __attribute__((always_inline)) Usart0::isBufferEmpty(void)
{
	return isBitSet(USART0_CONTROL_STATUS_A_REG, USART0_DATA_READY_BIT);
}
#endif

// -----------------------------------------------------------------------------
// Usart1 ----------------------------------------------------------------------

#if USART1_EXISTS
#endif

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __USART_HPP
