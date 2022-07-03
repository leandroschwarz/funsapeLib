/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.cpp
 * Module:			SPI Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Dependencies
// 

#include "funsapeLibSpi.hpp"
#ifndef __FUNSAPE_LIB_SPI_HPP
#	error	[funsapeLibSpi.cpp] Error 1 - Header file (funsapeLibSpi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_SPI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.cpp] Error 13 - Build mismatch between source (funsapeLibSpi.cpp) and header (funsapeLibSpi.hpp) files!
#endif

// 
// File exclusive - Constants
// 

#define SPI_DEFAULT_TIMEOUT				30
#define SPI_DUMMY_BYTE					0xDB
#define SPI_MAX_BUFFER_SIZE				200
#define SPI_MIN_BUFFER_SIZE				5
#define SPI_READ_FLAG					0x80
#define SPI_WRITE_FLAG					0x00

// 
// File exclusive - New data types
// 

// NONE

// 
// File exclusive - Macro-functions
// 

// NONE

// 
// Global variables
// 

// NONE

// 
// Static functions declarations
// 

// NONE

// 
// Class constructors
// 

Spi::Spi(void)
{
	// Resets data members
	this->_activateDefaultDevice = nullptr;
	this->_busy = false;
	this->_deactivateDefaultDevice = nullptr;
	this->_devSelFunctionSet = false;
	this->_initialized = false;
	this->_rxBufferData = nullptr;
	this->_rxBufferIndex = 0;
	this->_rxBufferLength = 0;
	this->_rxBufferMaxSize = 0;
	this->_spiHandler = nullptr;
	this->_timeout = SPI_DEFAULT_TIMEOUT;
	this->_txBufferData = nullptr;
	this->_txBufferIndex = 0;
	this->_txBufferLength = 0;
	this->_txBufferMaxSize = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

Spi::~Spi(void)
{
	// Returns successfully
	return;
}

// 
// Inherited methods - Public
// 

bool Spi::writeReg(uint8_t reg_p, uint8_t *buffData_p, uint16_t buffSize_p)
{
	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if(!this->_devSelFunctionSet) {
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if(buffData_p == nullptr) {
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if(((buffSize_p + 1) > this->_txBufferMaxSize) || ((buffSize_p + 1) > this->_rxBufferMaxSize)) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Transfer data to transmission buffer
	this->_txBufferData[0] = reg_p;
	memcpy(&this->_txBufferData[1], buffData_p, buffSize_p);
	this->_txBufferLength = buffSize_p + 1;
	this->_txBufferIndex = 0;

	// Send data
	if(!this->_transmit(SpiOperation::WRITE)) {
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Spi::readReg(uint8_t reg_p, uint8_t *buffData_p, uint16_t buffSize_p)
{
	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if(!this->_devSelFunctionSet) {
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if(buffData_p == nullptr) {
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if(((buffSize_p + 1) > this->_txBufferMaxSize) || ((buffSize_p + 1) > this->_rxBufferMaxSize)) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Transfer data to transmission buffer
	this->_txBufferData[0] = reg_p;
	for(uint16_t i = 1; i < (buffSize_p + 1); i++) {
		this->_txBufferData[i] = SPI_DUMMY_BYTE;
	}
	this->_txBufferLength = buffSize_p + 1;
	this->_txBufferIndex = 0;

	// Send data
	if(!this->_transmit(SpiOperation::READ)) {
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

bool Spi::sendData(uint8_t *buffData_p, uint16_t buffSize_p)
{
	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if(!this->_devSelFunctionSet) {
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if(buffData_p == nullptr) {
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if((buffSize_p > this->_txBufferMaxSize) || (buffSize_p > this->_rxBufferMaxSize)) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Transfer data to transmission buffer
	memcpy(this->_txBufferData, buffData_p, buffSize_p);
	this->_txBufferLength = buffSize_p;
	this->_txBufferIndex = 0;

	// Send data
	if(!this->_transmit(SpiOperation::SEND)) {
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

bool Spi::sendData(uint8_t *txBuffData_p, uint8_t *rxBuffData_p, uint16_t buffSize_p)
{
	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	// CHECK FOR ERROR - device selection functions not set
	if(!this->_devSelFunctionSet) {
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if((txBuffData_p == nullptr) || (rxBuffData_p == nullptr)) {
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to write
	if(buffSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if((buffSize_p > this->_txBufferMaxSize) || (buffSize_p > this->_rxBufferMaxSize)) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	}
	// CHECK FOR ERROR - peripheral is busy
	if(this->_busy) {
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		return false;
	}

	// Transfer data to transmission buffer
	memcpy(this->_txBufferData, txBuffData_p, buffSize_p);
	this->_txBufferLength = buffSize_p;
	this->_txBufferIndex = 0;

	// Send data
	if(!this->_transmit(SpiOperation::SEND)) {
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

bool Spi::setDevice(void (* actFunc_p)(void), void (* deactFunc_p)(void))
{
	// CHECK FOR ERROR - transmission buffer data is null pointer
	if((actFunc_p == nullptr) || (deactFunc_p == nullptr)) {
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

Handler::HandlerType Spi::getHandlerType(void)
{
	debugMessage(Error::NONE);
	return Handler::HandlerType::SPI;
}

// 
// Inherited methods - Protected
// 

// NONE

// 
// Class own methods - Public
// 

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
bool Spi::init(SPI_HandleTypeDef *handler_p, uint16_t bufSize_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Spi::init(uint8_t *handler_p, uint16_t bufSize_p)
#endif
{
	// Deinitialize peripheral
	if(this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if(handler_p == nullptr) {
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}
	if(bufSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	} else if(bufSize_p < SPI_MIN_BUFFER_SIZE) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if(bufSize_p > SPI_MAX_BUFFER_SIZE) {
		this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
		debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_spiHandler = handler_p;
	this->_rxBufferMaxSize = bufSize_p;
	this->_txBufferMaxSize = bufSize_p;

	// Initialize
	if(!this->_initialize()) {
		// Return error
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
bool Spi::init(SPI_HandleTypeDef *handler_p, uint16_t rxBufSize_p, uint16_t txBufSize_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Spi::init(uint8_t *handler_p, uint16_t rxBufSize_p, uint16_t txBufSize_p)
#endif
{
	// Deinitialize peripheral
	if(this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if(handler_p == nullptr) {
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}
	if((rxBufSize_p == 0) || (txBufSize_p == 0)) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	} else if((rxBufSize_p < SPI_MIN_BUFFER_SIZE) || (txBufSize_p < SPI_MIN_BUFFER_SIZE)) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if((rxBufSize_p > SPI_MAX_BUFFER_SIZE) || (txBufSize_p > SPI_MAX_BUFFER_SIZE)) {
		this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
		debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_spiHandler = handler_p;
	this->_rxBufferMaxSize = rxBufSize_p;
	this->_txBufferMaxSize = txBufSize_p;

	// Initialize
	if(!this->_initialize()) {
		// Return error
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Spi::setTimeout(uint16_t timeout_p)
{
	// Update data members
	this->_timeout = timeout_p;

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

Error Spi::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

// 
// Class own methods - Private
// 

bool Spi::_deinitialize(void)
{
	// Reset data members
	this->_spiHandler					= nullptr;
	this->_initialized					= false;
	this->_devSelFunctionSet			= false;
	this->_busy							= false;
	this->_activateDefaultDevice		= nullptr;
	this->_deactivateDefaultDevice		= nullptr;
	if(this->_rxBufferData != nullptr) {
		free(this->_rxBufferData);
		this->_rxBufferData				= nullptr;
	}
	this->_rxBufferMaxSize				= 0;
	this->_rxBufferIndex				= 0;
	this->_rxBufferLength				= 0;
	if(this->_txBufferData != nullptr) {
		free(this->_txBufferData);
		this->_txBufferData				= nullptr;
	}
	this->_txBufferMaxSize				= 0;
	this->_txBufferIndex				= 0;
	this->_txBufferLength				= 0;
	this->_timeout						= SPI_DEFAULT_TIMEOUT;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Spi::_initialize(void)
{
	// Allocate memory
	this->_rxBufferData = (uint8_t *)calloc(this->_rxBufferMaxSize, sizeof(uint8_t));
	this->_txBufferData = (uint8_t *)calloc(this->_txBufferMaxSize, sizeof(uint8_t));
	if((this->_rxBufferData == nullptr) || (this->_txBufferData == nullptr)) {
		this->_deinitialize();
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
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
		if(spiState != HAL_SPI_STATE_READY) {
			// Return error
			this->_initialized = false;
			this->_lastError = Error::NOT_READY;
			debugMessage(Error::NOT_READY);
			return false;
		}
	} while(0);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// Update data members
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Spi::_transmit(SpiOperation spiOperation_p)
{
	//     ////////////////     SEND DATA BY POOLING     ////////////////     //
	this->_busy = true;							// Mark peripheral as busy
	this->_activateDefaultDevice();				// Select slave device

	// Set operation parameters
	switch(spiOperation_p) {
	case SpiOperation::READ:
		this->_txBufferData[0] |= SPI_READ_FLAG;
		break;
	case SpiOperation::WRITE:
		this->_txBufferData[0] |= SPI_WRITE_FLAG;
		break;
	case SpiOperation::SEND:
		break;
	default:
		this->_lastError = Error::UNKNOWN;
		debugMessage(Error::UNKNOWN);
		return false;
	}

	// Try to transmit (ARM_STM32)
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	HAL_StatusTypeDef halStatus = HAL_SPI_TransmitReceive(
					this->_spiHandler,
					this->_txBufferData,
					this->_rxBufferData,
					this->_txBufferLength,
					this->_timeout
			);

	// Try to transmit (AVR)
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// Deselect slave device
	this->_deactivateDefaultDevice();

	// Check transmission status (ARM_STM32)
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	if(halStatus == HAL_TIMEOUT) {
		this->_lastError = Error::COMMUNICATION_TIMEOUT;
		debugMessage(Error::COMMUNICATION_TIMEOUT);
		// Mark peripheral as not busy before leave
		this->_busy = false;
		return false;
	}
	if(halStatus == HAL_ERROR) {
		uint32_t halSpiError = HAL_SPI_GetError(this->_spiHandler);
		switch(halSpiError) {
		case HAL_SPI_ERROR_FLAG:
			this->_lastError = Error::SPI_BUSY_FLAG;
			debugMessage(Error::SPI_BUSY_FLAG);
			break;
		case HAL_SPI_ERROR_CRC:
			this->_lastError = Error::SPI_CRC;
			debugMessage(Error::SPI_CRC);
			break;
		case HAL_SPI_ERROR_OVR:
			this->_lastError = Error::SPI_OVERRUN;
			debugMessage(Error::SPI_OVERRUN);
			break;
		case HAL_SPI_ERROR_MODF:
			this->_lastError = Error::SPI_MODE_FAULT;
			debugMessage(Error::SPI_MODE_FAULT);
			break;
		case HAL_SPI_ERROR_DMA:
			this->_lastError = Error::SPI_DMA;
			debugMessage(Error::SPI_DMA);
			break;
		default:
			// If you arrived here, something really strange happened
			this->_lastError = Error::UNKNOWN;
			debugMessage(Error::UNKNOWN);
			break;
		}
		// Mark peripheral as not busy before leave
		this->_busy = false;
		return false;
	}

	// Check transmission status (AVR)
#elif defined(_FUNSAPE_PLATFORM_AVR)
// TODO: AVR implementation
#endif

	// Mark peripheral as not busy
	this->_busy = false;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// 
// Class own methods - Protected
// 

// NONE
