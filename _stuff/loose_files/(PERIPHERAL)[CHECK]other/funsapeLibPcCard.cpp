/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibPcCard.cpp
 * Module:          PC CARD Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibPcCard.hpp"
#ifndef __FUNSAPE_LIB_PC_CARD_HPP
#   error [funsapeLibPcCard.cpp] Error - Header file (funsapeLibPcCard.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_PC_CARD_HPP != __BUILD_NUMBER__
#   error [funsapeLibPcCard.cpp] Error - Build mismatch between source (funsapeLibPcCard.cpp) and header (funsapeLibPcCard.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_PCCARD_MODULE_ENABLED)

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
 * @brief Construct a new PcCard:: PcCard object
 *
 * Detailed description of this element.
 *
 */
PcCard::PcCard(void)
{
    // Mark passage for debugging purpose
    debugMark("PcCard::PcCard(void)", DEBUG_CODE_PC_CARD);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the PcCard:: PcCard object
 *
 * Detailed description of this element.
 *
 */
PcCard::~PcCard(void)
{
    // Mark passage for debugging purpose
    debugMark("PcCard::~PcCard(void)", DEBUG_CODE_PC_CARD);

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
Error PcCard::getLastError(void)
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
bool_t PcCard::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("PcCard::_isInitialized(void)", DEBUG_CODE_PC_CARD);

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

#       endif   // defined(HAL_PCCARD_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibPcCard.cpp
// =============================================================================
