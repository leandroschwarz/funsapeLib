/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDebug.cpp
 * Module:          Debug Module for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibDebug.cpp] Error - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibDebug.cpp] Error - Build mismatch between source (funsapeLibDebug.cpp) and header (funsapeLibDebug.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define DEBUG_DEFAULT_TIME_OUT          30

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
    this->_hideSuccessMessages          = true;
    this->_initialized                  = false;
    this->_markLevel                    = 0;
    this->_busHandler                   = nullptr;
    this->_stopOnFirstError             = true;
    this->_timeOut                      = DEBUG_DEFAULT_TIME_OUT;

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

    //     ///////////////////     INITIALIZATION     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] handler_p    description of this parameter
 * @param[in] hideSuccess_p description of this parameter
 * @param[in] stopOnError_p description of this parameter
 * @param[in] markLevel_p   description of this parameter
 * @return true
 * @return false
 */
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)
bool_t Debug::init(UART_HandleTypeDef *handler_p, cbool_t hideSuccess_p, cbool_t stopOnError_p, cuint32_t markLevel_p)
#   else
bool_t Debug::init(uart16_t *handler_p, cbool_t hideSuccess_p, cbool_t stopOnError_p, uint8_t markLevel_p)
#   endif
#elif defined(_FUNSAPE_PLATFORM_AVR)
bool_t Debug::init(uart16_t *handler_p, cbool_t hideSuccess_p, cbool_t stopOnError_p, uint8_t markLevel_p)
#endif
{
    // Checks for errors
    if (!isPointerValid(handler_p)) {
        this->_lastError = Error::HANDLER_POINTER_NULL;
        return false;
    }

    // Update data members
    this->_hideSuccessMessages = hideSuccess_p;
    this->_initialized = true;
    this->_markLevel = markLevel_p;
    this->_busHandler = handler_p;
    this->_stopOnFirstError = stopOnError_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

    //     /////////////////     CONTROL AND STATUS     /////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
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
 * Detailed description of this element.
 *
 * @return
 */
uint32_t Debug::getMarkLevel(void)
{
    // Returns
    return this->_markLevel;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] hideSuccess_p description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::hideSuccess(cbool_t hideSuccess_p)
{
    // Update data members
    this->_hideSuccessMessages = hideSuccess_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in]   file_p          description of this parameter
 * @param[in]   line_p          description of this parameter
 * @param[in]   identifier_p    description of this parameter
 */
void Debug::logMark(cuint8_t *file_p, cuint16_t line_p, cuint8_t *identifier_p)
{
    char strTest[200];

    // Just exits if user doesn't want to log marks
    if (this->_markLevel == 0) {
        return;
    }

    // Logs message
    sprintf(strTest, "-> %s : %d @ %s\r", (char *)file_p, (uint16_t)line_p, (char *)identifier_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)
    HAL_UART_Transmit(this->_busHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#   else
    // TODO: Non-UART HAL ARM implementation
#   endif
#elif defined(_FUNSAPE_PLATFORM_AVR)
    // TODO: AVR implementation
#endif

    // Returns
    return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in]   errorCode_p description of this parameter
 * @param[in]   file_p      description of this parameter
 * @param[in]   line_p      description of this parameter
 */
void Debug::logMessage(cuint16_t errorCode_p, cuint8_t *file_p, cuint16_t line_p)
{
    char strTest[200];

    // Just exits if user doesn't want to log success messages
    if (this->_hideSuccessMessages) {
        if (errorCode_p == 0) {
            return;
        }
    }

    // Logs message
    sprintf(strTest, "%s:%d - %d\r", file_p, (uint16_t)line_p, (uint16_t)errorCode_p);
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)
    HAL_UART_Transmit(this->_busHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);
#   else
    // TODO: Non-UART HAL ARM implementation
#   endif
#elif defined(_FUNSAPE_PLATFORM_AVR)
    // TODO: AVR implementation
#endif

    // If must stop on first error
    if (this->_stopOnFirstError) {
        if (errorCode_p) {
            systemHalt();
        }
    }

    // Returns
    return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] markLevel_p   description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::setMarkLevel(cuint32_t markLevel_p)
{
    // Update data members
    this->_markLevel = markLevel_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] timeOut_p description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::setTimeOut(cuint16_t timeOut_p)
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
 * Detailed description of this element.
 *
 * @param[in] enabled_p description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::stopOnError(cbool_t stopOnError_p)
{
    // Update data members
    this->_stopOnFirstError = stopOnError_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in]   message_p
 */
bool_t Debug::printMessage(char *message_p)
{
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)
    HAL_StatusTypeDef halError;

    // Logs message
    halError = HAL_UART_Transmit(this->_busHandler, (uint8_t *)message_p, strlen(message_p), this->_timeOut);

    // Returns
    switch (halError) {
    case HAL_ERROR:
        this->_lastError = Error::COMMUNICATION_FAILED;
        return false;
    case HAL_BUSY:
        this->_lastError = Error::BUSY;
        return false;
    case HAL_TIMEOUT:
        this->_lastError = Error::COMMUNICATION_TIMEOUT;
        return false;
    case HAL_OK:
        break;
    }

#   else
    // TODO: Non-UART HAL ARM implementation
#   endif
#elif defined(_FUNSAPE_PLATFORM_AVR)
    // TODO: AVR implementation
#endif

    // Return successfully
    this->_lastError = Error::NONE;
    return true;
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
