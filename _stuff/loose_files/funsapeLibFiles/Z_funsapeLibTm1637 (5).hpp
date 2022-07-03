/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibTm1637.hpp
 * Module:          TM1637 Display module interface for FunSAPE++ Embedded
 *                      Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_TM1637_HPP
#define __FUNSAPE_LIB_TM1637_HPP                __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibTm1637.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibTm1637.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibTm1637.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibTm1637.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibTm1637.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibTm1637.hpp)!
#endif

#include "../funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#   error [funsapeLibTm1637.hpp] Error - Required module (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_NUMBER__
#   error [funsapeLibTm1637.hpp] Error - Build mismatch between required module (funsapeLibGpio.hpp) and current header file (funsapeLibTm1637.hpp)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

// NONE

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// void delayUs(uint16_t delay_p);

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
// Tm1637 - Class declaration
// =============================================================================

class Tm1637
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    enum class Constrast {
        PERCENT_6_25                    = 0,
        PERCENT_13                      = 1,
        PERCENT_26                      = 2,
        PERCENT_62_5                    = 3,
        PERCENT_68_75                   = 4,
        PERCENT_75                      = 5,
        PERCENT_81_25                   = 6,
        PERCENT_87_5                    = 7,
    };

private:
    //     //////////////////////     COMMANDS     //////////////////////     //
    enum class Command : uint8_t {
        DATA_SETTING                    = 0x40,
        DISPLAY_ADDRESS                 = 0xC0,
        DISPLAY_CONTROL                 = 0x80,
    };
    //     ////////////////////    BIT POSITIONS     ////////////////////     //
    enum class BitPos : uint8_t {

        // Command setting
        DATA_SETTING_MODE_READ          = 1,
        DATA_SETTING_FIXED_ADDRESS      = 2,
        DATA_SETTING_TEST_MODE          = 3,

        // Display address
        DISPLAY_ADDRESS                 = 0,

        // Display control
        DISPLAY_CONTROL_CONTRAST        = 0,
        DISPLAY_CONTROL_SHOW            = 3,
    };
    //     //////////////////////     BIT MASK     //////////////////////     //
    enum class BitMask : uint8_t {

        // Command setting
        DATA_SETTING_MODE_READ          = 0x01,
        DATA_SETTING_FIXED_ADDRESS      = 0x01,
        DATA_SETTING_TEST_MODE          = 0x01,

        // Display address
        DISPLAY_ADDRESS                 = 0x07,

        // Display control
        DISPLAY_CONTROL_CONTRAST        = 0x07,
        DISPLAY_CONTROL_SHOW            = 0x01,
    };

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operators overloading ---------------------------------------------------

public:
    // NONE

private:
    //     ////////////////////    BIT POSITIONS     ////////////////////     //
    // inlined friend uint8_t operator<<(uint8_t a, Tm1637::BitPos b);
    // inlined friend uint8_t operator>>(uint8_t a, Tm1637::BitPos b);

    // //     //////////////////////     BIT MASK     //////////////////////     //
    // inlined friend uint8_t operator&(uint8_t a, Tm1637::BitMask b);
    // inlined friend uint8_t &operator&=(uint8_t &a, Tm1637::BitMask b);

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    Tm1637(
            void
    );
    Tm1637(
            const Gpio *dioPin_p,
            const Gpio *clkPin_p
    );
    // TODO: Implement destructor function
    ~Tm1637(
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
            const Gpio *dioPin_p,
            const Gpio *clkPin_p
    );
    bool_t setAddressingMode(
            cbool_t useAutoIncrementMode_p
    );

    //     ///////////////////    DISPLAY CONTROL     ///////////////////     //
    bool_t setDisplayContrast(
            const Constrast contrastLevel_p
    );
    bool_t showDisplay(
            cbool_t showDisplay_p
    );
    bool_t writeDisplayData(
            cuint16_t displayValue_p,
            cuint8_t base_p = 10
    );
    bool_t writeDisplayData(
            cuint8_t segments_p,
            cuint8_t index_p,
            cbool_t showDot_p = false
    );

    //     ///////////////////     KEYPAD CONTROL     ///////////////////     //
    bool_t readKeypadData(
            uint8_t *keyPressed_p
    );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool _isInitialized(
            void
    );

    //     ///////////////     COMMUNICATION PROTOCOL     ///////////////     //
    void _sendStart(
            void
    );
    void _sendStop(
            void
    );
    void _writeByte(
            uint8_t byteToWrite_p
    );
    void _readByte(
            uint8_t *byteRead_p
    );
    bool _readAck(
            void
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     ////////////////    PERIPHERAL BUS HANDLER     ////////////////     //
    Gpio        *_clkPin;
    Gpio        *_dioPin;

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool        _initialized            : 1;
    Error       _lastError;
    bool        _useAutoIncrementMode   : 1;

    //     ///////////////////    DISPLAY CONTROL     ///////////////////     //
    Constrast   _contrastLevel;
    bool        _showDisplay;

protected:
    // NONE

}; // class Tm1637

// =============================================================================
// Tm1637 - Class overloading operators
// =============================================================================

// inlined uint8_t operator<<(uint8_t a, Tm1637::BitPos b)
// {
//  return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
// }

// inlined uint8_t operator>>(uint8_t a, Tm1637::BitPos b)
// {
//  return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
// }

// inlined uint8_t operator&(uint8_t a, Tm1637::BitMask b)
// {
//  return static_cast<uint8_t>(a & static_cast<uint8_t>(b));
// }

// inlined uint8_t &operator&=(uint8_t &a, Tm1637::BitMask b)
// {
//  return a = static_cast<uint8_t>(a & static_cast<uint8_t>(b));
// }

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
// Tm1637 - Class inline function definitions
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

#endif // __FUNSAPE_LIB_TM1637_HPP

// =============================================================================
// END OF FILE
// =============================================================================
