/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.cpp
 * Module:			SPI Peripheral Interface for FunSAPE++ Embedded Library
 *					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "funsapeLibSpi.hpp"
#ifndef __FUNSAPE_LIB_SPI_HPP
#	error	[funsapeLibSpi.cpp] Error 1 - Header file (funsapeLibSpi.hpp) must be corrupted!
#elif __FUNSAPE_LIB_SPI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.cpp] Error 9 - Build mismatch between source (funsapeLibSpi.cpp) and header (funsapeLibSpi.hpp) files!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		SPI_DEFAULT_TIME_OUT				30
#define		SPI_MIN_BUFFER_SIZE					5
#define		SPI_MAX_BUFFER_SIZE					200

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

Spi::Spi(void)
{
	// Reset data members
	this->_spiHandler					= nullptr;
	this->_initialized					= false;
	this->_devSelFunctiosSet			= false;
	this->_busy							= false;
	this->_activateDefaultDevice		= nullptr;
	this->_deactivateDefaultDevice		= nullptr;
	this->_rxBufferData					= nullptr;
	this->_rxBufferMaxSize				= 0;
	this->_rxBufferIndex				= 0;
	this->_rxBufferLength				= 0;
	this->_txBufferData					= nullptr;
	this->_txBufferMaxSize				= 0;
	this->_txBufferIndex				= 0;
	this->_txBufferLength				= 0;
	this->_timeOut						= SPI_DEFAULT_TIME_OUT;

	// Return successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return;
}

Spi::~Spi(void)
{
	// Return successfully
	return;
}

// =============================================================================
// Class inherited methods - public
// =============================================================================

