/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibHandler.hpp
 * Module:			Generic Handler Stream for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Include guard
// 

#ifndef __FUNSAPE_LIB_HANDLER_HPP
#define __FUNSAPE_LIB_HANDLER_HPP				__BUILD_TO_REPLACE__

// 
// System file dependencies
// 

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "..\funsapeLibGlobalDefines.hpp"
#if __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibHandler.hpp] Error 101 - Global definitions file (funsapeLibGlobalDefines.hpp) must be build __BUILD_TO_REPLACE__.
#endif

// -----------------------------------------------------------------------------
// Other header files ----------------------------------------------------------

// NONE

// 
// Undefining previous definitions
// 

// NONE

// 
// Constant definitions
// 

// NONE

// 
// Macro-function definitions
// *INDENT-OFF*
// 

// NONE

// *INDENT-ON*

// 
// New data types
// 

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (logic values) -------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (small and finite lists) ---------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (large or expansible lists) ------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (algebraic operated list elements) -----------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (bitwise operated flags) ---------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (bitwise operated flags) ---------------------------------------

// NONE

// 
// Extern global variables
// 

// NONE

// 
// Handler - Class declaration
// 

class Handler
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	typedef enum handlerType_e {
		HANDLER_TYPE_NONE				= 0,
		HANDLER_TYPE_OWI				= 1,
		HANDLER_TYPE_PARALLEL			= 2,
		HANDLER_TYPE_SERIAL				= 3,
		HANDLER_TYPE_SPI				= 4,
		HANDLER_TYPE_TWI				= 5,
		HANDLER_TYPE_UART				= 6,
	} handlerType_e;

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
	virtual bool sendData(uint8_t *buffData, uint16_t buffSize) = 0;
	virtual bool sendData(uint8_t *txBuffData, uint8_t *rxBuffData, uint16_t buffSize) = 0;
	virtual bool sendData(uint8_t reg, uint8_t *buffData, uint16_t buffSize) = 0;
	virtual bool sendData(uint8_t reg, uint8_t *txBuffData, uint8_t *rxBuffData, uint16_t buffSize) = 0;
	virtual bool setDevice(void (* actFunc)(void), void (* deactFunc)(void)) = 0;
	virtual handlerType_e getHandlerType(void) = 0;

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

};	// class Handler

// 
// Handler - Class inline function definitions
// 

// NONE

// 
// External default objects
// 

// NONE

// 
// General public functions declarations
// 

// NONE

// 
// General inline functions definitions
// 

// NONE

#endif	// __FUNSAPE_LIB_HANDLER_HPP
