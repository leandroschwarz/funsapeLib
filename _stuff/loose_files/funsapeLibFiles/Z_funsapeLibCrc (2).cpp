/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibCrc.cpp
 * Module:          CRC Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibCrc.hpp"
#ifndef __FUNSAPE_LIB_CRC_HPP
#   error [funsapeLibCrc.cpp] Error - Header file (funsapeLibCrc.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_CRC_HPP != __BUILD_NUMBER__
#   error [funsapeLibCrc.cpp] Error - Build mismatch between source (funsapeLibCrc.cpp) and header (funsapeLibCrc.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_CRC_MODULE_ENABLED)

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
 * @brief Construct a new Crc:: Crc object
 *
 * Detailed description of this element.
 *
 */
Crc::Crc(void)
{
    // Mark passage for debugging purpose
    debugMark("Crc::Crc(void)", DEBUG_CODE_CRC);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Crc:: Crc object
 *
 * Detailed description of this element.
 *
 */
Crc::~Crc(void)
{
    // Mark passage for debugging purpose
    debugMark("Crc::~Crc(void)", DEBUG_CODE_CRC);

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
Error Crc::getLastError(void)
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
bool_t Crc::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Crc::_isInitialized(void)", DEBUG_CODE_CRC);

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

#       endif   // defined(HAL_CRC_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibCrc.cpp
// =============================================================================
