/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibDebug.hpp
 * Module:			Debug module for FunSAPE++ Embedded Library project
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
#	error	[funsapeLibDebug.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDebug.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibDebug.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

// NONE

// =============================================================================
// Platform verification
// =============================================================================

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

#define debugMessage(code)				do{debug.logMessage((uint16_t)(code), (uint8_t *)__FILE__, __LINE__);}while(0)
#define debugMark(identifier, level)	do{if(debug.getMarkLevel() & level){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)

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
	Debug(
			void
	);
	// TODO: Implement destructor function
	~Debug(
			void
	);

	// -------------------------------------------------------------------------
	// Methods - Inherited methods ---------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	Error getLastError(
			void
	);
	uint32_t getMarkLevel(
			void
	);
	bool init(
			UART_HandleTypeDef *handler_p,
			bool hideSuccess_p			= true,
			bool stopOnError_p			= true,
			uint32_t markLevel_p		= 0
	);
	bool setTimeOut(
			uint16_t timeOut_p
	);

	//     //////////////    MESSAGE RELATED FUNCTIONS     //////////////     //
	void logMark(
			uint8_t *file_p,
			uint16_t line_p,
			uint8_t *identifier_p
	);
	void logMessage(
			uint16_t errorCode_p,
			uint8_t *file_p,
			uint16_t line_p
	);
	void printMessage(
			char *message_p
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
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
	UART_HandleTypeDef *_messageHandler;

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool		_hideSuccessMessages				: 1;
	bool		_initialized						: 1;
	Error		_lastError;
	uint32_t	_markLevel;
	bool		_stopOnFirstError					: 1;
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

// =============================================================================
// END OF FILE
// =============================================================================