bool Spi::sendData(uint8_t *buffData, uint16_t buffSize)
{
	// Check for errors
	if (!this->_initialized) {							// ERROR - peripheral not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}
	if (!this->_devSelFunctiosSet) {					// ERROR - device selection funtions not set
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(ERROR_COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	if (buffData == nullptr) {							// ERROR - transmission buffer data is null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		debugMessage(ERROR_BUFFER_POINTER_NULL);
		return false;
	}
	if (buffSize == 0) {								// ERROR - no bytes to send/receive
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		debugMessage(ERROR_BUFFER_SIZE_ZERO);
		return false;
	}
	if ((buffSize > this->_rxBufferMaxSize) ||
		(buffSize > this->_txBufferMaxSize)) {			// ERROR - message is too large to handle
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		debugMessage(ERROR_BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	if (this->_busy) {									// ERROR - peripheral is busy
		this->_lastError = ERROR_IS_BUSY;
		debugMessage(ERROR_IS_BUSY);
		return false;
	}

	// Transfer data between buffers
	memcpy(this->_txBufferData, buffData, buffSize);
	this->_txBufferLength = buffSize;
	this->_txBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(buffData, this->_rxBufferData, buffSize);
	this->_rxBufferLength = buffSize;
	this->_rxBufferIndex = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::sendData(uint8_t *txBuffData, uint8_t *rxBuffData, uint16_t buffSize)
{
	// Check for errors
	if (!this->_initialized) {							// ERROR - peripheral not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}
	if (!this->_devSelFunctiosSet) {					// ERROR - device selection funtions not set
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(ERROR_COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	if ((txBuffData == nullptr) ||
		(rxBuffData == nullptr)) {						// ERROR - transmission buffer data is null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		debugMessage(ERROR_BUFFER_POINTER_NULL);
		return false;
	}
	if (buffSize == 0) {								// ERROR - no bytes to send/receive
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		debugMessage(ERROR_BUFFER_SIZE_ZERO);
		return false;
	}
	if ((buffSize > this->_rxBufferMaxSize) ||
		(buffSize > this->_txBufferMaxSize)) {			// ERROR - message is too large to handle
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		debugMessage(ERROR_BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	if (this->_busy) {									// ERROR - peripheral is busy
		this->_lastError = ERROR_IS_BUSY;
		debugMessage(ERROR_IS_BUSY);
		return false;
	}

	// Transfer data between buffers
	memcpy(this->_txBufferData, txBuffData, buffSize);
	this->_txBufferLength = buffSize;
	this->_txBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(rxBuffData, this->_rxBufferData, buffSize);
	this->_rxBufferLength = buffSize;
	this->_rxBufferIndex = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::sendData(uint8_t reg, uint8_t *buffData, uint16_t buffSize)
{
	// Check for errors
	if (!this->_initialized) {							// ERROR - peripheral not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}
	if (!this->_devSelFunctiosSet) {					// ERROR - device selection funtions not set
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(ERROR_COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	if (buffData == nullptr) {							// ERROR - transmission buffer data is null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		debugMessage(ERROR_BUFFER_POINTER_NULL);
		return false;
	}
	if (buffSize == 0) {								// ERROR - no bytes to send/receive
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		debugMessage(ERROR_BUFFER_SIZE_ZERO);
		return false;
	}
	if (((buffSize + 1) > this->_rxBufferMaxSize) ||
		((buffSize + 1) > this->_txBufferMaxSize)) {		// ERROR - message is too large to handle
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		debugMessage(ERROR_BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	if (this->_busy) {									// ERROR - peripheral is busy
		this->_lastError = ERROR_IS_BUSY;
		debugMessage(ERROR_IS_BUSY);
		return false;
	}

	// Transfer data between buffers
	this->_txBufferData[0] = reg;
	memcpy(&this->_txBufferData[1], buffData, buffSize);
	this->_txBufferLength = buffSize + 1;
	this->_txBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(buffData, &this->_rxBufferData[1], buffSize);
	this->_rxBufferLength = buffSize;
	this->_rxBufferIndex = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::sendData(uint8_t reg, uint8_t *txBuffData, uint8_t *rxBuffData, uint16_t buffSize)
{
	// Check for errors
	if (!this->_initialized) {							// ERROR - peripheral not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}
	if (!this->_devSelFunctiosSet) {					// ERROR - device selection funtions not set
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(ERROR_COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	if ((txBuffData == nullptr) ||
		(rxBuffData == nullptr)) {						// ERROR - transmission buffer data is null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		debugMessage(ERROR_BUFFER_POINTER_NULL);
		return false;
	}
	if (buffSize == 0) {								// ERROR - no bytes to send/receive
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		debugMessage(ERROR_BUFFER_SIZE_ZERO);
		return false;
	}
	if (((buffSize + 1) > this->_rxBufferMaxSize) ||
		((buffSize + 1) > this->_txBufferMaxSize)) {	// ERROR - message is too large to handle
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		debugMessage(ERROR_BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	if (this->_busy) {									// ERROR - peripheral is busy
		this->_lastError = ERROR_IS_BUSY;
		debugMessage(ERROR_IS_BUSY);
		return false;
	}

	// Transfer data between buffers
	this->_txBufferData[0] = reg;
	memcpy(&this->_txBufferData[1], txBuffData, buffSize);
	this->_txBufferLength = buffSize + 1;
	this->_txBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(rxBuffData, &this->_rxBufferData[1], buffSize);
	this->_rxBufferLength = buffSize;
	this->_rxBufferIndex = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::setDevice(void (* actFunc)(void), void (* deactFunc)(void))
{
	// Check for errors
	if ((actFunc == nullptr) ||
		(deactFunc == nullptr)) {						// ERROR - transmission buffer data is null pointer
		this->_lastError = ERROR_FUNCTION_POINTER_NULL;
		debugMessage(ERROR_FUNCTION_POINTER_NULL);
		return false;
	}

	// Update data members
	this->_activateDefaultDevice = actFunc;
	this->_deactivateDefaultDevice = deactFunc;
	this->_devSelFunctiosSet = true;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

Handler::handlerType_e Spi::getHandlerType(void)
{
	debugMessage(ERROR_NONE);
	return Handler::HANDLER_TYPE_SPI;
}

// =============================================================================
// Class own methods - public
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
bool Spi::init(SPI_HandleTypeDef *handler, uint16_t bufSize)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Spi::init(uint8_t *handler, uint16_t bufSize)
#endif
{
	// Deinitialize peripheral
	if (this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if (handler == nullptr) {
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		debugMessage(ERROR_HANDLER_POINTER_NULL);
		return false;
	}
	if (bufSize == 0) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		debugMessage(ERROR_BUFFER_SIZE_ZERO);
		return false;
	} else if (bufSize < SPI_MIN_BUFFER_SIZE) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		debugMessage(ERROR_BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if (bufSize > SPI_MAX_BUFFER_SIZE) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;
		debugMessage(ERROR_BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_spiHandler = handler;
	this->_rxBufferMaxSize = bufSize;
	this->_txBufferMaxSize = bufSize;

	// Initialize
	if (!this->_initialize()) {
		// Return error
		return false;
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)						// ARM implementation
bool Spi::init(SPI_HandleTypeDef *handler, uint16_t rxBufSize, uint16_t txBufSize)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Spi::init(uint8_t *handler, uint16_t rxBufSize, uint16_t txBufSize)
#endif
{
	// Deinitialize peripheral
	if (this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if (handler == nullptr) {
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		debugMessage(ERROR_HANDLER_POINTER_NULL);
		return false;
	}
	if ((rxBufSize == 0) || (txBufSize == 0)) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		debugMessage(ERROR_BUFFER_SIZE_ZERO);
		return false;
	} else if ((rxBufSize < SPI_MIN_BUFFER_SIZE) || (txBufSize < SPI_MIN_BUFFER_SIZE)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		debugMessage(ERROR_BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if ((rxBufSize > SPI_MAX_BUFFER_SIZE) || (txBufSize > SPI_MAX_BUFFER_SIZE)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;
		debugMessage(ERROR_BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_spiHandler = handler;
	this->_rxBufferMaxSize = rxBufSize;
	this->_txBufferMaxSize = txBufSize;

	// Initialize
	if (!this->_initialize()) {
		// Return error
		return false;
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::setTimeOut(uint16_t timeOut)
{
	// Update data members
	this->_timeOut = timeOut;

	// Return successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

error_e Spi::getLastError(void)
{
	// Return error
	return this->_lastError;
}

// =============================================================================
// Class own methods - private
// =============================================================================

bool Spi::_deinitialize(void)
{
	// Reset data members
	this->_spiHandler					= nullptr;
	this->_initialized					= false;
	this->_devSelFunctiosSet			= false;
	this->_busy							= false;
	this->_activateDefaultDevice		= nullptr;
	this->_deactivateDefaultDevice		= nullptr;
	if (this->_rxBufferData != nullptr) {
		free(this->_rxBufferData);
		this->_rxBufferData				= nullptr;
	}
	this->_rxBufferMaxSize				= 0;
	this->_rxBufferIndex				= 0;
	this->_rxBufferLength				= 0;
	if (this->_txBufferData != nullptr) {
		free(this->_txBufferData);
		this->_txBufferData				= nullptr;
	}
	this->_txBufferMaxSize				= 0;
	this->_txBufferIndex				= 0;
	this->_txBufferLength				= 0;
	this->_timeOut						= SPI_DEFAULT_TIME_OUT;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::_initialize(void)
{
	// Allocate memory
	this->_rxBufferData = (uint8_t *)calloc(this->_rxBufferMaxSize, sizeof(uint8_t));
	this->_txBufferData = (uint8_t *)calloc(this->_txBufferMaxSize, sizeof(uint8_t));
	if ((this->_rxBufferData == nullptr) || (this->_txBufferData == nullptr)) {
		this->_deinitialize();
		this->_lastError = ERROR_MEMORY_ALLOCATION;
		debugMessage(ERROR_MEMORY_ALLOCATION);
		return false;
	}
	this->_rxBufferIndex = 0;
	this->_rxBufferLength = 0;
	this->_txBufferIndex = 0;
	this->_txBufferLength = 0;

	// Check peripheral status
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	do {
		HAL_SPI_StateTypeDef spiState = HAL_SPI_STATE_RESET;
		spiState = HAL_SPI_GetState(this->_spiHandler);
		if (spiState != HAL_SPI_STATE_READY) {
			// Return error
			this->_initialized = false;
			this->_lastError = ERROR_PERIPHERAL_NOT_READY;
			debugMessage(ERROR_PERIPHERAL_NOT_READY);
			return false;
		}
	} while (0);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// Update data members
	this->_initialized = true;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Spi::_transmit(void)
{
	// Send data by pooling
	this->_busy = true;							// Mark peripheral as busy
	this->_activateDefaultDevice();				// Select slave device
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)		// Try to transmit (ARM_STM32)
	HAL_StatusTypeDef halStatus = HAL_SPI_TransmitReceive(
			this->_spiHandler,
			this->_txBufferData,
			this->_rxBufferData,
			this->_txBufferLength,
			this->_timeOut
		);
#elif defined(_FUNSAPE_PLATFORM_AVR)			// Try to transmit (AVR)
	// TODO: AVR implementation
#endif
	this->_deactivateDefaultDevice();			// Deselect slave device
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)		// Check transmission status (ARM_STM32)
	if (halStatus == HAL_TIMEOUT) {
		this->_lastError = ERROR_COMMUNICATION_TIME_OUT;
		debugMessage(ERROR_COMMUNICATION_TIME_OUT);
		// Mark peripheral as not busy before leave
		this->_busy = false;
		return false;
	}
	if (halStatus == HAL_ERROR) {
		uint32_t halSpiError = HAL_SPI_GetError(this->_spiHandler);
		switch (halSpiError) {
			case HAL_SPI_ERROR_FLAG:
				this->_lastError = ERROR_SPI_BUSY_FLAG;
				debugMessage(ERROR_SPI_BUSY_FLAG);
				break;
			case HAL_SPI_ERROR_CRC:
				this->_lastError = ERROR_SPI_CRC;
				debugMessage(ERROR_SPI_CRC);
				break;
			case HAL_SPI_ERROR_OVR:
				this->_lastError = ERROR_SPI_OVERRUN;
				debugMessage(ERROR_SPI_OVERRUN);
				break;
			case HAL_SPI_ERROR_MODF:
				this->_lastError = ERROR_SPI_MODE_FAULT;
				debugMessage(ERROR_SPI_MODE_FAULT);
				break;
			case HAL_SPI_ERROR_DMA:
				this->_lastError = ERROR_SPI_DMA;
				debugMessage(ERROR_SPI_DMA);
				break;
			default:
				// If you arrived here, something really strange happened
				this->_lastError = ERROR_UNKNOWN;
				debugMessage(ERROR_UNKNOWN);
				break;
		}
		// Mark peripheral as not busy before leave
		this->_busy = false;
		return false;
	}
#elif defined(_FUNSAPE_PLATFORM_AVR)			// Check transmission status (AVR)
	// TODO: AVR implementation
#endif
	this->_busy = false;						// Mark peripheral as not busy

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

// =============================================================================
// Class own methods - protected
// =============================================================================

// NONE
