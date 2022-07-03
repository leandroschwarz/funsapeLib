/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibUsart.cpp
 * Module:          USART Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibUsart.hpp"
#ifndef __FUNSAPE_LIB_USART_HPP
#   error [funsapeLibUsart.cpp] Error - Header file (funsapeLibUsart.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_USART_HPP != __BUILD_NUMBER__
#   error [funsapeLibUsart.cpp] Error - Build mismatch between source (funsapeLibUsart.cpp) and header (funsapeLibUsart.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_USART_MODULE_ENABLED)

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

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

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Usart:: Usart object
 *
 * Detailed description of this element.
 *
 */
Usart::Usart(void)
{
    // Mark passage for debugging purpose
    debugMark("Usart::Usart(void)", DEBUG_CODE_USART);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Usart:: Usart object
 *
 * Detailed description of this element.
 *
 */
Usart::~Usart(void)
{
    // Mark passage for debugging purpose
    debugMark("Usart::~Usart(void)", DEBUG_CODE_USART);

    // Returns successfully
    debugMessage(Error::NONE);
    return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return Error
 */
Error Usart::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t Usart::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Usart::_isInitialized(void)", DEBUG_CODE_USART);

    // Checks initialization
    if (!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Platform verification - End
// =============================================================================

#       endif   // defined(HAL_USART_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibUsart.cpp
// =============================================================================
