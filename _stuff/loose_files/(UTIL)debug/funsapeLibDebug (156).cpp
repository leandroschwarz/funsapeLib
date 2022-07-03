/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibDebug.cpp
 * Module:			Debug Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibDebug.cpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDebug.cpp] Error 13 - Build mismatch between source (funsapeLibDebug.cpp) and header (funsapeLibDebug.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		DEBUG_DEFAULT_TIME_OUT				30

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

Debug debug;

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

Debug::Debug(void)
{
	// Resets data members
	this->_uartHandler					= nullptr;
	this->_initialized					= false;
	this->_stopOnFirstError				= true;
	this->_hideSuccessMessages			= true;
	this->_timeOut						= DEBUG_DEFAULT_TIME_OUT;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

Debug::~Debug(void)
{
	// Returns successfully
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
bool Debug::init(UART_HandleTypeDef *handler_p, bool hideSuccess_p, bool stopOnError_p, bool showMarks_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Debug::init(uint8_t *handler_p, bool hideSuccess_p, bool stopOnError_p, bool showMarks_p)
#endif
{
	// Checks for errors
	if(!isPointerValid(handler_p)) {
		this->_lastError = Error::HANDLER_POINTER_NULL;
		return false;
	}

	// Updates data members
	this->_uartHandler = handler_p;
	this->_hideSuccessMessages = hideSuccess_p;
	this->_stopOnFirstError = stopOnError_p;
	this->_showMarks = showMarks_p;
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool Debug::setTimeOut(uint16_t timeOut_p)
{
	// Updates data members
	this->_timeOut = timeOut_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

Error Debug::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

void Debug::logMessage(uint16_t errorCode_p, uint8_t *file_p, uint16_t line_p)
{
	char strTest[100];

	// Just exits if user doesn't want to log success messages
	if(this->_hideSuccessMessages) {
		if(errorCode_p == 0) {
			return;
		}
	}

	// Logs message
	sprintf(strTest, "%s:%d - %d\r", file_p, (uint16_t)line_p, (uint16_t)errorCode_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	HAL_UART_Transmit(this->_uartHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// If must stop on first error
	if(this->_stopOnFirstError) {
		if(errorCode_p) {
			systemHalt();
		}
	}

	// Returns
	return;
}

void Debug::logMark(uint8_t *file_p, uint16_t line_p)
{
	char strTest[100];

	// Just exits if user doesn't want to log marks
	if(!this->_showMarks) {
		return;
	}

	// Logs message
	sprintf(strTest, "-> %s:%d\r", file_p, (uint16_t)line_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	HAL_UART_Transmit(this->_uartHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// Returns
	return;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
