/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.cpp
 * Module:			SPI Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibSpi.hpp"
#ifndef __FUNSAPE_LIB_SPI_HPP
#	error	[funsapeLibSpi.cpp] Error 16 - Header file (funsapeLibSpi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_SPI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.cpp] Error 17 - Build mismatch between source (funsapeLibSpi.cpp) and header (funsapeLibSpi.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

cuint8_t spiDefaultTimeout = 30;
cuint8_t spiDummyByte = 0xFF;
cuint16_t spiBufferSizeMax = 2000;
cuint16_t spiBufferSizeMin = 5;
cuint8_t spiFlagRead = 0x80;
cuint8_t spiFlagWrite = 0x00;

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
 * @brief Construct a new Spi:: Spi object
 *
 * Detailed description of this element.
 *
 */
Spi::Spi(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::Spi(void)", DEBUG_CODE_SPI);

	// Resets data members
	this->_rxBufferData = nullptr;
	this->_txBufferData = nullptr;
	this->_clearData();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Destroy the Spi:: Spi object
 *
 * Detailed description of this element.
 *
 */
Spi::~Spi(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::~Spi(void)", DEBUG_CODE_SPI);

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
bool Spi::readReg(cuint8_t reg_p, uint8_t *buffData_p, cuint16_t buffSize_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::readReg(cuint8_t, uint8_t *, cuint16_t)", DEBUG_CODE_SPI);

	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - device not selected
	if((!this->_devSelFunctionSet) && (!this->_devSelGpioPinSet)) {
		// Returns error
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if(!isPointerValid(buffData_p)) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to read
	if(buffSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if(((buffSize_p + 1) > this->_txBufferMaxSize) || ((buffSize_p + 1) > this->_rxBufferMaxSize)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		// Returns error
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Update data members
	this->_txBufferIndex = 0;
	this->_txBufferLength = buffSize_p + 1;

	// Transfer data to transmission buffer
	this->_txBufferData[0] = reg_p;
	for(uint16_t i = 1; i < (buffSize_p + 1); i++) {
		this->_txBufferData[i] = spiDummyByte;
	}

	// Send data
	this->_operation = SpiOperation::READ;
	if(!this->_transmit()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Transfer data back from reception buffer
	memcpy(buffData_p, &this->_rxBufferData[1], buffSize_p);
	this->_rxBufferLength = buffSize_p;
	this->_rxBufferIndex = 0;

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
 * @param[in,out] buffData_p description of this parameter
 * @param[in] buffSize_p description of this parameter
 * @return true
 * @return false
 */
bool Spi::sendData(uint8_t *buffData_p, cuint16_t buffSize_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::sendData(uint8_t *, cuint16_t)", DEBUG_CODE_SPI);

	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if((!this->_devSelFunctionSet) && (!this->_devSelGpioPinSet)) {
		// Returns error
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if(!isPointerValid(buffData_p)) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if((buffSize_p > this->_txBufferMaxSize) || (buffSize_p > this->_rxBufferMaxSize)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		// Returns error
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Transfer data to transmission buffer
	memcpy(this->_txBufferData, buffData_p, buffSize_p);
	this->_txBufferLength = buffSize_p;
	this->_txBufferIndex = 0;

	// Send data
	this->_operation = SpiOperation::SEND;
	if(!this->_transmit()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Transfer data back from reception buffer
	memcpy(buffData_p, this->_rxBufferData, buffSize_p);
	this->_rxBufferLength = buffSize_p;
	this->_rxBufferIndex = 0;

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
 * @param[in] txBuffData_p description of this parameter
 * @param[out] rxBuffData_p description of this parameter
 * @param[in] buffSize_p description of this parameter
 * @return true
 * @return false
 */
bool Spi::sendData(cuint8_t *txBuffData_p, uint8_t *rxBuffData_p, cuint16_t buffSize_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::sendData(cuint8_t *, uint8_t *, cuint16_t)", DEBUG_CODE_SPI);

	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if((!this->_devSelFunctionSet) && (!this->_devSelGpioPinSet)) {
		// Returns error
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if((!isPointerValid(txBuffData_p)) || (!isPointerValid(rxBuffData_p))) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if((buffSize_p > this->_txBufferMaxSize) || (buffSize_p > this->_rxBufferMaxSize)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		// Returns error
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Transfer data to transmission buffer
	memcpy(this->_txBufferData, txBuffData_p, buffSize_p);
	this->_txBufferLength = buffSize_p;
	this->_txBufferIndex = 0;

	// Send data
	this->_operation = SpiOperation::SEND;
	if(!this->_transmit()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Transfer data back from reception buffer
	memcpy(rxBuffData_p, this->_rxBufferData, buffSize_p);
	this->_rxBufferLength = buffSize_p;
	this->_rxBufferIndex = 0;

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
bool Spi::writeReg(cuint8_t reg_p, cuint8_t *buffData_p, cuint16_t buffSize_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::writeReg(cuint8_t, cuint8_t *, cuint16_t)", DEBUG_CODE_SPI);

	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if((!this->_devSelFunctionSet) && (!this->_devSelGpioPinSet)) {
		// Returns error
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if(!isPointerValid(buffData_p)) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if(((buffSize_p + 1) > this->_txBufferMaxSize) || ((buffSize_p + 1) > this->_rxBufferMaxSize)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		// Returns error
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Update data members
	this->_txBufferIndex = 0;
	this->_txBufferLength = buffSize_p + 1;

	// Transfer data to transmission buffer
	this->_txBufferData[0] = reg_p;
	memcpy(&this->_txBufferData[1], buffData_p, buffSize_p);

	// Send data
	this->_operation = SpiOperation::WRITE;
	if(!this->_transmit()) {
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
 * @param[in] actFunc_p description of this parameter
 * @param[in] deactFunc_p description of this parameter
 * @return true
 * @return false
 */
bool Spi::setDevice(void (* actFunc_p)(void), void (* deactFunc_p)(void))
{
	// Mark passage for debugging purpose
	debugMark("Spi::setDevice(void (*)(void), void (*)(void))", DEBUG_CODE_SPI);

	// Clear data members
	this->_devSelFunctionSet = false;
	this->_devSelGpioPinSet = false;

	// CHECK FOR ERROR - invalid pointer to functions
	if((!isPointerValid(actFunc_p)) || (!isPointerValid(deactFunc_p))) {
		// Returns error
		this->_lastError = Error::FUNCTION_POINTER_NULL;
		debugMessage(Error::FUNCTION_POINTER_NULL);
		return false;
	}

	// Update data members
	this->_activateDefaultDevice = actFunc_p;
	this->_deactivateDefaultDevice = deactFunc_p;
	this->_devSelFunctionSet = true;

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
 * @param[in] csPin_p description of this parameter
 * @return true
 * @return false
 */
bool Spi::setDevice(Gpio *csPin_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::setDevice(Gpio *)", DEBUG_CODE_SPI);

	// Clear data members
	this->_devSelFunctionSet = false;
	this->_devSelGpioPinSet = false;

	// CHECK FOR ERROR - Gpio null pointer
	if(!isPointerValid(csPin_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Update data members
	this->_csPin = csPin_p;
	this->_devSelGpioPinSet = true;

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
Bus::BusType Spi::getBusType(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::getBusType(void)", DEBUG_CODE_SPI);

	// Returns bus type
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return Bus::BusType::SPI;
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
 * @param[in] mode_p description of this parameter
 * @return true
 * @return false
 */
bool Spi::changeMode(const SpiMode mode_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::changeMode(const SpiMode)", DEBUG_CODE_SPI);

	// Check for errors
	if(!isPointerValid(this->_busHandler)) {
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}

	// Check current mode
	if(this->_mode == mode_p) {			// Nothing to do
		// Returns successfully
		this->_lastError = Error::NONE;
		debugMessage(Error::NONE);
		return true;
	}

	// Set mode options
	switch(mode_p) {
	case SpiMode::MODE_0:
		this->_busHandler->Init.CLKPolarity = SPI_POLARITY_LOW;
		this->_busHandler->Init.CLKPhase = SPI_PHASE_1EDGE;
		break;
	case SpiMode::MODE_1:
		this->_busHandler->Init.CLKPolarity = SPI_POLARITY_LOW;
		this->_busHandler->Init.CLKPhase = SPI_PHASE_2EDGE;
		break;
	case SpiMode::MODE_2:
		this->_busHandler->Init.CLKPolarity = SPI_POLARITY_HIGH;
		this->_busHandler->Init.CLKPhase = SPI_PHASE_1EDGE;
		break;
	case SpiMode::MODE_3:
		this->_busHandler->Init.CLKPolarity = SPI_POLARITY_HIGH;
		this->_busHandler->Init.CLKPhase = SPI_PHASE_2EDGE;
		break;
	default:
		// Returns error
		this->_lastError = Error::UNKNOWN;
		debugMessage(Error::UNKNOWN);
		return false;
	}

	// Change mode
	if(HAL_SPI_Init(this->_busHandler) != HAL_OK) {
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}

	// Update variables
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
 * @return Error
 */
Error Spi::getLastError(void)
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
bool Spi::init(SPI_HandleTypeDef *busHandler_p, cuint16_t bufSize_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::init(SPI_HandleTypeDef *, cuint16_t)", DEBUG_CODE_SPI);

	// Deinitialize peripheral
	if(this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if(!isPointerValid(busHandler_p)) {
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}
	if(bufSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	} else if(bufSize_p < spiBufferSizeMin) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if(bufSize_p > spiBufferSizeMax) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
		debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_busHandler = busHandler_p;
	this->_rxBufferMaxSize = bufSize_p;
	this->_txBufferMaxSize = bufSize_p;

	// Initialize
	if(!this->_initialize()) {
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
 * @param[in] busHandler_p description of this parameter
 * @param[in] rxBufSize_p description of this parameter
 * @param[in] txBufSize_p description of this parameter
 * @return true
 * @return false
 */
bool Spi::init(SPI_HandleTypeDef *busHandler_p, cuint16_t rxBufSize_p, cuint16_t txBufSize_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::init(SPI_HandleTypeDef *, cuint16_t, cuint16_t)", DEBUG_CODE_SPI);

	// Deinitialize peripheral
	if(this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if(!isPointerValid(busHandler_p)) {
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}
	if((rxBufSize_p == 0) || (txBufSize_p == 0)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	} else if((rxBufSize_p < spiBufferSizeMin) || (txBufSize_p < spiBufferSizeMin)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if((rxBufSize_p > spiBufferSizeMax) || (txBufSize_p > spiBufferSizeMax)) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
		debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_busHandler = busHandler_p;
	this->_rxBufferMaxSize = rxBufSize_p;
	this->_txBufferMaxSize = txBufSize_p;

	// Initialize
	if(!this->_initialize()) {
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
bool Spi::setTimeout(cuint16_t timeout_p)
{
	// Mark passage for debugging purpose
	debugMark("Spi::setTimeout(cuint16_t)", DEBUG_CODE_SPI);

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
void Spi::_clearData(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::_clearData(void)", DEBUG_CODE_SPI);

	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
	this->_busHandler = nullptr;
	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	this->_busy = false;
	this->_csPin = nullptr;
	this->_activateDefaultDevice = nullptr;
	this->_deactivateDefaultDevice = nullptr;
	this->_devSelFunctionSet = false;
	this->_devSelGpioPinSet = false;
	this->_initialized = false;
	this->_operation = SpiOperation::READ;
	this->_timeout = spiDefaultTimeout;
	//     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
	this->_baudRatePrescaler = SpiPrescaler::PRESCALER_2;
	this->_crcCalculationEnabled = false;
	this->_crcPolynomial = 1;
	this->_csManagement = SpiCsManagement::NSS_SOFT;
	this->_dataSize16BitsEnabled = false;
	this->_direction = SpiDirection::DIRECTION_2_LINES;
	this->_lsbFirstEnabled = false;
	this->_masterModeEnabled = false;
	this->_mode = SpiMode::MODE_0;
	//     ////////////////////    DATA BUFFERS      ////////////////////     //
	if(this->_rxBufferData != nullptr) {
		free(this->_rxBufferData);
		this->_rxBufferData = nullptr;
	}
	this->_rxBufferIndex = 0;
	this->_rxBufferLength = 0;
	this->_rxBufferMaxSize = 0;
	if(this->_txBufferData != nullptr) {
		free(this->_txBufferData);
		this->_txBufferData = nullptr;
	}
	this->_txBufferIndex = 0;
	this->_txBufferLength = 0;
	this->_txBufferMaxSize = 0;

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
void Spi::_deinitialize(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::_deinitilize(void)", DEBUG_CODE_SPI);

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
bool Spi::_initialize(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::_initialize(void)", DEBUG_CODE_SPI);

	// Allocate memory
	this->_rxBufferData = (uint8_t *)calloc(this->_rxBufferMaxSize, sizeof(uint8_t));
	this->_txBufferData = (uint8_t *)calloc(this->_txBufferMaxSize, sizeof(uint8_t));
	if((!isPointerValid(this->_rxBufferData)) || (!isPointerValid(this->_txBufferData))) {
		this->_deinitialize();
		// Returns error
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return false;
	}
	this->_rxBufferIndex = 0;
	this->_rxBufferLength = 0;
	this->_txBufferIndex = 0;
	this->_txBufferLength = 0;

	// Check peripheral status
	HAL_SPI_StateTypeDef spiState = HAL_SPI_STATE_RESET;
	spiState = HAL_SPI_GetState(this->_busHandler);
	if(spiState != HAL_SPI_STATE_READY) {
		// Returns error
		this->_initialized = false;
		this->_lastError = Error::NOT_READY;
		debugMessage(Error::NOT_READY);
		return false;
	}

	// Update data members
	bool clkPolarity = (bool)(this->_busHandler->Init.CLKPolarity);
	bool clkPhase = (bool)(this->_busHandler->Init.CLKPhase);
	if(!(clkPolarity) && !(clkPhase)) {			// 00 => mode 0
		this->_mode = SpiMode::MODE_0;
	} else if((!clkPolarity) && (clkPhase)) {	// 01 => mode 1
		this->_mode = SpiMode::MODE_1;
	} else if((clkPolarity) && (!clkPhase)) {	// 10 => mode 2
		this->_mode = SpiMode::MODE_2;
	} else {									// 11 => mode 3
		this->_mode = SpiMode::MODE_3;
	}
	this->_baudRatePrescaler		= (SpiPrescaler)(this->_busHandler->Init.BaudRatePrescaler);
	this->_crcCalculationEnabled	= (bool)(this->_busHandler->Init.CRCCalculation);
	this->_crcPolynomial			= this->_busHandler->Init.CRCPolynomial;
	this->_csManagement				= (SpiCsManagement)(this->_busHandler->Init.NSS);
	this->_dataSize16BitsEnabled	= (bool)(this->_busHandler->Init.DataSize);
	this->_direction				= (SpiDirection)(this->_busHandler->Init.Direction);
	this->_lsbFirstEnabled			= (bool)(this->_busHandler->Init.FirstBit);
	this->_masterModeEnabled		= (bool)(this->_busHandler->Init.Mode);
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
bool Spi::_isInitialized(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::_isInitialized(void)", DEBUG_CODE_SPI);

	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
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
bool Spi::_transmit(void)
{
	// Mark passage for debugging purpose
	debugMark("Spi::_transmit(void)", DEBUG_CODE_SPI);

	//     ////////////////     SEND DATA BY POOLING     ////////////////     //
	this->_busy = true;							// Mark peripheral as busy
	if(this->_devSelFunctionSet) {				// Select device using function
		this->_activateDefaultDevice();
	} else if(this->_devSelGpioPinSet) {		// Select device using Gpio
		this->_csPin->clr();
	}

	// Set operation parameters
	switch(this->_operation) {
	case SpiOperation::READ:
		this->_txBufferData[0] |= spiFlagRead;
		break;
	case SpiOperation::WRITE:
		this->_txBufferData[0] |= spiFlagWrite;
		break;
	case SpiOperation::SEND:
		break;
	default:
		// Returns error
		this->_busy = false;			// Mark peripheral as not busy before leave
		this->_lastError = Error::UNKNOWN;
		debugMessage(Error::UNKNOWN);
		return false;
	}

	// Try to transmit
	HAL_StatusTypeDef halStatus = HAL_SPI_TransmitReceive(
					this->_busHandler,
					this->_txBufferData,
					this->_rxBufferData,
					this->_txBufferLength,
					this->_timeout
			);

	// Deselect slave device
	if(this->_devSelFunctionSet) {				// Deselect device using function
		this->_deactivateDefaultDevice();
	} else if(this->_devSelGpioPinSet) {		// Deselect device using Gpio
		this->_csPin->set();
	}

	// Check transmission status
	if(halStatus == HAL_TIMEOUT) {
		// Returns error
		this->_busy = false;			// Mark peripheral as not busy before leave
		this->_lastError = Error::COMMUNICATION_TIMEOUT;
		debugMessage(Error::COMMUNICATION_TIMEOUT);
		return false;
	}
	if(halStatus == HAL_ERROR) {
		// Returns error
		uint32_t halSpiError = HAL_SPI_GetError(this->_busHandler);
		switch(halSpiError) {
		case HAL_SPI_ERROR_FLAG:
			this->_lastError = Error::SPI_BUSY_FLAG;
			break;
		case HAL_SPI_ERROR_CRC:
			this->_lastError = Error::SPI_CRC;
			break;
		case HAL_SPI_ERROR_OVR:
			this->_lastError = Error::SPI_OVERRUN;
			break;
		case HAL_SPI_ERROR_MODF:
			this->_lastError = Error::SPI_MODE_FAULT;
			break;
		case HAL_SPI_ERROR_DMA:
			this->_lastError = Error::SPI_DMA;
			break;
		default:
			// If you arrived here, something really strange happened
			this->_lastError = Error::UNKNOWN;
			break;
		}
		this->_busy = false;			// Mark peripheral as not busy before leave
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
