/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibWwdg.hpp
 * Module:          WWDG Peripheral Interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_WWDG_HPP
#define __FUNSAPE_LIB_WWDG_HPP                  __BUILD_NUMBER__

// =============================================================================
// Dependencies - Global definitions
// =============================================================================

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibWwdg.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibWwdg.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibWwdg.hpp)!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#       if defined(HAL_WWDG_MODULE_ENABLED)

// =============================================================================
// Dependencies - FunSAPE Library header files
// =============================================================================

#include "src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibWwdg.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibWwdg.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibWwdg.hpp)!
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
// Wwdg - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Wwdg
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
    Wwdg(
            void
    );
    ~Wwdg(
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

}; // class Wwdg

// =============================================================================
// Wwdg - Class overloading operators
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
// Wwdg - Class inline function definitions
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
#           warning WWDG HAL module is not enabled, therefore, WWDG module will not be compiled.
#       endif   // defined(HAL_WWDG_MODULE_ENABLED)
#   else
#       warning HAL driver is not in use, therefore, WWDG module will not be compiled.
#   endif   // defined(USE_HAL_DRIVER)
#else
#   warning Platform unsupported, therefore, WWDG module will not be compiled.
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// Include guard - End
// =============================================================================

#endif // __FUNSAPE_LIB_WWDG_HPP

// =============================================================================
// END OF FILE - funsapeLibWwdg.hpp
// =============================================================================
