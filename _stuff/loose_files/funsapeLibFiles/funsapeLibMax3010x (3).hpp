/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibMax3010x.hpp
 * Module:          MAX3010x module interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * Note:            This library supports the following part ids: MAX30101,
 *                      MAX30102, and MAX30105. Only the MAX30102 part was
 *                      tested so far. This library doesn't support the MAX30100
 *                      device.
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_MAX3010X_HPP
#define __FUNSAPE_LIB_MAX3010X_HPP              __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error   [funsapeLibMax3010x.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMax3010x.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibMax3010x.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../src/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#   error   [funsapeLibMax3010x.hpp] Error 14 - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMax3010x.hpp] Error 15 - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibMax3010x.hpp)!
#endif

#include "../src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error   [funsapeLibMax3010x.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMax3010x.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibMax3010x.hpp)!
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
// Max3010x - Class declaration
// =============================================================================

class Max3010x
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    struct RegisterMap {
        uint8_t interruptStatus1;
        uint8_t interruptStatus2;
        uint8_t interruptEnable1;
        uint8_t interruptEnable2;
        uint8_t fifoWritePointer;
        uint8_t fifoOverflowCounter;
        uint8_t fifoReadPointer;
        uint8_t fifoConfiguration;
        uint8_t modeConfiguration;
        uint8_t spo2Configuration;
        uint8_t led1PulseAmplitude;
        uint8_t led2PulseAmplitude;
        uint8_t led3PulseAmplitude;
        uint8_t led4PulseAmplitude;
        uint8_t pilotPulseAmplitude;
        uint8_t multiLedModeControl1;
        uint8_t multiLedModeControl2;
        uint8_t dieTemperatureInteger;
        uint8_t dieTemperatureFraction;
        uint8_t dieTemperatureConfig;
        uint8_t proximityThreshold;
        uint8_t partId;
        uint8_t revisionId;
    };

    //     //////////////////     SAMPLE AVERAGING     //////////////////     //
    enum class Averaging : uint8_t {
        AVER_OFF                        = 0,
        AVER_2                          = 1,
        AVER_4                          = 2,
        AVER_8                          = 3,
        AVER_16                         = 4,
        AVER_32                         = 5,
    };

    //     /////////////////////     FULL SCALE     /////////////////////     //
    enum class FullScale : uint8_t {
        FSCAL_2048_NA                   = 0,
        FSCAL_4096_NA                   = 1,
        FSCAL_8192_NA                   = 2,
        FSCAL_16384_NA                  = 3
    };

    //     ///////////////////    INTERRUPT FLAGS     ///////////////////     //
    typedef enum InterruptFlags : uint8_t {
        NONE                            = 0x00,
        AMBIENT_LIGHT_CANCELLATION      = 0x01,
        FIFO_ALMOST_FULL                = 0x02,
        DIE_TEMPERATURE_READY           = 0x04,
        NEW_FIFO_DATA_READY             = 0x08,
        POWER_READY                     = 0x10,
        PROXIMITY_INTERRUPT             = 0x20,
        ALL                             = 0x3F
    } InterruptFlags;

    //     ////////////////////     MODE CONTROL     ////////////////////     //
    enum class Mode : uint8_t {
        NO_MODE                         = 0,
        HEART_RATE_MODE                 = 2,
        SPO2_MODE                       = 3,
        MULTI_LED_MODE                  = 7
    };

    //     ///////////////////////    PART ID     ///////////////////////     //
    enum class PartId : uint8_t {
        MAX30101                        = 0,
        MAX30102                        = 1,
        MAX30105                        = 2,
    };

    //     /////////////////////     RESOLUTION     /////////////////////     //
    enum class Resolution : uint8_t {
        RES_15_BITS                     = 0,
        RES_16_BITS                     = 1,
        RES_17_BITS                     = 2,
        RES_18_BITS                     = 3
    };

    //     /////////////////////    SAMPLE RATE     /////////////////////     //
    enum class SampleRate : uint8_t {
        RATE_50_HZ                      = 0,
        RATE_100_HZ                     = 1,
        RATE_200_HZ                     = 2,
        RATE_400_HZ                     = 3,
        RATE_800_HZ                     = 4,
        RATE_1000_HZ                    = 5,
        RATE_1600_HZ                    = 6,
        RATE_3200_HZ                    = 7
    };

    //     ////////////////    MULTILED SLOT CONTROL     ////////////////     //
    enum class Slot : uint8_t {
        SLOT_OFF                        = 0,
        SLOT_RED_AMP_LED                = 1,
        SLOT_INFRARED_AMP_LED           = 2,
        SLOT_GREEN_AMP_LED              = 3,
        SLOT_RED_AMP_PILOT              = 5,
        SLOT_INFRARED_AMP_PILOT         = 6,
        SLOT_GREEN_AMP_PILOT            = 7
    };

