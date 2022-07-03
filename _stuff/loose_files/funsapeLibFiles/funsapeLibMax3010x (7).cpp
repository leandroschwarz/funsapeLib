/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibMax3010x.cpp
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
// Dependencies
// =============================================================================

#include "funsapeLibMax3010x.hpp"
#ifndef __FUNSAPE_LIB_MAX3010X_HPP
#   error   [funsapeLibMax3010x.cpp] Error 16 - Header file (funsapeLibMax3010x.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_MAX3010X_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMax3010x.cpp] Error 17 - Build mismatch between source (funsapeLibMax3010x.cpp) and header (funsapeLibMax3010x.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define MAX3010X_DEVICE_ADDRESS         0x57
#define MAX3010X_FIFO_SIZE_IN_SAMPLES   32
#define MAX3010X_PART_ID                0x15
#define MAX3010X_BYTES_PER_CHANNEL      3
// #define MAX3010x_SERIAL_CLOCK_FREQUENCY_MAX      400'000UL
// #define MAX3010x_SERIAL_CLOCK_FREQUENCY_MIN      0UL
// #define MAX3010x_TEMPERATURE_CONVERSION_TIMEOUT  10

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

static void defaultCallbackFunction(void);

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief       Construct a new Max3010x::Max3010x object.
 * @details     The constructor also clears the class internal data. Since there
 *                  is no dynamic memory allocation procedures, the function
 *                  always sets internal the error variable to Error::NONE.
 * @param[in]   partId_p    part identifier. Refer to \ref{PartId} for details
 */
