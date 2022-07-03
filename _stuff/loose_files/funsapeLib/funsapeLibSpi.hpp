/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibSpi.hpp
 * Module:          SPI Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_SPI_HPP
#define __FUNSAPE_LIB_SPI_HPP                   __BUILD_NUMBER__

// =============================================================================
// Dependencies - Global definitions
// =============================================================================

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibSpi.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibSpi.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibSpi.hpp)!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#      if defined(HAL_SPI_MODULE_ENABLED)
#           if defined(HAL_GPIO_MODULE_ENABLED)

// =============================================================================
// Dependencies - FunSAPE Library header files
// =============================================================================

#include "src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibSpi.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibSpi.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibSpi.hpp)!
#endif

#include "src/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#   error [funsapeLibSpi.hpp] Error - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_NUMBER__
#   error [funsapeLibSpi.hpp] Error - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibSpi.hpp)!
#endif

#include "funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#   error [funsapeLibSpi.hpp] Error - Required module (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_NUMBER__
#   error [funsapeLibSpi.hpp] Error - Build mismatch between required module (funsapeLibGpio.hpp) and current header file (funsapeLibSpi.hpp)!
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
// Spi - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Spi : public Bus
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class SpiMode {
        MODE_0                          = 0,
        MODE_1                          = 1,
        MODE_2                          = 2,
        MODE_3                          = 3,
    };

private:
    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class SpiCsManagement {
        NSS_SOFT                        = SPI_NSS_SOFT,
        NSS_HARD_INPUT                  = SPI_NSS_HARD_INPUT,
        NSS_HARD_OUTPUT                 = SPI_NSS_HARD_OUTPUT,
    };

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class SpiDirection {
        DIRECTION_2_LINES               = SPI_DIRECTION_2LINES,
        DIRECTION_2_LINES_RXONLY        = SPI_DIRECTION_2LINES_RXONLY,
        DIRECTION_1_LINE                = SPI_DIRECTION_1LINE,
    };

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class SpiOperation {
        READ                            = 0,
        WRITE                           = 1,
        SEND                            = 2,
    };

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class SpiPrescaler {
        PRESCALER_2                     = SPI_BAUDRATEPRESCALER_2,
        PRESCALER_4                     = SPI_BAUDRATEPRESCALER_4,
        PRESCALER_8                     = SPI_BAUDRATEPRESCALER_8,
        PRESCALER_16                    = SPI_BAUDRATEPRESCALER_16,
        PRESCALER_32                    = SPI_BAUDRATEPRESCALER_32,
        PRESCALER_64                    = SPI_BAUDRATEPRESCALER_64,
        PRESCALER_128                   = SPI_BAUDRATEPRESCALER_128,
        PRESCALER_256                   = SPI_BAUDRATEPRESCALER_256,
    };

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operator overloading ----------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    Spi(
            void
    );
    ~Spi(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Inherited methods ---------------------------------------------

public:
    //     ////////////////////    DATA TRANSFER     ////////////////////     //
    bool_t readReg(
            cuint8_t reg_p,
            uint8_t *buffData_p,
            cuint16_t buffSize_p = 1
    );
    bool_t sendData(
            uint8_t *buffData_p,
            cuint16_t buffSize_p = 1
    );
    bool_t sendData(
            cuint8_t *txBuffData_p,
            uint8_t *rxBuffData_p,
            cuint16_t buffSize_p = 1
    );
    bool_t writeReg(
            cuint8_t reg_p,
            cuint8_t *buffData_p,
            cuint16_t buffSize_p = 1
    );

    //     //////////////////    PROTOCOL SPECIFIC     //////////////////     //
    bool_t setDevice(
            void (* actFunc_p)(void),
            void (* deactFunc_p)(void)
    );
    bool_t setDevice(
            Gpio *csPin_p
    );

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Bus::BusType getBusType(
            void
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool_t changeMode(
            const SpiMode mode_p
    );
    Error getLastError(
            void
    );
    bool_t init(
            SPI_HandleTypeDef *busHandler_p,
            cuint16_t bufSize_p = 20
    );
    bool_t init(
            SPI_HandleTypeDef *busHandler_p,
            cuint16_t rxBufSize_p,
            cuint16_t txBufSize_p
    );
    bool_t setTimeout(
            cuint16_t timeout_p
    );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    void _clearData(
            void
    );
    void _deinitialize(
            void
    );
    bool_t _initialize(
            void
    );
    bool_t _isInitialized(
            void
    );
    bool_t _transmit(
            void
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     /////////////////    PERIPHERAL HANDLER     //////////////////     //
    SPI_HandleTypeDef   *_busHandler;

    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    bool_t              _busy                   : 1;
    Gpio                *_csPin;
    void (*_activateDefaultDevice)(void);
    void (*_deactivateDefaultDevice)(void);
    bool_t              _devSelFunctionSet      : 1;
    bool_t              _devSelGpioPinSet       : 1;
    bool_t              _initialized            : 1;
    Error               _lastError;
    SpiOperation        _operation;
    uint16_t            _timeout;

    //     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
    SpiPrescaler        _baudRatePrescaler;
    bool_t              _crcCalculationEnabled  : 1;
    uint32_t            _crcPolynomial;
    SpiCsManagement     _csManagement;
    bool_t              _dataSize16BitsEnabled  : 1;
    SpiDirection        _direction;
    bool_t              _lsbFirstEnabled        : 1;
    bool_t              _masterModeEnabled      : 1;
    SpiMode             _mode;

    //     ////////////////////    DATA BUFFERS      ////////////////////     //
    uint8_t             *_rxBufferData;
    uint16_t            _rxBufferIndex;
    uint16_t            _rxBufferLength;
    uint16_t            _rxBufferMaxSize;
    uint8_t             *_txBufferData;
    uint16_t            _txBufferIndex;
    uint16_t            _txBufferLength;
    uint16_t            _txBufferMaxSize;

protected:
    // NONE

}; // class Spi

// =============================================================================
// Spi - Class overloading operators
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
// Spi - Class inline function definitions
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

#           else
#              pragma message "GPIO HAL module is not enabled, therefore, SPI module will not be compiled."
#           endif   // defined(HAL_GPIO_MODULE_ENABLED)
#       else
#           pragma message "SPI HAL module is not enabled, therefore, SPI module will not be compiled."
#       endif   // defined(HAL_SPI_MODULE_ENABLED)
#   else
#       pragma message "HAL driver is not in use, therefore, SPI module will not be compiled."
#   endif   // defined(USE_HAL_DRIVER)
#else
#   pragma message "Platform unsupported, therefore, SPI module will not be compiled."
#endif  // defined(_FUNSAPE_PLATFORM_ARM_STM32)

// =============================================================================
// Include guard - End
// =============================================================================

#endif // __FUNSAPE_LIB_SPI_HPP

// =============================================================================
// END OF FILE - funsapeLibSpi.hpp
// =============================================================================
