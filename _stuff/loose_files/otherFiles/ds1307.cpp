/* =============================================================================
 * Project:			FunSAPE MCU++ Embedded Library
 * File name:		ds1307.cpp
 * Module:			DS1307 Real Time Clock interface module for FunSAPE++
 * 					Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "ds1307.hpp"
#ifndef __DS1307_HPP
#	error	[ds1307.cpp] Error 1 - Header file (ds1307.hpp) must be corrupted!
#elif __DS1307_HPP != __BUILD_TO_REPLACE__
#	error	[ds1307.cpp] Error 9 - Build mismatch between source (ds1307.cpp) and header (ds1307.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define DS1307_SLAVE_ADDRESS			0x68
#define DS1307_ADDRESS_SECONDS			0x00
#define DS1307_ADDRESS_COUNTING			0x00
#define DS1307_ADDRESS_MINUTES			0x01
#define DS1307_ADDRESS_HOURS			0x02
#define DS1307_ADDRESS_TIME_FORMAT		0x02
#define DS1307_ADDRESS_WEEK_DAY			0x03
#define DS1307_ADDRESS_MONTH_DAY		0x04
#define DS1307_ADDRESS_MONTH			0x05
#define DS1307_ADDRESS_YEAR				0x06
#define DS1307_ADDRESS_CONTROL			0x07
#define DS1307_ADDRESS_RAM				0x08
#define DS1307_I2C_MIN_CLOCK_SPEED		0UL
#define DS1307_I2C_MAX_CLOCK_SPEED		100000UL

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

Ds1307::Ds1307(void)
{
	// Reset data members
	this->_initialized					= false;
	this->_lastError					= Error::NONE;
	this->_squareWaveActivation			= 0;
	this->_squareWaveOffValue			= 0;
	this->_squareWaveFrequency			= 0;
	this->_countingHalted				= false;
	this->_year							= 0;
	this->_month						= Month::JANUARY;
	this->_monthDay						= 1;
	this->_weekDay						= WeekDay::SATURDAY;
	this->_hours						= 0;
	this->_amPm							= false;
	this->_minutes						= 0;
	this->_seconds						= 0;
	this->_timeFormat					= false;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

// =============================================================================
// Class own methods - public
// =============================================================================

// NONE

// =============================================================================
// Class own methods - private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - protected
// =============================================================================

// NONE

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Static functions definitions
// =============================================================================

// NONE
