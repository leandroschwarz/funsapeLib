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
// System file dependencies
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
#	error	[funsapeLibHandler.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibHandler.hpp] Error 2 - Build mismatch between (funsapeLibHandler.hpp) and (funsapeLibDebug.hpp) header files!
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
	// New data types ----------------------------------------------------------

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
	// Operators overloading ---------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	// NONE

	// -------------------------------------------------------------------------
	// Methods -----------------------------------------------------------------

public:
	//     ////////////////////    DATA TRANSFER     ////////////////////     //
	virtual bool readReg(
			uint8_t reg_p,
			uint8_t *buffData_p,
			uint16_t buffSize_p
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};
	virtual bool sendData(
			uint8_t *buffData_p,
			uint16_t buffSize_p
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};
	virtual bool sendData(
			uint8_t *txBuffData_p,
			uint8_t *rxBuffData_p,
			uint16_t buffSize_p
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};
	virtual bool writeReg(
			uint8_t reg_p,
			uint8_t *buffData_p,
			uint16_t buffSize_p
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};

	//     //////////////////    PROTOCOL SPECIFIC     //////////////////     //
	virtual bool setAddress(
			uint16_t address_p,
			bool useLongAddress_p = false
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};
	virtual bool setDevice(
			void (* actFunc_p)(void),
			void (* deactFunc_p)(void)
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};

	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	virtual Handler::HandlerType getHandlerType(void) {
		// Returns default handler
		return HandlerType::NONE;
	}
	virtual Error getLastError(void) {
		// Returns last error
		return this->_lastError;
	}

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

public:
	// NONE

private:
	Error	_lastError;

protected:
	// NONE

};	// class Handler

// =============================================================================
// Handler - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
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

#endif	// __FUNSAPE_LIB_HANDLER_HPP
