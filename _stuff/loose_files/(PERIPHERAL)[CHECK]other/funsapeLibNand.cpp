/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibNand.cpp
 * Module:          NAND Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibNand.hpp"
#ifndef __FUNSAPE_LIB_NAND_HPP
#   error [funsapeLibNand.cpp] Error - Header file (funsapeLibNand.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_NAND_HPP != __BUILD_NUMBER__
#   error [funsapeLibNand.cpp] Error - Build mismatch between source (funsapeLibNand.cpp) and header (funsapeLibNand.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_NAND_MODULE_ENABLED)

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
 * @brief Construct a new Nand:: Nand object
 *
 * Detailed description of this element.
 *
 */
Nand::Nand(void)
{
    // Mark passage for debugging purpose
    debugMark("Nand::Nand(void)", DEBUG_CODE_NAND);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Nand:: Nand object
 *
 * Detailed description of this element.
 *
 */
Nand::~Nand(void)
{
    // Mark passage for debugging purpose
    debugMark("Nand::~Nand(void)", DEBUG_CODE_NAND);

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
Error Nand::getLastError(void)
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
bool_t Nand::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Nand::_isInitialized(void)", DEBUG_CODE_NAND);

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

#       endif   // defined(HAL_NAND_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibNand.cpp
// =============================================================================
