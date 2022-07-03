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
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibGpio.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibGpio.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibGpio.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibGpio.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibGpio.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibGpio.hpp)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

#if !defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   error [funsapeLibGpio.hpp] Error - Feature is unsupported or was not implemented for selected platform!
#elif !defined(HAL_GPIO_MODULE_ENABLED)
#   error [funsapeLibGpio.hpp] Error - Module was not enabled in platform HAL files!
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
    enum class Mode {
        INPUT_ANALOG					= 0,
        INPUT_FLOATING					= 1,
        INPUT_PULLED_DOWN				= 2,
        INPUT_PULLED_UP					= 3,
        OUTPUT_OPEN_DRAIN				= 4,
        OUTPUT_PUSH_PULL				= 5,
    };

    enum class Speed {
        INPUT							= 0,
        FREQ_LOW						= 1,
        FREQ_MEDIUM						= 2,
        FREQ_HIGH						= 3,
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
    Gpio(
            void
    );
    Gpio(
            GPIO_TypeDef *gpioPort_p,
            cuint16_t gpioPin_p
    );
    ~Gpio(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error getLastError(
            void
    );
    bool_t init(
            GPIO_TypeDef *gpioPort_p,
            cuint16_t gpioPin_p
    );

    //     /////////////////    PIN RELATED METHODS     /////////////////     //
    bool_t clr(
            void
    );
    bool_t cpl(
            void
    );
    LogicLevel read(
            void
    );
    bool_t set(
            void
    );
    bool_t setMode(
            Mode mode_p
    );
    bool_t setOutputSpeed(
            Speed speed_p
    );

    inlined bool_t low() {
        return this->clr();
    }
    inlined bool_t high() {
        return this->set();
    }
    inlined bool_t toggle() {
        return this->cpl();
    }

    void showData() {
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
    void _clearData(
            void
    );
    void _deinitialize(
            void
    );
    bool_t _initialize(
            GPIO_TypeDef *gpioPort_p,
            cuint16_t gpioPin_p
    );
    bool_t _isInitialized(
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

#endif // __FUNSAPE_LIB_GPIO_HPP

// =============================================================================
// END OF FILE
// =============================================================================

