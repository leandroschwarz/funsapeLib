/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDebug.hpp
 * Module:          Debug Module for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_DEBUG_HPP
#define __FUNSAPE_LIB_DEBUG_HPP                 __BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error   [funsapeLibDebug.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#   error   [funsapeLibDebug.hpp] Error 10 - Build mismatch between (funsapeLibDebug.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

// NONE

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

#define debugMessage(code)              do{debug.logMessage((uint16_t)(code), (uint8_t *)__FILE__, __LINE__);}while(0)
// BUG => choose one function
// #define debugMark(identifier, level)    do{if(level == getMarkLevel()){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)
// #define debugMark(identifier, level)    do{if(debug.getMarkLevel() & level){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)
#define debugMark(identifier, level)    do{if(debug.getMarkLevel() >= level){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)

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
// Debug - Class declaration
// =============================================================================

class Debug
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
    Debug(
            void
    );
    ~Debug(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Inherited methods ---------------------------------------------

public:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     ///////////////////     INITIALIZATION     ///////////////////     //

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
    // ARM implementation
#   if defined(HAL_UART_MODULE_ENABLED)
    // UART HAL ARM implementation
    bool_t init(
            UART_HandleTypeDef *handler_p,
            cbool_t hideSuccess_p               = true,
            cbool_t stopOnError_p               = true,
            cuint32_t markLevel_p               = 0
    );
#   else
    // Non-UART HAL ARM implementation
    // TODO: Non-UART HAL ARM implementation
    bool_t init(
            uint16_t *handler_p,
            cbool_t hideSuccess_p               = true,
            cbool_t stopOnError_p               = true,
            cuint32_t markLevel_p               = 0
    );
#   endif
#elif defined(_FUNSAPE_PLATFORM_AVR)
    // AVR implementation
    // TODO: AVR implementation
    bool_t init(
            uint16_t *handler_p,
            cbool_t hideSuccess_p               = true,
            cbool_t stopOnError_p               = true,
            cuint32_t markLevel_p               = 0
    );
#else
#   error [funsapeLibDebug.hpp] Error 11 - Platform unsupported!
#endif

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error getLastError(
            void
    );
    uint32_t getMarkLevel(
            void
    );
    bool_t hideSuccess(
            cbool_t hideSuccess_p
    );
    void logMark(
            cuint8_t *fileName_p,
            cuint16_t lineNumber_p,
            cuint8_t *identifier_p
    );
    void logMessage(
            cuint16_t errorCode_p,
            cuint8_t *fileName_p,
            cuint16_t lineNumber_p
    );
    uint8_t setMarkLevel(
            cuint32_t markLevel_p
    );
    bool_t setTimeOut(
            cuint16_t timeOut_p
    );
    bool_t stopOnError(
            cbool_t stopOnError_p
    );
    bool_t printMessage(
            char *message_p
    );

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     /////////////////    PERIPHERAL HANDLER     //////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_UART_MODULE_ENABLED)
    // ARM implementation
    UART_HandleTypeDef *_busHandler;
#elif defined(_FUNSAPE_PLATFORM_AVR)
    // AVR implementation
    // TODO: AVR implementation
    uint16_t *_busHandler;
#else
    uint16_t *_busHandler;
#endif

    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    bool_t      _hideSuccessMessages            : 1;
    bool_t      _initialized                    : 1;
    Error       _lastError;
    uint32_t    _markLevel;
    bool_t      _stopOnFirstError               : 1;
    uint16_t    _timeOut;

protected:
    // NONE

}; // class Debug

// =============================================================================
// Debug - Class overloading operators
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
// Debug - Class inline function definitions
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

extern Debug debug;

#endif // __FUNSAPE_LIB_DEBUG_HPP
