/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibDebug.hpp
 * Module:			Debug Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Include guard
// 

#ifndef __FUNSAPE_LIB_DEBUG_HPP
#define __FUNSAPE_LIB_DEBUG_HPP					__BUILD_TO_REPLACE__

// 
// Dependencies
// 

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibSpi.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.hpp] Error 11 - Build mismatch between (funsapeLibSpi.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibSpi.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibSpiBitBang.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif

// 
// Undefining previous definitions
// 

// NONE

// 
// Constant definitions
// 

// 
// Macro-function definitions
// *INDENT-OFF*
// 

#define debugMessage(code)				debug.logMessage(code, (uint8_t *)__FILE__, __LINE__)
// #define debugMessage(code)				do{}while(0)

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
// Debug - Class declaration
// 

class Debug
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
	Debug(void);
	~Debug(void);												// TODO: Implement function

	// -------------------------------------------------------------------------
	// Methods - Own methods ---------------------------------------------------

public:
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	bool init(
		UART_HandleTypeDef *handler_p,
		bool hideSuccess_p						= true,
		bool stopOnError_p						= true
	);
#elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
	// TODO: AVR implementation
	bool init(
		uint8_t *handler_p,
		bool hideSuccess_p						= true,
		bool stopOnError_p						= true
	);
#endif
	// NONE
	error_e getLastError(void);
	bool setTimeOut(uint16_t timeOut_p);
	void logMessage(uint16_t errorCode_p, uint8_t *file_p, uint16_t line_p);

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	UART_HandleTypeDef *_uartHandler;
#elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
	uint8_t *_uartHandler;										// TODO: AVR implementation
#endif

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool		_initialized						: 1;
	bool		_stopOnFirstError					: 1;
	bool		_hideSuccessMessages				: 1;
	error_e		_lastError;
	uint16_t	_timeOut;

protected:
	// NONE

}; // class Spi

// 
// Spi - Class overloading operators
// 

// NONE

// 
// Spi - Class inline function definitions
// 

// NONE

// 
// External default objects
// 

extern Debug debug;

// 
// General public functions declarations
// 

// NONE

// 
// General inline functions definitions
// 

// NONE

#endif // __FUNSAPE_LIB_DEBUG_HPP
