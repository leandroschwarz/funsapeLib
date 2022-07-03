/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibGpio.cpp
 * Module:			GPIO Peripheral Module for FunSAPE++ Embedded Library
 * 						project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#	error	[funsapeLibGpio.cpp] Error 16 - Header file (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibGpio.cpp] Error 17 - Build mismatch between source (funsapeLibGpio.cpp) and header (funsapeLibGpio.hpp) files!
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
 * @brief Construct a new Gpio:: Gpio object
 *
 * Detailed description of this element.
 *
 */
Gpio::Gpio(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::Gpio(void)", DEBUG_CODE_GPIO);

	// Resets data members
	this->_clearData();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Construct a new Gpio:: Gpio object
 *
 * Detailed description of this element.
 *
 * @param[in] gpioPort_p description of this parameter
 * @param[in] gpioPin_p description of this parameter
 */
Gpio::Gpio(GPIO_TypeDef *gpioPort_p, cuint16_t gpioPin_p)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::Gpio(GPIO_TypeDef *, cuint16_t)", DEBUG_CODE_GPIO);

	// Deinitialize peripheral
	if(this->_initialized) {
		this->_deinitialize();
	}

	// Initialize GPIO
	if(!this->_initialize(gpioPort_p, gpioPin_p)) {
		// Returns error
		debugMessage(this->_lastError);
		return;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Destroy the Gpio:: Gpio object
 *
 * Detailed description of this element.
 *
 */
Gpio::~Gpio(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::~Gpio(void)", DEBUG_CODE_GPIO);

	// Returns successfully
	return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return Error
 */
Error Gpio::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] gpioPort_p description of this parameter
 * @param[in] gpioPin_p description of this parameter
 * @return true
 * @return false
 */
bool Gpio::init(GPIO_TypeDef *gpioPort_p, cuint16_t gpioPin_p)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::init(GPIO_TypeDef *, cuint16_t)", DEBUG_CODE_GPIO);

	// Initialize GPIO
	if(!this->_initialize(gpioPort_p, gpioPin_p)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     ///////////////////    PIN RELATED METHODS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool Gpio::clr(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::clr(void)", DEBUG_CODE_GPIO);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Clear pin
	clrIoPin(this->_port, this->_pin);

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool Gpio::cpl(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::cpl(void)", DEBUG_CODE_GPIO);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Toggle pin
	cplIoPin(this->_port, this->_pin);

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool Gpio::set(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::set(void)", DEBUG_CODE_GPIO);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Set pin
	setIoPin(this->_port, this->_pin);

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void Gpio::_clearData(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::_clearData(void)", DEBUG_CODE_GPIO);

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	this->_initialized	= false;
	//     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
	this->_pin			= 0;
	this->_port			= nullptr;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void Gpio::_deinitialize(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::_deinitialize(void)", DEBUG_CODE_GPIO);

	// Resets data members
	this->_clearData();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] gpioPort_p description of this parameter
 * @param[in] gpioPin_p description of this parameter
 * @return true
 * @return false
 */
bool Gpio::_initialize(GPIO_TypeDef *gpioPort_p, cuint16_t gpioPin_p)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::_initialize(GPIO_TypeDef *, cuint16_t)", DEBUG_CODE_GPIO);

	// CHECK FOR ERROR - port address invalid
	if(!isPointerValid(gpioPort_p)) {
		// Returns error
		this->_lastError = Error::GPIO_PORT_INVALID;
		debugMessage(Error::GPIO_PORT_INVALID);
		return false;
	}
	// CHECK FOR ERROR - pin number invalid
	if(!IS_GPIO_PIN(gpioPin_p)) {
		// Returns error
		this->_lastError = Error::GPIO_PIN_INVALID;
		debugMessage(Error::GPIO_PIN_INVALID);
		return false;
	}

	// Update class members
	this->_initialized = true;
	this->_pin = gpioPin_p;
	this->_port = gpioPort_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool Gpio::_isInitialized(void)
{
	// Mark passage for debugging purpose
	debugMark("Gpio::_isInitialized(void)", DEBUG_CODE_GPIO);

	// Checks initialization
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// END OF FILE
// =============================================================================
