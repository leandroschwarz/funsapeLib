/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibLcdParallel.cpp
 * Module:			LCD Parallel Interface Handler for FunSAPE++ Embedded
 * 					Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "funsapeLibLcdParallel.hpp"
#if __FUNSAPE_LIB_LCD_PARALLEL_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcdParallel.cpp] Error 102 - Build mismatch on header and source code files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define LCD_FUNCTION_SET_CMD			0x20
#	define LCD_FUNCTION_4_BITS					(0 << 4)
#	define LCD_FUNCTION_8_BITS					(1 << 4)
#	define LCD_FUNCTION_1_LINE					(0 << 3)
#	define LCD_FUNCTION_2_LINES					(1 << 3)
#	define LCD_FUNCTION_5x8_FONT				(0 << 2)
#	define LCD_FUNCTION_5x10_FONT				(1 << 2)


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
// File exclusive - Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

LcdParallel::LcdParallel(void)
{
	// Resets data members
	this->_controlGpio					= nullptr;
	this->_controlRs					= 0;
	this->_controlRw					= 0;
	this->_dataFirst					= 0;
	this->_dataLength					= DATA_LENGTH_4_BITS;
	this->_dataPortSet					= false;
	this->_initialized					= false;
	this->_useBusyFlag					= false;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return;
}

LcdParallel::~LcdParallel(void)
{
	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return;
}

// =============================================================================
// Class inherited methods - Public
// =============================================================================

Handler::handlerType_e LcdParallel::getHandlerType(void)
{
	// Checks initialization
	if(!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return Handler::HANDLER_NOT_CONFIGURED;
	}

	// Returns handler type
	return Handler::HANDLER_LCD_PARALLEL;
}

