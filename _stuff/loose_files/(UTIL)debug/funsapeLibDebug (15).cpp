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
 * @brief Construct a new Debug:: Debug object
 *
 */
Debug::Debug(void)
{
	// Resets data members
	this->_debugDeviceMax30102			= false;
	this->_debugDeviceMpu9250			= false;
	this->_debugModuleGpio				= false;
	this->_debugModuleSpi				= false;
	this->_debugModuleStopwatch			= false;
	this->_debugModuleTwi				= false;
	this->_debugModuleUart				= false;
	this->_hideSuccessMessages			= true;
	this->_initialized					= false;
	this->_markLevel					= 0;
	this->_stopOnFirstError				= true;
	this->_timeOut						= DEBUG_DEFAULT_TIME_OUT;
	this->_uartHandler					= nullptr;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

/**
 * @brief Destroy the Debug:: Debug object
 *
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
 *
 * @param[out] handler_p
 * @param[in] hideSuccess_p
 * @param[in] stopOnError_p
 * @param[in] markLevel_p
 * @return true
 * @return false
 */
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_UART_MODULE_ENABLED)
// ARM implementation
bool Debug::init(UART_HandleTypeDef *handler_p, bool hideSuccess_p, bool stopOnError_p, uint8_t markLevel_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)
// AVR implementation
// TODO: AVR implementation
bool Debug::init(uint8_t *handler_p, bool hideSuccess_p, bool stopOnError_p, uint8_t markLevel_p)
#else
bool Debug::init(uint8_t *handler_p, bool hideSuccess_p, bool stopOnError_p, uint8_t markLevel_p)
#endif
{
	// CHECK FOR ERROR - handler pointer invalid
	if(!isPointerValid(handler_p)) {
		this->_lastError = Error::HANDLER_POINTER_NULL;
		return false;
	}

	// Update data members
	this->_hideSuccessMessages = hideSuccess_p;
	this->_initialized = true;
	this->_markLevel = markLevel_p;
	this->_stopOnFirstError = stopOnError_p;
	this->_uartHandler = handler_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

/**
 * @brief
 *
 * @param[in] timeOut_p
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
 *
 * @return Error
 */
Error Debug::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

/**
 * @brief
 *
 * @param[in] errorCode_p
 * @param[in] file_p
 * @param[in] line_p
 */
void Debug::logMessage(uint16_t errorCode_p, const uint8_t *fileName_p, uint16_t lineNumber_p)
{
	char strTest[200];

	// Just exits if user doesn't want to log success messages
	if(this->_hideSuccessMessages) {
		if(errorCode_p == 0) {
			return;
		}
	}

	// Logs message
	sprintf(strTest, "%s:%d - %d\r", fileName_p, (uint16_t)lineNumber_p, (uint16_t)errorCode_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_UART_MODULE_ENABLED)
	// ARM implementation
	HAL_UART_Transmit(this->_uartHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
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

/**
 * @brief
 *
 * @param[in] file_p
 * @param[in] line_p
 * @param[in] identifier_p
 */
void Debug::logMark(const uint8_t *fileName_p, uint16_t lineNumber_p, const uint8_t *identifier_p)
{
	char strTest[200];

	// Just exits if user doesn't want to log marks
	if(this->_markLevel == 0) {
		return;
	}

	// Logs message
	sprintf(strTest, "-> %s : %d @ %s\r", (char *)fileName_p, (uint16_t)lineNumber_p, (char *)identifier_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_UART_MODULE_ENABLED)
	// ARM implementation
	HAL_UART_Transmit(this->_uartHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
#endif

	// Returns
	return;
}

void Debug::debugModuleGpio(bool enable_p)
{
	// Update data members
	this->_debugModuleGpio = true;
}

void Debug::debugModuleUart(bool enable_p)
{
	// Update data members
	this->_debugModuleUart = true;
}

void Debug::debugModuleSpi(bool enable_p)
{
	// Update data members
	this->_debugModuleSpi = true;
}

void Debug::debugModuleTwi(bool enable_p)
{
	// Update data members
	this->_debugModuleTwi = true;
}

void Debug::debugModuleStopwatch(bool enable_p)
{
	// Update data members
	this->_debugModuleStopwatch = true;
}

void Debug::debugDeviceMax30102(bool enabled_p)
{
	// Update data members
	this->_debugDeviceMax30102 = true;
}

void Debug::debugDeviceMpu9250(bool enabled_p)
{
	// Update data members
	this->_debugDeviceMpu9250 = true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
