/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibIrda.cpp
 * Module:          IRDA Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibIrda.hpp"
#ifndef __FUNSAPE_LIB_IRDA_HPP
#   error [funsapeLibIrda.cpp] Error - Header file (funsapeLibIrda.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_IRDA_HPP != __BUILD_NUMBER__
#   error [funsapeLibIrda.cpp] Error - Build mismatch between source (funsapeLibIrda.cpp) and header (funsapeLibIrda.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_IRDA_MODULE_ENABLED)

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
 * @brief Construct a new Irda:: Irda object
 *
 * Detailed description of this element.
 *
 */
Irda::Irda(void)
{
    // Mark passage for debugging purpose
    debugMark("Irda::Irda(void)", DEBUG_CODE_IRDA);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Irda:: Irda object
 *
 * Detailed description of this element.
 *
 */
Irda::~Irda(void)
{
    // Mark passage for debugging purpose
    debugMark("Irda::~Irda(void)", DEBUG_CODE_IRDA);

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
Error Irda::getLastError(void)
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
bool_t Irda::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Irda::_isInitialized(void)", DEBUG_CODE_IRDA);

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

#       endif   // defined(HAL_IRDA_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibIrda.cpp
// =============================================================================
