/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibEth.cpp
 * Module:          ETH Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibEth.hpp"
#ifndef __FUNSAPE_LIB_ETH_HPP
#   error [funsapeLibEth.cpp] Error - Header file (funsapeLibEth.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_ETH_HPP != __BUILD_NUMBER__
#   error [funsapeLibEth.cpp] Error - Build mismatch between source (funsapeLibEth.cpp) and header (funsapeLibEth.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_ETH_MODULE_ENABLED)

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
 * @brief Construct a new Eth:: Eth object
 *
 * Detailed description of this element.
 *
 */
Eth::Eth(void)
{
    // Mark passage for debugging purpose
    debugMark("Eth::Eth(void)", DEBUG_CODE_ETH);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Eth:: Eth object
 *
 * Detailed description of this element.
 *
 */
Eth::~Eth(void)
{
    // Mark passage for debugging purpose
    debugMark("Eth::~Eth(void)", DEBUG_CODE_ETH);

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
Error Eth::getLastError(void)
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
bool_t Eth::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Eth::_isInitialized(void)", DEBUG_CODE_ETH);

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

#       endif   // defined(HAL_ETH_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibEth.cpp
// =============================================================================
