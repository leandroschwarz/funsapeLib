/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	spi.cpp
 * Module:		SPI Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 *				Fábio Cabral Pacheco
 * Build:		1002
 * Date:		July 04, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "spi.hpp"
#if __SPI_HPP != 1002
#	error [spi.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// Header files
#include "system.hpp"
#include <stdlib.h>
#include <avr/interrupt.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		SPI_REG_CONTROL					SPCR
#	define	SPI_BIT_INTERRUPT_ENABLE		SPIE
#	define	SPI_BIT_ENABLE					SPE
#	define	SPI_BIT_DATA_ORDER				DORD
#	define	SPI_BIT_MASTER_SELECT			MSTR
#	define	SPI_BIT_CLOCK_POLARITY			CPOL
#	define	SPI_BIT_CLOCK_PHASE				CPHA
#	define	SPI_BIT_CLOCK_RATE_SELECT_0		SPR0
#define		SPI_REG_STATUS					SPSR
#	define	SPI_BIT_INTERRUPT_FLAG			SPIF
#	define	SPI_BIT_WRITE_COLLISION_FLAG	WCOL
#	define	SPI_BIT_DOUBLE_SPEED			SPI2X
#define		SPI_REG_DATA					SPDR

#define		SPI_MAX_CLOCK_PRESCALER			128
#define		SPI_MIN_BUFFER_SIZE				5
#define		SPI_MAX_BUFFER_SIZE				100
#define		SPI_DEFAULT_TIME_OUT			20

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// *INDENT-OFF*
// =============================================================================

#if		SPI0_EXISTS
	Spi0 spi0;
#endif
#if		SPI1_EXISTS
	Spi1 spi1;
#endif
// *INDENT-ON*

// =============================================================================
// Class constructors
// =============================================================================

// -----------------------------------------------------------------------------
// Spi0 ------------------------------------------------------------------------

#if		SPI0_EXISTS
Spi0::Spi0()
{
	// Reset data members
	this->bufferData_ = NULL;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;
	this->bufferMaxSize_ = 0;
	this->clockPrescaler_ = SPI_PRESCALER_4;
	this->doubleSpeed_ = false;
	this->masterInitialized_ = false;
	this->slaveInitialized_ = false;
	this->useInterrupt_ = false;
	this->lastError_ = ERROR_NONE;
	this->lsbFirst_ = false;
	this->mode_ = SPI_MODE_0;
	this->timeOut_ = SPI_DEFAULT_TIME_OUT;
	this->activateDefaultDevice_ = NULL;
	this->deactivateDefaultDevice_ = NULL;
	this->callbacksReady_ = false;
	return;
}
#endif

// =============================================================================
// Class public methods
// =============================================================================

// -----------------------------------------------------------------------------
// Spi0 ------------------------------------------------------------------------

