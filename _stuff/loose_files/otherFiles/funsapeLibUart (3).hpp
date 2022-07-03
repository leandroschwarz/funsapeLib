/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibUart.hpp
 * Module:			UART Peripheral Interface for FunSAPE++ Embedded Library
 *					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_UART_HPP
#define __FUNSAPE_LIB_UART_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Project configuration file --------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibUart.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibUart.hpp] Error 11 - Build mismatch between (funsapeLibUart.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

// NONE

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables => shortening the name of single data types ----------------

// NONE

// -----------------------------------------------------------------------------
// Single variables => enumerations understood as logic values -----------------

// NONE

// -----------------------------------------------------------------------------
// Single variables => small enumerations that can be understood as a single variable

// NONE

// -----------------------------------------------------------------------------
// Enumerations => large enumerations or enumerations with expansible number of element

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet's values

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Flags => enumerations that can be combined in the form of flags

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Structures, Unions and Bit Fields ---------------------------

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Classes -----------------------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Uart - Class declaration
// =============================================================================

class Uart
{
	// New data types - Public
public:
	// NONE
	enum uartReceptionError_f {
		UART_RECEPTION_ERROR_NONE				= 0,
		UART_RECEPTION_ERROR_FRAME				= 1,
		UART_RECEPTION_ERROR_PARITY				= 2,
		UART_RECEPTION_ERROR_BUFFER_OVERFLOW	= 4
	};

	// Operators overloading - Public
public:
	// NONE

	// Constructors - Public
public:
	Uart(void);
	~Uart(void);												// TODO: Implement function

	// Methods - Public
public:
#ifdef _FUNSAPE_PLATFORM_ARM_STM32				// ARM implementation
	bool init(UART_HandleTypeDef *handler, uint16_t bufSize = 20);
	bool init(UART_HandleTypeDef *handler, uint16_t rxBufSize, uint16_t txBufSize);
#endif
#ifdef _FUNSAPE_PLATFORM_AVR									// TODO: AVR implementation
	// bool init(uint16_t bufSize = 20);
	// bool init(uint16_t rxBufSize, uint16_t txBufSize);
#endif
	bool sendData(uint8_t *msg, uint16_t msgSize = 1);
	bool receiveData(uint8_t *msg, uint16_t msgSize = 1);
//	void flushReceptionBuffer(void);
	bool setTimeOut(uint16_t timeOut);
	error_e getLastError(void);
//	uartReceptionError_f getReceptionErrors(void);
//	uint8_t receiveData(void);
//	uint16_t receiveData9bits(void);
//	int sendDataStd(char data, FILE *stream);
//	bool setTransmissionMode(spiTransmissionMode_t mode);		// TODO: Implement function

	// Methods - Private
private:
	bool _initialize(void);
	bool _deinitialize(void);
	bool _transmit(void);

	// Data members - Private
private:
#ifdef _FUNSAPE_PLATFORM_ARM_STM32
	UART_HandleTypeDef *_spiHandler;
#endif
	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool	_initialized						: 1;
	bool	_callbacksReady						: 1;
	bool	_busy								: 1;
	void	(*_activateDefaultDevice)(void);
	void	(*_deactivateDefaultDevice)(void);
//	spiTransmissionMode_t _spiTransmissionMode;
	error_e _lastError;

	//     ////////////////////    DATA BUFFERS      ////////////////////     //
	uint8_t		*_rxBufferData;
	uint16_t	_rxBufferMaxSize;
	uint16_t	_rxBufferIndex;
	uint16_t	_rxBufferLength;
	uint8_t		*_txBufferData;
	uint16_t	_txBufferMaxSize;
	uint16_t	_txBufferIndex;
	uint16_t	_txBufferLength;
	uint16_t	_timeOut;

}; // class Uart

// =============================================================================
// Uart - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Uart - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_UART_HPP
