/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibHcd.cpp
 * Module:          HCD Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibHcd.hpp"
#ifndef __FUNSAPE_LIB_HCD_HPP
#   error [funsapeLibHcd.cpp] Error - Header file (funsapeLibHcd.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HCD_HPP != __BUILD_NUMBER__
#   error [funsapeLibHcd.cpp] Error - Build mismatch between source (funsapeLibHcd.cpp) and header (funsapeLibHcd.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_HCD_MODULE_ENABLED)

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
 * @brief Construct a new Hcd:: Hcd object
 *
 * Detailed description of this element.
 *
 */
Hcd::Hcd(void)
{
    // Mark passage for debugging purpose
    debugMark("Hcd::Hcd(void)", DEBUG_CODE_HCD);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Hcd:: Hcd object
 *
 * Detailed description of this element.
 *
 */
Hcd::~Hcd(void)
{
    // Mark passage for debugging purpose
    debugMark("Hcd::~Hcd(void)", DEBUG_CODE_HCD);

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
Error Hcd::getLastError(void)
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
bool_t Hcd::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Hcd::_isInitialized(void)", DEBUG_CODE_HCD);

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

#       endif   // defined(HAL_HCD_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibHcd.cpp
// =============================================================================
