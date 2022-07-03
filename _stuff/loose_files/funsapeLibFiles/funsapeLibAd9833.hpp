/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibAd9833.hpp
 * Module:          AD9833 Waveform Generator interface for FunSAPE++ Embedded
 *                      Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_AD9833_HPP
#define __FUNSAPE_LIB_AD9833_HPP                __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibAd9833.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibAd9833.hpp] Error - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibAd9833.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibAd9833.hpp] Error - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibAd9833.hpp] Error - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibAd9833.hpp)!
#endif

#include "../src/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#   error [funsapeLibAd9833.hpp] Error - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_NUMBER__
#   error [funsapeLibAd9833.hpp] Error - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibAd9833.hpp)!
#endif

#include "../funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#   error [funsapeLibAd9833.hpp] Error - Required module (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_NUMBER__
#   error [funsapeLibAd9833.hpp] Error - Build mismatch between required module (funsapeLibGpio.hpp) and current header file (funsapeLibAd9833.hpp)!
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
// Ad9833 - Class declaration
// =============================================================================

class Ad9833
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    enum class Channel : uint8_t {
        CHANNEL_0                       = 0,
        CHANNEL_1                       = 1,
    };
    enum class Mode : uint8_t {
        OFF,
        SINE,
        SQUARE1,
        SQUARE2,
        TRIANGLE,
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
    Ad9833(
            void
    );
    // TODO: Implement destructor function
    ~Ad9833(
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
            Bus *busHandler_p,
            Gpio *fsyncPin_p
    );

    //     ////////////////////////     MODE     ////////////////////////     //
    // inline Mode getMode(void) const {
    //     return this->_mode;
    // };
    bool_t setMode(
            Mode mode_p
    );

    //     //////////////////////    FREQUENCY     //////////////////////     //
    // Channel getActiveFrequency(
    //         void
    // );
    bool_t setActiveFrequency(
            Channel chan_p
    );
    // inline float getFrequency(Channel chan) const {
    //     return _freq[chan];
    // };
    bool_t setFrequency(
            Channel chan,
            float freq
    );

    bool_t start(
            void
    );
    bool_t stop(
            bool_t hold_p = false
    );

    //     ////////////////////////    PHASE     ////////////////////////     //
    // Channel getActivePhase(
    //         void
    // );
    bool_t setActivePhase(
            Channel channel_p
    );
    // inline uint16_t getPhase(Channel channel_p) {
    //     return _phase_p[chan];
    // };
    bool_t setPhase(
            Channel channel_p,
            uint16_t phase_p
    );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool_t _isInitialized(
            void
    );
    bool_t _sendData(
            void
    );
    // Convenience calculations
    uint32_t _calcFreq(float f_p); // Calculate AD9833 frequency register from a frequency
    uint16_t _calcPhase(float a_p);// Calculate AD9833 phase register from phase

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     ////////////////    PERIPHERAL BUS HANDLER     ////////////////     //
    Bus         *_busHandler;
    Gpio        *_fsyncPin;

    //     /////////////////     HARDWARE REGISTERS     /////////////////     //
    uint16_t    _regControl;            // control register image
    uint32_t    _regFrequency[2];       // frequency registers
    uint32_t    _regPhase[2];           // phase registers

    //     //////////////////////     SETTINGS     //////////////////////     //
    Mode        _modeLast;              // last set mode
    float       _freq[2];               // last frequencies set
    uint16_t    _phase[2];              // last phase setting


    // // SPI related
    // void dumpCmd(uint16_t reg);       // debug routine
    // void spiSend(uint16_t data);      // do the actual physical communications task

protected:
    // NONE

}; // class Ad9833

// =============================================================================
// Ad9833 - Class overloading operators
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
// Ad9833 - Class inline function definitions
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

#endif // __FUNSAPE_LIB_AD9833_HPP

// =============================================================================
// END OF FILE
// =============================================================================
