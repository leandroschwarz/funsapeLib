/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibFlash.cpp
 * Module:          FLASH Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibFlash.hpp"
#ifndef __FUNSAPE_LIB_FLASH_HPP
#   error [funsapeLibFlash.cpp] Error - Header file (funsapeLibFlash.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_FLASH_HPP != __BUILD_NUMBER__
#   error [funsapeLibFlash.cpp] Error - Build mismatch between source (funsapeLibFlash.cpp) and header (funsapeLibFlash.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_FLASH_MODULE_ENABLED)

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
 * @brief Construct a new Flash:: Flash object
 *
 * Detailed description of this element.
 *
 */
Flash::Flash(void)
{
    // Mark passage for debugging purpose
    debugMark("Flash::Flash(void)", DEBUG_CODE_FLASH);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Flash:: Flash object
 *
 * Detailed description of this element.
 *
 */
Flash::~Flash(void)
{
    // Mark passage for debugging purpose
    debugMark("Flash::~Flash(void)", DEBUG_CODE_FLASH);

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
Error Flash::getLastError(void)
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
bool_t Flash::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Flash::_isInitialized(void)", DEBUG_CODE_FLASH);

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

#       endif   // defined(HAL_FLASH_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibFlash.cpp
// =============================================================================
