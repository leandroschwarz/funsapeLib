/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDac.cpp
 * Module:          DAC Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibDac.hpp"
#ifndef __FUNSAPE_LIB_DAC_HPP
#   error [funsapeLibDac.cpp] Error - Header file (funsapeLibDac.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DAC_HPP != __BUILD_NUMBER__
#   error [funsapeLibDac.cpp] Error - Build mismatch between source (funsapeLibDac.cpp) and header (funsapeLibDac.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_DAC_MODULE_ENABLED)

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
 * @brief Construct a new Dac:: Dac object
 *
 * Detailed description of this element.
 *
 */
Dac::Dac(void)
{
    // Mark passage for debugging purpose
    debugMark("Dac::Dac(void)", DEBUG_CODE_DAC);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Dac:: Dac object
 *
 * Detailed description of this element.
 *
 */
Dac::~Dac(void)
{
    // Mark passage for debugging purpose
    debugMark("Dac::~Dac(void)", DEBUG_CODE_DAC);

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
Error Dac::getLastError(void)
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
bool_t Dac::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Dac::_isInitialized(void)", DEBUG_CODE_DAC);

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

#       endif   // defined(HAL_DAC_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibDac.cpp
// =============================================================================
