/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibRcc.cpp
 * Module:          RCC Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibRcc.hpp"
#ifndef __FUNSAPE_LIB_RCC_HPP
#   error [funsapeLibRcc.cpp] Error - Header file (funsapeLibRcc.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_RCC_HPP != __BUILD_NUMBER__
#   error [funsapeLibRcc.cpp] Error - Build mismatch between source (funsapeLibRcc.cpp) and header (funsapeLibRcc.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_RCC_MODULE_ENABLED)

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
 * @brief Construct a new Rcc:: Rcc object
 *
 * Detailed description of this element.
 *
 */
Rcc::Rcc(void)
{
    // Mark passage for debugging purpose
    debugMark("Rcc::Rcc(void)", Debug::CodeIndex::RCC_MODULE);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Rcc:: Rcc object
 *
 * Detailed description of this element.
 *
 */
Rcc::~Rcc(void)
{
    // Mark passage for debugging purpose
    debugMark("Rcc::~Rcc(void)", Debug::CodeIndex::RCC_MODULE);

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
Error Rcc::getLastError(void)
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
bool_t Rcc::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Rcc::_isInitialized(void)", Debug::CodeIndex::RCC_MODULE);

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

#       endif   // defined(HAL_RCC_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibRcc.cpp
// =============================================================================
