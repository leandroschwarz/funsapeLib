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
#	error	[funsapeLibTwi.cpp] Error 1 - Header file (funsapeLibTwi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_TWI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibTwi.cpp] Error 13 - Build mismatch between source (funsapeLibTwi.cpp) and header (funsapeLibTwi.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define TWI_DEFAULT_TIMEOUT				30
#define TWI_MAX_BUFFER_SIZE				200
#define TWI_MIN_BUFFER_SIZE				5
#define TWI_READ_FLAG					0x01
#define TWI_WRITE_FLAG					0x00

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

Twi::Twi(void)
{
	// Resets data members
	this->_busy							= false;
	this->_devAddress					= 0;
	this->_devAddressSet				= false;
	this->_devAddressLong				= false;
	this->_initialized					= false;
	this->_regAddress					= 0;
	this->_timeout						= TWI_DEFAULT_TIMEOUT;
	this->_bufferData					= nullptr;
	this->_bufferMaxSize				= 0;
	this->_bufferIndex					= 0;
	this->_bufferLength					= 0;
	this->_twiHandler					= nullptr;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

Twi::~Twi(void)
{
	// Returns successfully
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

bool Twi::writeReg(uint8_t reg_p, uint8_t *buffData_p, uint16_t buffSize_p)
{
	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	// CHECK FOR ERROR - device address not set
	if(!this->_devAddressSet) {
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - buffer data is null pointer
	if(!isPointerValid(buffData_p)) {
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if((buffSize_p > this->_bufferMaxSize)) {
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

	// Update data members
	this->_regAddress = reg_p;
	this->_bufferLength = buffSize_p;
	this->_bufferIndex = 0;
	memcpy(this->_bufferData, buffData_p, buffSize_p);

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	HAL_StatusTypeDef twiStatus = HAL_OK;

	// Writing at bus
	this->_busy = true;							// Mark peripheral as busy
	twiStatus = HAL_I2C_Mem_Write(this->_twiHandler, (this->_devAddress << 1), this->_regAddress, I2C_MEMADD_SIZE_8BIT, this->_bufferData, this->_bufferLength, this->_timeout);

	// Check transmission status
	if(twiStatus == HAL_TIMEOUT) {
		this->_lastError = Error::COMMUNICATION_TIMEOUT;
		debugMessage(Error::COMMUNICATION_TIMEOUT);
		this->_busy = false;					// Mark peripheral as not busy before leave
		return false;
	} else if(twiStatus == HAL_BUSY) {
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		this->_busy = false;					// Mark peripheral as not busy before leave
		return false;
	} else if(twiStatus == HAL_ERROR) {
		uint32_t halTwiError = HAL_I2C_GetError(this->_twiHandler);

		switch(halTwiError) {
		case HAL_I2C_ERROR_OVR:
			this->_lastError = Error::TWI_OVERRUN;
			debugMessage(Error::TWI_OVERRUN);
			break;
		case HAL_I2C_ERROR_DMA:
			this->_lastError = Error::TWI_DMA;
			debugMessage(Error::TWI_DMA);
			break;
		case HAL_I2C_ERROR_DMA_PARAM:
			this->_lastError = Error::TWI_DMA_PARAM;
			debugMessage(Error::TWI_DMA_PARAM);
			break;
		case HAL_I2C_ERROR_BERR:
			this->_lastError = Error::TWI_BUS_ERROR;
			debugMessage(Error::TWI_BUS_ERROR);
			break;
		case HAL_I2C_ERROR_ARLO:
			this->_lastError = Error::TWI_ARBITRATION_LOST;
			debugMessage(Error::TWI_ARBITRATION_LOST);
			break;
		case HAL_I2C_ERROR_AF:
			this->_lastError = Error::TWI_AF_FLAG;
			debugMessage(Error::TWI_AF_FLAG);
			break;
		case HAL_I2C_ERROR_SIZE:
			this->_lastError = Error::TWI_SIZE_MANAGEMENT_ERROR;
			debugMessage(Error::TWI_SIZE_MANAGEMENT_ERROR);
			break;
		case HAL_I2C_ERROR_TIMEOUT:
			this->_lastError = Error::COMMUNICATION_TIMEOUT;
			debugMessage(Error::COMMUNICATION_TIMEOUT);
			break;
		default:
			// If you arrived here, something really strange happened
			this->_lastError = Error::UNKNOWN;
			debugMessage(Error::UNKNOWN);
			break;
		}
		this->_busy = false;					// Mark peripheral as not busy before leave
		return false;
	}

	// Try to transmit (AVR)
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

bool Twi::readReg(uint8_t reg_p, uint8_t *buffData_p, uint16_t buffSize_p)
{
	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	// CHECK FOR ERROR - device address not set
	if(!this->_devAddressSet) {
		this->_lastError = Error::COMMUNICATION_NO_DEVICE_SELECTED;
		debugMessage(Error::COMMUNICATION_NO_DEVICE_SELECTED);
		return false;
	}
	// CHECK FOR ERROR - buffer data is null pointer
	if(!isPointerValid(buffData_p)) {
		this->_lastError = Error::BUFFER_POINTER_NULL;
		debugMessage(Error::BUFFER_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - no bytes to read
	if(buffSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - message is too large to handle
	if((buffSize_p > this->_bufferMaxSize)) {
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

	// Update data members
	this->_regAddress = reg_p;
	this->_bufferLength = buffSize_p;
	this->_bufferIndex = 0;

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	HAL_StatusTypeDef twiStatus = HAL_OK;

	// Reading from bus
	this->_busy = true;							// Mark peripheral as busy
	twiStatus = HAL_I2C_Mem_Read(this->_twiHandler, (this->_devAddress << 1), this->_regAddress, I2C_MEMADD_SIZE_8BIT, this->_bufferData, this->_bufferLength, this->_timeout);

	// Check transmission status
	if(twiStatus == HAL_TIMEOUT) {
		this->_lastError = Error::COMMUNICATION_TIMEOUT;
		debugMessage(Error::COMMUNICATION_TIMEOUT);
		this->_busy = false;					// Mark peripheral as not busy before leave
		return false;
	} else if(twiStatus == HAL_BUSY) {
		this->_lastError = Error::IS_BUSY;
		debugMessage(Error::IS_BUSY);
		this->_busy = false;					// Mark peripheral as not busy before leave
		return false;
	} else if(twiStatus == HAL_ERROR) {
		uint32_t halTwiError = HAL_I2C_GetError(this->_twiHandler);

		switch(halTwiError) {
		case HAL_I2C_ERROR_OVR:
			this->_lastError = Error::TWI_OVERRUN;
			debugMessage(Error::TWI_OVERRUN);
			break;
		case HAL_I2C_ERROR_DMA:
			this->_lastError = Error::TWI_DMA;
			debugMessage(Error::TWI_DMA);
			break;
		case HAL_I2C_ERROR_DMA_PARAM:
			this->_lastError = Error::TWI_DMA_PARAM;
			debugMessage(Error::TWI_DMA_PARAM);
			break;
		case HAL_I2C_ERROR_BERR:
			this->_lastError = Error::TWI_BUS_ERROR;
			debugMessage(Error::TWI_BUS_ERROR);
			break;
		case HAL_I2C_ERROR_ARLO:
			this->_lastError = Error::TWI_ARBITRATION_LOST;
			debugMessage(Error::TWI_ARBITRATION_LOST);
			break;
		case HAL_I2C_ERROR_AF:
			this->_lastError = Error::TWI_AF_FLAG;
			debugMessage(Error::TWI_AF_FLAG);
			break;
		case HAL_I2C_ERROR_SIZE:
			this->_lastError = Error::TWI_SIZE_MANAGEMENT_ERROR;
			debugMessage(Error::TWI_SIZE_MANAGEMENT_ERROR);
			break;
		case HAL_I2C_ERROR_TIMEOUT:
			this->_lastError = Error::COMMUNICATION_TIMEOUT;
			debugMessage(Error::COMMUNICATION_TIMEOUT);
			break;
		default:
			// If you arrived here, something really strange happened
			this->_lastError = Error::UNKNOWN;
			debugMessage(Error::UNKNOWN);
			break;
		}
		this->_busy = false;					// Mark peripheral as not busy before leave
		return false;
	}

	// Try to transmit (AVR)
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// TODO: AVR implementation
#endif

	// Mark peripheral as not busy
	this->_busy = false;

	// Transfer data back from reception buffer
	memcpy(buffData_p, this->_bufferData, buffSize_p);
	this->_bufferIndex = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Twi::setAddress(uint16_t address_p, bool useLongAddress_p)
{
	// Update data members
	this->_devAddress = address_p;
	this->_devAddressLong = useLongAddress_p;
	this->_devAddressSet = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

Handler::HandlerType Twi::getHandlerType(void)
{
	debugMessage(Error::NONE);
	return Handler::HandlerType::TWI;
}

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
bool Twi::init(I2C_HandleTypeDef *handler_p, uint16_t bufSize_p)
#elif defined(_FUNSAPE_PLATFORM_AVR)							// TODO: AVR implementation
bool Twi::init(uint8_t *handler_p, uint16_t bufSize_p)
#endif
{
	// Deinitialize peripheral
	if(this->_initialized) {
		this->_deinitialize();
	}

	// Check for errors
	if(!isPointerValid(handler_p)) {
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	}
	if(bufSize_p == 0) {
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	} else if(bufSize_p < TWI_MIN_BUFFER_SIZE) {
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return false;
	} else if(bufSize_p > TWI_MAX_BUFFER_SIZE) {
		this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
		debugMessage(Error::BUFFER_SIZE_TOO_LARGE);
		return false;
	}

	// Update data members
	this->_twiHandler = handler_p;
	this->_bufferMaxSize = bufSize_p;

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

bool Twi::setTimeout(uint16_t timeout_p)
{
	// Update data members
	this->_timeout = timeout_p;

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

Error Twi::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

bool Twi::_deinitialize(void)
{
	// Resets data members
	this->_busy							= false;
	this->_devAddress					= 0;
	this->_devAddressSet				= false;
	this->_devAddressLong				= false;
	this->_initialized					= false;
	this->_regAddress					= 0;
	this->_twiHandler					= nullptr;
	if(this->_bufferData != nullptr) {
		free(this->_bufferData);
		this->_bufferData				= nullptr;
	}
	this->_bufferMaxSize				= 0;
	this->_bufferIndex					= 0;
	this->_bufferLength					= 0;
	this->_timeout						= TWI_DEFAULT_TIMEOUT;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Twi::_initialize(void)
{
	// Allocate memory
	this->_bufferData = (uint8_t *)calloc(this->_bufferMaxSize, sizeof(uint8_t));
	if(!isPointerValid(this->_bufferData)) {
		this->_deinitialize();
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return false;
	}
	this->_bufferIndex = 0;
	this->_bufferLength = 0;

	// Check peripheral status
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
	do {
		HAL_I2C_StateTypeDef twiState = HAL_I2C_STATE_RESET;
		twiState = HAL_I2C_GetState(this->_twiHandler);
		switch(twiState) {
		case HAL_I2C_STATE_RESET:
		case HAL_I2C_STATE_READY:
			// Everything was OK!
			break;
		default:
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

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
