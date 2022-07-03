/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibGpio.hpp
 * Module:          GPIO Peripheral Interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_GPIO_HPP
#define __FUNSAPE_LIB_GPIO_HPP                  __BUILD_NUMBER__

// =============================================================================
// Dependencies - Global definitions
// =============================================================================

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibGpio.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibGpio.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibGpio.hpp)!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#       if defined(HAL_GPIO_MODULE_ENABLED)

// =============================================================================
// Dependencies - FunSAPE Library header files
// =============================================================================

#include "src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibGpio.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibGpio.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibGpio.hpp)!
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
// Gpio - Class declaration
// =============================================================================

// @CLASS Gpio
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Gpio
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    // @ENUM Gpio::Mode
    enum class Mode : uint8_t {
        INPUT_ANALOG                    = 0,
        INPUT_FLOATING                  = 1,
        INPUT_PULLED_DOWN               = 2,
        INPUT_PULLED_UP                 = 3,
        OUTPUT_OPEN_DRAIN               = 4,
        OUTPUT_PUSH_PULL                = 5,
    };
    // @ENUM Gpio::Speed
    enum class Speed : uint8_t {
        INPUT                           = 0,
        FREQ_LOW                        = 1,
        FREQ_MEDIUM                     = 2,
        FREQ_HIGH                       = 3,
    };

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
    Gpio(                               // @FUNC Gpio::Gpio(void)
            void
    );
    Gpio(                               // @FUNC Gpio::Gpio(GPIO_TypeDef *, cuint16_t)
            GPIO_TypeDef *gpioPort_p,
            cuint16_t gpioPin_p
    );
    ~Gpio(                              // @FUNC Gpio::~Gpio(void)
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error getLastError(                 // @FUNC Gpio::getLastError(void)
            void
    );
    bool_t init(                        // @FUNC Gpio::init(GPIO_TypeDef *, cuint16_t)
            GPIO_TypeDef *gpioPort_p,
            cuint16_t gpioPin_p
    );

    //     /////////////////    PIN RELATED METHODS     /////////////////     //
    bool_t clr(                         // @FUNC Gpio::clr(void)
            void
    );
    bool_t cpl(                         // @FUNC Gpio::cpl(void)
            void
    );
    LogicLevel read(                    // @FUNC Gpio::read(void)
            void
    );
    bool_t set(                         // @FUNC Gpio::set(void)
            void
    );
    bool_t setMode(                     // @FUNC Gpio::setMode(const Gpio::Mode)
            const Gpio::Mode mode_p
    );
    bool_t setOutputSpeed(              // @FUNC Gpio::setOutputSpeed(const Gpio::Speed)
            const Gpio::Speed speed_p
    );

    inlined bool_t low(void) {          // @FUNC Gpio::low(void)
        return this->clr();
    }
    inlined bool_t high(void) {         // @FUNC Gpio::high(void)
        return this->set();
    }
    inlined bool_t toggle(void) {       // @FUNC Gpio::toggle(void)
        return this->cpl();
    }

    void showData(void) {               // @FUNC Gpio::showData(void)
        char auxBuffer[100];
        sprintf(auxBuffer, "0x%08lx 0x%08lx 0x%08lx 0x%08lx\r",
                this->_port->CRH,
                this->_port->CRL,
                this->_port->IDR,
                this->_port->ODR
        );
        debug.printMessage(auxBuffer);
    }

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    void _clearData(                    // @FUNC Gpio::_clearData(void)
            void
    );
    void _deinitialize(                 // @FUNC Gpio::_deinitialize(void)
            void
    );
    bool_t _initialize(                 // @FUNC Gpio::_initialize(GPIO_TypeDef *, cuint16_t)
            GPIO_TypeDef *gpioPort_p,
            cuint16_t gpioPin_p
    );
    bool_t _isInitialized(              // @FUNC Gpio::_isInitialized(void)
            void
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    uint16_t        _pinMask;
    GPIO_TypeDef    *_port;

private:
    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    bool_t          _initialized                : 1;
    Error           _lastError;

    //     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
    uint8_t         _pinIndex;
    Mode            _mode;
    Speed           _outMaxSpeed;
    LogicLevel      _outLogicLevel;

protected:
// NONE

}; // class Gpio

// =============================================================================
// Gpio - Class overloading operators
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
// Gpio - Class inline function definitions
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
#           warning GPIO HAL module is not enabled, therefore, GPIO module will not be compiled.
#       endif   // defined(HAL_GPIO_MODULE_ENABLED)
#   else
#       warning HAL driver is not in use, therefore, GPIO module will not be compiled.
#   endif   // defined(USE_HAL_DRIVER)
#else
#   warning Platform unsupported, therefore, GPIO module will not be compiled.
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// Include guard - End
// =============================================================================

#endif // __FUNSAPE_LIB_GPIO_HPP

// =============================================================================
// END OF FILE - funsapeLibGpio.hpp
// =============================================================================
