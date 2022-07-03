/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibWwdg.cpp
 * Module:          WWDG Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibWwdg.hpp"
#ifndef __FUNSAPE_LIB_WWDG_HPP
#   error [funsapeLibWwdg.cpp] Error - Header file (funsapeLibWwdg.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_WWDG_HPP != __BUILD_NUMBER__
#   error [funsapeLibWwdg.cpp] Error - Build mismatch between source (funsapeLibWwdg.cpp) and header (funsapeLibWwdg.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_WWDG_MODULE_ENABLED)

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
 * @brief Construct a new Wwdg:: Wwdg object
 *
 * Detailed description of this element.
 *
 */
Wwdg::Wwdg(void)
{
    // Mark passage for debugging purpose
    debugMark("Wwdg::Wwdg(void)", DEBUG_CODE_WWDG);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Wwdg:: Wwdg object
 *
 * Detailed description of this element.
 *
 */
Wwdg::~Wwdg(void)
{
    // Mark passage for debugging purpose
    debugMark("Wwdg::~Wwdg(void)", DEBUG_CODE_WWDG);

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
Error Wwdg::getLastError(void)
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
bool_t Wwdg::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Wwdg::_isInitialized(void)", DEBUG_CODE_WWDG);

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

#       endif   // defined(HAL_WWDG_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibWwdg.cpp
// =============================================================================
