/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibStopwatch.hpp
 * Module:			Stopwatch Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Include guard
// 

#ifndef __FUNSAPE_LIB_STOPWATCH_HPP
#define __FUNSAPE_LIB_STOPWATCH_HPP				__BUILD_TO_REPLACE__

// 
// Dependencies
// 

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibStopwatch.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibStopwatch.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibStopwatch.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibStopwatch.hpp)!
#endif

// 
// Platform verification
// 

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

// NONE

// 
// Extern global variables
// 

// NONE

// 
// Stopwatch - Class declaration
// 

class Stopwatch
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	// NONE

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
	Stopwatch(
			uint32_t timeout_p = 0
	);
	// TODO: Implement destructor function
	~Stopwatch(
			void
	);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	uint32_t elapsed(
			void
	);
	bool isTimedOut(
			bool stopOnSuccess_p = false
	);
	void start(
			uint32_t timeout_p = 0
	);
	bool waitUntilTimedOut(
			bool stopOnSuccess_p = false
	);

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool		_started				= false;
	uint32_t	_timeout				= 0;
	uint32_t	_valueInitial			= 0;
	uint32_t	_valueMark				= 0;
	Error		_lastError;

protected:
	// NONE

}; // class Stopwatch

// 
// Stopwatch - Class overloading operators
// 

// NONE

// 
// Global variables
// 

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// 
// Stopwatch - Class inline function definitions
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

// 
// External default objects
// 

// NONE

#endif // __FUNSAPE_LIB_STOPWATCH_HPP

// 
// END OF FILE
// 
