/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibI2S.cpp
 * Module:          I2S Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibI2S.hpp"
#ifndef __FUNSAPE_LIB_I2S_HPP
#   error [funsapeLibI2S.cpp] Error - Header file (funsapeLibI2S.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_I2S_HPP != __BUILD_NUMBER__
#   error [funsapeLibI2S.cpp] Error - Build mismatch between source (funsapeLibI2S.cpp) and header (funsapeLibI2S.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_I2S_MODULE_ENABLED)

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
 * @brief Construct a new I2S:: I2S object
 *
 * Detailed description of this element.
 *
 */
I2S::I2S(void)
{
    // Mark passage for debugging purpose
    debugMark("I2S::I2S(void)", Debug::CodeIndex::I2S_MODULE);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the I2S:: I2S object
 *
 * Detailed description of this element.
 *
 */
I2S::~I2S(void)
{
    // Mark passage for debugging purpose
    debugMark("I2S::~I2S(void)", Debug::CodeIndex::I2S_MODULE);

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
Error I2S::getLastError(void)
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
bool_t I2S::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("I2S::_isInitialized(void)", Debug::CodeIndex::I2S_MODULE);

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

#       endif   // defined(HAL_I2S_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibI2S.cpp
// =============================================================================
