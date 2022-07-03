/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibDebug.hpp
 * Module:			Debug Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_DEBUG_HPP
#define __FUNSAPE_LIB_DEBUG_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibDebug.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDebug.hpp] Error 10 - Build mismatch between (funsapeLibDebug.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

// NONE

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

#define debugMessage(code)				debug.logMessage((uint16_t)(code), (uint8_t *)__FILE__, __LINE__)
#define debugMark(identifier, level)	do{if(level == debug._markLevel){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)

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
// Debug - Class declaration
// =============================================================================

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
	// Methods - Inherited methods ---------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	bool init(
			UART_HandleTypeDef *handler_p,
			bool hideSuccess_p						= true,
			bool stopOnError_p						= true,
			uint8_t markLevel_p						= 0
	);
#elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
	// TODO: AVR implementation
	bool init(
			uint8_t *handler_p,
			bool hideSuccess_p						= true,
			bool stopOnError_p						= true,
			uint8_t markLevel_p						= 0
	);
#endif
	Error getLastError(void);
	bool setTimeOut(uint16_t timeOut_p);
	void logMessage(uint16_t errorCode_p, uint8_t *file_p, uint16_t line_p);
	void logMark(uint8_t *file_p, uint16_t line_p, uint8_t *identifier_p);

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
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_UART_MODULE_ENABLED)
	// ARM implementation
	UART_HandleTypeDef *_uartHandler;
#elif defined(_FUNSAPE_PLATFORM_AVR)							// AVR implementation
	uint8_t *_uartHandler;										// TODO: AVR implementation
#else
	uint8_t *_uartHandler;
#endif

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool		_initialized						: 1;
	bool		_stopOnFirstError					: 1;
	bool		_hideSuccessMessages				: 1;
	Error		_lastError;
	uint8_t		_markLevel;
	uint16_t	_timeOut;

protected:
	// NONE

}; // class Debug

// =============================================================================
// Debug - Class overloading operators
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
// Debug - Class inline function definitions
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

extern Debug debug;

#endif // __FUNSAPE_LIB_DEBUG_HPP
