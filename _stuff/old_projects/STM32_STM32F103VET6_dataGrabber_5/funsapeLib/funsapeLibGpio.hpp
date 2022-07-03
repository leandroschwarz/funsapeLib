/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibGpio.hpp
 * Module:			GPIO Peripheral Module for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_GPIO_HPP
#define __FUNSAPE_LIB_GPIO_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibGpio.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibGpio.hpp] Error 10 - Build mismatch between (funsapeLibGpio.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "util/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#	error	[funsapeLibGpio.hpp] Error 12 - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibGpio.hpp] Error 103 - Required module (funsapeLibBus.hpp) must be build __BUILD_TO_REPLACE__.
#endif
#include "util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibGpio.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibGpio.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
#endif

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

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Gpio - Class declaration
// =============================================================================

class Gpio
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
	Gpio(
			void
	);
	// TODO: Implement destructor function
	~Gpio(
			void
	);

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
	//     ////////////////////     ---     ////////////////////     //
	bool init(
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
			// ARM implementation
			GPIO_TypeDef *gpioPort_p,
			uint16_t gpioPin_p
#elif defined(_FUNSAPE_PLATFORM_AVR)
			// AVR implementation
			uint8_t *gpioPort_p,
			uint16_t gpioPin_p
			// TODO: AVR implementation
#else // _FUNSAPE_PLATFORM_AVR
			uint8_t *gpioPort_p,
			uint16_t gpioPin_p
#endif // NONE OF THE OTHERS
	);
	bool set(void);
	bool clr(void);
	bool cpl(void);
	Error getLastError(void);

protected:
// NONE

// -------------------------------------------------------------------------
// Properties
// -------------------------------------------------------------------------

public:
// NONE

private:
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
	// ARM implementation
	GPIO_TypeDef						*_gpioPort;
	uint16_t							_gpioPin;
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
#else // _FUNSAPE_PLATFORM_AVR
	uint8_t								*_gpioPort;
	uint16_t							_gpioPin;
#endif // NONE OF THE OTHERS
	Error		_lastError;
	bool		_initialize				: 1;

protected:
// NONE

protected:
// NONE

}; // class Gpio

// =============================================================================
// Gpio - Class overloading operators
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
// Gpio - Class inline function definitions
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

#endif // __FUNSAPE_LIB_GPIO_HPP
