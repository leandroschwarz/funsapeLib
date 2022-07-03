/* =============================================================================
 * Project:		FunSAPE++ AVR Library
 * File name:	sevenSegments.cpp
 * Module:		Seven Segments Display cotroller class
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * Purpose:		Seven segments display controller with support to common anode
 *				and common cathode displays. Special characters dash  (0xFD), U
 *				(0xFE) and display off (0xFF) were also implented.
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "sevenSegments.hpp"
#if __SEVEN_SEGMENTS_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [sevenSegments.cpp] Build mismatch on header and source code files.
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


SevenSegments::SevenSegments(displayType_t type, uint8_t value, bool point)
{
	// Update data members
	this->_type = type;
	this->_value = value;
	this->_pointStatus = point;

	// Return successfully
	return;
}


// =============================================================================
// Class public methods
// =============================================================================

void SevenSegments::setValue(uint8_t newValue)
{
	this->_value = (newValue < DISPLAY_OFF) ? newValue : DISPLAY_OFF;
	return;
}

void SevenSegments::setPoint(bool newPointState)
{
	this->_pointStatus = newPointState;
	return;
}

uint8_t SevenSegments::getSegments(void)
{
	uint8_t segments = 0;

	switch (this->_value) {		  // PGFEDCBA
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
	case 0x0A:			segments = 0b01110111;	break;
	case 0x0B:			segments = 0b01111100;	break;
	case 0x0C:			segments = 0b00111001;	break;
	case 0x0D:			segments = 0b01011110;	break;
	case 0x0E:			segments = 0b01111001;	break;
	case 0x0F:			segments = 0b01110001;	break;
	case DISPLAY_DASH:	segments = 0b01000000;	break;	// Dash
	case DISPLAY_U:		segments = 0b00111110;	break;	// U
	case DISPLAY_OFF:	segments = 0b00000000;	break;	// OFF
	}
	// Point
	segments |= this->_pointStatus << 7;
	// Display type
	segments = (this->_type) ? ~segments : segments;

	return segments;
}

// =============================================================================
// Class private methods
// =============================================================================

// NONE

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