Max3010x::Max3010x(PartId partId_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::Max3010x(PartId)", Debug::CodeIndex::MAX3010X_MODULE);

    // Resets data members
    this->_clearData();
    this->_busHandler                           = nullptr;
    this->_initialized                          = false;
    this->_intAmbLightCancOverflowCallback      = defaultCallbackFunction;
    this->_intDieTempReadyCallback              = defaultCallbackFunction;
    this->_intFifoAlmostFullCallback            = defaultCallbackFunction;
    this->_intNewFifoDataReadyCallback          = defaultCallbackFunction;
    this->_intPowerReadyCallback                = defaultCallbackFunction;
    this->_intProximityCallback                 = defaultCallbackFunction;
    this->_partRevision                         = 0;

    // Update data members
    switch (partId_p) {
    case PartId::MAX30101:
    case PartId::MAX30102:
    case PartId::MAX30105:
        this->_partId = partId_p;
        break;
    default:
        // Return error
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief       Destroy the Max3010x::Max3010x object.
 * @details     Since there is no dynamic memory allocation procedures, the
 *                  function has nothing to do and just returns normally.
 */
Max3010x::~Max3010x(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::~Max3010x(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Returns successfully
    return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ////////////////////     DEVICE OPERATION     ////////////////////     //

/**
 * @brief       Configures the full scale of the ADC.
 * @details     Configures the full scale of the ADC. The ADC has programmable
 *                  full-scale ranges from 2 µA to 16 µA.
 * @param[in]   fullScale_p     ADC's full scale value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setFullScale(FullScale fullScale_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setFullScale(FullScale)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff = (
                    (fullScale_p << Bit::ADC_RGE) |
                    (this->_sampleRate << Bit::SR) |
                    (this->_resolution << Bit::LED_PW)
            );
    if (!this->_write(Register::SPO2_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fullScale = fullScale_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the device in Heart Rate mode.
 * @details     Configures the device in Heart Rate mode. In this mode, each
 *                  sample consist of 3-byte output into the FIFO. By default,
 *                  only the red LED is used to capture optical data and
 *                  determine the user’s heart rate and/or photoplethysmogram
 *                  (PPG). The function also configures the red LED amplitude
 *                  of the device and turns the infrared LED channel off. The
 *                  LED current can be programmed from 0 to 51 mA with proper
 *                  supply voltage.
 * @note        Actual measured LED's current amplitude for each part can vary widely due to
 *                  the trimming methodology.
 * @note        The user can select either the red LED or the infrared LED
 *                  channel for heart rate measurements. If the infrared channel
 *                  is desired, the user must turn the red LED OFF, by calling
 *                  the setRedLedCurrent() with the parameter 0, and call the
 *                  setInfraredLedCurrent() to configure its amplitide.
 * @attention   Heart Rate mode using the infrared LED was not tested and its
 *                  not clear if it really works. Further tests must be
 *                  executed.
 * @param[in]   redLedCurrent_p     LED's current amplitude value in
 *                                      milliamperes. Value must be [0 - 51] for
 *                                      mA current.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setHeartRateMode(uint8_t redLedCurrent_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setHeartRateMode(uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[4];
    uint8_t auxSize = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments
    if (redLedCurrent_p > 51) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }

    // Process function arguments
    redLedCurrent_p *= 5;               // Transform to register value

    // Configure mode
    auxBuff[0] = (
                    (this->_shuttedDown << Bit::SHDN) |
                    (Mode::HEART_RATE_MODE << Bit::MODE)
            );
    if (!this->_write(Register::MODE_CONFIGURATION, auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Configure LEDs
    auxBuff[0] = (redLedCurrent_p << Bit::LED1_PA);
    auxBuff[1] = (0 << Bit::LED2_PA);
    switch (this->_partId) {
    case PartId::MAX30101:
        auxBuff[2] = (0 << Bit::LED3_PA);
        auxBuff[3] = (0 << Bit::LED4_PA);
        auxSize = 4;
        break;
    case PartId::MAX30102:
        auxSize = 2;
        break;
    case PartId::MAX30105:
        auxBuff[2] = (0 << Bit::LED3_PA);
        auxSize = 3;
        break;
    default:
        // Returns error
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return false;
    }
    if (!this->_write(Register::LED1_PULSE_AMPLITUDE, auxBuff, auxSize)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_activeLedsCounter = 1;
    this->_led1Current = redLedCurrent_p;
    this->_led2Current = 0;
    this->_led3Current = 0;
    this->_led4Current = 0;
    this->_modeControl = Mode::HEART_RATE_MODE;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the device in Multi-LED mode.
 * @details     Configures the device in Multi-LED mode. In this mode, each
 *                  sample is split into up to four time slots, SLOT1 through
 *                  SLOT4. It is possible to set which LED is active in each
 *                  time slot, making for a very flexible configuration.
 *                  Each slot generates a 3-byte output into the FIFO. One
 *                  sample comprises all active slots, for example if SLOT1 and
 *                  SLOT2 are non-zero, then one sample is 2 x 3 = 6 bytes.
 *                  The slots should be enabled in order (i.e., SLOT1 should not
 *                  be disabled if SLOT2 is enabled).
 * @note        The currents of the LEDs remain unchanged. User must call the
 *                  setRedLedCurrent() and setInfraredLedCurrent() functions in
 *                  order to proper configure the device.
 * @param[in]   slot1_p     Determine the active LED in that time slot.
 * @param[in]   slot2_p     Determine the active LED in that time slot.
 * @param[in]   slot3_p     Determine the active LED in that time slot.
 * @param[in]   slot4_p     Determine the active LED in that time slot.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                          getLastError() function.
 */
bool Max3010x::setMultiLedMode(Slot slot1_p, Slot slot2_p, Slot slot3_p, Slot slot4_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setMultiLedMode(Slot, Slot, Slot, Slot)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if (this->_partId == PartId::MAX30101) {                // MAX30101
        if (
                (slot1_p > Slot::SLOT_GREEN_AMP_LED) ||
                (slot2_p > Slot::SLOT_GREEN_AMP_LED) ||
                (slot3_p > Slot::SLOT_GREEN_AMP_LED) ||
                (slot4_p > Slot::SLOT_GREEN_AMP_LED)
        ) {
            // Returns error
            this->_lastError = Error::ARGUMENT_VALUE_INVALID;
            debugMessage(Error::ARGUMENT_VALUE_INVALID);
            return false;
        }
    } else if (this->_partId == PartId::MAX30102) {     // MAX30102
        if (
                (slot1_p > Slot::SLOT_INFRARED_AMP_LED) ||
                (slot2_p > Slot::SLOT_INFRARED_AMP_LED) ||
                (slot3_p > Slot::SLOT_INFRARED_AMP_LED) ||
                (slot4_p > Slot::SLOT_INFRARED_AMP_LED)
        ) {
            // Returns error
            this->_lastError = Error::ARGUMENT_VALUE_INVALID;
            debugMessage(Error::ARGUMENT_VALUE_INVALID);
            return false;
        }
    } else {                                            // MAX30105
        doNothing();
    }
    // Check function arguments for errors
    if (slot1_p == Slot::SLOT_OFF) {                // Then, slots 2, 3, and 4 MUST be OFF
        if ((slot2_p != Slot::SLOT_OFF) || (slot3_p != Slot::SLOT_OFF) || (slot4_p != Slot::SLOT_OFF)) {
            // Returns error
            this->_lastError = Error::ARGUMENTS_MISMATCH;
            debugMessage(Error::ARGUMENTS_MISMATCH);
            return false;
        }
        this->_activeLedsCounter = 0;
    } else if (slot2_p == Slot::SLOT_OFF) {     // Then, slots 3, and 4 MUST be OFF
        if ((slot3_p != Slot::SLOT_OFF) || (slot4_p != Slot::SLOT_OFF)) {
            // Returns error
            this->_lastError = Error::ARGUMENTS_MISMATCH;
            debugMessage(Error::ARGUMENTS_MISMATCH);
            return false;
        }
        this->_activeLedsCounter = 1;
    } else if (slot3_p == Slot::SLOT_OFF) {     // Then, slot 4 MUST be OFF
        if (slot4_p != Slot::SLOT_OFF) {
            // Returns error
            this->_lastError = Error::ARGUMENTS_MISMATCH;
            debugMessage(Error::ARGUMENTS_MISMATCH);
            return false;
        }
        this->_activeLedsCounter = 2;
    } else if (slot4_p == Slot::SLOT_OFF) {     // Ok, just set active LEDs to 3
        this->_activeLedsCounter = 3;
    } else {                                    // Ok, just set active LEDs to 4
        this->_activeLedsCounter = 4;
    }

    // Configure device
    auxBuff[0] = (
                    (slot2_p << Bit::SLOT2) |
                    (slot1_p << Bit::SLOT1)
            );
    auxBuff[1] = (
                    (slot4_p << Bit::SLOT4) |
                    (slot3_p << Bit::SLOT3)
            );
    if (!this->_write(Register::MULTI_LED_CONTROL_1, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    auxBuff[0] = (
                    (this->_shuttedDown << Bit::SHDN) |
                    (Mode::MULTI_LED_MODE << Bit::MODE)
            );
    if (!this->_write(Register::MODE_CONFIGURATION, auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_modeControl = Mode::MULTI_LED_MODE;
    this->_slot1 = slot1_p;
    this->_slot2 = slot2_p;
    this->_slot3 = slot3_p;
    this->_slot4 = slot4_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the channel's output resolution.
 * @details     Configures the channel's output resolution of the ADC sensor.
 *                  The output resolution of the ADC sensor is directly related
 *                  to LED pulse width (the IR and Red have the same pulse
 *                  width), and therefore, indirectly sets the integration time
 *                  of the ADC in each sample.
 * @param[in]   resolution_p        ADC sensor full scale.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setResolution(Resolution resolution_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setResolution(Resolution)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff = (
                    (this->_fullScale << Bit::ADC_RGE) |
                    (this->_sampleRate << Bit::SR) |
                    (resolution_p << Bit::LED_PW)
            );
    if (!this->_write(Register::SPO2_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_resolution = resolution_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the ADC's output sample rate.
 * @details     Configures the ADC's output sample rate. The ADC output data
 *                  rate can be programmed from 50 sps to 3200 sps. The maximum
 *                  sample rate for the ADC depends on the selected pulse width,
 *                  which in turn, determines the ADC resolution. For instance,
 *                  if the ADCresolution is set to 15 bits (pulse width = 69
 *                  µs), then all sample rates are selectable. However, if the
 *                  ADC resolution is set to 18 bits (pulse width = 411 µs),
 *                  then the samples rates are limited in accordance to the
 *                  selected operation mode.
 * @note        The actual FIFO ouput rate is a combination of the ADC's output
 *                  sample rate and the FIFO averaging. (PUT EQUATION HERE!!!)
 * @attention   If the user selects a sample rate that is too high for the
 *                  selected resolution, the highest possible sample rate is
 *                  programmed instead into the register.
 * @param[in]   resolution_p        ADC sensor full scale.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setSampleRate(SampleRate sampleRate_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setSampleRate(SampleRate)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff = (
                    (this->_fullScale << Bit::ADC_RGE) |
                    (sampleRate_p << Bit::SR) |
                    (this->_resolution << Bit::LED_PW)
            );
    if (!this->_write(Register::SPO2_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_sampleRate = sampleRate_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the device in SpO2 mode.
 * @details     Configures the device in SpO2 mode. In this mode, each sample
 *                  consist of 6-byte (2 x 3 bytes) output into the FIFO. Both
 *                  red and infrared LEDs are active. In the FIFO, the red
 *                  channel isstored first, followed by the infrared channel.
 *                  The function also configures the LEDs' amplitudes of the
 *                  device. The LED current can be programmed from 0 to 51 mA
 *                  with proper supply voltage.
 * @note        Actual measured LED's current amplitude for each part can vary widely due to
 *                  the trimming methodology.
 * @note        In SpO2 mode, the user must use the internal temperature sensor
 *                  information to calibrate the temperature dependence of the
 *                  SpO2 sub-system. The device output data is relatively
 *                  insensitive to the wavelength of the IR LED, where the Red
 *                  LED's wavelength is critical to correct interpretation of
 *                  the data. An SpO2 algorithm used with the MAX3010x output
 *                  signal can compensate for the associated SpO2 error with
 *                  ambient temperature changes.
 * @param[in]   redLedCurrent_p         LED's current amplitude value.
 * @param[in]   infraredLedCurrent_p    LED's current amplitude value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setSpo2Mode(uint8_t redLedCurrent_p, uint8_t infraredLedCurrent_p)
{
    // Local variables
    uint8_t auxBuff[4];
    uint8_t auxSize = 0;

    // Mark passage for debugging purpose
    debugMark("Max3010x::setSpo2Mode(uint8_t, uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments
    if ((redLedCurrent_p > 51) || (infraredLedCurrent_p > 51)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }

    // Process function arguments
    redLedCurrent_p *= 5;               // Transform to register value
    infraredLedCurrent_p *= 5;          // Transform to register value

    // Configure mode
    auxBuff[0] = (
                    (this->_shuttedDown << Bit::SHDN) |
                    (Mode::SPO2_MODE << Bit::MODE)
            );
    if (!this->_write(Register::MODE_CONFIGURATION, auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Configure LEDs
    auxBuff[0] = (redLedCurrent_p << Bit::LED1_PA);
    auxBuff[1] = (infraredLedCurrent_p << Bit::LED2_PA);
    switch (this->_partId) {
    case PartId::MAX30101:
        auxBuff[2] = (0 << Bit::LED3_PA);
        auxBuff[3] = (0 << Bit::LED4_PA);
        auxSize = 4;
        break;
    case PartId::MAX30102:
        auxSize = 2;
        break;
    case PartId::MAX30105:
        auxBuff[2] = (0 << Bit::LED3_PA);
        auxSize = 3;
        break;
    default:
        // Returns error
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return false;
    }
    if (!this->_write(Register::LED1_PULSE_AMPLITUDE, auxBuff, auxSize)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_activeLedsCounter = 2;
    this->_led1Current = redLedCurrent_p;
    this->_led2Current = infraredLedCurrent_p;
    this->_led3Current = 0;
    this->_led4Current = 0;
    this->_modeControl = Mode::SPO2_MODE;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     //////////////////////    DEVICE STATUS     //////////////////////     //

/**
 * @brief       Returns the code of the last error.
 * @details     Returns the FunSAPE++ Embedded Library error code of the last
 *              operation. If the operation was successfull, the return code
 *              will be \ref{Error::NONE}.
 * @return      Error   \ref{Error} error code.
 */
Error Max3010x::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

/**
 * @brief       Initializes the device.
 * @details     Initializes the handler and communicates with the device to
 *                  check it's part ID.
 * @param[in]   handler_p   \ref{Twi} class handler associated with the device
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::init(Bus *busHandler_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::init(Bus *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];

    // Resets data members
    this->_clearData();
    this->_busHandler                           = nullptr;
    this->_initialized                          = false;
    this->_partRevision                         = 0;

    // Check function arguments for errors
    if (!isPointerValid(busHandler_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    } else if (busHandler_p->getBusType() != Bus::BusType::TWI) {
        // Returns error
        this->_lastError = Error::HANDLER_NOT_SUPPORTED;
        debugMessage(Error::HANDLER_NOT_SUPPORTED);
        return false;
    }

    // Update data members
    this->_busHandler = busHandler_p;
    this->_initialized = true;

    // Retrieve device's part and revision IDs
    if (!this->_read(Register::PART_ID, auxBuff, 2)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Check device ID
    if (auxBuff[1] != MAX3010X_PART_ID) {
        // Returns error
        this->_initialized = false;
        this->_lastError = Error::DEVICE_ID_MATCH_FAILED;
        debugMessage(Error::DEVICE_ID_MATCH_FAILED);
        return false;
    }
    // FIXME: Write to some registers to discover the real device.

    // Reset device
    delayMs(100);
    if (!this->_reset()) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_initialized                  = true;
    this->_partRevision                 = auxBuff[0];

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Resets the device.
 * @details     Sends a reset signal to device and clears internal class data members.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                          getLastError() function.
 */
bool Max3010x::reset(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::reset(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Sends reset signal
    this->_reset();

    // Clears data members
    this->_clearData();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Makes the device to enter in power-save mode.
 * @details     Makes the device to enter in power-save mode. While power-save
 *                  mode, the LEDs, the ADC sensor, and all interrupts are tuned
 *                  off, but all device registers retain their values (allowing
 *                  quick wake-up). Also, write/read operations function as
 *                  normal.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::shutDown(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::shutDown(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff = (
                    (1 << Bit::SHDN) |
                    (this->_modeControl << Bit::MODE)
            );
    if (!this->_write(Register::MODE_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_shuttedDown = true;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Wakes up the device when in power-save mode.
 * @details     Wakes up the device when in power-save mode. Since all device
 *                  registers retain their values during power-save mode, all
 *                  previous device configuration are active and the device
 *                  responds accordingly imediately (quick wake-up).
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                          getLastError() function.
 */
bool Max3010x::wakeUp(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::wakeUp(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff = (
                    (0 << Bit::SHDN) |
                    (this->_modeControl << Bit::MODE)
            );
    if (!this->_write(Register::MODE_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_shuttedDown = false;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     /////////////////    FIFO CONTROL AND STATUS     /////////////////     //

/**
 * @brief       Configures the FIFO.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[in]   averages_p                  number of ADC samples to be averaged
 *                                              to each sample in FIFO.
 * @param[in]   overflowEnable_p            if true, new samples will replace
 *                                              old ones in FIFO when FIFO is
 *                                              full.
 * @param[in]   remainingSpaceInterrupt_p   controls the number of samples
 *                                              spaces are remaining in FIFO
 *                                              when a FIFO almost full
 *                                              interruption is generated.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::configFifo(Averaging averages_p, bool overflowEnable_p, uint8_t remainingSpaceInterrupt_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::configFifo(SampleAveraging, bool, uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff = (
                    (averages_p << Bit::FIFO_SMP_AVE) |
                    (overflowEnable_p << Bit::FIFO_ROLLOVER_EN) |
                    (remainingSpaceInterrupt_p << Bit::FIFO_A_FULL)
            );
    if (!this->_write(Register::FIFO_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoAlmostFullValue = remainingSpaceInterrupt_p;
    this->_fifoRolloverOnFull = overflowEnable_p;
    this->_sampleAveraging = averages_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Flushes data from the FIFO.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::flushFifo(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::flushFifo(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[3];

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Configure device
    auxBuff[0] = 0;
    auxBuff[1] = 0;
    auxBuff[2] = 0;
    if (!this->_write(Register::FIFO_WRITE_POINTER, auxBuff, 3)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoOverflowCounter = 0;
    this->_fifoReadPointer = 0;
    this->_fifoWritePointer = 0;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Gets the occupation of the FIFO.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  samplesWaiting_p    number of samples available in the FIFO.
 * @param[out]  samplesLost_p       number of samples that where lost in the
 *                                      FIFO.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoOccupation(uint8_t *samplesWaiting_p, uint8_t *samplesLost_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoOccupation(uint8_t *, uint8_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t samples;

    // Check function arguments for errors
    if (!isPointerValid(samplesWaiting_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Gets FIFO information
    if (!this->_getFifoInfo()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Calculate number of available samples
    if (this->_fifoWritePointer >= this->_fifoReadPointer) {
        samples = this->_fifoWritePointer;
        samples -= this->_fifoReadPointer;
    } else {
        samples = MAX3010X_FIFO_SIZE_IN_SAMPLES;
        samples -= this->_fifoReadPointer;
        samples += this->_fifoWritePointer;
    }
    *samplesWaiting_p = samples;
    if (isPointerValid(samplesLost_p)) {
        *samplesLost_p = this->_fifoOverflowCounter;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, converting it to a 2-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[in]   samplesCount_p          number of samples to be read from the
 *                                          FIFO.
 * @param[out]  redLedBuffer_p          pointer to the buffer that will hold the
 *                                          red LED channel values.
 * @param[out]  infraredLedBuffer_p     pointer to the buffer that will hold the
 *                                          infrared LED channel values.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoData(uint8_t samplesCount_p, uint16_t *redLedBuffer_p, uint16_t *infraredLedBuffer_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoData(uint8_t, uint16_t *, uint16_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (samplesCount_p == 0) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }
    if (!isPointerValid(redLedBuffer_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads data from FIFO
    if (samplesCount_p > MAX3010X_FIFO_SIZE_IN_SAMPLES) {
        samplesCount_p = MAX3010X_FIFO_SIZE_IN_SAMPLES;
    }
    if (!this->_readFifoData(samplesCount_p, redLedBuffer_p, infraredLedBuffer_p, nullptr, nullptr, sizeof(uint16_t))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, converting it to a 4-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[in]   samplesCount_p          number of samples to be read from the
 *                                          FIFO.
 * @param[out]  redLedBuffer_p          pointer to the buffer that will hold the
 *                                          red LED channel values.
 * @param[out]  infraredLedBuffer_p     pointer to the buffer that will hold the
 *                                          infrared LED channel values.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoData(uint8_t samplesCount_p, uint32_t *redLedBuffer_p, uint32_t *infraredLedBuffer_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoData(uint8_t, uint32_t *, uint32_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (samplesCount_p == 0) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }
    if (!isPointerValid(redLedBuffer_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads data from FIFO
    if (samplesCount_p > MAX3010X_FIFO_SIZE_IN_SAMPLES) {
        samplesCount_p = MAX3010X_FIFO_SIZE_IN_SAMPLES;
    }
    if (!this->_readFifoData(samplesCount_p, redLedBuffer_p, infraredLedBuffer_p, nullptr, nullptr, sizeof(uint32_t))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, converting it to a 2-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[in]   samplesCount_p          number of samples to be read from the
 *                                          FIFO.
 * @param[out]  ledBuff1_p  pointer to the buffer that will hold the led channel
 *                              1 values.
 * @param[out]  ledBuff2_p  pointer to the buffer that will hold the led channel
 *                              2 values.
 * @param[out]  ledBuff3_p  pointer to the buffer that will hold the led channel
 *                              3 values.
 * @param[out]  ledBuff4_p  pointer to the buffer that will hold the led channel
 *                              4 values.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x:: getFifoData(uint8_t samplesCount_p, uint16_t *ledBuff1_p, uint16_t *ledBuff2_p, uint16_t *ledBuff3_p,
        uint16_t *ledBuff4_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoData(uint8_t, uint16_t *, uint16_t *, uint16_t *, uint16_t *)",
            Debug::CodeIndex::MAX3010X_MODULE);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (samplesCount_p == 0) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }
    if ((!isPointerValid(ledBuff1_p)) || (!isPointerValid(ledBuff2_p)) || (!isPointerValid(ledBuff3_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads data from FIFO
    if (samplesCount_p > MAX3010X_FIFO_SIZE_IN_SAMPLES) {
        samplesCount_p = MAX3010X_FIFO_SIZE_IN_SAMPLES;
    }
    if (!this->_readFifoData(samplesCount_p, ledBuff1_p, ledBuff2_p, ledBuff3_p, ledBuff4_p, sizeof(uint16_t))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, converting it to a 4-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[in]   samplesCount_p          number of samples to be read from the
 *                                          FIFO.
 * @param[out]  ledBuff1_p  pointer to the buffer that will hold the led channel
 *                              1 values.
 * @param[out]  ledBuff2_p  pointer to the buffer that will hold the led channel
 *                              2 values.
 * @param[out]  ledBuff3_p  pointer to the buffer that will hold the led channel
 *                              3 values.
 * @param[out]  ledBuff4_p  pointer to the buffer that will hold the led channel
 *                              4 values.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x:: getFifoData(uint8_t samplesCount_p, uint32_t *ledBuff1_p, uint32_t *ledBuff2_p, uint32_t *ledBuff3_p,
        uint32_t *ledBuff4_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoData(uint8_t, uint32_t *, uint32_t *, uint32_t *, uint32_t *)",
            Debug::CodeIndex::MAX3010X_MODULE);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (samplesCount_p == 0) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }
    if ((!isPointerValid(ledBuff1_p)) || (!isPointerValid(ledBuff2_p)) || (!isPointerValid(ledBuff3_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads data from FIFO
    if (samplesCount_p > MAX3010X_FIFO_SIZE_IN_SAMPLES) {
        samplesCount_p = MAX3010X_FIFO_SIZE_IN_SAMPLES;
    }
    if (!this->_readFifoData(samplesCount_p, ledBuff1_p, ledBuff2_p, ledBuff3_p, ledBuff4_p, sizeof(uint32_t))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the newest sample and converting it to
 *                  a 2-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  redValue_p          pointer to the variable that will hold the
 *                                      red LED channel value.
 * @param[out]  infraredValue_p     pointer to the variable that will hold the
 *                                      infrared LED channel value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoNewestSample(uint16_t *redValue_p, uint16_t *infraredValue_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoNewestSample(uint16_t *, uint16_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxInfrared = 0;
    uint32_t auxRed = 0;
    uint8_t readPointer = 0;
    uint8_t resolutionFix = 0;
    uint8_t writePointer = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(redValue_p)) && (!isPointerValid(infraredValue_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    switch (this->_activeLedsCounter) {
    case 1:
        channels = 1;
        break;
    case 2 ... 4:
        channels = 2;
        break;
    case 0:
        // Returns error
        this->_lastError = Error::MODE_NOT_SUPPORTED;
        debugMessage(Error::MODE_NOT_SUPPORTED);
        return false;
    }

    // Move FIFO pointers to read only the newest sample
    if (!this->_getFifoWritePointer(&writePointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    readPointer = (writePointer == 0) ? (MAX3010X_FIFO_SIZE_IN_SAMPLES - 1) : (writePointer - 1);
    if (!this->_setFifoReadPointer(readPointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(redValue_p)) {
        auxRed = (uint32_t)auxBuff[0];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[1];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[2];
        auxRed >>= resolutionFix;
    }
    if (isPointerValid(infraredValue_p)) {
        auxInfrared = (uint32_t)auxBuff[3];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[4];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[5];
        auxInfrared >>= resolutionFix;
    }

    // Truncates the sample size to 16 bits
    if (resolutionFix < 2) {
        resolutionFix = 2 - resolutionFix;
        auxRed >>= resolutionFix;
        auxInfrared >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(redValue_p)) {
        *redValue_p = (uint16_t)auxRed;
    }
    if (isPointerValid(infraredValue_p)) {
        *infraredValue_p = (uint16_t)auxInfrared;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the newest sample and converting it to
 *                  a 4-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  redValue_p          pointer to the variable that will hold the
 *                                      red LED channel value.
 * @param[out]  infraredValue_p     pointer to the variable that will hold the
 *                                      infrared LED channel value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoNewestSample(uint32_t *redValue_p, uint32_t *infraredValue_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoNewestSample(uint32_t *, uint32_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxInfrared = 0;
    uint32_t auxRed = 0;
    uint8_t readPointer = 0;
    uint8_t resolutionFix = 0;
    uint8_t writePointer = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(redValue_p)) && (!isPointerValid(infraredValue_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    switch (this->_activeLedsCounter) {
    case 1:
        channels = 1;
        break;
    case 2 ... 4:
        channels = 2;
        break;
    case 0:
        // Returns error
        this->_lastError = Error::MODE_NOT_SUPPORTED;
        debugMessage(Error::MODE_NOT_SUPPORTED);
        return false;
    }

    // Move FIFO pointers to read only the newest sample
    if (!this->_getFifoWritePointer(&writePointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    readPointer = (writePointer == 0) ? (MAX3010X_FIFO_SIZE_IN_SAMPLES - 1) : (writePointer - 1);
    if (!this->_setFifoReadPointer(readPointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(redValue_p)) {
        auxRed = (uint32_t)auxBuff[0];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[1];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[2];
        auxRed >>= resolutionFix;
    }
    if (isPointerValid(infraredValue_p)) {
        auxInfrared = (uint32_t)auxBuff[3];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[4];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[5];
        auxInfrared >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(redValue_p)) {
        *redValue_p = (uint32_t)auxRed;
    }
    if (isPointerValid(infraredValue_p)) {
        *infraredValue_p = (uint32_t)auxInfrared;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the newest sample and converting it to
 *                  a 2-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  led1Value_p_p           pointer to the variable that will hold the
 *                                  channel 1 LED value.
 * @param[out]  led2Value_p         pointer to the variable that will hold the
 *                                  channel 2 LED value.
 * @param[out]  led3Value_p         pointer to the variable that will hold the
 *                                  channel 3 LED value.
 * @param[out]  led4Value_p         pointer to the variable that will hold the
 *                                  channel 4 LED value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoNewestSample(uint16_t *led1Value_p, uint16_t *led2Value_p, uint16_t *led3Value_p,
        uint16_t *led4Value_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoNewestSample(uint16_t *, uint16_t *, uint16_t *, uint16_t *)",
            Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[4 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxLed1Value = 0;
    uint32_t auxLed2Value = 0;
    uint32_t auxLed3Value = 0;
    uint32_t auxLed4Value = 0;
    uint8_t readPointer = 0;
    uint8_t resolutionFix = 0;
    uint8_t writePointer = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(led1Value_p)) && (!isPointerValid(led2Value_p)) && (!isPointerValid(led3Value_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    channels = this->_activeLedsCounter;

    // Move FIFO pointers to read only the newest sample
    if (!this->_getFifoWritePointer(&writePointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    readPointer = (writePointer == 0) ? (MAX3010X_FIFO_SIZE_IN_SAMPLES - 1) : (writePointer - 1);
    if (!this->_setFifoReadPointer(readPointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(led1Value_p)) {
        auxLed1Value = (uint32_t)auxBuff[0];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[1];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[2];
        auxLed1Value >>= resolutionFix;
    }
    if (isPointerValid(led2Value_p)) {
        auxLed2Value = (uint32_t)auxBuff[3];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[4];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[5];
        auxLed2Value >>= resolutionFix;
    }
    if (isPointerValid(led3Value_p)) {
        auxLed3Value = (uint32_t)auxBuff[6];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[7];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[8];
        auxLed3Value >>= resolutionFix;
    }
    if (isPointerValid(led4Value_p)) {
        auxLed4Value = (uint32_t)auxBuff[9];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[10];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[11];
        auxLed4Value >>= resolutionFix;
    }

    // Truncates the sample size to 16 bits
    if (resolutionFix < 2) {
        resolutionFix = 2 - resolutionFix;
        auxLed1Value >>= resolutionFix;
        auxLed2Value >>= resolutionFix;
        auxLed3Value >>= resolutionFix;
        auxLed4Value >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(led1Value_p)) {
        *led1Value_p = (uint16_t)auxLed1Value;
    }
    if (isPointerValid(led2Value_p)) {
        *led2Value_p = (uint16_t)auxLed2Value;
    }
    if (isPointerValid(led3Value_p)) {
        *led3Value_p = (uint16_t)auxLed3Value;
    }
    if (isPointerValid(led4Value_p)) {
        *led4Value_p = (uint16_t)auxLed4Value;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the newest sample and converting it to
 *                  a 4-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  led1Value_p_p           pointer to the variable that will hold the
 *                                  channel 1 LED value.
 * @param[out]  led2Value_p         pointer to the variable that will hold the
 *                                  channel 2 LED value.
 * @param[out]  led3Value_p         pointer to the variable that will hold the
 *                                  channel 3 LED value.
 * @param[out]  led4Value_p         pointer to the variable that will hold the
 *                                  channel 4 LED value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoNewestSample(uint32_t *led1Value_p, uint32_t *led2Value_p, uint32_t *led3Value_p,
        uint32_t *led4Value_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoNewestSample(uint32_t *, uint32_t *, uint32_t *, uint32_t *)",
            Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[4 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxLed1Value = 0;
    uint32_t auxLed2Value = 0;
    uint32_t auxLed3Value = 0;
    uint32_t auxLed4Value = 0;
    uint8_t readPointer = 0;
    uint8_t resolutionFix = 0;
    uint8_t writePointer = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(led1Value_p)) && (!isPointerValid(led2Value_p)) && (!isPointerValid(led3Value_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    channels = this->_activeLedsCounter;

    // Move FIFO pointers to read only the newest sample
    if (!this->_getFifoWritePointer(&writePointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    readPointer = (writePointer == 0) ? (MAX3010X_FIFO_SIZE_IN_SAMPLES - 1) : (writePointer - 1);
    if (!this->_setFifoReadPointer(readPointer)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(led1Value_p)) {
        auxLed1Value = (uint32_t)auxBuff[0];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[1];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[2];
        auxLed1Value >>= resolutionFix;
    }
    if (isPointerValid(led2Value_p)) {
        auxLed2Value = (uint32_t)auxBuff[3];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[4];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[5];
        auxLed2Value >>= resolutionFix;
    }
    if (isPointerValid(led3Value_p)) {
        auxLed3Value = (uint32_t)auxBuff[6];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[7];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[8];
        auxLed3Value >>= resolutionFix;
    }
    if (isPointerValid(led4Value_p)) {
        auxLed4Value = (uint32_t)auxBuff[9];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[10];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[11];
        auxLed4Value >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(led1Value_p)) {
        *led1Value_p = (uint16_t)auxLed1Value;
    }
    if (isPointerValid(led2Value_p)) {
        *led2Value_p = (uint16_t)auxLed2Value;
    }
    if (isPointerValid(led3Value_p)) {
        *led3Value_p = (uint16_t)auxLed3Value;
    }
    if (isPointerValid(led4Value_p)) {
        *led4Value_p = (uint16_t)auxLed4Value;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the oldest sample and converting it to
 *                  a 2-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  redValue_p          pointer to the buffer that will hold the
 *                                      red LED channel values.
 * @param[out]  infraredValue_p     pointer to the buffer that will hold the
 *                                      infrared LED channel values.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoOldestSample(uint16_t *redValue_p, uint16_t *infraredValue_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoOldestSample(uint16_t *, uint16_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxInfrared = 0;
    uint32_t auxRed = 0;
    uint8_t resolutionFix = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(redValue_p)) && (!isPointerValid(infraredValue_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    switch (this->_activeLedsCounter) {
    case 1:
        channels = 1;
        break;
    case 2 ... 4:
        channels = 2;
        break;
    case 0:
        // Returns error
        this->_lastError = Error::MODE_NOT_SUPPORTED;
        debugMessage(Error::MODE_NOT_SUPPORTED);
        return false;
    }

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(redValue_p)) {
        auxRed = (uint32_t)auxBuff[0];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[1];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[2];
        auxRed >>= resolutionFix;
    }
    if (isPointerValid(infraredValue_p)) {
        auxInfrared = (uint32_t)auxBuff[3];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[4];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[5];
        auxInfrared >>= resolutionFix;
    }

    // Truncates the sample size to 16 bits
    if (resolutionFix < 2) {
        resolutionFix = 2 - resolutionFix;
        auxRed >>= resolutionFix;
        auxInfrared >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(redValue_p)) {
        *redValue_p = (uint16_t)auxRed;
    }
    if (isPointerValid(infraredValue_p)) {
        *infraredValue_p = (uint16_t)auxInfrared;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the oldest sample and converting it to
 *                  a 4-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  redValue_p          pointer to the buffer that will hold the
 *                                      red LED channel values.
 * @param[out]  infraredValue_p     pointer to the buffer that will hold the
 *                                      infrared LED channel values.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoOldestSample(uint32_t *redValue_p, uint32_t *infraredValue_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoOldestSample(uint32_t *, uint32_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[6];
    uint32_t auxInfrared = 0;
    uint32_t auxRed = 0;
    uint8_t resolutionFix = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(redValue_p)) && (!isPointerValid(infraredValue_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    switch (this->_activeLedsCounter) {
    case 1:
        channels = 1;
        break;
    case 2 ... 4:
        channels = 2;
        break;
    case 0:
        // Returns error
        this->_lastError = Error::MODE_NOT_SUPPORTED;
        debugMessage(Error::MODE_NOT_SUPPORTED);
        return false;
    }

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(redValue_p)) {
        auxRed = (uint32_t)auxBuff[0];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[1];
        auxRed <<= 8;
        auxRed |= (uint32_t)auxBuff[2];
        auxRed >>= resolutionFix;
    }
    if (isPointerValid(infraredValue_p)) {
        auxInfrared = (uint32_t)auxBuff[3];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[4];
        auxInfrared <<= 8;
        auxInfrared |= (uint32_t)auxBuff[5];
        auxInfrared >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(redValue_p)) {
        *redValue_p = (uint32_t)auxRed;
    }
    if (isPointerValid(infraredValue_p)) {
        *infraredValue_p = (uint32_t)auxInfrared;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the oldest sample and converting it to
 *                  a 2-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  led1Value_p_p       pointer to the variable that will hold the
 *                                      channel 1 LED value.
 * @param[out]  led2Value_p         pointer to the variable that will hold the
 *                                      channel 2 LED value.
 * @param[out]  led3Value_p         pointer to the variable that will hold the
 *                                      channel 3 LED value.
 * @param[out]  led4Value_p         pointer to the variable that will hold the
 *                                      channel 4 LED value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoOldestSample(uint16_t *led1Value_p, uint16_t *led2Value_p, uint16_t *led3Value_p,
        uint16_t *led4Value_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoOldestSample(uint16_t *, uint16_t *, uint16_t *, uint16_t *)",
            Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[4 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxLed1Value = 0;
    uint32_t auxLed2Value = 0;
    uint32_t auxLed3Value = 0;
    uint32_t auxLed4Value = 0;
    uint8_t resolutionFix = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(led1Value_p)) && (!isPointerValid(led2Value_p)) && (!isPointerValid(led3Value_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    channels = this->_activeLedsCounter;

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(led1Value_p)) {
        auxLed1Value = (uint32_t)auxBuff[0];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[1];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[2];
        auxLed1Value >>= resolutionFix;
    }
    if (isPointerValid(led2Value_p)) {
        auxLed2Value = (uint32_t)auxBuff[3];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[4];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[5];
        auxLed2Value >>= resolutionFix;
    }
    if (isPointerValid(led3Value_p)) {
        auxLed3Value = (uint32_t)auxBuff[6];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[7];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[8];
        auxLed3Value >>= resolutionFix;
    }
    if (isPointerValid(led4Value_p)) {
        auxLed4Value = (uint32_t)auxBuff[9];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[10];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[11];
        auxLed4Value >>= resolutionFix;
    }

    // Truncates the sample size to 16 bits
    if (resolutionFix < 2) {
        resolutionFix = 2 - resolutionFix;
        auxLed1Value >>= resolutionFix;
        auxLed2Value >>= resolutionFix;
        auxLed3Value >>= resolutionFix;
        auxLed4Value >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(led1Value_p)) {
        *led1Value_p = (uint16_t)auxLed1Value;
    }
    if (isPointerValid(led2Value_p)) {
        *led2Value_p = (uint16_t)auxLed2Value;
    }
    if (isPointerValid(led3Value_p)) {
        *led3Value_p = (uint16_t)auxLed3Value;
    }
    if (isPointerValid(led4Value_p)) {
        *led4Value_p = (uint16_t)auxLed4Value;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads FIFO data, getting the oldest sample and converting it to
 *                  a 4-bytes per channel.
 * @details     // TODO: Detailed explanation of the FIFO operation
 * @param[out]  led1Value_p_p       pointer to the variable that will hold the
 *                                      channel 1 LED value.
 * @param[out]  led2Value_p         pointer to the variable that will hold the
 *                                      channel 2 LED value.
 * @param[out]  led3Value_p         pointer to the variable that will hold the
 *                                      channel 3 LED value.
 * @param[out]  led4Value_p         pointer to the variable that will hold the
 *                                      channel 4 LED value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getFifoOldestSample(uint32_t *led1Value_p, uint32_t *led2Value_p, uint32_t *led3Value_p,
        uint32_t *led4Value_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getFifoOldestSample(uint32_t *, uint32_t *, uint32_t *, uint32_t *)",
            Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[4 * MAX3010X_BYTES_PER_CHANNEL];
    uint32_t auxLed1Value = 0;
    uint32_t auxLed2Value = 0;
    uint32_t auxLed3Value = 0;
    uint32_t auxLed4Value = 0;
    uint8_t resolutionFix = 0;
    uint8_t channels = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments for errors
    if ((!isPointerValid(led1Value_p)) && (!isPointerValid(led2Value_p)) && (!isPointerValid(led3Value_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    channels = this->_activeLedsCounter;

    // Reads a sample from the FIFO
    if (!this->_read(Register::FIFO_DATA, auxBuff, (MAX3010X_BYTES_PER_CHANNEL * channels))) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Calculates the LED channel values
    if (isPointerValid(led1Value_p)) {
        auxLed1Value = (uint32_t)auxBuff[0];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[1];
        auxLed1Value <<= 8;
        auxLed1Value |= (uint32_t)auxBuff[2];
        auxLed1Value >>= resolutionFix;
    }
    if (isPointerValid(led2Value_p)) {
        auxLed2Value = (uint32_t)auxBuff[3];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[4];
        auxLed2Value <<= 8;
        auxLed2Value |= (uint32_t)auxBuff[5];
        auxLed2Value >>= resolutionFix;
    }
    if (isPointerValid(led3Value_p)) {
        auxLed3Value = (uint32_t)auxBuff[6];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[7];
        auxLed3Value <<= 8;
        auxLed3Value |= (uint32_t)auxBuff[8];
        auxLed3Value >>= resolutionFix;
    }
    if (isPointerValid(led4Value_p)) {
        auxLed4Value = (uint32_t)auxBuff[9];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[10];
        auxLed4Value <<= 8;
        auxLed4Value |= (uint32_t)auxBuff[11];
        auxLed4Value >>= resolutionFix;
    }

    // Update values of the function arguments
    if (isPointerValid(led1Value_p)) {
        *led1Value_p = (uint16_t)auxLed1Value;
    }
    if (isPointerValid(led2Value_p)) {
        *led2Value_p = (uint16_t)auxLed2Value;
    }
    if (isPointerValid(led3Value_p)) {
        *led3Value_p = (uint16_t)auxLed3Value;
    }
    if (isPointerValid(led4Value_p)) {
        *led4Value_p = (uint16_t)auxLed4Value;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     /////////////////    INTERRUPT CALLBACKS     /////////////////     //

/**
 * @brief
 * @details
 * @param[in]   callbackFunction_p
 * @return      true
 * @return      false
 */
bool Max3010x::setIntAmbLightCancOverflowCallback(void (* callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setIntAmbLightCancOverflowCallback(void (*)(void))", Debug::CodeIndex::MAX3010X_MODULE);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intAmbLightCancOverflowCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @param[in]   callbackFunction_p
 * @return      true
 * @return      false
 */
bool Max3010x::setIntDieTempReadyCallback(void (* callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setIntDieTempReadyCallback(void (*)(void))", Debug::CodeIndex::MAX3010X_MODULE);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intDieTempReadyCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @param[in]   callbackFunction_p
 * @return      true
 * @return      false
 */
bool Max3010x::setIntFifoAlmostFullCallback(void (* callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setIntFifoAlmostFullCallback(void (*)(void))", Debug::CodeIndex::MAX3010X_MODULE);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intFifoAlmostFullCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @param[in]   callbackFunction_p
 * @return      true
 * @return      false
 */
bool Max3010x::setIntNewFifoDataReadyCallback(void (* callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setIntNewFifoDataReadyCallback(void (*)(void))", Debug::CodeIndex::MAX3010X_MODULE);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intNewFifoDataReadyCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @param[in]   callbackFunction_p
 * @return      true
 * @return      false
 */
bool Max3010x::setIntPowerReadyCallback(void (* callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setIntPowerReadyCallback(void (*)(void))", Debug::CodeIndex::MAX3010X_MODULE);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intPowerReadyCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @param[in]   callbackFunction_p
 * @return      true
 * @return      false
 */
bool Max3010x::setIntProximityCallback(void (* callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setIntProximityCallback(void (*)(void))", Debug::CodeIndex::MAX3010X_MODULE);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intProximityCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}


//     ////////////////////    INTERRUPT CONTROL     ////////////////////     //

/**
 * @brief       Disables the interrupt pin to relay an internal interrupt.
 * @details     Disables the interrupt pin to relay an internal interrupt.
 *              // TODO: Explain the operation of the deactivation mask
 * @note        Since its not possible to turn on or off the power on interrupt,
 *                  the function just ignores it from the interrupt mask.
 * @param[in]   deactivationMask_p  interrupts to be disabled.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::disableInterrupts(InterruptFlags deactivationMask_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::disableInterrupts(InterruptFlags)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];
    bool intAmbLightCancOverflow        = false;
    bool intDieTempReady                = false;
    bool intFifoAlmostFull              = false;
    bool intNewFifoDataReady            = false;
    bool intProximity                   = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Populate the variables with the class members values
    intAmbLightCancOverflow = this->_intAmbLightCancOverflowEnable;
    intDieTempReady = this->_intDieTempReadyEnable;
    intFifoAlmostFull = this->_intFifoAlmostFullEnable;
    intNewFifoDataReady = this->_intNewFifoDataReadyEnable;
    intProximity = this->_intProximityEnable;

    // Process the deactivation mask
    if (deactivationMask_p & InterruptFlags::AMBIENT_LIGHT_CANCELLATION) {
        intAmbLightCancOverflow = false;
    }
    if (deactivationMask_p & InterruptFlags::DIE_TEMPERATURE_READY) {
        intDieTempReady  = false;
    }
    if (deactivationMask_p & InterruptFlags::FIFO_ALMOST_FULL) {
        intFifoAlmostFull = false;
    }
    if (deactivationMask_p & InterruptFlags::NEW_FIFO_DATA_READY) {
        intNewFifoDataReady = false;
    }
    if (deactivationMask_p & InterruptFlags::PROXIMITY_INTERRUPT) {
        intProximity  = false;
    }

    // Configure device
    auxBuff[0] = (
                    (intFifoAlmostFull << Bit::A_FULL_EN) |
                    (intNewFifoDataReady << Bit::DATA_RDY_EN) |
                    (intAmbLightCancOverflow << Bit::ALC_OVF_EN) |
                    (intProximity << Bit::PROX_INT_EN)
            );
    auxBuff[1] = (intDieTempReady << Bit::DIE_TEMP_RDY_EN);
    if (!this->_write(Register::INTERRUPT_ENABLE_1, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update class data members
    this->_intAmbLightCancOverflowEnable = intAmbLightCancOverflow;
    this->_intDieTempReadyEnable = intDieTempReady;
    this->_intFifoAlmostFullEnable = intFifoAlmostFull;
    this->_intNewFifoDataReadyEnable = intNewFifoDataReady;
    this->_intProximityEnable = intProximity;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Enables the interrupt pin to relay an internal interrupt.
 * @details     Enables the interrupt pin to relay an internal interrupt.
 *              // TODO: Explain the operation of the activation mask
 * @note        Since its not possible to turn on or off the power on interrupt,
 *                  the function just ignores it from the interrupt mask.
 * @param[in]   activationMask_p    interrupts to be enabled.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::enableInterrupts(InterruptFlags activationMask_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::enableInterrupts(InterruptFlags)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];
    bool intAmbLightCancOverflow        = false;
    bool intDieTempReady                = false;
    bool intFifoAlmostFull              = false;
    bool intNewFifoDataReady            = false;
    bool intProximity                   = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Populate the variables with the class members values
    intAmbLightCancOverflow = this->_intAmbLightCancOverflowEnable;
    intDieTempReady = this->_intDieTempReadyEnable;
    intFifoAlmostFull = this->_intFifoAlmostFullEnable;
    intNewFifoDataReady = this->_intNewFifoDataReadyEnable;
    intProximity = this->_intProximityEnable;

    // Process the activation mask
    if (activationMask_p & InterruptFlags::AMBIENT_LIGHT_CANCELLATION) {
        intAmbLightCancOverflow = true;
    }
    if (activationMask_p & InterruptFlags::DIE_TEMPERATURE_READY) {
        intDieTempReady  = true;
    }
    if (activationMask_p & InterruptFlags::FIFO_ALMOST_FULL) {
        intFifoAlmostFull = true;
    }
    if (activationMask_p & InterruptFlags::NEW_FIFO_DATA_READY) {
        intNewFifoDataReady = true;
    }
    if (this->_partId == PartId::MAX30105) {
        if (activationMask_p & InterruptFlags::PROXIMITY_INTERRUPT) {
            intProximity  = true;
        }
    } else {
        intProximity  = false;
    }

    // Configure device
    auxBuff[0] = (
                    (intFifoAlmostFull << Bit::A_FULL_EN) |
                    (intNewFifoDataReady << Bit::DATA_RDY_EN) |
                    (intAmbLightCancOverflow << Bit::ALC_OVF_EN) |
                    (intProximity << Bit::PROX_INT_EN)
            );
    auxBuff[1] = (intDieTempReady << Bit::DIE_TEMP_RDY_EN);
    if (!this->_write(Register::INTERRUPT_ENABLE_1, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update class data members
    this->_intAmbLightCancOverflowEnable = intAmbLightCancOverflow;
    this->_intDieTempReadyEnable = intDieTempReady;
    this->_intFifoAlmostFullEnable = intFifoAlmostFull;
    this->_intNewFifoDataReadyEnable = intNewFifoDataReady;
    this->_intProximityEnable = intProximity;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ////////////////////     INTERRUPT STATUS     ////////////////////     //

/**
 * @brief
 * @details
 * @return  true
 * @return  false
 */
bool Max3010x::generalIrqHandler(void)
{
    // Note: Since this function is called asynchronously, no debug mark passage
    // was placed for caution. The same happens with the updateInterruptStatus()
    // function that is called inside this function.

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Read the interrupt status registers
    if (!this->_updateInterruptStatus()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Call callback functions
    if (this->_intAmbLightCancOverflowStatus) {
        this->_intAmbLightCancOverflowCallback();
        this->_intAmbLightCancOverflowStatus = false;
    }
    if (this->_intDieTempReadyStatus) {
        this->_intDieTempReadyCallback();
        this->_intDieTempReadyStatus = false;
    }
    if (this->_intFifoAlmostFullStatus) {
        this->_intFifoAlmostFullCallback();
        this->_intFifoAlmostFullStatus = false;
    }
    if (this->_intNewFifoDataReadyStatus) {
        this->_intNewFifoDataReadyCallback();
        this->_intNewFifoDataReadyStatus = false;
    }
    if (this->_intPowerReadyStatus) {
        this->_intPowerReadyCallback();
        this->_intPowerReadyStatus = false;
    }
    if (this->_intProximityStatus) {
        this->_intProximityCallback();
        this->_intProximityStatus = false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////////    LED CURRENT     ///////////////////////     //

/**
 * @brief       Configures the green LED's amplitude.
 * @details     Configures the green LED's amplitude of the device.
 *              The LED's current amplitude can be programmed from 0 to 51 mA with proper
 *                  supply voltage.
 * @note        Actual measured LED's current amplitude for each part can vary widely due to
 *                  the trimming methodology.
 * @param[in]   infraredLedCurrent_p        LED's current amplitude value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setGreenLedCurrent(uint8_t greenLed1Current_p, uint8_t greenLed2Current_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setPilotLedCurrent(uint8_t, uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];
    uint8_t auxSize = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check part ID
    if (this->_partId == PartId::MAX30102) {
        // Returns error
        this->_lastError = Error::FEATURE_NOT_SUPPORTED;
        debugMessage(Error::FEATURE_NOT_SUPPORTED);
        return false;
    }
    if ((this->_partId == PartId::MAX30105) && (greenLed2Current_p != 0)) {
        // Returns error
        this->_lastError = Error::FEATURE_NOT_SUPPORTED;
        debugMessage(Error::FEATURE_NOT_SUPPORTED);
        return false;
    }
    // Check function arguments
    if ((greenLed1Current_p > 51) || (greenLed2Current_p > 51)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }

    // Process function arguments
    greenLed1Current_p *= 5;            // Transform to register value
    greenLed2Current_p *= 5;            // Transform to register value

    // Configure LEDs
    auxBuff[0] = (greenLed1Current_p << Bit::LED3_PA);
    if (this->_partId == PartId::MAX30101) {
        auxBuff[1] = (greenLed2Current_p << Bit::LED4_PA);
        auxSize = 2;
    } else {
        auxSize = 1;
    }

    if (!this->_write(Register::LED3_PULSE_AMPLITUDE, auxBuff, auxSize)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_led3Current = greenLed1Current_p;
    this->_led3Current = greenLed2Current_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the infrared LED's amplitude.
 * @details     Configures the infrared LED's amplitude of the device.
 *              The LED's current amplitude can be programmed from 0 to 51 mA with proper
 *                  supply voltage.
 * @note        Actual measured LED's current amplitude for each part can vary widely due to
 *                  the trimming methodology.
 * @param[in]   infraredLedCurrent_p        LED's current amplitude value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setInfraredLedCurrent(uint8_t infraredLedCurrent_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setInfraredLedCurrent(uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments
    if (infraredLedCurrent_p > 51) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }

    // Process function arguments
    infraredLedCurrent_p *= 5;          // Transform to register value

    // Configure LEDs
    auxBuff = (infraredLedCurrent_p << Bit::LED2_PA);
    if (!this->_write(Register::LED2_PULSE_AMPLITUDE, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_led2Current = infraredLedCurrent_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the pilot LED's amplitude.
 * @details     Configures the pilot LED's amplitude of the device. The
 *              LED's current amplitude can be programmed from 0 to 51 mA with proper
 *                  supply voltage.
 * @note        Actual measured LED's current amplitude for each part can vary widely due to
 *                  the trimming methodology.
 * @param[in]   redLedCurrent_p     LED's current amplitude value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setPilotLedCurrent(uint8_t pilotLedCurrent_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setPilotLedCurrent(uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check part ID
    if (this->_partId != PartId::MAX30105) {
        // Returns error
        this->_lastError = Error::FEATURE_NOT_SUPPORTED;
        debugMessage(Error::FEATURE_NOT_SUPPORTED);
        return false;
    }
    // Check function arguments
    if (pilotLedCurrent_p > 51) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }

    // Process function arguments
    pilotLedCurrent_p *= 5;             // Transform to register value

    // Configure LEDs
    auxBuff = (pilotLedCurrent_p << Bit::PILOT_PA);
    if (!this->_write(Register::PROXIMITY_LED_AMPLITUDE, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_pilotCurrent = pilotLedCurrent_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Configures the red LED's amplitude.
 * @details     Configures the red LED's amplitude of the device. The
 *              LED's current amplitude can be programmed from 0 to 51 mA with proper
 *                  supply voltage.
 * @note        Actual measured LED's current amplitude for each part can vary widely due to
 *                  the trimming methodology.
 * @param[in]   redLedCurrent_p     LED's current amplitude value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::setRedLedCurrent(uint8_t redLedCurrent_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::setRedLedCurrent(uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // Check function arguments
    if (redLedCurrent_p > 51) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    }

    // Process function arguments
    redLedCurrent_p *= 5;               // Transform to register value

    // Configure LEDs
    auxBuff = (redLedCurrent_p << Bit::LED1_PA);
    if (!this->_write(Register::LED1_PULSE_AMPLITUDE, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_led1Current = redLedCurrent_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ////////////////////     TEMPERATURE DATA     ////////////////////     //

/**
 * @brief       Reads internal sensor internal temperature.
 * @details     // TODO: Detailed explanation of the sensor internal temperature
 * @param[out]  tempValue_p     pointer to the temperature value variable.
 * @param[in]   tempScale_p     to which thermometric scale the value must be
 *                                  converted.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getTemperature(float *tempValue_p, ThermometricScale tempScale_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getTemperature(float *, ThermometricScale)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];
    float auxFloat = 0.0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(tempValue_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads die temperature register
    if (!this->_read(Register::DIE_TEMPERATURE_INTEGER, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Calculates temperature
    // FIXME: Implement a better algorithm using the fact that (1/16 = 0.0625 = 1 >> 4)
    auxFloat = (float)auxBuff[0] + ((float)auxBuff[1] / 16);

    switch (tempScale_p) {
    case ThermometricScale::CELSIUS:
        *tempValue_p = auxFloat;
        break;
    case ThermometricScale::DELISLE:
    case ThermometricScale::FAHRENHEIT:
    case ThermometricScale::KELVIN:
    case ThermometricScale::NEWTON:
    case ThermometricScale::RANKINE:
    case ThermometricScale::REAUMUR:
    case ThermometricScale::ROMER:
        // TODO: Implement the conversion between thermometric scales
        // Returns error
        this->_lastError = Error::UNDER_DEVELOPMENT;
        debugMessage(Error::UNDER_DEVELOPMENT);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads internal sensor internal temperature.
 * @details     // TODO: Detailed explanation of the sensor internal temperature
 * @param[out]  tempInteger_p
 * @param[out]  tempFrac_p
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::getTemperature(int8_t *tempInteger_p, uint8_t *tempFrac_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::getTemperature(int8_t *, uint8_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[2];

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if ((!isPointerValid(tempInteger_p)) || (!isPointerValid(tempFrac_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads die temperature register
    if (!this->_read(Register::DIE_TEMPERATURE_INTEGER, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns temperature
    *tempInteger_p = (int8_t)auxBuff[0];
    *tempFrac_p = (uint8_t)auxBuff[1];

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @return true
 * @return false
 */
bool Max3010x::startTemperatureRead(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::startTemperatureRead(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Starts a temperature acquisiton
    auxBuff = (1 << Bit::TEMP_EN);
    if (!this->_write(Register::DIE_TEMPERATURE_CONFIG, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update class data members
    this->_dieTempEnable = true;
    this->_dieTempInteger = 0;
    this->_dieTempFraction = 0;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     /////////////////     CONTROL AND STATUS     /////////////////     //

/**
 * @brief       Clears class internal data.
 * @details     Clears class internal data related to device's internal
 *                  registers. The other class members may be unaffected.
 */
void Max3010x::_clearData(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_clearData(void)", Debug::CodeIndex::MAX3010X_MODULE);

    //     //////////////////     DEVICE OPERATION     //////////////////     //
    this->_fullScale                            = FullScale::FSCAL_2048_NA;
    this->_modeControl                          = Mode::NO_MODE;
    this->_resolution                           = Resolution::RES_15_BITS;
    this->_sampleRate                           = SampleRate::RATE_50_HZ;
    //     ////////////////////    DEVICE STATUS     ////////////////////     //
    this->_modeSelected                         = false;
    this->_shuttedDown                          = false;
    //     ///////////////    FIFO CONTROL AND STATUS     ///////////////     //
    this->_activeLedsCounter                    = 0;
    this->_fifoAlmostFullValue                  = 0;
    this->_fifoOverflowCounter                  = 0;
    this->_fifoReadPointer                      = 0;
    this->_fifoRolloverOnFull                   = false;
    this->_fifoWritePointer                     = 0;
    this->_sampleAveraging                      = Averaging::AVER_OFF;
    //     //////////////////    INTERRUPT CONTROL     //////////////////     //
    this->_intAmbLightCancOverflowEnable        = false;
    this->_intDieTempReadyEnable                = false;
    this->_intFifoAlmostFullEnable              = false;
    this->_intNewFifoDataReadyEnable            = false;
    this->_intProximityEnable                   = false;
    //     //////////////////     INTERRUPT STATUS     //////////////////     //
    this->_intAmbLightCancOverflowStatus        = false;
    this->_intDieTempReadyStatus                = false;
    this->_intFifoAlmostFullStatus              = false;
    this->_intNewFifoDataReadyStatus            = false;
    this->_intPowerReadyStatus                  = false;
    this->_intProximityStatus                   = false;
    //     /////////////////////    LED CURRENT     /////////////////////     //
    this->_led1Current                          = 0;
    this->_led2Current                          = 0;
    this->_led3Current                          = 0;
    this->_led4Current                          = 0;
    this->_pilotCurrent                         = 0;
    //     ////////////////////     SLOT CONTROL     ////////////////////     //
    this->_slot1                                = Slot::SLOT_OFF;
    this->_slot2                                = Slot::SLOT_OFF;
    this->_slot3                                = Slot::SLOT_OFF;
    this->_slot4                                = Slot::SLOT_OFF;
    this->_slotsCounter                         = 0;
    //     //////////////////     TEMPERATURE DATA     //////////////////     //
    this->_dieTempEnable                        = false;
    this->_dieTempFraction                      = 0;
    this->_dieTempInteger                       = 0;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief       Checks if the device was initialized.
 * @details     Checks if the device was initialized.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @attention   This function only verifies the device initialization, it does
 *                  not verifies if the device is configured properly.
 * @return      true        Device is initialized.
 * @return      false       Device is not initialized.
 */
bool Max3010x::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_isInitialized(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Checks if device is initialized
    if (!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 * @details
 * @return      true
 * @return      false
 */
bool Max3010x::_reset(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_reset(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Sends reset signal to device
    auxBuff = (1 << Bit::RESET);
    if (!this->_write(Register::MODE_CONFIGURATION, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    delayMs(100);

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////////     INTERRUPTS     ///////////////////////     //

/**
 * @brief       Reads the sensor interrupt status.
 * @details     Reads the sensor interrupt status.
 *              // TODO: Explain the operation of the interrupt
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_updateInterruptStatus(void)
{
    // Local variables
    uint8_t auxBuff[2];

    // Note: This function is also called by the generalIrqHandler() function.
    // Since the generalIrqHandler() function is called asynchronously,
    // triggered by interrupts, no debug mark passage was placed for caution.

    // Read the interrupt status registers
    if (!this->_read(Register::INTERRUPT_STATUS_1, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update class data members
    if (this->_intAmbLightCancOverflowEnable) {
        this->_intAmbLightCancOverflowStatus = (bool)(auxBuff[0] & (1 << Bit::ALC_OVF_INT));
    }
    if (this->_intFifoAlmostFullEnable) {
        this->_intFifoAlmostFullStatus = (bool)(auxBuff[0] & (1 << Bit::A_FULL_INT));
    }
    if (this->_intNewFifoDataReadyEnable) {
        this->_intNewFifoDataReadyStatus = (bool)(auxBuff[0] & (1 << Bit::DATA_RDY_INT));
    }
    if (this->_intProximityEnable) {
        this->_intProximityStatus = (bool)(auxBuff[0] & (1 << Bit::PROX_INT));
    }
    if (this->_intDieTempReadyEnable) {
        this->_intDieTempReadyStatus = (bool)(auxBuff[1] & (1 << Bit::DIE_TEMP_RDY_INT));
    }
    this->_intPowerReadyStatus = (bool)(auxBuff[0] & (1 << Bit::PWR_RDY_INT));

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     //////////////////    REGISTER MANIPULATION     //////////////////     //

/**
 * @brief       Reads registers from the device.
 * @details     Reads registers from the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @param[in]   regAddr_p       device's register addess.
 * @param[out]  dataBuffer_p    pointer to buffer.
 * @param[in]   bytesToRead_p   number of bytes to read from the device.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_read(Register regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToRead_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_read(Register, uint8_t *, uint16_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Selects device
    if (!this->_busHandler->setDevice(MAX3010X_DEVICE_ADDRESS)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Read data from device
    if (!this->_busHandler->readReg((uint8_t)regAddr_p, dataBuffer_p, bytesToRead_p)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Writes registers at the device.
 * @details     Writes registers at the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @param[in]   regAddr_p       device's register addess.
 * @param[out]  dataBuffer_p    pointer to buffer.
 * @param[in]   bytesToWrite_p  number of bytes to write at the device.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_write(Register regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToWrite_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_write(Register, uint8_t *, uint16_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Selects device
    if (!this->_busHandler->setDevice(MAX3010X_DEVICE_ADDRESS)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Read data from device
    if (!this->_busHandler->writeReg((uint8_t)regAddr_p, dataBuffer_p, bytesToWrite_p)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     /////////////////    FIFO CONTROL AND STATUS     /////////////////     //

/**
 * @brief       Gets FIFO status registers from the device.
 * @details     Gets FIFO status registers from the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_getFifoInfo(void)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_getFifoInfo(void)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff[3];

    // Reads from device
    if (!this->_read(Register::FIFO_WRITE_POINTER, auxBuff, 3)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoWritePointer = auxBuff[0];
    this->_fifoOverflowCounter = auxBuff[1];
    this->_fifoReadPointer = auxBuff[2];

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Gets the FIFO's read pointer from the device.
 * @details     Gets the FIFO's read pointer from the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @param[out]  pointer_p       pointer to variable to store the value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_getFifoReadPointer(uint8_t *pointer_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_getFifoReadPointer(uint8_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Reads from device
    if (!this->_read(Register::FIFO_READ_POINTER, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoReadPointer = auxBuff;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Gets the FIFO's write pointer from the device.
 * @details     Gets the FIFO's write pointer from the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @param[out]  pointer_p       pointer to variable to store the value.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_getFifoWritePointer(uint8_t *pointer_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_getFifoWritePointer(uint8_t *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Reads from device
    if (!this->_read(Register::FIFO_WRITE_POINTER, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoWritePointer = auxBuff;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Reads data from device's FIFO.
 * @details     Reads data from device's FIFO.
 * @note        This function is private, so before calling it, the
 *                  programmer must ensure that all parameters are checked.
 * @param[in]   samples_p   number of samples to be read from FIFO.
 * @param[out]  buff1       buffer to red LED values.
 * @param[out]  buff2       buffer to infrared LED values.
 * @param[in]   varSize     size of the variable to be stored.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_readFifoData(uint8_t samples_p, void *led1Buffer_p, void *led2Buffer_p, void *led3Buffer_p,
        void *led4Buffer_p, uint8_t varSize_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_readFifoData(uint8_t, void *, void*, void*, void*, uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t *auxBuff = nullptr;
    uint8_t resolutionFix = 0;
    uint16_t bufferSizeinBytes = 0;
    uint16_t counter = 0;

    int32_t aux32 = 0;
    uint16_t *buff16ptr1 = (uint16_t *)led1Buffer_p;
    uint16_t *buff16ptr2 = (uint16_t *)led2Buffer_p;
    uint16_t *buff16ptr3 = (uint16_t *)led3Buffer_p;
    uint16_t *buff16ptr4 = (uint16_t *)led4Buffer_p;
    uint32_t *buff32ptr1 = (uint32_t *)led1Buffer_p;
    uint32_t *buff32ptr2 = (uint32_t *)led2Buffer_p;
    uint32_t *buff32ptr3 = (uint32_t *)led3Buffer_p;
    uint32_t *buff32ptr4 = (uint32_t *)led4Buffer_p;

    // Evaluate buffer size
    switch (this->_modeControl) {
    case Mode::HEART_RATE_MODE:
        bufferSizeinBytes = MAX3010X_BYTES_PER_CHANNEL * samples_p * 1;
        break;
    case Mode::SPO2_MODE:
        bufferSizeinBytes = MAX3010X_BYTES_PER_CHANNEL * samples_p * 2;
        break;
    case Mode::MULTI_LED_MODE:
        bufferSizeinBytes = MAX3010X_BYTES_PER_CHANNEL * samples_p * this->_activeLedsCounter;
        break;
    default:
        // Returns error
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return false;
    }

    // Memory allocation
    auxBuff = (uint8_t *)calloc(bufferSizeinBytes, sizeof(uint8_t));
    if (!isPointerValid(auxBuff)) {
        // Returns error
        this->_lastError = Error::MEMORY_ALLOCATION_FAILED;
        debugMessage(Error::MEMORY_ALLOCATION_FAILED);
        return false;
    }

    // Reads from device
    if (!this->_read(Register::FIFO_DATA, auxBuff, bufferSizeinBytes)) {
        // Returns error
        free(auxBuff);
        debugMessage(this->_lastError);
        return false;
    }

    // Evaluates the number of shifts to fix the value due to resolution
    switch (this->_resolution) {
    case Resolution::RES_15_BITS:
        resolutionFix = 3;
        break;
    case Resolution::RES_16_BITS:
        resolutionFix = 2;
        break;
    case Resolution::RES_17_BITS:
        resolutionFix = 1;
        break;
    case Resolution::RES_18_BITS:
        resolutionFix = 0;
        break;
    }

    // Decode data
    counter = 0;
    for (uint8_t i = 0; i < samples_p; i++) {
        switch (this->_modeControl) {
        case Mode::HEART_RATE_MODE:
            // Process red LED value
            aux32 = (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 >>= resolutionFix;
            // Truncates the sample size to 16 bits
            if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                aux32 >>= (2 - resolutionFix);
            }
            // Update function arguments
            if (isPointerValid(led1Buffer_p)) {
                if (varSize_p == sizeof(uint16_t)) {
                    *buff16ptr1++ = (uint16_t)(aux32);
                } else {
                    *buff32ptr1++ = (uint32_t)(aux32);
                }
            }
            break;

        case Mode::SPO2_MODE:
            // Process red LED value
            aux32 = (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 >>= resolutionFix;
            // Truncates the sample size to 16 bits
            if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                aux32 >>= (2 - resolutionFix);
            }
            // Update function arguments
            if (isPointerValid(led1Buffer_p)) {
                if (varSize_p == sizeof(uint16_t)) {
                    *buff16ptr1++ = (uint16_t)(aux32);
                } else {
                    *buff32ptr1++ = (uint32_t)(aux32);
                }
            }
            // Process infrared LED value
            aux32 = (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 >>= resolutionFix;
            // Truncates the sample size to 16 bits
            if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                aux32 >>= (2 - resolutionFix);
            }
            // Update function arguments
            if (isPointerValid(led2Buffer_p)) {
                if (varSize_p == sizeof(uint16_t)) {
                    *buff16ptr2++ = (uint16_t)(aux32);
                } else {
                    *buff32ptr2++ = (uint32_t)(aux32);
                }
            }
            break;

        case Mode::MULTI_LED_MODE:
            // Process first LED value
            aux32 = (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 <<= 8;
            aux32 |= (uint32_t)(auxBuff[counter++]);
            aux32 >>= resolutionFix;
            // Truncates the sample size to 16 bits
            if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                aux32 >>= (2 - resolutionFix);
            }
            // Update function arguments
            if (isPointerValid(led1Buffer_p)) {
                if (varSize_p == sizeof(uint16_t)) {
                    *buff16ptr1++ = (uint16_t)(aux32);
                } else {
                    *buff32ptr1++ = (uint32_t)(aux32);
                }
            }
            // Process second LED value
            if (this->_activeLedsCounter > 1) {
                aux32 = (uint32_t)(auxBuff[counter++]);
                aux32 <<= 8;
                aux32 |= (uint32_t)(auxBuff[counter++]);
                aux32 <<= 8;
                aux32 |= (uint32_t)(auxBuff[counter++]);
                aux32 >>= resolutionFix;
                // Truncates the sample size to 16 bits
                if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                    aux32 >>= (2 - resolutionFix);
                }
                // Update function arguments
                if (isPointerValid(led2Buffer_p)) {
                    if (varSize_p == sizeof(uint16_t)) {
                        *buff16ptr2++ = (uint16_t)(aux32);
                    } else {
                        *buff32ptr2++ = (uint32_t)(aux32);
                    }
                }
            }
            // Process third LED value
            if (this->_activeLedsCounter > 2) {
                aux32 = (uint32_t)(auxBuff[counter++]);
                aux32 <<= 8;
                aux32 |= (uint32_t)(auxBuff[counter++]);
                aux32 <<= 8;
                aux32 |= (uint32_t)(auxBuff[counter++]);
                aux32 >>= resolutionFix;
                // Truncates the sample size to 16 bits
                if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                    aux32 >>= (2 - resolutionFix);
                }
                // Update function arguments
                if (isPointerValid(led3Buffer_p)) {
                    if (varSize_p == sizeof(uint16_t)) {
                        *buff16ptr3++ = (uint16_t)(aux32);
                    } else {
                        *buff32ptr3++ = (uint32_t)(aux32);
                    }
                }
            }
            // Process fouth LED value
            if (this->_activeLedsCounter == 4) {
                aux32 = (uint32_t)(auxBuff[counter++]);
                aux32 <<= 8;
                aux32 |= (uint32_t)(auxBuff[counter++]);
                aux32 <<= 8;
                aux32 |= (uint32_t)(auxBuff[counter++]);
                aux32 >>= resolutionFix;
                // Truncates the sample size to 16 bits
                if ((resolutionFix < 2) && (varSize_p == sizeof(uint16_t))) {
                    aux32 >>= (2 - resolutionFix);
                }
                // Update function arguments
                if (isPointerValid(led4Buffer_p)) {
                    if (varSize_p == sizeof(uint16_t)) {
                        *buff16ptr4++ = (uint16_t)(aux32);
                    } else {
                        *buff32ptr4++ = (uint32_t)(aux32);
                    }
                }
            }
            break;
        }
    }
    free(auxBuff);

    // Update data members
    if (!this->_getFifoInfo()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Sets the FIFO's read pointer at the device.
 * @details     Sets the FIFO's read pointer at the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @param[in]   pointer_p       value to be stored into the register.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_setFifoReadPointer(uint8_t pointer_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_setFifoReadPointer(uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Writes at device
    auxBuff = (pointer_p << Bit::FIFO_RD_PTR);
    if (!this->_write(Register::FIFO_READ_POINTER, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoReadPointer = auxBuff;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief       Sets the FIFO's write pointer at the device.
 * @details     Sets the FIFO's write pointer at the device.
 * @note        This function is private, so before calling it, the programmer
 *                  must ensure that all parameters are checked.
 * @param[in]   pointer_p       value to be stored into the register.
 * @return      true        Operation successfull.
 * @return      false       Operation failed. Retrieve error calling
 *                              getLastError() function.
 */
bool Max3010x::_setFifoWritePointer(uint8_t pointer_p)
{
    // Mark passage for debugging purpose
    debugMark("Max3010x::_setFifoWritePointer(uint8_t)", Debug::CodeIndex::MAX3010X_MODULE);

    // Local variables
    uint8_t auxBuff;

    // Writes at device
    auxBuff = (pointer_p << Bit::FIFO_WR_PTR);
    if (!this->_write(Register::FIFO_WRITE_POINTER, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_fifoWritePointer = auxBuff;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////     TEMPORARY - USED FOR DEBUG     ///////////////     //

bool Max3010x::readAllRegs(RegisterMap *regMap_p)
{
    uint8_t auxBuff[11];

    debugMark("Max3010x::readAllRegs(RegisterMap *)", Debug::CodeIndex::MAX3010X_MODULE);

    // Reading register 0x00 to 0x06
    if (!this->_read(Register::INTERRUPT_STATUS_1, auxBuff, 7)) {
        // Retruns error
        debugMessage(this->_lastError);
        return false;
    }
    regMap_p->interruptStatus1      = auxBuff[0];       // Int status 1
    regMap_p->interruptStatus2      = auxBuff[1];       // Int status 2
    regMap_p->interruptEnable1      = auxBuff[2];       // Int enable 1
    regMap_p->interruptEnable2      = auxBuff[3];       // Int enable 2
    regMap_p->fifoWritePointer      = auxBuff[4];       // Fifo write
    regMap_p->fifoOverflowCounter   = auxBuff[5];       // Fifo overflow
    regMap_p->fifoReadPointer       = auxBuff[6];       // Fifo read

    // Reading register 0x08 to 0x12
    if (!this->_read(Register::FIFO_CONFIGURATION, auxBuff, 11)) {
        // Retruns error
        debugMessage(this->_lastError);
        return false;
    }
    regMap_p->fifoConfiguration         = auxBuff[0];   // Fifo configuration
    regMap_p->modeConfiguration         = auxBuff[1];   // Mode configuration
    regMap_p->spo2Configuration         = auxBuff[2];   // SpO2 configuration
    // RESERVED                                         // RESERVED
    regMap_p->led1PulseAmplitude        = auxBuff[4];   // Pulse amplitude LED 1
    regMap_p->led2PulseAmplitude        = auxBuff[5];   // Pulse amplitude LED 2
    regMap_p->led3PulseAmplitude        = auxBuff[6];   // Pulse amplitude LED 3
    regMap_p->led4PulseAmplitude        = auxBuff[7];   // Pulse amplitude LED 4
    regMap_p->pilotPulseAmplitude       = auxBuff[8];   // Pilot LED amplitude
    regMap_p->multiLedModeControl1      = auxBuff[9];   // Multi LED control 1
    regMap_p->multiLedModeControl2      = auxBuff[10];  // Multi LED control 2

    // Reading register 0x1F to 0x21
    if (!this->_read(Register::DIE_TEMPERATURE_INTEGER, auxBuff, 3)) {
        // Retruns error
        debugMessage(this->_lastError);
        return false;
    }
    regMap_p->dieTemperatureInteger     = auxBuff[0];   // Temperature integer
    regMap_p->dieTemperatureFraction    = auxBuff[1];   // Temperature fraction
    regMap_p->dieTemperatureConfig      = auxBuff[2];   // Temperature config

    // Reading register 0x30
    if (!this->_read(Register::PROXIMITY_INTERRUPT_THRESHOLD, auxBuff, 1)) {
        // Retruns error
        debugMessage(this->_lastError);
        return false;
    }
    regMap_p->proximityThreshold        = auxBuff[0];   // Proximity threshold

    // Reading register 0xFE
    if (!this->_read(Register::REVISION_ID, auxBuff, 2)) {
        // Retruns error
        debugMessage(this->_lastError);
        return false;
    }
    regMap_p->revisionId                = auxBuff[0];   // Revision ID
    regMap_p->partId                    = auxBuff[1];   // Part ID

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

/**
 * @brief
 * // TODO: document this function
 */
static void defaultCallbackFunction(void)
{
    return;
}

// =============================================================================
// END OF FILE
// =============================================================================
