/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibGpio.cpp
 * Module:			GPIO Peripheral Module for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#	error	[funsapeLibGpio.cpp] Error 1 - Header file (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibGpio.cpp] Error 13 - Build mismatch between source (funsapeLibGpio.cpp) and header (funsapeLibGpio.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Gpio::Gpio object
 *
 */
Gpio::Gpio(void)
{
	// Resets data members
	this->_initialize					= false;
	this->_gpioPin						= 0;
	this->_gpioPort						= nullptr;

	// Returns successfully
	this->_lastError					= Error::NONE;
	return;
}

/**
 * @brief Destroy the Gpio::Gpio object
 *
 */
Gpio::~Gpio(void)
{
	// Returns successfully
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
// ARM implementation
bool Gpio::init(GPIO_TypeDef *gpioPort_p, uint16_t gpioPin_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)
// AVR implementation
bool Gpio::init(uint8_t *gpioPort_p, uint16_t gpioPin_p)
#else // _FUNSAPE_PLATFORM_AVR
bool Gpio::init(uint8_t *gpioPort_p, uint16_t gpioPin_p)
#endif // NONE OF THE OTHERS
{
	// CHECK FOR ERROR - port address invalid
	if(!isPointerValid(gpioPort_p)) {
		// Return error
		this->_lastError = Error::GPIO_PORT_INVALID;
		debugMessage(Error::GPIO_PORT_INVALID);
		return false;
	}
	// CHECK FOR ERROR - pin number invalid
	if(!IS_GPIO_PIN(gpioPin_p)) {
		// Return error
		this->_lastError = Error::GPIO_PIN_INVALID;
		debugMessage(Error::GPIO_PIN_INVALID);
		return false;
	}

	// Update class members
	this->_initialize = true;
	this->_gpioPin = gpioPin_p;
	this->_gpioPort = gpioPort_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Gpio::set(void)
{
	// CHECK FOR ERROR - not initialized
	if(this->_initialize) {
		// Return error
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
	// ARM implementation
	HAL_GPIO_WritePin(this->_gpioPort, this->_gpioPin, GPIO_PIN_SET);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
#endif

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Gpio::clr(void)
{
	// Checks initialization
	if(this->_initialize) {
		// Return error
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
	// ARM implementation
	HAL_GPIO_WritePin(this->_gpioPort, this->_gpioPin, GPIO_PIN_RESET);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
#endif

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Gpio::cpl(void)
{
	// Checks initialization
	if(this->_initialize) {
		// Return error
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
	// ARM implementation
	HAL_GPIO_TogglePin(this->_gpioPort, this->_gpioPin);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
#endif

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * @return Error
 */
Error Gpio::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
