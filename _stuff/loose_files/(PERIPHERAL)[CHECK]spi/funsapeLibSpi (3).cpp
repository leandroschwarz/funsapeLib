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
#define		SPI_MAX_BUFFER_SIZE					200
#define		SPI_MIN_BUFFER_SIZE					5

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
	this->_activateDefaultDevice		= NULL;
	this->_busy							= false;
	this->_callbacksReady				= false;
	this->_deactivateDefaultDevice		= NULL;
	this->_initialized					= false;
	this->_rxBufferData					= NULL;
	this->_rxBufferIndex				= 0;
	this->_rxBufferLength				= 0;
	this->_rxBufferMaxSize				= 0;
#ifdef _FUNSAPE_PLATFORM_ARM_STM32
	this->_spiHandler					= NULL;
#endif
	this->_timeOut						= SPI_DEFAULT_TIME_OUT;
	this->_txBufferData					= NULL;
	this->_txBufferIndex				= 0;
	this->_txBufferLength				= 0;
	this->_txBufferMaxSize				= 0;
//	this->_spiTransmissionMode			= SPI_TRANS_MODE_POOLING;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

Spi::~Spi(void)
{
	// Return successfully
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

#ifdef _FUNSAPE_PLATFORM_ARM_STM32

bool Spi::init(SPI_HandleTypeDef *handler, uint16_t bufSize)
{
	// Deinitialize peripheral
	if (this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if (handler == NULL) {
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		return false;
	}
	if (bufSize == 0) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		return false;
	} else if (bufSize < SPI_MIN_BUFFER_SIZE) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	} else if (bufSize > SPI_MAX_BUFFER_SIZE) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;
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

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::init(SPI_HandleTypeDef *handler, uint16_t rxBufSize, uint16_t txBufSize)
{
	// Deinitialize peripheral
	if (this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if (handler == NULL) {
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		return false;
	}
	if ((rxBufSize == 0) || (txBufSize == 0)) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		return false;
	} else if ((rxBufSize < SPI_MIN_BUFFER_SIZE) || (txBufSize < SPI_MIN_BUFFER_SIZE)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	} else if ((rxBufSize > SPI_MAX_BUFFER_SIZE) || (txBufSize > SPI_MAX_BUFFER_SIZE)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;
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

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

#else

// bool Spi::init(uint16_t bufSize = 20)
// {
// 	// Deinitialize peripheral
// 	if (this->_initialized) {
// 		this->_deinitialize();
// 	}

// 	// Check for errors
// 	if (bufSize == 0) {
// 		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
// 		return false;
// 	} else if (bufSize < SPI_MIN_BUFFER_SIZE) {
// 		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
// 		return false;
// 	} else if (bufSize > SPI_MAX_BUFFER_SIZE) {
// 		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;
// 		return false;
// 	}

// 	// Update data members
// 	this->_rxBufferMaxSize = bufSize;
// 	this->_txBufferMaxSize = bufSize;

// 	// Initialize
// 	if (!this->_initialize()) {
// 		// Return error
// 		return false;
// 	}

// 	// Return successfully
// 	this->_lastError = ERROR_NONE;
// 	return true;
// }

// bool Spi::init(uint16_t rxBufSize, uint16_t txBufSize)
// {
// 	// Deinitialize peripheral
// 	if (this->_initialized) {
// 		this->_deinitialize();
// 	}

// 	// Check for errors
// 	if ((rxBufSize == 0) || (txBufSize == 0)) {
// 		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
// 		return false;
// 	} else if ((rxBufSize < SPI_MIN_BUFFER_SIZE) || (txBufSize < SPI_MIN_BUFFER_SIZE)) {
// 		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
// 		return false;
// 	} else if ((rxBufSize > SPI_MAX_BUFFER_SIZE) || (txBufSize > SPI_MAX_BUFFER_SIZE)) {
// 		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;
// 		return false;
// 	}

// 	// Update data members
// 	this->_rxBufferMaxSize = rxBufSize;
// 	this->_txBufferMaxSize = txBufSize;

// 	// Initialize
// 	if (!this->_initialize()) {
// 		// Return error
// 		return false;
// 	}

// 	// Return successfully
// 	this->_lastError = ERROR_NONE;
// 	return true;
// }

#endif

// bool Spi::setTrasnmissionMode(spiTransmissionMode_t mode)
// {
// 	// Update data members
// 	this->_spiTransmissionMode = mode;

// 	// Return successfully
// 	this->_lastError = ERROR_NONE;
// 	return true;
// }

bool Spi::setTimeOut(uint16_t timeOut)
{
	// Update data members
	this->_timeOut = timeOut;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::setDefaultDevice(void actFuncCall(void), void deactFuncCall(void))
{
	// Check for errors
	if ((actFuncCall == NULL) || (deactFuncCall == NULL)) {
		this->_lastError = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}

	// Update data members
	this->_activateDefaultDevice = actFuncCall;
	this->_deactivateDefaultDevice = deactFuncCall;
	this->_callbacksReady = true;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::sendData(uint8_t *msg, uint16_t msgSize)
{
	// Check for errors
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (!this->_callbacksReady) {
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		return false;
	}
	if (msg == NULL) {
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (msgSize == 0) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if ((msgSize > this->_rxBufferMaxSize) || (msgSize > this->_txBufferMaxSize)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	}

	// Check if peripheral is busy
	if (this->_busy) {
		this->_lastError = ERROR_IS_BUSY;
		return false;
	}

	// Transfer data between buffers
	memcpy(this->_txBufferData, msg, msgSize);
	this->_txBufferLength = msgSize;
	this->_txBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(msg, this->_rxBufferData, msgSize);
	this->_rxBufferLength = msgSize;
	this->_rxBufferIndex = 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::sendData(uint8_t *txMsg, uint8_t *rxMsg, uint16_t msgSize)
{
	// Check for errors
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (!this->_callbacksReady) {
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		return false;
	}
	if ((txMsg == NULL) || (rxMsg == NULL)) {
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (msgSize == 0) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if ((msgSize > this->_rxBufferMaxSize) || (msgSize > this->_txBufferMaxSize)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	}

	// Check if peripheral is busy
	if (this->_busy) {
		this->_lastError = ERROR_IS_BUSY;
		return false;
	}

	// Transfer data between buffers
	memcpy(this->_txBufferData, txMsg, msgSize);
	this->_txBufferLength = msgSize;
	this->_txBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(rxMsg, this->_rxBufferData, msgSize);
	this->_rxBufferLength = msgSize;
	this->_rxBufferIndex = 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::sendData(uint8_t reg, uint8_t *msg, uint16_t msgSize)
{
	// Check for errors
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (!this->_callbacksReady) {
		this->_lastError = ERROR_COMMUNICATION_NO_DEVICE_SELECTED;
		return false;
	}
	if (msg == NULL) {
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (msgSize == 0) {
		this->_lastError = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if (((msgSize + 1) > this->_rxBufferMaxSize) || ((msgSize + 1) > this->_txBufferMaxSize)) {
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	}

	// Check if peripheral is busy
	if (this->_busy) {
		this->_lastError = ERROR_IS_BUSY;
		return false;
	}

	// Transfer data between buffers
	this->_txBufferData[0] = reg;
	memcpy(&this->_txBufferData[1], msg, msgSize);
	this->_txBufferLength = msgSize + 1;
	this->_rxBufferLength = msgSize + 1;
	this->_txBufferIndex = 0;
	this->_rxBufferIndex = 0;

	// Send data
	if (!this->_transmit()) {
		return false;
	}

	// Transfer data between buffers
	memcpy(msg, &this->_rxBufferData[1], msgSize);

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

error_e Spi::getLastError(void)
{
	// Return error
	return this->_lastError;
}

// =============================================================================
// Class private methods
// =============================================================================

bool Spi::_deinitialize(void)
{
	// Reset data members
	this->_activateDefaultDevice		= NULL;
	this->_busy							= false;
	this->_callbacksReady				= false;
	this->_deactivateDefaultDevice		= NULL;
	this->_initialized					= false;
	if (this->_rxBufferData != NULL) {
		free(this->_rxBufferData);
		this->_rxBufferData				= NULL;
	}
	this->_rxBufferIndex				= 0;
	this->_rxBufferLength				= 0;
	this->_rxBufferMaxSize				= 0;
#ifdef _FUNSAPE_PLATFORM_ARM_STM32
	this->_spiHandler					= NULL;
#endif
// 	this->_spiTransmissionMode			= SPI_TRANS_MODE_POOLING;
	this->_timeOut						= SPI_DEFAULT_TIME_OUT;
	if (this->_txBufferData != NULL) {
		free(this->_txBufferData);
		this->_txBufferData				= NULL;
	}
	this->_txBufferIndex				= 0;
	this->_txBufferLength				= 0;
	this->_txBufferMaxSize				= 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::_initialize(void)
{
//	uint32_t aux32 = 0;
//	uint8_t aux8 = 0;

	// Allocate memory
	this->_rxBufferData = (uint8_t *)calloc(this->_rxBufferMaxSize, sizeof(uint8_t));
	this->_txBufferData = (uint8_t *)calloc(this->_txBufferMaxSize, sizeof(uint8_t));
	if ((this->_rxBufferData == NULL) || (this->_txBufferData == NULL)) {
		this->_deinitialize();
		this->_lastError = ERROR_MEMORY_ALLOCATION;
		return false;
	}

	// Check peripheral status
#ifdef _FUNSAPE_PLATFORM_ARM_STM32
	HAL_SPI_StateTypeDef spiState = HAL_SPI_STATE_RESET;

	spiState = HAL_SPI_GetState(this->_spiHandler);
	if (spiState != HAL_SPI_STATE_READY) {
		// Return error
		this->_initialized = false;
		this->_lastError = ERROR_PERIPHERAL_NOT_READY;
		return false;
	}
#endif

	// Update data members
	this->_initialized = true;
	this->_rxBufferIndex = 0;
	this->_rxBufferLength = 0;
	this->_txBufferIndex = 0;
	this->_txBufferLength = 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Spi::_transmit(void)
{
	// Activate device
	this->_activateDefaultDevice();

	// Send data
#ifdef _FUNSAPE_PLATFORM_ARM_STM32
	HAL_StatusTypeDef halStatus = HAL_ERROR;
	uint32_t halSpiError = 0;

	// Send data by pooling
	this->_busy = true;

//	char teste[50];
//	sprintf(teste, "this->_spiHandler = %p, this->_txBufferData = %p\r", (void *)this->_spiHandler,
//		(void *)this->_txBufferData);
//	HAL_UART_Transmit(&huart1, (uint8_t *)teste, strlen(teste), 30);

	halStatus = HAL_SPI_TransmitReceive(this->_spiHandler, this->_txBufferData, this->_rxBufferData, this->_txBufferLength,
			this->_timeOut);

	this->_deactivateDefaultDevice();
	this->_busy = false;

	// Check transmission status
	if (halStatus == HAL_TIMEOUT) {
		this->_lastError = ERROR_COMMUNICATION_TIME_OUT;
		return false;
	}
	if (halStatus == HAL_ERROR) {
		switch (halSpiError) {
			case HAL_SPI_ERROR_FLAG:
				this->_lastError = ERROR_SPI_BUSY_FLAG;
				break;
			case HAL_SPI_ERROR_CRC:
				this->_lastError = ERROR_SPI_CRC;
				break;
			case HAL_SPI_ERROR_OVR:
				this->_lastError = ERROR_SPI_OVERRUN;
				break;
			case HAL_SPI_ERROR_MODF:
				this->_lastError = ERROR_SPI_MODE_FAULT;
				break;
			case HAL_SPI_ERROR_DMA:
				this->_lastError = ERROR_SPI_DMA;
				break;
			default:
				// If you arrived here, something really strange happened
				this->_lastError = ERROR_UNKNOWN;
				break;
		}
		this->_busy = false;
		return false;
	}
#endif

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE
