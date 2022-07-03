/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibTimer.cpp
 * Module:          TIMER Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibTimer.hpp"
#ifndef __FUNSAPE_LIB_TIMER_HPP
#   error [funsapeLibTimer.cpp] Error - Header file (funsapeLibTimer.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_TIMER_HPP != __BUILD_NUMBER__
#   error [funsapeLibTimer.cpp] Error - Build mismatch between source (funsapeLibTimer.cpp) and header (funsapeLibTimer.hpp) files!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_TIM_MODULE_ENABLED)

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
 * @brief Construct a new Timer:: Timer object
 *
 * Detailed description of this element.
 *
 */
Timer::Timer(void)
{
    // Mark passage for debugging purpose
    debugMark("Timer::Timer(void)", DEBUG_CODE_TIMER);

    // Resets data members

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Timer:: Timer object
 *
 * Detailed description of this element.
 *
 */
Timer::~Timer(void)
{
    // Mark passage for debugging purpose
    debugMark("Timer::~Timer(void)", DEBUG_CODE_TIMER);

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
Error Timer::getLastError(void)
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
bool_t Timer::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Timer::_isInitialized(void)", DEBUG_CODE_TIMER);

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

#       endif   // defined(HAL_TIM_MODULE_ENABLED)
#   endif   // defined(USE_HAL_DRIVER)
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// END OF FILE - funsapeLibTimer.cpp
// =============================================================================