#if		SPI0_EXISTS
bool Spi0::setDefaultDevice(void actFuncCall(void), void deactFuncCall(void))
{
	// Check for errors
	if ((actFuncCall == NULL) || (deactFuncCall == NULL)) {
		this->lastError_ = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}

	this->activateDefaultDevice_ = actFuncCall;
	this->deactivateDefaultDevice_ = deactFuncCall;
	this->callbacksReady_ = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi0::sendData(uint8_t *msg, uint8_t msgSize)
{
	// Check for errors
	if (!this->masterInitialized_) {
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

	// Activate device
	this->holdGpio();
	this->activateDefaultDevice_();

	// Send by pooling
	while (this->bufferIndex_ < this->bufferLength_) {
		// Send data
		SPI_REG_DATA = this->bufferData_[this->bufferIndex_];
		// Wait transfer finishes
		waitUntilBitIsSet(SPI_REG_STATUS, SPI_BIT_INTERRUPT_FLAG);
		// Get data
		this->bufferData_[this->bufferIndex_] = SPI_REG_DATA;
		// Next byte
		this->bufferIndex_++;
	}

	// Deactivate device
	this->deactivateDefaultDevice_();
	this->releaseGpio();

	// Transfer data between buffers
	memcpy(msg, this->bufferData_, msgSize);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi0::sendData(uint8_t reg, uint8_t *msg, uint8_t msgSize)
{
	// Check for errors
	if (!this->masterInitialized_) {
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

	// Activate device
	this->holdGpio();
	this->activateDefaultDevice_();

	// Send by pooling
	while (this->bufferIndex_ < this->bufferLength_) {
		// Send data
		SPI_REG_DATA = this->bufferData_[this->bufferIndex_];
		// Wait transfer finishes
		waitUntilBitIsSet(SPI_REG_STATUS, SPI_BIT_INTERRUPT_FLAG);
		// Get data
		this->bufferData_[this->bufferIndex_] = SPI_REG_DATA;
		// Next byte
		this->bufferIndex_++;
	}

	// Deactivate device
	this->deactivateDefaultDevice_();
	this->releaseGpio();

	// Transfer data between buffers
	memcpy(msg, &this->bufferData_[1], msgSize);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi0::init(uint32_t maxSpeed, uint8_t bufferSize)
{
	uint32_t aux32 = 0;
	uint8_t aux8 = 0;

	// Disables SPI peripheral
	SPI_REG_STATUS = 0;
	SPI_REG_CONTROL = 0;
	aux8 = SPI_REG_STATUS;
	aux8 = SPI_REG_DATA;

	// Reset data members
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;
	this->bufferMaxSize_ = 0;
	if (this->bufferData_ != NULL) {
		free(this->bufferData_);
		this->bufferData_ = NULL;
	}
	this->clockPrescaler_ = SPI_PRESCALER_4;
	this->doubleSpeed_ = false;
	this->masterInitialized_ = false;
	this->lastError_ = ERROR_NONE;

	// Check for errors
	if (maxSpeed == 0) {						// Clock speed
		this->lastError_ = ERROR_CLOCK_SPEED_TOO_LOW;
		return false;
	}
	if (bufferSize < SPI_MIN_BUFFER_SIZE) {		// Buffer size
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	} else if (bufferSize > SPI_MAX_BUFFER_SIZE) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_LARGE;
		return false;
	}

	// Allocate memory
	this->bufferData_ = (uint8_t *)calloc(bufferSize, sizeof(uint8_t));
	if (this->bufferData_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return false;
	}
	this->bufferMaxSize_ = bufferSize;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;

	// Clock prescaler selection
	aux32 = systemStatus.getCpuClock();
	aux32 /= maxSpeed;
	aux8 = (uint8_t)aux32;
	if (aux8 > SPI_MAX_CLOCK_PRESCALER) {
		this->lastError_ = ERROR_CLOCK_SPEED_TOO_HIGH;
		return false;
	} else if (aux8 > 64) {	// Prescaler result factor = 128
		this->clockPrescaler_ = SPI_PRESCALER_128;
	} else if (aux8 > 32) {	// Prescaler result factor = 64
		this->clockPrescaler_ = SPI_PRESCALER_64;
	} else if (aux8 > 16) {	// Prescaler result factor = 32
		this->clockPrescaler_ = SPI_PRESCALER_64;
		this->doubleSpeed_ = true;
	} else if (aux8 > 8) {	// Prescaler result factor = 16
		this->clockPrescaler_ = SPI_PRESCALER_16;
	} else if (aux8 > 4) {	// Prescaler result factor = 8
		this->clockPrescaler_ = SPI_PRESCALER_16;
		this->doubleSpeed_ = true;
	} else if (aux8 > 2) {	// Prescaler result factor = 4
		this->clockPrescaler_ = SPI_PRESCALER_4;
	} else {				// Prescaler result factor = 2
		this->clockPrescaler_ = SPI_PRESCALER_4;
		this->doubleSpeed_ = true;
	}

	// Update data members
	this->masterInitialized_ = true;

	// Update SPI registers
	aux8 = (this->doubleSpeed_ << SPI_BIT_DOUBLE_SPEED);
	SPI_REG_STATUS = aux8;
	aux8 =	(this->useInterrupt_ << SPI_BIT_INTERRUPT_ENABLE) |
			(1 << SPI_BIT_ENABLE) |
			(this->lsbFirst_ << SPI_BIT_DATA_ORDER) |
			(this->masterInitialized_ << SPI_BIT_MASTER_SELECT) |
			(((this->mode_ >> 1) & 0x01) << SPI_BIT_CLOCK_POLARITY) |
			((this->mode_ & 0x01) << SPI_BIT_CLOCK_PHASE) |
			(this->clockPrescaler_ << SPI_BIT_CLOCK_RATE_SELECT_0);
	SPI_REG_CONTROL = aux8;

	// GPIO configuration
	this->holdGpio();

	// Clears Interrupt flags
	aux8 = SPI_REG_STATUS;
	aux8 = SPI_REG_DATA;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Spi0::setMode(spiMode_e mode, bool lsbFirst)
{
	// Local variables
	vuint8_t aux8 = 0;

	// Disables SPI peripheral
	SPI_REG_STATUS = 0;
	SPI_REG_CONTROL = 0;
	aux8 = SPI_REG_STATUS;
	aux8 = SPI_REG_DATA;

	// Reset all data members
	this->lastError_ = ERROR_NONE;
	this->lsbFirst_ = false;
	this->mode_ = SPI_MODE_0;

	// Update data members
	this->mode_ = mode;
	this->lsbFirst_ = lsbFirst;

	// Update SPI registers
	aux8 = (this->doubleSpeed_ << SPI_BIT_DOUBLE_SPEED);
	SPI_REG_STATUS = aux8;
	aux8 =	(this->useInterrupt_ << SPI_BIT_INTERRUPT_ENABLE) |
			(1 << SPI_BIT_ENABLE) |
			(this->lsbFirst_ << SPI_BIT_DATA_ORDER) |
			(this->masterInitialized_ << SPI_BIT_MASTER_SELECT) |
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
			(this->masterInitialized_ << SPI_BIT_MASTER_SELECT) |
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

error_e Spi0::getLastError(void)
{
	return this->lastError_;
}

void Spi0::interruptHandler(void)
{
	return;
}
#endif

// =============================================================================
// Class private methods
// =============================================================================

// -----------------------------------------------------------------------------
// Spi0 ------------------------------------------------------------------------

#if		SPI0_EXISTS
void Spi0::holdGpio(void)
{
	clrBit(SPI0_MISO_OUT, SPI0_MISO_BIT);
	clrBit(SPI0_MOSI_OUT, SPI0_MOSI_BIT);
	clrBit(SPI0_SCLK_OUT, SPI0_SCLK_BIT);
	clrBit(SPI0_MISO_DIR, SPI0_MISO_BIT);
	setBit(SPI0_MOSI_DIR, SPI0_MOSI_BIT);
	setBit(SPI0_SCLK_DIR, SPI0_SCLK_BIT);

	setBit(SPI0_SS_OUT, SPI0_SS_BIT);
	setBit(SPI0_SS_DIR, SPI0_SS_BIT);

	return;
}

void Spi0::releaseGpio(void)
{
	clrBit(SPI0_MISO_OUT, SPI0_MISO_BIT);
	clrBit(SPI0_MOSI_OUT, SPI0_MOSI_BIT);
	clrBit(SPI0_SCLK_OUT, SPI0_SCLK_BIT);
	clrBit(SPI0_MISO_DIR, SPI0_MISO_BIT);
	clrBit(SPI0_MOSI_DIR, SPI0_MOSI_BIT);
	clrBit(SPI0_SCLK_DIR, SPI0_SCLK_BIT);

	setBit(SPI0_SS_OUT, SPI0_SS_BIT);
	clrBit(SPI0_SS_DIR, SPI0_SS_BIT);

	return;
}
#endif

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */

// =============================================================================
// General public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Interruption handlers
// =============================================================================

// -----------------------------------------------------------------------------
// Spi0 ------------------------------------------------------------------------

#if		SPI0_EXISTS
ISR(SPI_STC_vect)
{
	spi0.interruptHandler();
}
#endif