private:
    //     ////////////////////     BIT POSITION     ////////////////////     //
    enum class Bit : uint8_t {
        A_FULL_INT                      = 7,    // Interrupt Status 1
        DATA_RDY_INT                    = 6,
        ALC_OVF_INT                     = 5,
        PROX_INT                        = 4,
        PWR_RDY_INT                     = 0,
        DIE_TEMP_RDY_INT                = 1,    // Interrupt Status 2
        A_FULL_EN                       = 7,    // Interrupt Enable 1
        DATA_RDY_EN                     = 6,
        ALC_OVF_EN                      = 5,
        PROX_INT_EN                     = 4,
        DIE_TEMP_RDY_EN                 = 1,    // Interrupt Enable 2
        FIFO_WR_PTR                     = 0,    // FIFO Write Pointer
        FIFO_OVF_COUNTER                = 0,    // FIFO Overflow Counter
        FIFO_RD_PTR                     = 0,    // FIFO Read Pointer
        FIFO_DATA                       = 0,    // FIFO Data Register
        FIFO_SMP_AVE                    = 5,    // FIFO Configuration
        FIFO_ROLLOVER_EN                = 4,
        FIFO_A_FULL                     = 0,
        SHDN                            = 7,    // Mode Configuration
        RESET                           = 6,
        MODE                            = 0,
        ADC_RGE                         = 5,    // SPO2 Configuration
        SR                              = 2,
        LED_PW                          = 0,
        LED1_PA                         = 0,    // LED Pulse Amplitude 1
        LED2_PA                         = 0,    // LED Pulse Amplitude 2
        LED3_PA                         = 0,    // LED Pulse Amplitude 3
        LED4_PA                         = 0,    // LED Pulse Amplitude 4
        PILOT_PA                        = 0,    // Proximity Mode LED Pulse Amplitude
        SLOT2                           = 4,    // Multi-LED Mode Control Register 1
        SLOT1                           = 0,
        SLOT4                           = 4,    // Multi-LED Mode Control Register 2
        SLOT3                           = 0,
        // Temperature Data
        TINT                            = 0,    // Die Temperature Integer
        TFRAC                           = 0,    // Die Temperature Fraction
        TEMP_EN                         = 0,    // Die Temperature Config
        // Proximity Function
        PROX_INT_THRESH                 = 0,    // Proximity Interrupt Threshold
    };
    //     //////////////////     REGISTER ADDRESS     //////////////////     //
    enum class Register : uint8_t {
        // Status registers
        INTERRUPT_STATUS_1              = 0x00,
        INTERRUPT_STATUS_2              = 0x01,
        INTERRUPT_ENABLE_1              = 0x02,
        INTERRUPT_ENABLE_2              = 0x03,

        // FIFO registers
        FIFO_WRITE_POINTER              = 0x04,
        FIFO_OVERFLOW_COUNTER           = 0x05,
        FIFO_READ_POINTER               = 0x06,
        FIFO_DATA                       = 0x07,

        // Configuration registers
        FIFO_CONFIGURATION              = 0x08,
        MODE_CONFIGURATION              = 0x09,
        SPO2_CONFIGURATION              = 0x0A,
        LED1_PULSE_AMPLITUDE            = 0x0C,
        LED2_PULSE_AMPLITUDE            = 0x0D,
        LED3_PULSE_AMPLITUDE            = 0x0E,
        PROXIMITY_LED_AMPLITUDE         = 0x10,
        MULTI_LED_CONTROL_1             = 0x11,
        MULTI_LED_CONTROL_2             = 0x12,

        // Die temperature registers
        DIE_TEMPERATURE_INTEGER         = 0x1F,
        DIE_TEMPERATURE_FRACTION        = 0x20,
        DIE_TEMPERATURE_CONFIG          = 0x21,

        // Proximity function registers
        PROXIMITY_INTERRUPT_THRESHOLD   = 0x30,

        // Part ID registers
        REVISION_ID                     = 0xFE,
        PART_ID                         = 0xFF
    };

