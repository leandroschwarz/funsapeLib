/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibBus.hpp
 * Module:			Generic Bus Stream for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_BUS_HPP
#define __FUNSAPE_LIB_BUS_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibBus.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibBus.hpp] Error 10 - Build mismatch between (funsapeLibBus.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibBus.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibBus.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
#endif

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Bus - Class declaration
// =============================================================================

class Bus
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	enum class BusType {
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
	// Operator overloading ----------------------------------------------------

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
	// Methods - Inherited methods ---------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

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

	//     ////////////////     TWI PROTOCOL METHODS     ////////////////     //
	virtual bool setAddress(
			uint16_t address_p,
			bool useLongAddress_p = false
	) {
		// Returns error
		this->_lastError = Error::FEATURE_UNSUPPORTED;
		debugMessage(Error::FEATURE_UNSUPPORTED);
		return false;
	};

	//     ////////////////     SPI PROTOCOL METHODS     ////////////////     //
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
	virtual Bus::BusType getBusType(void) {
		// Returns default handler
		return BusType::NONE;
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
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
	Error	_lastError;

protected:
	// NONE

}; // class Bus

// =============================================================================
// Bus - Class overloading operators
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
// Bus - Class inline function definitions
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

#endif // __FUNSAPE_LIB_BUS_HPP
















/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibBus.hpp
 * Module:			Generic Bus Stream for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_BUS_HPP
#define __FUNSAPE_LIB_BUS_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibBus.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibBus.hpp] Error 10 - Build mismatch between (funsapeLibBus.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibBus.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibBus.hpp] Error 2 - Build mismatch between (funsapeLibBus.hpp) and (funsapeLibDebug.hpp) header files!
#endif

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Bus - Class declaration
// =============================================================================

class Bus
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	enum class BusType {
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
	// Methods - Inherited methods ---------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

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
	virtual Bus::BusType getBusType(void) {
		// Returns default handler
		return BusType::NONE;
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

};	// class Bus

// =============================================================================
// Bus - Class inline function definitions
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

#endif	// __FUNSAPE_LIB_BUS_HPP
