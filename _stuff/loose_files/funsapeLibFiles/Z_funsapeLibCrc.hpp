/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibCrc.hpp
 * Module:          CRC Peripheral Interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_CRC_HPP
#define __FUNSAPE_LIB_CRC_HPP                   __BUILD_NUMBER__

// =============================================================================
// Dependencies - Global definitions
// =============================================================================

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibCrc.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibCrc.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibCrc.hpp)!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#       if defined(HAL_CRC_MODULE_ENABLED)

// =============================================================================
// Dependencies - FunSAPE Library header files
// =============================================================================

#include "src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibCrc.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibCrc.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibCrc.hpp)!
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// NONE

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Crc - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Crc
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operators overloading ---------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    Crc(
            void
    );
    ~Crc(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error getLastError(
            void
    );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool_t _isInitialized(
            void
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    bool_t          _initialized                : 1;
    Error           _lastError;

protected:
// NONE

}; // class Crc

// =============================================================================
// Crc - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// Crc - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

// =============================================================================
// Platform verification - End
// =============================================================================

#       else
#           warning CRC HAL module is not enabled, therefore, CRC module will not be compiled.
#       endif   // defined(HAL_CRC_MODULE_ENABLED)
#   else
#       warning HAL driver is not in use, therefore, CRC module will not be compiled.
#   endif   // defined(USE_HAL_DRIVER)
#else
#   warning Platform unsupported, therefore, CRC module will not be compiled.
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// Include guard - End
// =============================================================================

#endif // __FUNSAPE_LIB_CRC_HPP

// =============================================================================
// END OF FILE - funsapeLibCrc.hpp
// =============================================================================
