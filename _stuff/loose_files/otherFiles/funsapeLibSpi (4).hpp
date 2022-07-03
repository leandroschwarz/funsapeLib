/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.hpp
 * Module:			SPI Peripheral Module for FunSAPE++ Embedded Library project
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
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibSpi.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.hpp] Error 10 - Build mismatch between (funsapeLibSpi.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

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
// *INDENT-OFF*
// =============================================================================

// NONE

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

enum SpiInstance {
	SPI_1								= 1,
	SPI_2								= 2,
	SPI_3								= 3
};

enum SpiMode {
	FULL_DUPLEX_MASTER					= 1,
	FULL_DUPLEX_SLAVE					= 2,
	HALF_DUPLEX_MASTER					= 3,
	HALF_DUPLEX_SLAVE					= 4,
	RECEIVE_ONLY_MASTER					= 5,
	RECEIVE_ONLY_SLAVE					= 6,
	TRANSMIT_ONLY_MASTER				= 7,
	TRANSMIT_ONLY_SLAVE					= 8,
};

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Spi - Class declaration
// =============================================================================

class Spi
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	// NONE

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
	Spi(SpiInstance spiInstance_p);
	~Spi(void);												// TODO: Implement function

	// -------------------------------------------------------------------------
	// Methods - Inherited methods
	// -------------------------------------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods
	// -------------------------------------------------------------------------

public:
	bool setTimeOut(uint16_t timeOut_p);
	Error getLastError(void);
	bool init(void);

// #if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
// #elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
// #endif


private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool				_initialized			: 1;
	uint16_t			_timeOut;
	Error				_lastError;
	SPI_HandleTypeDef	_spiHandler;

protected:
	// NONE

}; // class Spi

// =============================================================================
// Spi - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

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

#endif // __FUNSAPE_LIB_SPI_HPP
