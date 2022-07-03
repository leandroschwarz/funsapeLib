/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	gpdseArmSpi.cpp
 * Module:		SPI interface module for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "gpdseArmSpi.h"
#if __GPDSE_ARM_SPI_H != __BUILD_MACRO_TO_BE_CHANGED__
#	error Build mismatch on header and source code files (gpdseArmSpi).
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
// Class constructors
// =============================================================================

Spi::Spi()
{
	// Reset data members
	this->spiHandler_ = NULL;
	this->initialized_ = false;
	this->lastError_ = ERROR_NONE;
	this->activateDefaultDevice_ = NULL;
	this->deactivateDefaultDevice_ = NULL;
	this->callbacksReady_ = false;
	this->bufferData_ = NULL;
	this->bufferMaxSize_ = 0;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;
	this->timeOut_ = GPDSE_DEFAULT_SPI_TIMEOUT;
// 	this->useInterrupt_ = false;
// 	this->lsbFirst_ = false;
// 	this->tempBytesToSend_ = 0;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool Spi::init(SPI_HandleTypeDef *halSpiHandler, uint8_t bufferSize)
{
	// Check for errors
	if (halSpiHandler == NULL) {				// HAL SPI Handler
		this->lastError_ = ERROR_HANDLER_POINTER_NULL;
		return false;
	}
	if (bufferSize < SPI_MIN_BUFFER_SIZE) {		// Buffer size
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	} else if (bufferSize > SPI_MAX_BUFFER_SIZE) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_LARGE;
		return false;
	}

	// Reset data members
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;
	this->bufferMaxSize_ = 0;
	// Free memory
	if (this->bufferData_ != NULL) {
		free(this->bufferData_);
		this->bufferData_ = NULL;
	}
	this->initialized_ = false;
	this->lastError_ = ERROR_NONE;

	// Allocate memory
	this->bufferData_ = (uint8_t *)calloc(bufferSize, sizeof(uint8_t));
	if (this->bufferData_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return false;
	}
	this->bufferMaxSize_ = bufferSize;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;

	// Update handler
	this->spiHandler_ = halSpiHandler;

	// Update data members
	this->initialized_ = true;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi::setDefaultDevice(void actFuncCall(void), void deactFuncCall(void))
{
	// Check for errors
	if ((actFuncCall == NULL) || (deactFuncCall == NULL)) {
		this->lastError_ = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}

	// Update data members
	this->activateDefaultDevice_ = actFuncCall;
	this->deactivateDefaultDevice_ = deactFuncCall;
	this->callbacksReady_ = true;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi::sendData(uint8_t *msg, uint16_t msgSize)
{
	// Check for errors
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (!this->callbacksReady_) {
		this->lastError_ = ERROR_NO_DEVICE_SELECTED;
		return false;
	}
	if (msg == NULL) {
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (msgSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if (msgSize > this->bufferMaxSize_) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	}

	// Transfer data between buffers
	memcpy(this->bufferData_, msg, msgSize);
	this->bufferLength_ = msgSize;
	this->bufferIndex_ = 0;

	// Activate slave device
	this->activateDefaultDevice_();

	// Send data in blocking mode
	HAL_SPI_TransmitReceive(this->spiHandler_, this->bufferData_, this->bufferData_, this->bufferLength_, this->timeOut_);

	// Deactivate slave device
	this->deactivateDefaultDevice_();

	// Transfer data between buffers
	memcpy(msg, this->bufferData_, msgSize);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi::sendData(uint8_t reg, uint8_t *msg, uint16_t msgSize)
{
	// Check for errors
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (!this->callbacksReady_) {
		this->lastError_ = ERROR_NO_DEVICE_SELECTED;
		return false;
	}
	if (msg == NULL) {
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (msgSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if ((msgSize + 1) > this->bufferMaxSize_) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	}

	// Transfer data between buffers
	this->bufferData_[0] = reg;
	memcpy(&this->bufferData_[1], msg, msgSize);
	this->bufferLength_ = msgSize + 1;
	this->bufferIndex_ = 0;

	// Activate slave device
	this->activateDefaultDevice_();

	// Send data in blocking mode
	HAL_SPI_TransmitReceive(this->spiHandler_, this->bufferData_, this->bufferData_, this->bufferLength_, this->timeOut_);

	// Deactivate slave device
	this->deactivateDefaultDevice_();

	// Transfer data between buffers
	memcpy(msg, &this->bufferData_[1], msgSize);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

/*
bool Spi0::setInterrupt(controlMode_t controlMode)
{
	// Local variables
	vuint8_t aux8 = 0;

	// Reset data members
	this->lastError_ = ERROR_NONE;
	this->lsbFirst_ = false;
	this->mode_ = SPI_MODE_0;

	// Update SPI registers
	aux8 =	(this->useInterrupt_ << SPI_BIT_INTERRUPT_ENABLE) |
			(1 << SPI_BIT_ENABLE) |
			(this->lsbFirst_ << SPI_BIT_DATA_ORDER) |
			(this->initialized_ << SPI_BIT_MASTER_SELECT) |
			(((this->mode_ >> 1) & 0x01) << SPI_BIT_CLOCK_POLARITY) |
			((this->mode_ & 0x01) << SPI_BIT_CLOCK_PHASE) |
			(this->clockPrescaler_ << SPI_BIT_CLOCK_RATE_SELECT_0);
	SPI_REG_CONTROL = aux8;

	// Clears Interrupt flags
	aux8 = SPI_REG_STATUS;
	aux8 = SPI_REG_DATA;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
*/

error_e Spi::getLastError(void)
{
	return this->lastError_;
}

// =============================================================================
// Class private methods
// =============================================================================


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

// NONE
