/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibIwdg.cpp
 * Module:          IWDG Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibIwdg.hpp"
#ifndef __FUNSAPE_LIB_IWDG_HPP
#   error [funsapeLibIwdg.cpp] Error - Header file (funsapeLibIwdg.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_IWDG_HPP != __BUILD_NUMBER__
#   error [funsapeLibIwdg.cpp] Error - Build mismatch between source (funsapeLibIwdg.cpp) and header (funsapeLibIwdg.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_IWDG_MODULE_ENABLED)

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
 * @brief Construct a new Iwdg:: Iwdg object
 *
 * Detailed description of this element.
 *
 */
Iwdg::Iwdg(void)
{
    // Mark passage for debugging purpose
    debugMark("Iwdg::Iwdg(void)", DEBUG_CODE_IWDG);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Iwdg:: Iwdg object
 *
 * Detailed description of this element.
 *
 */
Iwdg::~Iwdg(void)
{
    // Mark passage for debugging purpose
    debugMark("Iwdg::~Iwdg(void)", DEBUG_CODE_IWDG);

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
Error Iwdg::getLastError(void)
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
bool_t Iwdg::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Iwdg::_isInitialized(void)", DEBUG_CODE_IWDG);

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

#       endif   // defined(HAL_IWDG_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibIwdg.cpp
// =============================================================================
