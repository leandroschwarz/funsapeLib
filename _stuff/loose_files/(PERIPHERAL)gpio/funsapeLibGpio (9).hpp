


#include "util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibGpio.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibGpio.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif

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
