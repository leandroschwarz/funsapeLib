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
#	error	[funsapeLibDebug.cpp] Error 16 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDebug.cpp] Error 17 - Build mismatch between source (funsapeLibDebug.cpp) and header (funsapeLibDebug.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define DEBUG_DEFAULT_TIME_OUT			30

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

/**
 * @brief
 * @details
 */
Debug::Debug(void)
{
	// Resets data members
	this->_hideSuccessMessages			= true;
	this->_initialized					= false;
	this->_markLevel					= 0;
	this->_messageHandler				= nullptr;
	this->_stopOnFirstError				= true;
	this->_timeOut						= DEBUG_DEFAULT_TIME_OUT;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

/**
 * @brief
 * @details
 */
Debug::~Debug(void)
{
	// TODO: Implement destructor function

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

/**
 * @brief
 * @details
 * @param[out]	handler_p
 * @param[in]	hideSuccess_p
 * @param[in]	stopOnError_p
 * @param[in]	markLevel_p
 * @return true
 * @return false
 */
bool Debug::init(UART_HandleTypeDef *handler_p, bool hideSuccess_p, bool stopOnError_p, uint32_t markLevel_p)
{
	// Checks for errors
	if(!isPointerValid(handler_p)) {
		this->_lastError = Error::HANDLER_POINTER_NULL;
		return false;
	}

	// Update data members
	this->_hideSuccessMessages = hideSuccess_p;
	this->_initialized = true;
	this->_markLevel = markLevel_p;
	this->_messageHandler = handler_p;
	this->_stopOnFirstError = stopOnError_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

/**
 * @brief
 * @details
 * @param[in]	timeOut_p
 * @return true
 * @return false
 */
bool Debug::setTimeOut(uint16_t timeOut_p)
{
	// Update data members
	this->_timeOut = timeOut_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

/**
 * @brief
 * @details
 * @return Error
 */
Error Debug::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

/**
 * @brief
 * @details
 * @param[in]	errorCode_p
 * @param[in]	file_p
 * @param[in]	line_p
 */
void Debug::logMessage(uint16_t errorCode_p, uint8_t *file_p, uint16_t line_p)
{
	// Local variables
	char strTest[200];

	// Just exits if user doesn't want to log success messages
	if(this->_hideSuccessMessages) {
		if(errorCode_p == 0) {
			return;
		}
	}

	// Logs message
	sprintf(strTest, "%s:%d - 0x%04x\r", file_p, (uint16_t)line_p, (uint16_t)errorCode_p);
	HAL_UART_Transmit(this->_messageHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);

	// If must stop on first error
	if(this->_stopOnFirstError) {
		if(errorCode_p) {
			systemHalt();
		}
	}

	// Returns
	return;
}

/**
 * @brief
 * @details
 * @param[in]	message_p
 */
void Debug::printMessage(char *message_p)
{

	// Logs message
	HAL_UART_Transmit(this->_messageHandler, (uint8_t *)message_p, strlen(message_p), this->_timeOut);

	// Returns
	return;
}

/**
 * @brief
 * @details
 * @param[in]	file_p
 * @param[in]	line_p
 * @param[in]	identifier_p
 */
void Debug::logMark(uint8_t *file_p, uint16_t line_p, uint8_t *identifier_p)
{
	char strTest[200];

	// Just exits if user doesn't want to log marks
	if(this->_markLevel == 0) {
		return;
	}

	// Logs message
	sprintf(strTest, "-> %s : %d @ %s\r", (char *)file_p, (uint16_t)line_p, (char *)identifier_p);
	HAL_UART_Transmit(this->_messageHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);

	// Returns
	return;
}

/**
 * @brief
 * @details
 * @return
 */
uint32_t Debug::getMarkLevel(void)
{
	// Returns
	return this->_markLevel;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// END OF FILE
// =============================================================================
