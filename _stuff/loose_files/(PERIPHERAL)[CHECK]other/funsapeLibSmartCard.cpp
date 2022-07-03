/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibSmartCard.cpp
 * Module:          SMART_CARD Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibSmartCard.hpp"
#ifndef __FUNSAPE_LIB_SMART_CARD_HPP
#   error [funsapeLibSmartCard.cpp] Error - Header file (funsapeLibSmartCard.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_SMART_CARD_HPP != __BUILD_NUMBER__
#   error [funsapeLibSmartCard.cpp] Error - Build mismatch between source (funsapeLibSmartCard.cpp) and header (funsapeLibSmartCard.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_SMARTCARD_MODULE_ENABLED)

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
 * @brief Construct a new SmartCard:: SmartCard object
 *
 * Detailed description of this element.
 *
 */
SmartCard::SmartCard(void)
{
    // Mark passage for debugging purpose
    debugMark("SmartCard::SmartCard(void)", DEBUG_CODE_SMART_CARD);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the SmartCard:: SmartCard object
 *
 * Detailed description of this element.
 *
 */
SmartCard::~SmartCard(void)
{
    // Mark passage for debugging purpose
    debugMark("SmartCard::~SmartCard(void)", DEBUG_CODE_SMART_CARD);

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
Error SmartCard::getLastError(void)
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
bool_t SmartCard::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("SmartCard::_isInitialized(void)", DEBUG_CODE_SMART_CARD);

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

#       endif   // defined(HAL_SMARTCARD_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibSmartCard.cpp
// =============================================================================