protected:
    //     /////////////////////     RESOLUTION     /////////////////////     //

    // -------------------------------------------------------------------------
    // Operators overloading ---------------------------------------------------

public:
    // NONE

private:
    inlined friend uint8_t operator<<(uint8_t a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(uint8_t a, Max3010x::Bit b);

    inlined friend uint8_t operator<<(Max3010x::Averaging a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(Max3010x::Averaging a, Max3010x::Bit b);

    inlined friend uint8_t operator<<(Max3010x::FullScale a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(Max3010x::FullScale a, Max3010x::Bit b);

    inlined friend uint8_t operator<<(Max3010x::Mode a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(Max3010x::Mode a, Max3010x::Bit b);

    inlined friend uint8_t operator<<(Max3010x::Resolution a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(Max3010x::Resolution a, Max3010x::Bit b);

    inlined friend uint8_t operator<<(Max3010x::SampleRate a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(Max3010x::SampleRate a, Max3010x::Bit b);

    inlined friend uint8_t operator<<(Max3010x::Slot a, Max3010x::Bit b);
    inlined friend uint8_t operator>>(Max3010x::Slot a, Max3010x::Bit b);

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    Max3010x(
            PartId partId_p = PartId::MAX30102
    );
    ~Max3010x(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     //////////////////     DEVICE OPERATION     //////////////////     //
    bool setFullScale(
            FullScale fullScale_p
    );
    bool setHeartRateMode(
            uint8_t redLedCurrent_p = 1
    );
    bool setMultiLedMode(
            Slot slot1_p,
            Slot slot2_p = Slot::SLOT_OFF,
            Slot slot3_p = Slot::SLOT_OFF,
            Slot slot4_p = Slot::SLOT_OFF
    );
    bool setResolution(
            Resolution resolution_p
    );
    bool setSampleRate(
            SampleRate sampleRate_p
    );
    bool setSpo2Mode(
            uint8_t redLedCurrent_p = 1,
            uint8_t infraredLedCurrent_p = 1
    );

    //     ////////////////////    DEVICE STATUS     ////////////////////     //
    Error getLastError(
            void
    );
    bool init(
            Bus *busHandler_p
    );
    bool reset(
            void
    );
    bool shutDown(
            void
    );
    bool wakeUp(
            void
    );

    //     ///////////////    FIFO CONTROL AND STATUS     ///////////////     //
    bool configFifo(
            Averaging sampleAveraging_p,
            bool rolloverOnFull_p,
            uint8_t fifoAlmostFullValue_p
    );
    bool flushFifo(
            void
    );
    bool getFifoOccupation(
            uint8_t *samplesWaiting_p,
            uint8_t *samplesLost_p = nullptr
    );
    bool getFifoData(
            uint8_t samplesCount_p,
            uint16_t *redLedBuffer_p,
            uint16_t *infraredLedBuffer_p = nullptr
    );
    bool getFifoData(
            uint8_t samplesCount_p,
            uint32_t *redLedBuffer_p,
            uint32_t *infraredLedBuffer_p = nullptr
    );
    bool getFifoData(
            uint8_t samplesCount_p,
            uint16_t *ledBuff1_p,
            uint16_t *ledBuff2_p,
            uint16_t *ledBuff3_p,
            uint16_t *ledBuff4_p = nullptr
    );
    bool getFifoData(
            uint8_t samplesCount_p,
            uint32_t *ledBuff1_p,
            uint32_t *ledBuff2_p,
            uint32_t *ledBuff3_p,
            uint32_t *ledBuff4_p = nullptr
    );
    bool getFifoNewestSample(
            uint16_t *redValue_p,
            uint16_t *infraredValue_p = nullptr
    );
    bool getFifoNewestSample(
            uint32_t *redValue_p,
            uint32_t *infraredValue_p = nullptr
    );
    bool getFifoNewestSample(
            uint16_t *led1Value_p,
            uint16_t *led2Value_p,
            uint16_t *led3Value_p,
            uint16_t *led4Value_p = nullptr
    );
    bool getFifoNewestSample(
            uint32_t *led1Value_p,
            uint32_t *led2Value_p,
            uint32_t *led3Value_p,
            uint32_t *led4Value_p = nullptr
    );
    bool getFifoOldestSample(
            uint16_t *redValue_p,
            uint16_t *infraredValue_p = nullptr
    );
    bool getFifoOldestSample(
            uint32_t *redValue_p,
            uint32_t *infraredValue_p = nullptr
    );
    bool getFifoOldestSample(
            uint16_t *led1Value_p,
            uint16_t *led2Value_p,
            uint16_t *led3Value_p,
            uint16_t *led4Value_p = nullptr
    );
    bool getFifoOldestSample(
            uint32_t *led1Value_p,
            uint32_t *led2Value_p,
            uint32_t *led3Value_p,
            uint32_t *led4Value_p = nullptr
    );

    //     /////////////////    INTERRUPT CALLBACKS     /////////////////     //
    bool setIntAmbLightCancOverflowCallback(
            void (* callbackFunction_p)(void)
    );
    bool setIntDieTempReadyCallback(
            void (* callbackFunction_p)(void)
    );
    bool setIntFifoAlmostFullCallback(
            void (* callbackFunction_p)(void)
    );
    bool setIntNewFifoDataReadyCallback(
            void (* callbackFunction_p)(void)
    );
    bool setIntPowerReadyCallback(
            void (* callbackFunction_p)(void)
    );
    bool setIntProximityCallback(
            void (* callbackFunction_p)(void)
    );

    //     //////////////////    INTERRUPT CONTROL     //////////////////     //
    bool disableInterrupts(
            InterruptFlags deactivationMask_p
    );
    bool enableInterrupts(
            InterruptFlags activationMask_p
    );

    //     //////////////////     INTERRUPT STATUS     //////////////////     //
    bool generalIrqHandler(
            void
    );

    //     /////////////////////    LED CURRENT     /////////////////////     //
    bool setGreenLedCurrent(
            uint8_t greenLed1Current_p = 1,
            uint8_t greenLed2Current_p = 0
    );
    bool setInfraredLedCurrent(
            uint8_t infraredLedCurrent_p = 1
    );
    bool setPilotLedCurrent(
            uint8_t pilotLedCurrent_p = 1
    );
    bool setRedLedCurrent(
            uint8_t redLedCurrent_p = 1
    );

    //     ////////////////////     SLOT CONTROL     ////////////////////     //
    // NONE

    //     //////////////////     TEMPERATURE DATA     //////////////////     //
    bool getTemperature(
            float *tempValue_p,
            ThermometricScale tempScale_p = ThermometricScale::CELSIUS
    );
    bool getTemperature(
            int8_t *tempInteger_p,
            uint8_t *tempFrac_p
    );
    // bool getTemperature(
    // TODO: Implement a function to return the temperature in fixed point notation
    //  Q16_4 * tempValue_p
    // );
    bool startTemperatureRead(
            void
    );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    void _clearData(
            void
    );
    bool _isInitialized(
            void
    );
    bool _reset(
            void
    );

    //     /////////////////////     INTERRUPTS     /////////////////////     //
    bool _updateInterruptStatus(
            void
    );

    //     ////////////////    REGISTER MANIPULATION     ////////////////     //
    bool _read(
            Register regAddr_p,
            uint8_t *dataBuffer_p,
            uint16_t bytesToRead_p
    );
    bool _write(
            Register regAddr_p,
            uint8_t *dataBuffer_p,
            uint16_t bytesToWrite_p
    );

    //     ///////////////    FIFO CONTROL AND STATUS     ///////////////     //
    bool _getFifoInfo(
            void
    );
    bool _getFifoReadPointer(
            uint8_t *pointer_p
    );
    bool _getFifoWritePointer(
            uint8_t *pointer_p
    );
    bool _readFifoData(
            uint8_t samples_p,
            void *led1Buffer_p,
            void *led2Buffer_p,
            void *led3Buffer_p,
            void *led4Buffer_p,
            uint8_t varSize_p
    );
    bool _setFifoReadPointer(
            uint8_t pointer_p
    );
    bool _setFifoWritePointer(
            uint8_t pointer_p
    );

    //     /////////////     TEMPORARY - USED FOR DEBUG     /////////////     //
public:
    bool readAllRegs(
            RegisterMap *regMap_p
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

    //     //////////////////     DEVICE OPERATION     //////////////////     //
    FullScale   _fullScale;
    Mode        _modeControl;
    Resolution  _resolution;
    SampleRate  _sampleRate;

    //     ////////////////////    DEVICE STATUS     ////////////////////     //
    bool    _initialized                        : 1;
    Error   _lastError;
    bool    _modeSelected                       : 1;
    PartId  _partId;
    uint8_t _partRevision;
    bool    _shuttedDown                        : 1;

    //     ///////////////    FIFO CONTROL AND STATUS     ///////////////     //
    uint8_t     _activeLedsCounter              : 3;
    uint8_t     _fifoAlmostFullValue            : 4;
    uint8_t     _fifoOverflowCounter            : 5;
    uint8_t     _fifoReadPointer                : 5;
    bool        _fifoRolloverOnFull             : 1;
    uint8_t     _fifoWritePointer               : 5;
    Averaging   _sampleAveraging;

    //     /////////////////    INTERRUPT CALLBACKS     /////////////////     //
    void (*_intAmbLightCancOverflowCallback)(void);
    void (*_intDieTempReadyCallback)(void);
    void (*_intFifoAlmostFullCallback)(void);
    void (*_intNewFifoDataReadyCallback)(void);
    void (*_intPowerReadyCallback)(void);
    void (*_intProximityCallback)(void);

    //     //////////////////    INTERRUPT CONTROL     //////////////////     //
    bool    _intAmbLightCancOverflowEnable      : 1;
    bool    _intDieTempReadyEnable              : 1;
    bool    _intFifoAlmostFullEnable            : 1;
    bool    _intNewFifoDataReadyEnable          : 1;
    bool    _intProximityEnable                 : 1;

    //     //////////////////     INTERRUPT STATUS     //////////////////     //
    bool    _intAmbLightCancOverflowStatus      : 1;
    bool    _intDieTempReadyStatus              : 1;
    bool    _intFifoAlmostFullStatus            : 1;
    bool    _intNewFifoDataReadyStatus          : 1;
    bool    _intPowerReadyStatus                : 1;
    bool    _intProximityStatus                 : 1;

    //     /////////////////////    LED CURRENT     /////////////////////     //
    uint8_t _led1Current;
    uint8_t _led2Current;
    uint8_t _led3Current;
    uint8_t _led4Current;
    uint8_t _pilotCurrent;

    //     ////////////////////     SLOT CONTROL     ////////////////////     //
    Slot    _slot1;
    Slot    _slot2;
    Slot    _slot3;
    Slot    _slot4;
    uint8_t _slotsCounter                       : 3;

    //     //////////////////     TEMPERATURE DATA     //////////////////     //
    bool    _dieTempEnable                      : 1;
    uint8_t _dieTempFraction                    : 4;
    uint8_t _dieTempInteger;

protected:
    // NONE

}; // class Max3010x

// =============================================================================
// Max3010x - Class overloading operators
// =============================================================================

// -----------------------------------------------------------------------------
// Bit position operator overloading -------------------------------------------

inlined uint8_t operator<<(uint8_t a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(uint8_t a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Max3010x::Averaging a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(Max3010x::Averaging a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Max3010x::FullScale a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(Max3010x::FullScale a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Max3010x::Mode a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(Max3010x::Mode a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Max3010x::Resolution a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(Max3010x::Resolution a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Max3010x::SampleRate a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(Max3010x::SampleRate a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Max3010x::Slot a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(Max3010x::Slot a, Max3010x::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

// -----------------------------------------------------------------------------
// Interrupt flags operator overloading ----------------------------------------

inlined Max3010x::InterruptFlags operator|(Max3010x::InterruptFlags a, Max3010x::InterruptFlags b)
{
    return static_cast<Max3010x::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inlined Max3010x::InterruptFlags &operator|=(Max3010x::InterruptFlags &a, Max3010x::InterruptFlags b)
{
    return a = static_cast<Max3010x::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
}

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
// Max3010x - Class inline function definitions
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

#endif // __FUNSAPE_LIB_MAX3010X_HPP

// =============================================================================
// END OF FILE
// =============================================================================
