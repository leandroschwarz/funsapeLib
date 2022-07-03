/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	gpdseArmI2c.cpp
 * Module:		I2C interface module for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */
// =============================================================================
// System file dependencies
// =============================================================================

#include "gpdseArmI2c.hpp"
#if __GPDSE_ARM_I2C_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error Build mismatch on header and source code files (gpdseArmI2c).
#endif

#include <string.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		I2C_MAX_BIT_RATE				400000UL
#define		I2C_MIN_BIT_RATE				1000UL
#define		I2C_MIN_BUFFER_SIZE				10
#define		I2C_MAX_BUFFER_SIZE				100
#define		I2C_DEFAULT_TIME_OUT			20

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
// Class constructors
// =============================================================================

I2c::I2c(I2C_HandleTypeDef * i2cHandler, uint8_t buffSize)
{
	// Check for errors - i2cHandler pointer
	if (i2cHandler == NULL) {
		this->lastError_ = ERROR_ARGUMENT_POINTER_NULL;
		this->lastErrorDetails_ = 0;
		return;
	}
	if (buffSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		this->lastErrorDetails_ = 0;
		return;
	}
	if (buffSize < 3) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		this->lastErrorDetails_ = 0;
		return;
	}	

	// Allocate memory
	this->bufferData_ = NULL;
	this->bufferData_ = (uint8_t *)calloc(buffSize, sizeof(uint8_t));
	if (this->bufferData_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		this->lastErrorDetails_ = 0;
		return;
	}
	
	// Update class data
	this->i2cHandler = i2cHandler;
	this->initialized_ = true;
	this->bufferMaxSize_ = buffSize;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;
	this->timeOut_ = I2C_DEFAULT_TIME_OUT;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	this->lastErrorDetails_ = 0;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool I2c::sendData(uint8_t devAddress, twiOperation_t readWrite, uint8_t reg,
        uint8_t *msg, uint8_t msgSize)
{
	uint8_t halError = 0;
	// Check for errors - NOT Initialized
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		this->lastErrorDetails_ = 0;
		return false;
	}
	// Check for errors - Message pointer
	if ((msgSize > 0) && (msg == NULL)) {
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		this->lastErrorDetails_ = 0;
		return false;
	}

	this->bufAdd = (uint32_t)msg;
	
	HAL_I2C_Mem_Read(&hi2c1, (0x68 << 1), 0x75, I2C_MEMADD_SIZE_8BIT, msg, 1,
	        30);
	
	systemHalt();

	
	
	// Prepare to send data
	if (readWrite == TWI_WRITE) {		// Write operation

		// Transfer data
		memcpy(this->bufferData_, msg, msgSize);
		this->bufferIndex_ = 0;
		this->bufferLength_ = msgSize;

		// Write at device 
		halError = HAL_I2C_Mem_Write(this->i2cHandler, (devAddress << 1), reg,
		        I2C_MEMADD_SIZE_8BIT, this->bufferData_, this->bufferLength_,
		        this->timeOut_);
		if (!halError) {		// Function failed
			this->lastError_ = ERROR_UNKNOWN;
			this->lastErrorDetails_ = halError;
			uint16_t aux16 = HAL_I2C_GetState(this->i2cHandler);
			this->lastErrorDetails_ |= (aux16 << 8);
			return false;
		}
	} else {							// Read operation

		// Write at device then read from device
		halError = HAL_I2C_Mem_Read(this->i2cHandler, (devAddress << 1), reg,
		        I2C_MEMADD_SIZE_8BIT, this->bufferData_, this->bufferLength_,
		        this->timeOut_);
		if (halError != HAL_OK) {		// Function failed
			this->lastError_ = ERROR_UNKNOWN;
			this->lastErrorDetails_ = halError;
			uint16_t aux16 = HAL_I2C_GetState(this->i2cHandler);
			this->lastErrorDetails_ |= (aux16 << 8);
			return false;
		}

		// Transfer data
		memcpy(msg, this->bufferData_, msgSize);
	}

	this->lastError_ = ERROR_NONE;
	this->lastErrorDetails_ = 0;
	return true;
}

error_e I2c::getLastError(uint16_t * details)
{
	if (details != NULL) {
		*details = this->lastErrorDetails_;
	}
	return this->lastError_;
}

