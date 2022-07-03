/* =============================================================================
 * Project:		FunSAPE++ AVR Library
 * File name:	muxDisplay.hpp
 * Module:		Seven Segments Multiplexed Display cotroller class
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "muxDisplay.hpp"
#if __MUX_DISPLAY_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [muxDisplay.cpp] Build mismatch on header and source code files.
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
// Class constructors
// =============================================================================


MuxDisplay::MuxDisplay(uint8_t displaySize)
{
	// Update data members
	this->_initialized		= false;
	this->_minutes			= 0;
	this->_type				= DISPLAY_COMMON_ANODE;
	this->_segmetsSplited	= false;
	this->_displayAsClock	= false;
	this->_value			= 0;
	this->_hours			= 0;
	this->_segAdDir			= NULL;
	this->_segAdOut			= NULL;
	this->_segAdOffset		= 0;
	this->_segEpDir			= NULL;
	this->_segEpOut			= NULL;
	this->_segEpOffset		= 0;
	this->_controlDir		= NULL;
	this->_controlOut		= NULL;
	this->_controlOffset	= 0;
	this->_displayIndex		= 0;
	this->_displaySize		= 0;
	this->_displayData		= NULL;

	// Check for errors
	if (displaySize == 0) {
		this->_lastError = ERROR_ARGUMENT_CANNOT_BE_ZERO;
		return;
	} else if (displaySize > 8) {
		this->_lastError = ERROR_ARGUMENTS_WRONG_NUMBER;
		return;
	}

	// Memory allocation
	this->_displayData = (uint8_t *)calloc(displaySize, sizeof(uint8_t));
	if (this->_displayData == NULL) {
		this->_lastError = ERROR_MEMORY_ALLOCATION;
		return;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

error_e MuxDisplay::getLastError(void)
{
	return this->_lastError;
}

bool MuxDisplay::setValue(uint32_t counter)
{
	this->_value = counter;
	for (uint8_t i = 0; i < this->_displaySize; i++) {
		this->_displayData[i] = this->_getSegments((uint8_t)(counter % 10), false);
		counter /= 10;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool MuxDisplay::setValue(uint8_t hours, uint8_t minutes)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool MuxDisplay::setGpio(GpioTypeDef_e segments, GpioTypeDef_e control, uint8_t controlOffset)
{
	// Check for errors
	if ((segments == GPIO_NONE) || (control == GPIO_NONE)) {
		this->_lastError = ERROR_GPIO_UNSUPPORTED;
		return false;
	}
	if ((controlOffset + this->_displaySize) > 8) {
		this->_lastError = ERROR_WRONG_CONFIGURATION;
		return false;
	}

	// Update data members
	this->_segmetsSplited = false;
	this->_segAdDir = (reg_t *)((3 * segments) + 1);
	this->_segAdOut = (reg_t *)((3 * segments) + 2);
	this->_segAdOffset = 0;
	this->_segEpDir = NULL;
	this->_segEpOut = NULL;
	this->_segEpOffset = 0;
	this->_controlDir = (reg_t *)(3 * control + 1);
	this->_controlOut = (reg_t *)(3 * control + 2);
	this->_controlOffset = controlOffset;

	// GPIO configuration
	* (this->_segAdOut) = 0xFF;
	* (this->_segAdDir) = 0xFF;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool MuxDisplay::setGpio(GpioTypeDef_e segAD, uint8_t segAdOffset, GpioTypeDef_e segEP, uint8_t segEpOffset,
		GpioTypeDef_e control, uint8_t controlOffset)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool MuxDisplay::update(void)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

// =============================================================================
// Class private methods
// =============================================================================

uint8_t MuxDisplay::_getSegments(uint8_t value, bool pointStatus)
{
	uint8_t segments = 0;

	switch (value) {			// PGFEDCBA
	case 0x00:			segments = 0b00111111;	break;
	case 0x01:			segments = 0b00000110;	break;
	case 0x02:			segments = 0b01011011;	break;
	case 0x03:			segments = 0b01001111;	break;
	case 0x04:			segments = 0b01100110;	break;
	case 0x05:			segments = 0b01101101;	break;
	case 0x06:			segments = 0b01111101;	break;
	case 0x07:			segments = 0b00000111;	break;
	case 0x08:			segments = 0b01111111;	break;
	case 0x09:			segments = 0b01101111;	break;
	}
	// Point
	segments |= pointStatus << 7;
	// Display type
	segments = (this->_type) ? ~segments : segments;

	return segments;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Interruption handlers
// =============================================================================

// NONE
