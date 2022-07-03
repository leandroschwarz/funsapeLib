/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibSdCard.cpp
 * Module:          SD CARD Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibSdCard.hpp"
#ifndef __FUNSAPE_LIB_SD_CARD_HPP
#   error [funsapeLibSdCard.cpp] Error - Header file (funsapeLibSdCard.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_SD_CARD_HPP != __BUILD_NUMBER__
#   error [funsapeLibSdCard.cpp] Error - Build mismatch between source (funsapeLibSdCard.cpp) and header (funsapeLibSdCard.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_SD_MODULE_ENABLED)

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
 * @brief Construct a new Sd:: Sd object
 *
 * Detailed description of this element.
 *
 */
Sd::Sd(void)
{
    // Mark passage for debugging purpose
    debugMark("Sd::Sd(void)", Debug::CodeIndex::SD_CARD_MODULE);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Sd:: Sd object
 *
 * Detailed description of this element.
 *
 */
Sd::~Sd(void)
{
    // Mark passage for debugging purpose
    debugMark("Sd::~Sd(void)", Debug::CodeIndex::SD_CARD_MODULE);

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
Error Sd::getLastError(void)
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
bool_t Sd::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Sd::_isInitialized(void)", Debug::CodeIndex::SD_CARD_MODULE);

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

#       endif   // defined(HAL_SD_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibSdCard.cpp
// =============================================================================
