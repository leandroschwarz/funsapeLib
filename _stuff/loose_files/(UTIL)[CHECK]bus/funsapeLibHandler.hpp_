/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibHandler.hpp
 * Module:			Generic Handler Stream for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_HANDLER_HPP
#define __FUNSAPE_LIB_HANDLER_HPP				__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibHandler.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibHandler.hpp] Error 10 - Build mismatch between (funsapeLibHandler.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[max30102.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[max30102.hpp] Error 2 - Build mismatch between (max30102.hpp) and (funsapeLibDebug.hpp) header files!
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
// Handler - Class declaration
// =============================================================================

class Handler
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	enum class HandlerType {
		NONE							= 0,
		OWI								= 1,
		PARALLEL						= 2,
		SERIAL							= 3,
		SPI								= 4,
		TWI								= 5,
		UART							= 6,
	};

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
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Virtual methods
	// -------------------------------------------------------------------------

public:
	virtual bool write(
			uint8_t reg_p,
			uint8_t *buffData_p,
			uint16_t buffSize_p
	) = 0;
	virtual bool read(
			uint8_t reg_p,
			uint8_t *rxBuffData_p,
			uint16_t buffSize_p
	) = 0;
	virtual HandlerType getHandlerType(void) = 0;
	virtual bool setTwiAddress(uint16_t twiAddress_p, bool useLongAddress = false) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};
	virtual Error getLastError(void) = 0;

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
	// NONE

protected:
	Error	_lastError;
	// NONE

};	// class Handler

// =============================================================================
// Handler - Class overloading operators
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
// Handler - Class inline function definitions
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

// NONE

#endif	// __FUNSAPE_LIB_HANDLER_HPP
