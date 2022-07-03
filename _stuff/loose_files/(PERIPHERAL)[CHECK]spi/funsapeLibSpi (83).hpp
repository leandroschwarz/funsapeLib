/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.hpp
 * Module:			SPI Peripheral Interface for FunSAPE++ Embedded Library
 *					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_SPI_HPP
#define __FUNSAPE_LIB_SPI_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Project configuration file --------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibSpi.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.hpp] Error 11 - Build mismatch between (funsapeLibSpi.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
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

typedef enum spiOperation_t {
	SPI_WRITE	= 0x00,
	SPI_READ	= 0x80
} spiOperation_t;

// typedef enum spiTransmissionMode_t {
// 	SPI_TRANS_MODE_POOLING				= 0,
// 	SPI_TRANS_MODE_INTERRUPT			= 1,
// #ifdef _FUNSAPE_PLATFORM_ARM_STM32
// 	SPI_TRANS_MODE_DMA					= 2,
// #endif
// } spiTransmissionMode_t;

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
// Spi - Class declaration
// =============================================================================

class Spi
{
	// New data types - Public
public:
	// NONE

	// Operators overloading - Public
public:
	// NONE

	// Constructors - Public
public:
	Spi(void);
	~Spi(void);													// TODO: Implement function

	// Methods - Public
public:
#ifdef _FUNSAPE_PLATFORM_ARM_STM32				// ARM implementation
	bool init(SPI_HandleTypeDef *handler, uint16_t bufSize = 20);
	bool init(SPI_HandleTypeDef *handler, uint16_t rxBufSize, uint16_t txBufSize);
#endif
#ifdef _FUNSAPE_PLATFORM_AVR									// TODO: AVR implementation
	// bool init(uint16_t bufSize = 20);
	// bool init(uint16_t rxBufSize, uint16_t txBufSize);
#endif
	bool sendData(uint8_t *msg, uint16_t msgSize = 1);
	bool sendData(uint8_t *txMsg, uint8_t *rxMsg, uint16_t msgSize = 1);
	bool sendData(uint8_t reg, uint8_t *msg, uint16_t msgSize = 1);
	bool setDefaultDevice(void actFuncCall(void), void deactFuncCall(void));
	bool setTimeOut(uint16_t timeOut);
//	bool setTransmissionMode(spiTransmissionMode_t mode);		// TODO: Implement function
	error_e getLastError(void);

	// Methods - Private
private:
	bool _initialize(void);
	bool _deinitialize(void);
	bool _transmit(void);

	// Data members - Private
private:
#ifdef _FUNSAPE_PLATFORM_ARM_STM32
	SPI_HandleTypeDef *_spiHandler;
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

}; // class Spi

// =============================================================================
// Spi - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Spi - Class inline function definitions
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

#endif // __FUNSAPE_LIB_SPI_HPP