bool LcdParallel::sendData(uint8_t *buffData, uint16_t buffSize)
{


	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

error_e LcdParallel::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_AVR)
bool LcdParallel::setDataPort(Gpio *dataGpio_p, uint8_t dataFirst_p, dataLength_b dataLength_p)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
bool LcdParallel::setDataPort(GPIO_TypeDef *dataGpio_p, uint8_t dataFirst_p, dataLength_b dataLength_p)
#endif
{
	// Checks for errors
	if(!isPointerValid(dataGpio_p)) {
		// Returns error
		this->_lastError = ERROR_ARGUMENT_POINTER_NULL;
		debugMessage(ERROR_ARGUMENT_POINTER_NULL);
		return false;
	}

	// Updates data members
	this->_dataGpio = dataGpio_p;
	this->_dataFirst = dataFirst_p;
	this->_dataLength = dataLength_p;
	this->_dataPortSet = true;

	// Checks initialization
	this->_initialized = (this->_dataPortSet && this->_controlPortSet) ? true : false;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

#if defined(_FUNSAPE_PLATFORM_AVR)
bool LcdParallel::setControlPort(Gpio *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
bool LcdParallel::setControlPort(GPIO_TypeDef *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p)
#endif
{
	// Checks for errors
	if(!isPointerValid(controlGpio_p)) {
		// Returns error
		this->_lastError = ERROR_ARGUMENT_POINTER_NULL;
		debugMessage(ERROR_ARGUMENT_POINTER_NULL);
		return false;
	}

	// Updates data members
	this->_controlGpio = controlGpio_p;
	this->_controlE = controlE_p;
	this->_controlRs = controlRs_p;
	this->_useBusyFlag = false;
	this->_controlPortSet = true;

	// Checks initialization
	this->_initialized = (this->_dataPortSet && this->_controlPortSet) ? true : false;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

#if defined(_FUNSAPE_PLATFORM_AVR)
bool LcdParallel::setControlPort(Gpio *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p, uint8_t controlRw_p)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
bool LcdParallel::setControlPort(GPIO_TypeDef *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p,
		uint8_t controlRw_p)
#endif
{
	// Checks for errors
	if(!isPointerValid(controlGpio_p)) {
		// Returns error
		this->_lastError = ERROR_ARGUMENT_POINTER_NULL;
		debugMessage(ERROR_ARGUMENT_POINTER_NULL);
		return false;
	}

	// Updates data members
	this->_controlGpio = controlGpio_p;
	this->_controlE = controlE_p;
	this->_controlRs = controlRs_p;
	this->_controlRw = controlRw_p;
	this->_useBusyFlag = true;
	this->_controlPortSet = true;

	// Checks initialization
	this->_initialized = (this->_dataPortSet && this->_controlPortSet) ? true : false;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool LcdParallel::init(void)
{
	// Local variables
	uint8_t aux8 = 0;

	// Checks for initialization
	if(!this->_initialized) {
		// Returns error
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}

	// Performs initialization procedure
	aux8 = LCD_FUNCTION_SET_CMD | LCD_FUNCTION_8_BITS;

}

// =============================================================================
// Class own methods - Private
// =============================================================================

bool LcdParallel::_sendData4Bits(uint8_t data_p)
{

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool LcdParallel::_sendData8Bits(uint8_t data_p)
{
	// Checks if 4 bits interface is selected
	if(this->_dataLength == DATA_LENGTH_4_BITS) {

	} else {
#if defined(_FUNSAPE_PLATFORM_AVR)
		// TODO: Implement for AVR
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
		this->_dataGpio->
#endif

	}


	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool LcdParallel::_holdGpio(void)
{
	// Checks for errors
	if(!this->_initialized) {
		// Returns error
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}

	// Configures control port (E pin)
	if(this->_controlE > 7) {			// High nibble
		setMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (this->_controlE - 8)));		// Output
		clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (this->_controlE - 8)) + 2));	// Push-pull
	} else {							// Low nibble
		setMaskOffset(this->_controlGpio->CRL, 0x03, (4 * this->_controlE));				// Output
		clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * this->_controlE) + 2));		// Push-pull
	}

	// Configures control port (RS pin)
	if(this->_controlRs > 7) {			// High nibble
		setMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (this->_controlRs - 8)));		// Output
		clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (this->_controlRs - 8)) + 2));	// Push-pull
	} else {							// Low nibble
		setMaskOffset(this->_controlGpio->CRL, 0x03, (4 * this->_controlRs));				// Output
		clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * this->_controlRs) + 2));		// Push-pull
	}

	if(this->_useBusyFlag) {
		// Configures control port (Rw pin)
		if(this->_controlRw > 7) {			// High nibble
			setMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (this->_controlRw - 8)));		// Output
			clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (this->_controlRw - 8)) + 2));	// Push-pull
		} else {							// Low nibble
			setMaskOffset(this->_controlGpio->CRL, 0x03, (4 * this->_controlRw));				// Output
			clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * this->_controlRw) + 2));		// Push-pull
		}
	}

	// Configures data port
	uint8_t dataLength = (this->_dataLength == DATA_LENGTH_8_BITS) ? 8 : 4;
	for(uint8_t i = 0; i < dataLength; i++) {
		uint8_t auxPin = i + this->_dataFirst;
		if(auxPin > 7) {			// High nibble
			setMaskOffset(this->_dataGpio->CRH, 0x03, (4 * (auxPin - 8)));			// Output
			clrMaskOffset(this->_dataGpio->CRH, 0x03, ((4 * (auxPin - 8)) + 2));		// Push-pull
		} else {					// Low nibble
			setMaskOffset(this->_dataGpio->CRL, 0x03, (4 * auxPin));					// Output
			clrMaskOffset(this->_dataGpio->CRL, 0x03, ((4 * auxPin) + 2));			// Push-pull
		}
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool LcdParallel::_releaseGpio(void)
{
	// Checks for errors
	if(!this->_initialized) {
		// Returns error
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}

	// Configures control port (E pin)
	if(this->_controlE > 7) {			// High nibble
		clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (this->_controlE - 8)) + 2));	// Analog Mode
		setMaskOffset(this->_controlGpio->CRH, 0x01, ((4 * (this->_controlE - 8)) + 2));	// Floating input
		clrMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (this->_controlE - 8)));		// Input
	} else {							// Low nibble
		clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * this->_controlE) + 2));		// Analog Mode
		setMaskOffset(this->_controlGpio->CRL, 0x01, ((4 * this->_controlE) + 2));		// Floating input
		clrMaskOffset(this->_controlGpio->CRL, 0x03, (4 * this->_controlE));				// Input
	}

	// Configures control port (RS pin)
	if(this->_controlRs > 7) {			// High nibble
		clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (this->_controlRs - 8)) + 2));	// Analog Mode
		setMaskOffset(this->_controlGpio->CRH, 0x01, ((4 * (this->_controlRs - 8)) + 2));	// Floating input
		clrMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (this->_controlRs - 8)));		// Input
	} else {							// Low nibble
		clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * this->_controlRs) + 2));		// Analog Mode
		setMaskOffset(this->_controlGpio->CRL, 0x01, ((4 * this->_controlRs) + 2));		// Floating input
		clrMaskOffset(this->_controlGpio->CRL, 0x03, (4 * this->_controlRs));				// Input
	}

	if(this->_useBusyFlag) {
		// Configures control port (Rw pin)
		if(this->_controlRw > 7) {			// High nibble
			clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (this->_controlRw - 8)) + 2));	// Analog Mode
			setMaskOffset(this->_controlGpio->CRH, 0x01, ((4 * (this->_controlRw - 8)) + 2));	// Floating input
			clrMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (this->_controlRw - 8)));		// Input
		} else {							// Low nibble
			clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * this->_controlRw) + 2));		// Analog Mode
			setMaskOffset(this->_controlGpio->CRL, 0x01, ((4 * this->_controlRw) + 2));		// Floating input
			clrMaskOffset(this->_controlGpio->CRL, 0x03, (4 * this->_controlRw));				// Input
		}
	}

	// Configures data port
	uint8_t dataLength = (this->_dataLength == DATA_LENGTH_8_BITS) ? 8 : 4;
	for(uint8_t i = 0; i < dataLength; i++) {
		uint8_t auxPin = i + this->_dataFirst;
		if(auxPin > 7) {			// High nibble
			clrMaskOffset(this->_controlGpio->CRH, 0x03, ((4 * (auxPin - 8)) + 2));	// Analog Mode
			setMaskOffset(this->_controlGpio->CRH, 0x01, ((4 * (auxPin - 8)) + 2));	// Floating input
			clrMaskOffset(this->_controlGpio->CRH, 0x03, (4 * (auxPin - 8)));			// Input
		} else {					// Low nibble
			clrMaskOffset(this->_controlGpio->CRL, 0x03, ((4 * auxPin) + 2));			// Analog Mode
			setMaskOffset(this->_controlGpio->CRL, 0x01, ((4 * auxPin) + 2));			// Floating input
			clrMaskOffset(this->_controlGpio->CRL, 0x03, (4 * auxPin));				// Input
		}
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