/*

bool I2c::init(uint32_t clockSpeed, uint8_t bufferSize)
{
	// Local variables
	uint32_t aux32 = 0;
	uint16_t aux16 = 0;
	uint8_t aux8 = 0;

	// Disables TWI and reset all registers
	clrBit(I2c_CONTROL_REG, I2c_ENABLE_BIT);
	I2c_CONTROL_REG = 0x00;
	I2c_STATUS_REG = 0x00;
	I2c_BIT_RATE_REG = 0;
	I2c_SLAVE_ADDRESS_REG = 0;
	I2c_SLAVE_MASK_REG -= 0;

	// Check for errors - Clock speed
	if (clockSpeed < TWI_MIN_BIT_RATE) {
		this->lastError_ = ERROR_CLOCK_SPEED_TOO_LOW;
		return false;
	} else if (clockSpeed > TWI_MAX_BIT_RATE) {
		this->lastError_ = ERROR_CLOCK_SPEED_TOO_HIGH;
		return false;
	}
	// Check for errors - Buffer size
	if (bufferSize < TWI_MIN_BUFFER_SIZE) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	} else if (bufferSize > TWI_MAX_BUFFER_SIZE) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_LARGE;
		return false;
	}

	// Allocate memory (Free first if already allocated)
	if (this->bufferData_ != NULL) {
		free(this->bufferData_);
		this->bufferData_ = NULL;
	}
	this->bufferData_ = (uint8_t *)calloc((bufferSize + 2), sizeof(uint8_t));
	if (this->bufferData_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return false;
	}
	this->bufferMaxSize_ = bufferSize + 2;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;

	// Evaluate BIT RATE and PRESCALER
	aux32 = systemStatus.getCpuClock();
	aux32 /= clockSpeed;
	aux16 = (uint16_t)aux32;
	aux16 -= 16;
	aux16 /= 2;
	if (aux16 <= 255) {						// Prescaler 1
		aux8 = (uint8_t)aux16;
	} else if (aux16 <= 1020) {				// Prescaler 4
		aux8 = (uint8_t)(aux16 / 4);
		I2c_STATUS_REG |= (1 << I2c_CLOCK_PRESCALER_BIT);
	} else if (aux16 <= 4080) {				// Prescaler 16
		aux8 = (uint8_t)(aux16 / 16);
		I2c_STATUS_REG |= (2 << I2c_CLOCK_PRESCALER_BIT);
	} else if (aux16 <= 16320) {			// Prescaler 64
		aux8 = (uint8_t)(aux16 / 64);
		I2c_STATUS_REG |= (3 << I2c_CLOCK_PRESCALER_BIT);
	}

	// Update TWI registers
	I2c_BIT_RATE_REG = aux8;
	I2c_DATA_REG = 0xFF;						// Release SDA
	setBit(I2c_CONTROL_REG, I2c_ENABLE_BIT);	// Activate TWI interface

	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool I2c::setTimeOut(uint8_t timeOut)
{
	this->timeOut_ = timeOut;
	this->lastError_ = ERROR_NONE;
	return true;
}

// =============================================================================
// Class private methods
// =============================================================================

bool I2c::waitWhileIsBusy(void)
{
	// Local variables
	uint32_t stopwatchMark;
	uint32_t stopwatchDeadline;

	// Evaluates stopwatch deadline
	if (this->timeOut_ == 0) {
		stopwatchDeadline = 0xFFFFFFFF;
	} else {
		stopwatchDeadline = systemStatus.readStopwatch() + this->timeOut_;
	}

	// Wait until TWI is ready for next transmission
	do {
		stopwatchMark = systemStatus.readStopwatch();
		if (stopwatchMark > stopwatchDeadline) {
			this->lastError_ = ERROR_COMMUNICATION_TIME_OUT;
			return false;
		}
	} while (isBitSet(I2c_CONTROL_REG, I2c_INT_ENABLE_BIT));

	this->lastError_ = ERROR_NONE;
	return true;
}

bool I2c::startTrasmission(void)
{
	this->lastTransOK_ = false;
	this->state_ = TWI_NO_STATE;
	I2c_CONTROL_REG = 	(1 << I2c_ENABLE_BIT) |
						(1 << I2c_INT_ENABLE_BIT) |
						(1 << I2c_INT_FLAG_BIT) |
						(1 << I2c_START_CONDITION_BIT);

	this->lastError_ = ERROR_NONE;
	return true;
}


bool I2c::readFromBuffer(uint8_t *msg, uint8_t msgSize)
{
	// Check for errors
	if (!this->lastTransOK_) {
		this->lastError_ = ERROR_COMMUNICATION_FAILED;
		return false;
	}
	for (uint8_t i = 0; i < msgSize; i++) {
		msg[i] = this->bufferData_[i + 1];
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
*/
// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Interruption handlers
// =============================================================================
/*
ISR(TWI_vect)
{
	twi.interruptHandler();
}
*/

uint32_t I2c::getHandle(void)
{
	return (uint32_t)this->i2cHandler;
}

uint32_t I2c::getBufAdd(void)
{
	return (uint32_t)this->bufAdd;
}
