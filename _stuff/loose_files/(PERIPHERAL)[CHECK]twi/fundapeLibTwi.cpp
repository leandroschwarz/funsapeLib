/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibTwi.cpp
 * Module:			TWI Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibTwi.hpp"
#ifndef __FUNSAPE_LIB_TWI_HPP
#	error	[funsapeLibTwi.cpp] Error 1 - Header file (funsapeLibTwi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_TWI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibTwi.cpp] Error 13 - Build mismatch between source (funsapeLibTwi.cpp) and header (funsapeLibTwi.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define TWI_DEFAULT_TIMEOUT				30
#define TWI_MAX_BUFFER_SIZE				200
#define TWI_MIN_BUFFER_SIZE				5
#define TWI_READ_FLAG					0x01
#define TWI_WRITE_FLAG					0x00

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
 * @brief Construct a new Twi::Twi object
 *
 */
Twi::Twi(void)
{
	// Resets data members
	this->_bufferData					= nullptr;
	this->_bufferIndex					= 0;
	this->_bufferLength					= 0;
	this->_bufferMaxSize				= 0;
	this->_busy							= false;
	this->_devAddress					= 0;
	this->_devAddressLong				= false;
	this->_devAddressSet				= false;
	this->_initialized					= false;
	this->_regAddress					= 0;
	this->_timeout						= TWI_DEFAULT_TIMEOUT;
	this->_twiHandler					= nullptr;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Destroy the Twi::Twi object
 *
 */
Twi::~Twi(void)
{
	// Returns successfully
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================


