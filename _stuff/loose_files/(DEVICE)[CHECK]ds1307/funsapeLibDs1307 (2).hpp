/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibDs1307.hpp
 * Module:			DS1307 module interface for FunSAPE++ Embedded Library
 * 						project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_DS1307_HPP
#define __FUNSAPE_LIB_DS1307_HPP				__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibDs1307.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDs1307.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#	error	[funsapeLibDs1307.hpp] Error 14 - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDs1307.hpp] Error 15 - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

#include "../util/funsapeLibDateTime.hpp"
#ifndef __FUNSAPE_LIB_DATE_TIME_HPP
#	error	[funsapeLibDs1307.hpp] Error 14 - Required module (funsapeLibDateTime.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DATE_TIME_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDs1307.hpp] Error 15 - Build mismatch between required module (funsapeLibDateTime.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibDs1307.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibDs1307.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibDs1307.hpp)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

// NONE

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// #define DS1307_SLAVE_ADDRESS		0x68
// #define DS1307_ADDRESS_SECONDS		0x00
// #define DS1307_ADDRESS_COUNTING		0x00
// #define DS1307_ADDRESS_MINUTES		0x01
// #define DS1307_ADDRESS_HOURS		0x02
// #define DS1307_ADDRESS_TIME_FORMAT	0x02
// #define DS1307_ADDRESS_WEEK_DAY		0x03
// #define DS1307_ADDRESS_MONTH_DAY	0x04
// #define DS1307_ADDRESS_MONTH		0x05
// #define DS1307_ADDRESS_YEAR			0x06
// #define DS1307_ADDRESS_CONTROL		0x07
// #define DS1307_ADDRESS_RAM			0x08
// #define DS1307_I2C_MIN_CLOCK_SPEED	0UL
// #define DS1307_I2C_MAX_CLOCK_SPEED	100000UL

// #ifndef TWI_MASTER_READ
// #	define TWI_MASTER_READ				1
// #endif
// #ifndef TWI_MASTER_WRITE
// #	define TWI_MASTER_WRITE			0
// #endif

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

class Ds1307
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    enum class SquareWave {
        OFF_LOW							= 0,
        OFF_HIGH						= 1,
        CLOCK_1_HZ						= 2,
        CLOCK_4_KHZ						= 3,
        CLOCK_8_KHZ						= 4,
        CLOCK_32_KHZ					= 5
    };

private:
    enum class Register {
        SECONDS							= 0x00,
        MINUTES							= 0x01,
        HOURS							= 0x02,
        DAY								= 0x03,
        DATE							= 0x04,
        MONTH							= 0x05,
        YEAR							= 0x06,
        CONTROL							= 0x07,
        RAM_START						= 0x08,
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
    );
    // TODO: Implement destructor function
    ~Ds1307(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool	countingResume(
            void
    );
    bool	countingStop(
            void
    );
    Error	getLastError(
            void
    );
    bool	init(
            Bus *busHandler_p
    );
    bool	setSquareWaveGenerator(
            SquareWave squareWave_p
    );

//	bool	setControl(funsapeLibDs1307CountingStatus_t counting, funsapeLibDs1307SquareWaveGenerator_t squareWave, funsapeLibDs1307TimeFormat_t timeFormat);

    //     ///////////////    DATE HANDLING FUNCTIONS     ///////////////     //
    bool	getDate(
            uint16_t *year_p,
            uint8_t *month_p,
            uint8_t *monthDay_p,
            uint8_t *weekDay_p = nullptr
    );
    bool	getDateTime(
            DateTime *dateTime_p
    );
    bool	setDate(
            uint16_t year_p,
            uint8_t month_p,
            uint8_t monthDay_p
    );
    bool	setDateTime(
            DateTime dateTime_p
    );

    //     ///////////////    TIME HANDLING FUNCTIONS     ///////////////     //
    bool	getTime(
            uint8_t *hours_p,
            uint8_t *minutes_p,
            uint8_t *seconds_p,
            DateTime::TimeFormat timeFormat_p = DateTime::TimeFormat::FORMAT_24_HOURS,
            DateTime::AmPmFlag *amPmFlag_p = nullptr
    );
    bool	setTime(
            uint8_t hours_p,
            uint8_t minutes_p,
            uint8_t seconds_p,
            DateTime::TimeFormat timeFormat_p = DateTime::TimeFormat::FORMAT_24_HOURS,
            DateTime::AmPmFlag amPmFlag_p = DateTime::AmPmFlag::AM
    );

    //     /////////////    RAM DATA HANDLING FUNCTIONS     /////////////     //
    bool	getRamData(
            uint8_t position_p,
            uint8_t *buffer_p,
            uint8_t size_p = 1
    );
    bool	setRamData(
            uint8_t position_p,
            uint8_t *buffer_p,
            uint8_t size_p = 1
    );

private:
    void _clearData(
            void
    );
    bool _getData(
            void
    );
    bool _isInitialized(
            void
    );
    bool _sendData(
            void
    );
    bool _setCounting(
            bool counting_p
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     ////////////////    PERIPHERAL BUS HANDLER     ////////////////     //
    Bus			*_busHandler;

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool		_countingHalted			: 1;
    bool		_initialized			: 1;
    Error		_lastError;
    SquareWave	_squareWave;

    //     /////////////    DEVICE DATA (DATE AND TIME)     /////////////     //
    DateTime	_dateTime;

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

#endif // __FUNSAPE_LIB_DS1307_HPP

// =============================================================================
// END OF FILE
// =============================================================================
