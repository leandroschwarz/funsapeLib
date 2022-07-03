/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibDebug.cpp
 * Module:			Debug Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// System file dependencies
// 

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibDebug.cpp] Error 1 - Header file (funsapeLibDebug.hpp) must be corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDebug.cpp] Error 9 - Build mismatch between source (funsapeLibDebug.cpp) and header (funsapeLibDebug.hpp) files!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 
// File exclusive - Constants
// 

#define		DEBUG_DEFAULT_TIME_OUT				30

// 
// File exclusive - New data types
// 

// NONE

// 
// File exclusive - Macro-functions
// 

// NONE

// 
// Global variables
// 

Debug debug;

// 
// Static functions declarations
// 

// NONE

// 
// Class constructors
// 

Debug::Debug(void)
{
	// Reset data members
	this->_uartHandler					= nullptr;
	this->_initialized					= false;
	this->_stopOnFirstError				= true;
	this->_hideSuccessMessages			= true;
	this->_timeOut						= DEBUG_DEFAULT_TIME_OUT;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

Debug::~Debug(void)
{
	// Return successfully
	return;
}

// 
// Class own methods - public
// 

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
bool Debug::init(UART_HandleTypeDef *handler_p, bool hideSuccess_p, bool stopOnError_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Debug::init(uint8_t *handler_p, bool hideSuccess_p, bool stopOnError_p)
#endif
{
	// Check for errors
	if (handler_p == nullptr) {
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		return false;
	}

	// Update data members
	this->_uartHandler = handler_p;
	this->_hideSuccessMessages = hideSuccess_p;
	this->_stopOnFirstError = stopOnError_p;
	this->_initialized = true;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Debug::setTimeOut(uint16_t timeOut_p)
{
	// Update data members
	this->_timeOut = timeOut_p;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

error_e Debug::getLastError(void)
{
	// Return error
	return this->_lastError;
}

void Debug::logMessage(uint16_t errorCode_p, uint8_t *file_p, uint16_t line_p)
{
	char strTest[100];

	// Just exit if user doesn't want to log success messages
	if (this->_hideSuccessMessages) {
		if (errorCode_p == 0) {
			return;
		}
	}

	// Log message
	sprintf(strTest, "%s:%d - %d\r", file_p, (uint16_t)line_p, (uint16_t)errorCode_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
	HAL_UART_Transmit(this->_uartHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// If any error occurred, show it on LED
//	for (uint8_t i = 0; i < errorCode_p; i++) {
//		ledPulseOn(200);
//	}

	// If must stop on first error
	if (this->_stopOnFirstError) {
		if (errorCode_p) {
			systemHalt();
		}
	}

	// Returns
	return;
}

// 
// Class own methods - private
// 

// NONE

// 
// Class own methods - protected
// 

// NONE
