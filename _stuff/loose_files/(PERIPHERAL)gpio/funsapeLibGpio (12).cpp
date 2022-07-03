/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibGpio.cpp
 * Module:          GPIO Peripheral Module for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#   error [funsapeLibGpio.cpp] Error - Header file (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_NUMBER__
#   error [funsapeLibGpio.cpp] Error - Build mismatch between source (funsapeLibGpio.cpp) and header (funsapeLibGpio.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

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

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Gpio:: Gpio object
 *
 * Detailed description of this element.
 *
 */
Gpio::Gpio(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::Gpio(void)", DEBUG_CODE_GPIO);

    // Resets data members
    this->_clearData();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Construct a new Gpio:: Gpio object
 *
 * Detailed description of this element.
 *
 * @param[in] gpioPort_p description of this parameter
 * @param[in] gpioPin_p description of this parameter
 */
Gpio::Gpio(GPIO_TypeDef *gpioPort_p, cuint16_t gpioPin_p)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::Gpio(GPIO_TypeDef *, cuint16_t)", DEBUG_CODE_GPIO);

    // Deinitialize peripheral
    if (this->_initialized) {
        this->_deinitialize();
    }

    // Initialize GPIO
    if (!this->_initialize(gpioPort_p, gpioPin_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Gpio:: Gpio object
 *
 * Detailed description of this element.
 *
 */
Gpio::~Gpio(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::~Gpio(void)", DEBUG_CODE_GPIO);

    // Returns successfully
    debugMessage(Error::NONE);
    return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return Error
 */
Error Gpio::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] gpioPort_p description of this parameter
 * @param[in] gpioPin_p description of this parameter
 * @return true
 * @return false
 */
bool_t Gpio::init(GPIO_TypeDef *gpioPort_p, cuint16_t gpioPin_p)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::init(GPIO_TypeDef *, cuint16_t)", DEBUG_CODE_GPIO);

    // Initialize GPIO
    if (!this->_initialize(gpioPort_p, gpioPin_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////    PIN RELATED METHODS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t Gpio::clr(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::clr(void)", DEBUG_CODE_GPIO);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Clear pin
    clrIoPin(this->_port, this->_pinMask);

    // Update data members
    this->_outLogicLevel = LogicLevel::LOW;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t Gpio::cpl(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::cpl(void)", DEBUG_CODE_GPIO);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Toggle pin
    cplIoPin(this->_port, this->_pinMask);

    // Update data members
    this->_outLogicLevel = ((bool)(this->_outLogicLevel)) ?
            LogicLevel::LOW :
            LogicLevel::HIGH;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return LogicLevel
 */
LogicLevel Gpio::read(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::read(void)", DEBUG_CODE_GPIO);

    // Local variables
    uint32_t aux32 = 0;
    LogicLevel auxLogicLevel = LogicLevel::LOW;

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error => default value
        debugMessage(this->_lastError);
        return LogicLevel::LOW;
    }

    // Read pin value
    aux32 = this->_port->IDR;
    auxLogicLevel = (isBitSet(aux32, this->_pinIndex)) ?
            LogicLevel::HIGH :
            LogicLevel::LOW;

    // Update data members
    this->_outLogicLevel = LogicLevel::LOW;

    // Returns value
    debugMessage(Error::NONE);
    return auxLogicLevel;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t Gpio::set(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::set(void)", DEBUG_CODE_GPIO);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Set pin
    setIoPin(this->_port, this->_pinMask);

    // Update data members
    this->_outLogicLevel = LogicLevel::HIGH;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] mode_p description of this parameter
 * @return true
 * @return false
 */
bool_t Gpio::setMode(Mode mode_p)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::setMode(Mode)", DEBUG_CODE_GPIO);

    // Local variables
    uint32_t auxCrRegister = 0;
    uint32_t auxOdrRegister = 0;
    uint32_t auxMask = 0;
    uint8_t auxPinOffset = 0;
    uint8_t auxSpeed = 0;
    bool_t useRegisterLowBank = false;

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Copy the configuration register
    if (this->_pinIndex < 8) {			// LOW BANK
        auxCrRegister = this->_port->CRL;
        auxPinOffset = this->_pinIndex;
        useRegisterLowBank = true;
    } else {							// HIGH BANK
        auxCrRegister = this->_port->CRH;
        auxPinOffset = this->_pinIndex - 8;
        useRegisterLowBank = false;
    }
    auxOdrRegister = this->_port->ODR;
    auxPinOffset *= 4;

    // Checking if pin speed is set
    if ((mode_p == Mode::OUTPUT_OPEN_DRAIN) || (mode_p == Mode::OUTPUT_PUSH_PULL)) {
        auxSpeed = (this->_outMaxSpeed == Speed::INPUT) ?
                (uint8_t)(Speed::FREQ_LOW) :
                (uint8_t)(this->_outMaxSpeed);
    }

    // Process the value of the register copy
    switch (mode_p) {
    case Mode::INPUT_ANALOG:
        auxMask = 0b0000;
        break;
    case Mode::INPUT_FLOATING:
        auxMask = 0b0100;
        break;
    case Mode::INPUT_PULLED_DOWN:
        auxMask = 0b1000;
        clrBit(auxOdrRegister, this->_pinIndex);
        break;
    case Mode::INPUT_PULLED_UP:
        auxMask = 0b1000;
        setBit(auxOdrRegister, this->_pinIndex);
        break;
    case Mode::OUTPUT_OPEN_DRAIN:
        auxMask = 0b0100 | (uint32_t)auxSpeed;
        break;
    case Mode::OUTPUT_PUSH_PULL:
        auxMask = 0b0000 | (uint32_t)auxSpeed;
        break;
    }
    clrMask(auxCrRegister, 0b1111, auxPinOffset);
    setMask(auxCrRegister, auxMask, auxPinOffset);

    // Update registers
    this->_port->ODR = auxOdrRegister;
    if (useRegisterLowBank) {			// LOW BANK
        this->_port->CRL = auxCrRegister;
    } else {							// HIGH BANK
        this->_port->CRH = auxCrRegister;
    }

    // Update data members
    this->_mode = mode_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] speed_p description of this parameter
 * @return true
 * @return false
 */
bool_t Gpio::setOutputSpeed(Speed speed_p)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::setOutputSpeed(Speed)", DEBUG_CODE_GPIO);

    // Checks initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update gpio registers
    switch (this->_mode) {
    case Mode::INPUT_ANALOG:
    case Mode::INPUT_FLOATING:
    case Mode::INPUT_PULLED_DOWN:
    case Mode::INPUT_PULLED_UP:			// Just update data members
        this->_outMaxSpeed = speed_p;
        break;
    case Mode::OUTPUT_OPEN_DRAIN:
    case Mode::OUTPUT_PUSH_PULL:		// Update data members and registers
        if (speed_p == Speed::INPUT) {
            // Returns error
            this->_lastError = Error::MODE_NOT_SUPPORTED;
            debugMessage(this->_lastError);
            return false;
        } else {
            this->_outMaxSpeed = speed_p;
            this->setMode(this->_mode);
        }
        break;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void Gpio::_clearData(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::_clearData(void)", DEBUG_CODE_GPIO);

    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    this->_initialized  = false;
    //     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
    this->_pinIndex     = 0;
    this->_pinMask      = 1 << 0;
    this->_port         = nullptr;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void Gpio::_deinitialize(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::_deinitialize(void)", DEBUG_CODE_GPIO);

    // Resets data members
    this->_clearData();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] gpioPort_p description of this parameter
 * @param[in] gpioPin_p description of this parameter
 * @return true
 * @return false
 */
bool_t Gpio::_initialize(GPIO_TypeDef *gpioPort_p, cuint16_t gpioPin_p)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::_initialize(GPIO_TypeDef *, cuint16_t)", DEBUG_CODE_GPIO);

    // Local variables
    uint32_t auxOdrRegister = 0;
    uint32_t auxCrRegister = 0;
    uint8_t auxPinOffset = 0;
    uint8_t auxPinIndex = 0;
    uint16_t auxPinMask = 0;
    LogicLevel auxLogicLevel = LogicLevel::LOW;
    Mode auxMode = Mode::INPUT_ANALOG;
    Speed auxSpeed = Speed::INPUT;

    // CHECK FOR ERROR - port address invalid
    if (!isPointerValid(gpioPort_p)) {
        // Returns error
        this->_lastError = Error::GPIO_PORT_INVALID;
        debugMessage(Error::GPIO_PORT_INVALID);
        return false;
    }
    // CHECK FOR ERROR - pin number invalid
    if (!IS_GPIO_PIN(gpioPin_p)) {
        // Returns error
        this->_lastError = Error::GPIO_PIN_INVALID;
        debugMessage(Error::GPIO_PIN_INVALID);
        return false;
    }

    // Process the pin mask
    auxPinIndex = 0;
    auxPinMask = gpioPin_p;
    auxPinMask >>= 1;
    while (auxPinMask) {
        auxPinMask >>= 1;
        auxPinIndex++;
    }

    // Retrieve current pin configuration
    if (auxPinIndex < 8) {              // LOW BANK
        auxCrRegister = this->_port->CRL;
        auxPinOffset = auxPinIndex;
    } else {                            // HIGH BANK
        auxCrRegister = this->_port->CRH;
        auxPinOffset = this->_pinIndex - 8;
    }
    auxOdrRegister = this->_port->ODR;
    auxPinOffset *= 4;

    // Process retrieved data
    auxLogicLevel = (isBitSet(auxOdrRegister, auxPinIndex)) ?
            LogicLevel::HIGH :
            LogicLevel::LOW;
    auxCrRegister >>= auxPinOffset;
    auxSpeed = (Speed)(auxCrRegister & 0b11);
    auxCrRegister >>= 2;
    auxCrRegister &= 0b11;
    if (auxSpeed == Speed::INPUT) {
        switch (auxCrRegister) {
        case 0b00:
            auxMode = Mode::INPUT_ANALOG;
            break;
        case 0b01:
            auxMode = Mode::INPUT_FLOATING;
            break;
        case 0b10:
            auxMode = (auxLogicLevel == LogicLevel::HIGH) ?
                    (Mode::INPUT_PULLED_UP) : (Mode::INPUT_PULLED_DOWN);
            break;
        default:
            break;
        }
    }

    // Update class members
    this->_initialized = true;
    this->_pinIndex = auxPinIndex;
    this->_pinMask = (1 << auxPinIndex);
    this->_port = gpioPort_p;
    this->_mode = auxMode;
    this->_outMaxSpeed = auxSpeed;
    this->_outLogicLevel = auxLogicLevel;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool_t Gpio::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Gpio::_isInitialized(void)", DEBUG_CODE_GPIO);

    // Checks initialization
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

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// END OF FILE
// =============================================================================
