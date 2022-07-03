/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibTwi.cpp
 * Module:			TWI Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibTwi.hpp"
#ifndef __FUNSAPE_LIB_TWI_HPP
#	error	[funsapeLibTwi.cpp] Error 16 - Header file (funsapeLibTwi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_TWI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibTwi.cpp] Error 17 - Build mismatch between source (funsapeLibTwi.cpp) and header (funsapeLibTwi.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

cuint8_t twiDefaultTimeout = 30;
cuint8_t twiBufferSizeMax = 200;
cuint8_t twiBufferSizeMin = 5;

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
 * @brief Construct a new Twi:: Twi object
 *
 * Detailed description of this element.
 *
 */
Twi::Twi(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::Twi(void)", DEBUG_CODE_TWI);

    // Resets data members
    this->_bufferData = nullptr;
    this->_deinitialize();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

/**
 * @brief Destroy the Twi:: Twi object
 *
 * Detailed description of this element.
 *
 */
Twi::~Twi(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::~Twi(void)", DEBUG_CODE_TWI);

    // Returns successfully
    return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

//     //////////////////////    DATA TRANSFER     //////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] reg_p description of this parameter
 * @param[out] buffData_p description of this parameter
 * @param[in] buffSize_p description of this parameter
 * @return true
 * @return false
 */
bool Twi::readReg(cuint8_t reg_p, uint8_t *buffData_p, cuint16_t buffSize_p)
{
    // Mark passage for debugging purpose
    debugMark("Twi::readReg(cuint8_t, uint8_t *, cuint16_t)", DEBUG_CODE_TWI);

    // CHECK FOR ERROR - peripheral not initialized
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // CHECK FOR ERROR - device not selected
    if (!this->_devAddressSet) {
        // Returns error
        this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
        debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
        return false;
    }
    // CHECK FOR ERROR - buffer data is null pointer
    if (!isPointerValid(buffData_p)) {
        // Returns error
        this->_lastError = Error::BUFFER_POINTER_NULL;
        debugMessage(Error::BUFFER_POINTER_NULL);
        return false;
    }
    // CHECK FOR ERROR - no bytes to read
    if (buffSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        debugMessage(Error::BUFFER_SIZE_ZERO);
        return false;
    }
    // CHECK FOR ERROR - message is too large to handle
    if ((buffSize_p > this->_bufferMaxSize)) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
        return false;
    }
    // CHECK FOR ERROR - peripheral is busy
    if (this->_busy) {
        // Returns error
        this->_lastError = Error::IS_BUSY;
        debugMessage(Error::IS_BUSY);
        return false;
    }

    // Update data members
    this->_bufferIndex = 0;
    this->_bufferLength = buffSize_p;
    this->_regAddress = reg_p;

    // Send data
    this->_operation = TwiOperation::READ;
    if (!this->_transmit()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Transfer data back from reception buffer
    memcpy(buffData_p, this->_bufferData, buffSize_p);
    this->_bufferIndex = 0;

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
 * @param[in] reg_p description of this parameter
 * @param[in] buffData_p description of this parameter
 * @param[in] buffSize_p description of this parameter
 * @return true
 * @return false
 */
bool Twi::writeReg(cuint8_t reg_p, cuint8_t *buffData_p, cuint16_t buffSize_p)
{
    // Mark passage for debugging purpose
    debugMark("Twi::writeReg(cuint8_t, cuint8_t *, cuint16_t)", DEBUG_CODE_TWI);

    // CHECK FOR ERROR - peripheral not initialized
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }
    // CHECK FOR ERROR - device not selected
    if (!this->_devAddressSet) {
        // Returns error
        this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
        debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
        return false;
    }
    // CHECK FOR ERROR - buffer data is null pointer
    if (!isPointerValid(buffData_p) && (buffSize_p != 0)) {
        // Returns error
        this->_lastError = Error::BUFFER_POINTER_NULL;
        debugMessage(Error::BUFFER_POINTER_NULL);
        return false;
    }
    // CHECK FOR ERROR - message is too large to handle
    if ((buffSize_p > this->_bufferMaxSize)) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
        return false;
    }
    // CHECK FOR ERROR - peripheral is busy
    if (this->_busy) {
        // Returns error
        this->_lastError = Error::IS_BUSY;
        debugMessage(Error::IS_BUSY);
        return false;
    }

    // Update data members
    this->_bufferIndex = 0;
    this->_bufferLength = buffSize_p;
    this->_regAddress = reg_p;

    // Transfer data to transmission buffer
    if (buffSize_p > 0) {
        memcpy(this->_bufferData, buffData_p, buffSize_p);
    }

    // Send data
    this->_operation = TwiOperation::WRITE;
    if (!this->_transmit()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ////////////////////    PROTOCOL SPECIFIC     ////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] address_p description of this parameter
 * @param[in] useLongAddress_p description of this parameter
 * @return true
 * @return false
 */
bool Twi::setDevice(cuint16_t address_p, cbool useLongAddress_p)
{
    // Mark passage for debugging purpose
    debugMark("Twi::setDevice(cuint16_t, cbool)", DEBUG_CODE_TWI);

    // Update data members
    this->_devAddress = address_p;
    this->_devAddressSet = true;
    this->_useLongAddress = useLongAddress_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return Bus::BusType
 */
Bus::BusType Twi::getBusType(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::getBusType(void)", DEBUG_CODE_TWI);

    // Returns bus type
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return Bus::BusType::TWI;
}

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

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
Error Twi::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] busHandler_p description of this parameter
 * @param[in] bufSize_p description of this parameter
 * @return true
 * @return false
 */
bool Twi::init(I2C_HandleTypeDef *busHandler_p, cuint16_t bufSize_p)
{
    // Mark passage for debugging purpose
    debugMark("Twi::init(I2C_HandleTypeDef *, cuint16_t)", DEBUG_CODE_TWI);

    // Deinitialize peripheral
    if (this->_initialized) {
        this->_deinitialize();
    }

    // Check for errors
    if (!isPointerValid(busHandler_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (bufSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        debugMessage(Error::BUFFER_SIZE_ZERO);
        return false;
    } else if (bufSize_p < twiBufferSizeMin) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
        return false;
    } else if (bufSize_p > twiBufferSizeMax) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
        return false;
    }

    // Update data members
    this->_busHandler = busHandler_p;
    this->_bufferMaxSize = bufSize_p;

    // Initialize
    if (!this->_initialize()) {
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
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] timeout_p description of this parameter
 * @return true
 * @return false
 */
bool Twi::setTimeout(cuint16_t timeout_p)
{
    // Mark passage for debugging purpose
    debugMark("Twi::setTimeout(cuint16_t)", DEBUG_CODE_TWI);

    // Update data members
    this->_timeout = timeout_p;

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
void Twi::_clearData(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::_clearData(void)", DEBUG_CODE_TWI);

    //     /////////////////    PERIPHERAL HANDLER     //////////////////     //
    this->_busHandler = nullptr;
    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    this->_busy = false;
    this->_devAddress = 0;
    this->_devAddressSet = false;
    this->_initialized = false;
    this->_operation = TwiOperation::READ;
    this->_regAddress = 0;
    this->_timeout = twiDefaultTimeout;
    //     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
    this->_clockSpeed = 100000UL;
    this->_ownAddress1 = 0;
    this->_ownAddress2 = 0;
    this->_useDualAddress = false;
    this->_useDutyCycle16_9 = false;
    this->_useGeneralCall = false;
    this->_useLongAddress = false;
    this->_useNoStretchMode = false;
    //     ////////////////////    DATA BUFFERS      ////////////////////     //
    if (this->_bufferData != nullptr) {
        free(this->_bufferData);
        this->_bufferData				= nullptr;
    }
    this->_bufferIndex = 0;
    this->_bufferLength = 0;
    this->_bufferMaxSize = 0;

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
void Twi::_deinitialize(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::_deinitialize(void)", DEBUG_CODE_TWI);

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
 * @return true
 * @return false
 */
bool Twi::_initialize(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::_initialize(void)", DEBUG_CODE_TWI);

    // Allocate memory
    this->_bufferData = (uint8_t *)calloc(this->_bufferMaxSize, sizeof(uint8_t));
    if (!isPointerValid(this->_bufferData)) {
        this->_deinitialize();
        // Returns error
        this->_lastError = Error::MEMORY_ALLOCATION;
        debugMessage(Error::MEMORY_ALLOCATION);
        return false;
    }
    this->_bufferIndex = 0;
    this->_bufferLength = 0;

    // Check peripheral status
    HAL_I2C_StateTypeDef twiState = HAL_I2C_STATE_RESET;
    twiState = HAL_I2C_GetState(this->_busHandler);
    if (twiState != HAL_I2C_STATE_READY) {
        // Returns error
        this->_initialized = false;
        this->_lastError = Error::NOT_READY;
        debugMessage(Error::NOT_READY);
        return false;
    }

    // Update data members
    this->_clockSpeed = (uint32_t)(this->_busHandler->Init.ClockSpeed);
    this->_ownAddress1 = (uint16_t)(this->_busHandler->Init.OwnAddress1);
    this->_ownAddress2 = (uint16_t)(this->_busHandler->Init.OwnAddress2);
    this->_useDualAddress = (bool)(this->_busHandler->Init.DualAddressMode);
    this->_useDutyCycle16_9 = (bool)(this->_busHandler->Init.DutyCycle);
    this->_useGeneralCall = (bool)(this->_busHandler->Init.GeneralCallMode);
    this->_useLongAddress = (bool)(this->_busHandler->Init.AddressingMode);
    this->_useNoStretchMode = (bool)(this->_busHandler->Init.NoStretchMode);
    this->_initialized = true;

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
bool Twi::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::_isInitialized(void)", DEBUG_CODE_TWI);

    // CHECK FOR ERROR - peripheral not initialized
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
 *
 * Detailed description of this element.
 *
 * @return true
 * @return false
 */
bool Twi::_transmit(void)
{
    // Mark passage for debugging purpose
    debugMark("Twi::_transmit(void)", DEBUG_CODE_TWI);

    // Local variables
    HAL_StatusTypeDef twiStatus = HAL_OK;

    // Reading from/Writing at bus
    this->_busy = true;							// Mark peripheral as busy
    if (this->_operation == TwiOperation::READ) {
        twiStatus = HAL_I2C_Mem_Read(this->_busHandler, (this->_devAddress << 1), this->_regAddress, I2C_MEMADD_SIZE_8BIT,
                        this->_bufferData, this->_bufferLength, this->_timeout);
    } else if (this->_operation == TwiOperation::WRITE) {
        twiStatus = HAL_I2C_Mem_Write(this->_busHandler, (this->_devAddress << 1), this->_regAddress, I2C_MEMADD_SIZE_8BIT,
                        this->_bufferData, this->_bufferLength, this->_timeout);
    } else {
        // Returns error
        this->_busy = false;					// Mark peripheral as not busy before leave
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return false;
    }

    // Check transmission status
    if (twiStatus == HAL_TIMEOUT) {
        // Returns error
        this->_busy = false;					// Mark peripheral as not busy before leave
        this->_lastError = Error::COMMUNICATION_TIMEOUT;
        debugMessage(Error::COMMUNICATION_TIMEOUT);
        return false;
    } else if (twiStatus == HAL_BUSY) {
        // Returns error
        this->_busy = false;					// Mark peripheral as not busy before leave
        this->_lastError = Error::IS_BUSY;
        debugMessage(Error::IS_BUSY);
        return false;
    } else if (twiStatus == HAL_ERROR) {
        // Returns error
        uint32_t halTwiError = HAL_I2C_GetError(this->_busHandler);

        switch (halTwiError) {
        case HAL_I2C_ERROR_OVR:
            this->_lastError = Error::TWI_OVERRUN;
            break;
        case HAL_I2C_ERROR_DMA:
            this->_lastError = Error::TWI_DMA;
            break;
        case HAL_I2C_ERROR_DMA_PARAM:
            this->_lastError = Error::TWI_DMA_PARAM;
            break;
        case HAL_I2C_ERROR_BERR:
            this->_lastError = Error::TWI_BUS_ERROR;
            break;
        case HAL_I2C_ERROR_ARLO:
            this->_lastError = Error::TWI_ARBITRATION_LOST;
            break;
        case HAL_I2C_ERROR_AF:
            this->_lastError = Error::TWI_AF_FLAG;
            break;
        case HAL_I2C_ERROR_SIZE:
            this->_lastError = Error::TWI_SIZE_MANAGEMENT_ERROR;
            break;
        case HAL_I2C_ERROR_TIMEOUT:
            this->_lastError = Error::COMMUNICATION_TIMEOUT;
            break;
        default:
            // If you arrived here, something really strange happened
            this->_lastError = Error::UNKNOWN;
            break;
        }
        this->_busy = false;					// Mark peripheral as not busy before leave
        debugMessage(this->_lastError);
        return false;
    }

    // Mark peripheral as not busy
    this->_busy = false;

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
