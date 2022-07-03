/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.hpp
 * Module:			SPI Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Include guard
// 

#ifndef __FUNSAPE_LIB_SPI_HPP
#define __FUNSAPE_LIB_SPI_HPP					__BUILD_TO_REPLACE__

// 
// Dependencies
// 

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibSpi.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.hpp] Error 10 - Build mismatch between (funsapeLibSpi.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibSpi.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibSpi.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif
#include "util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibSpi.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibSpi.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
#endif

// 
// Undefining previous definitions
// 

// NONE

// 
// Constant definitions
// 

// NONE

// 
// Macro-function definitions
// *INDENT-OFF*
// 

// NONE

// *INDENT-ON*

// 
// New data types
// 

// NONE

// 
// Extern global variables
// 

// NONE

// 
// Spi - Class declaration
// 

class Spi : public Handler
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	enum SpiOperation {
		WRITE							= 0,
		READ							= 1,
		SEND							= 2,
	};

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------

public:
	Spi(
			void
	);
	// TODO: Implement destructor function
	~Spi(
			void
	);

	// -------------------------------------------------------------------------
	// Methods - Inherited methods
	// -------------------------------------------------------------------------

public:
	//     ////////////////////    DATA TRANSFER     ////////////////////     //
	bool readReg(
			uint8_t reg_p,
			uint8_t *buffData_p,
			uint16_t buffSize_p = 1
	);
	bool sendData(
			uint8_t *buffData_p,
			uint16_t buffSize_p = 1
	);
	bool sendData(
			uint8_t *txBuffData_p,
			uint8_t *rxBuffData_p,
			uint16_t buffSize_p = 1
	);
	bool writeReg(
			uint8_t reg_p,
			uint8_t *buffData_p,
			uint16_t buffSize_p = 1
	);

	//     //////////////////    PROTOCOL SPECIFIC     //////////////////     //
	Handler::HandlerType getHandlerType(
			void
	);
	bool setDevice(
			void (* actFunc_p)(void),
			void (* deactFunc_p)(void)
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods
	// -------------------------------------------------------------------------

public:
	bool init(
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_SPI_MODULE_ENABLED)
			// ARM implementation
			SPI_HandleTypeDef *handler_p,
#elif defined(_FUNSAPE_PLATFORM_AVR)
			// AVR implementation
			// TODO: AVR implementation
			uint8_t *handler_p,
#else // _FUNSAPE_PLATFORM_AVR
			uint8_t *handler_p,
#endif // NONE OF THE OTHERS
			uint16_t bufSize_p = 20
	);
	bool init(
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_SPI_MODULE_ENABLED)
			// ARM implementation
			SPI_HandleTypeDef *handler_p,
#elif defined(_FUNSAPE_PLATFORM_AVR)
			// AVR implementation
			// TODO: AVR implementation
			uint8_t *handler_p,
#else // _FUNSAPE_PLATFORM_AVR
			uint8_t *handler_p,
#endif // NONE OF THE OTHERS
			uint16_t rxBufSize_p,
			uint16_t txBufSize_p
	);

	Error getLastError(void);
	bool setTimeout(
			uint16_t timeout_p
	);

private:
	bool _initialize(
			void
	);
	bool _deinitialize(
			void
	);
	bool _transmit(
			SpiOperation spiOperation_p
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_SPI_MODULE_ENABLED)
	// ARM implementation
	SPI_HandleTypeDef *_spiHandler;
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
	uint8_t *_spiHandler;
#else // _FUNSAPE_PLATFORM_AVR
	uint8_t *_spiHandler;
#endif // NONE OF THE OTHERS

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool				_initialized			: 1;
	bool				_devSelFunctionSet		: 1;
	bool				_busy					: 1;
	void	(*_activateDefaultDevice)(void);
	void	(*_deactivateDefaultDevice)(void);
	uint16_t			_timeout;
	Error				_lastError;

	//     ////////////////////    DATA BUFFERS      ////////////////////     //
	uint8_t		*_rxBufferData;
	uint16_t	_rxBufferMaxSize;
	uint16_t	_rxBufferIndex;
	uint16_t	_rxBufferLength;
	uint8_t		*_txBufferData;
	uint16_t	_txBufferMaxSize;
	uint16_t	_txBufferIndex;
	uint16_t	_txBufferLength;

protected:
	// NONE

protected:
	// NONE

}; // class Spi

// 
// Spi - Class overloading operators
// 

// NONE

// 
// Global variables
// 

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// 
// Spi - Class inline function definitions
// 

// NONE

// 
// General public functions declarations
// 

// NONE

// 
// General inline functions definitions
// 

// NONE

// 
// External default objects
// 

#endif // __FUNSAPE_LIB_SPI_HPP
