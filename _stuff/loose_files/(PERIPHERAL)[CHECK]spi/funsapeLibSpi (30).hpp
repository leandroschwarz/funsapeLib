//! ==================== <<<<< BEGIN DOCUMENTATION >>>>>
//! \brief teste
//!
//! ==================== <<<<<  END DOCUMENTATION  >>>>>

/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.hpp
 * Module:			SPI Peripheral Interface for FunSAPE++ Embedded Library
 *					project
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

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibSpi.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.hpp] Error 11 - Build mismatch between (funsapeLibSpi.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
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

#define SPI_DUMMY_BYTE					0xDB

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

/**
  * @brief SPI general class
  *
 */
class Spi : public Handler
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	/**
	  * @brief SPI operation modes
	  *
	 */
	typedef enum spiOperation_t {
		SPI_WRITE	= 0x00,
		SPI_READ	= 0x80
	} spiOperation_t;

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading ---------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	Spi (void);
	~Spi (void);													// TODO: Implement function

	// -------------------------------------------------------------------------
	// Methods - Inherited public methods --------------------------------------

public:
	//! ==================== <<<<< BEGIN DOCUMENTATION >>>>>
	//! \brief 1
	//!
	//! \param buffData 2
	//! \param buffSize 3
	//! \return true 4
	//! \return false 5
	//! ==================== <<<<<  END DOCUMENTATION  >>>>>
	bool sendData (uint8_t *buffData, uint16_t buffSize = 1);
	bool sendData (uint8_t *txBuffData, uint8_t *rxBuffData, uint16_t buffSize = 1);
	bool sendData (uint8_t reg, uint8_t *buffData, uint16_t buffSize = 1);
	bool sendData (uint8_t reg, uint8_t *txBuffData, uint8_t *rxBuffData, uint16_t buffSize = 1);
	bool setDevice (void (* actFunc) (void), void (* deactFunc) (void));
	Handler::handlerType_e getHandlerType (void);

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Own methods ---------------------------------------------------

public:
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	bool init (SPI_HandleTypeDef *handler, uint16_t bufSize = 20);
	bool init (SPI_HandleTypeDef *handler, uint16_t rxBufSize, uint16_t txBufSize);
#elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
	// TODO: AVR implementation
	bool init (uint8_t *handler, uint16_t bufSize = 20);
	bool init (uint8_t *handler, uint16_t rxBufSize, uint16_t txBufSize);
#endif
	// NONE
	bool setTimeOut (uint16_t timeOut);
	error_e getLastError (void);

private:
	bool _deinitialize (void);			//!<
	bool _initialize (void);
	bool _transmit (void);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	SPI_HandleTypeDef *_spiHandler;
#elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
	uint8_t *_spiHandler;										// TODO: AVR implementation
#endif

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool	_initialized						: 1;
	bool	_devSelFunctiosSet					: 1;
	bool	_busy								: 1;
	void	(*_activateDefaultDevice) (void);
	void	(*_deactivateDefaultDevice) (void);
	error_e	_lastError;
//	spiTransmissionMode_t _spiTransmissionMode;

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

protected:
	// NONE

}; // class Spi

// 
// Spi - Class overloading operators
// 

// NONE

// 
// Spi - Class inline function definitions
// 

// NONE

// 
// External default objects
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

#endif // __FUNSAPE_LIB_SPI_HPP
