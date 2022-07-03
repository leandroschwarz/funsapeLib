/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibAdc.cpp
 * Module:          ADC Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibAdc.hpp"
#ifndef __FUNSAPE_LIB_ADC_HPP
#   error [funsapeLibAdc.cpp] Error - Header file (funsapeLibAdc.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_ADC_HPP != __BUILD_NUMBER__
#   error [funsapeLibAdc.cpp] Error - Build mismatch between source (funsapeLibAdc.cpp) and header (funsapeLibAdc.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_ADC_MODULE_ENABLED)

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
 * @brief Construct a new Adc:: Adc object
 *
 * Detailed description of this element.
 *
 */
Adc::Adc(void)
{
    // Mark passage for debugging purpose
    debugMark("Adc::Adc(void)", DEBUG_CODE_ADC);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Adc:: Adc object
 *
 * Detailed description of this element.
 *
 */
Adc::~Adc(void)
{
    // Mark passage for debugging purpose
    debugMark("Adc::~Adc(void)", DEBUG_CODE_ADC);

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
Error Adc::getLastError(void)
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
bool_t Adc::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Adc::_isInitialized(void)", DEBUG_CODE_ADC);

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

#       endif   // defined(HAL_ADC_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibAdc.cpp
// =============================================================================
