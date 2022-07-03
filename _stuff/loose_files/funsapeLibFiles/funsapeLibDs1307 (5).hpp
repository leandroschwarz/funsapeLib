/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDs1307.hpp
 * Module:          DS1307 module interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard - Begin
// =============================================================================

#ifndef __FUNSAPE_LIB_DS1307_HPP
#define __FUNSAPE_LIB_DS1307_HPP                __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibDs1307.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibDs1307.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../src/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#   error [funsapeLibDs1307.hpp] Error - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_NUMBER__
#   error [funsapeLibDs1307.hpp] Error - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

#include "../src/funsapeLibDateTime.hpp"
#ifndef __FUNSAPE_LIB_DATETIME_HPP
#   error [funsapeLibDs1307.hpp] Error - Required module (funsapeLibDateTime.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DATETIME_HPP != __BUILD_TO_REPLACE__
#   error [funsapeLibDs1307.hpp] Error - Build mismatch between required module (funsapeLibDateTime.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

#include "../src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibDs1307.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibDs1307.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

// =============================================================================
// Platform verification - Begin
// =============================================================================

#if !defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   warning [funsapeLibDs1307.hpp] Warning - Feature is unsupported or was not implemented for selected platform!
#elif !defined(HAL_I2C_MODULE_ENABLED)
#   warning [funsapeLibDs1307.hpp] Warning - Module was not enabled in platform HAL files!
#else

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
// Ds1307 - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Ds1307
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
    enum class SquareWave {
        OFF_LOW                         = 0,
        OFF_HIGH                        = 1,
        CLOCK_1_HZ                      = 2,
        CLOCK_4_KHZ                     = 3,
        CLOCK_8_KHZ                     = 4,
        CLOCK_32_KHZ                    = 5
    };

private:
    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class Register {
        SECONDS                         = 0x00,
        MINUTES                         = 0x01,
        HOURS                           = 0x02,
        DAY                             = 0x03,
        DATE                            = 0x04,
        MONTH                           = 0x05,
        YEAR                            = 0x06,
        CONTROL                         = 0x07,
        RAM_START                       = 0x08,
    };

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
    Ds1307(
            void
    );
    ~Ds1307(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Inherited methods ---------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool_t countingResume(
            void
    );
    bool_t countingStop(
            void
    );
    Error getLastError(
            void
    );
    bool_t init(
            Bus *busHandler_p
    );
    bool_t setSquareWaveGenerator(
            SquareWave squareWave_p
    );
//     bool_t setControl(funsapeLibDs1307CountingStatus_t counting, funsapeLibDs1307SquareWaveGenerator_t squareWave, funsapeLibDs1307TimeFormat_t timeFormat);

    //     ///////////////    DATE HANDLING FUNCTIONS     ///////////////     //
    bool_t getDate(
            uint16_t *year_p,
            uint8_t *month_p,
            uint8_t *monthDay_p,
            uint8_t *weekDay_p = nullptr
    );
    bool_t getDateTime(
            DateTime *dateTime_p
    );
    bool_t setDate(
            cuint16_t year_p,
            cuint8_t month_p,
            cuint8_t monthDay_p
    );
    bool_t setDateTime(
            const DateTime dateTime_p
    );

    //     ///////////////    TIME HANDLING FUNCTIONS     ///////////////     //
    bool_t getTime(
            uint8_t *hours_p,
            uint8_t *minutes_p,
            uint8_t *seconds_p,
            const DateTime::TimeFormat timeFormat_p = DateTime::TimeFormat::FORMAT_24_HOURS,
            DateTime::AmPmFlag *amPmFlag_p = nullptr
    );
    bool_t setTime(
            cuint8_t hours_p,
            cuint8_t minutes_p,
            cuint8_t seconds_p,
            const DateTime::TimeFormat timeFormat_p = DateTime::TimeFormat::FORMAT_24_HOURS,
            const DateTime::AmPmFlag amPmFlag_p = DateTime::AmPmFlag::AM
    );

    //     /////////////    RAM DATA HANDLING FUNCTIONS     /////////////     //
    bool_t getRamData(
            cuint8_t position_p,
            uint8_t *buffer_p,
            cuint8_t size_p = 1
    );
    bool_t setRamData(
            cuint8_t position_p,
            cuint8_t *buffer_p,
            cuint8_t size_p = 1
    );

private:
    void _clearData(
            void
    );
    bool_t _getData(
            void
    );
    bool_t _isInitialized(
            void
    );
    bool_t _sendData(
            void
    );
    bool_t _setCounting(
            bool_t counting_p
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     ////////////////    PERIPHERAL BUS HANDLER     ////////////////     //
    Bus         *_busHandler;

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool        _countingHalted         : 1;
    bool        _initialized            : 1;
    Error       _lastError;
    SquareWave  _squareWave;

    //     /////////////    DEVICE DATA (DATE AND TIME)     /////////////     //
    DateTime    _dateTime;

protected:
    // NONE

}; // class Ds1307

// =============================================================================
// Ds1307 - Class overloading operators
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
// Ds1307 - Class inline function definitions
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

#endif

// =============================================================================
// Include guard - End
// =============================================================================

#endif // __FUNSAPE_LIB_DS1307_HPP

// =============================================================================
// END OF FILE
// =============================================================================
