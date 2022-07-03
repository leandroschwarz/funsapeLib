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
#	error	[funsapeLibStopwatch.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.hpp] Error 10 - Build mismatch between (funsapeLibStopwatch.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

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
	// New data types
	// -------------------------------------------------------------------------

public:
	// NONE

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
	Stopwatch(uint32_t timeout_p = 0);
	~Stopwatch(void);											// TODO: Implement function

	// -------------------------------------------------------------------------
	// Methods - Inherited methods
	// -------------------------------------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods
	// -------------------------------------------------------------------------

public:
	void start(
			uint32_t timeout_p = 0
	);
	uint32_t elapsed(void);
	bool isTimedOut(
			bool stopOnSuccess_p = false
	);

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
public:
	bool _started = false;
	uint32_t _valueInitial = 0;
	uint32_t _valueMark = 0;
	uint32_t _timeout = 0;

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
