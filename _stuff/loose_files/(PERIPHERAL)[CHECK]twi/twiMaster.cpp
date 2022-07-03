/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	twiMaster.cpp
 * Module:		TWI Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 *				Fábio Cabral Pacheco
 * Build:		1002
 * Date:		June 14, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "twiMaster.hpp"
#if __TWI_MASTER_HPP != 1002
#	error [twiMaster.cpp] Error 102 - Build mismatch on header and source code files.
#endif

#include "system.hpp"

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		TWI_REG_BIT_RATE				TWBR
#define		TWI_REG_STATUS					TWSR
#	define	TWI_BIT_STATUS					TWS3
#	define	TWI_BIT_PRESCALER				TWPS0
#define		TWI_REG_SLAVE_ADDRESS			TWAR
#	define	TWI_BIT_SLAVE_ADDRESS			TWA0
#	define	TWI_BIT_GENERAL_CALL_ENABLE		TWGCE
#define		TWI_REG_DATA					TWDR
#define		TWI_REG_CONTROL					TWCR
#	define	TWI_BIT_INTERRUPT_FLAG			TWINT
#	define	TWI_BIT_ENABLE_ACK				TWEA
#	define	TWI_BIT_START_CONDITION			TWSTA
#	define	TWI_BIT_STOP_CONDITION			TWSTO
#	define	TWI_BIT_WRITE_COLLISION_FLAG	TWWC
#	define	TWI_BIT_ENABLE					TWEN
#	define	TWI_BIT_INTERRUPT_ENABLE		TWIE
#define		TWI_REG_ADDRESS_MASK			TWAMR
#	define	TWI_BIT_ADDRESS_MASK			TWAM0

#define		TWI_MAX_BIT_RATE				400000UL
#define		TWI_MIN_BIT_RATE				1000UL
#define		TWI_MIN_BUFFER_SIZE				10
#define		TWI_MAX_BUFFER_SIZE				100
#define		TWI_DEFAULT_TIME_OUT			20

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
// =============================================================================

TwiMaster twiMaster;

// =============================================================================
// Class constructors
// =============================================================================

TwiMaster::TwiMaster()
{
	// Reset data members
	this->initialized_ = false;
	this->lastTransOK_ = false;
	this->bufferData_ = NULL;
	this->bufferMaxSize_ = 0;
	this->bufferIndex_ = 0;
	this->bufferLength_ = 0;
	this->timeOut_ = TWI_DEFAULT_TIME_OUT;
	this->lastError_ = ERROR_NONE;

	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool TwiMaster::init(uint32_t clockSpeed, uint8_t bufferSize)
{
	// Local variables
	uint32_t aux32 = 0;
	uint16_t aux16 = 0;
	uint8_t aux8 = 0;

	// Disables TWI and reset all registers
	clrBit(TWI_REG_CONTROL, TWI_BIT_ENABLE);
	TWI_REG_CONTROL = 0x00;
	TWI_REG_STATUS = 0x00;
	TWI_REG_BIT_RATE = 0;
	TWI_REG_SLAVE_ADDRESS = 0;
	TWI_REG_ADDRESS_MASK -= 0;

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
		TWI_REG_STATUS |= (1 << TWI_BIT_PRESCALER);
	} else if (aux16 <= 4080) {				// Prescaler 16
		aux8 = (uint8_t)(aux16 / 16);
		TWI_REG_STATUS |= (2 << TWI_BIT_PRESCALER);
	} else if (aux16 <= 16320) {			// Prescaler 64
		aux8 = (uint8_t)(aux16 / 64);
		TWI_REG_STATUS |= (3 << TWI_BIT_PRESCALER);
	}

	// Update TWI registers
	TWI_REG_BIT_RATE = aux8;
	TWI_REG_DATA = 0xFF;						// Release SDA
	setBit(TWI_REG_CONTROL, TWI_BIT_ENABLE);	// Activate TWI interface

	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool TwiMaster::setTimeOut(uint8_t timeOut)
{
	this->timeOut_ = timeOut;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool TwiMaster::sendData(uint8_t devAddress, twiOperation_t readWrite, uint8_t reg, uint8_t *msg, uint8_t msgSize)
{
	// Check for errors - NOT Initialized
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	// Check for errors - Message pointer
	if ((msgSize > 0) && (msg == NULL)) {
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}

	// Wait last transmission ends
	if (!waitWhileIsBusy()) {
		return false;
	}

	// Prepare to send data
	if (readWrite == TWI_WRITE) {		// Write operation
		this->bufferData_[0] = (devAddress << 1) | TWI_WRITE;
		this->bufferData_[1] = reg;
		for (uint8_t i = 1; i < msgSize; i++) {
			this->bufferData_[i + 1] = msg[i];
		}
		this->bufferLength_ = msgSize + 2;
		this->startTrasmission();
	} else {							// Read operation
		// Set pointer first
		this->bufferData_[0] = (devAddress << 1) | TWI_WRITE;
		this->bufferData_[1] = reg;
		this->bufferLength_ = 2;
		this->startTrasmission();
		// Wait last transmission ends
		if (!waitWhileIsBusy()) {
			return false;
		}
		// Read data
		this->bufferData_[0] = (devAddress << 1) | TWI_READ;
		this->bufferLength_ = msgSize + 1;
		this->startTrasmission();
		if (!waitWhileIsBusy()) {
			return false;
		}
		if (!this->readFromBuffer(msg, msgSize)) {
			return false;
		}
	}

	this->lastError_ = ERROR_NONE;
	return true;;
}

error_e TwiMaster::getLastError(void)
{
	return this->lastError_;
}

void TwiMaster::interruptHandler(void)
{
	static uint8_t twiBufferPointer;

	switch (TWI_REG_STATUS & 0xFC) {
	case TWI_START:				// START has been transmitted
	case TWI_REP_START:			// Repeated START has been transmitted
		twiBufferPointer = 0;	// Set buffer pointer to the TWI Address location
	case TWI_MTX_ADR_ACK:		// SLA+W has been transmitted and ACK received
	case TWI_MTX_DATA_ACK:		// Data byte has been transmitted and ACK received
		if (twiBufferPointer < this->bufferLength_) {
			TWDR = this->bufferData_[twiBufferPointer++];
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
		} else {			// Send STOP after last byte
			this->lastTransOK_ = true;	// Set status bits to completed successfully
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
		}
		break;
	case TWI_MRX_DATA_ACK:		// Data byte has been received and ACK transmitted
		this->bufferData_[twiBufferPointer++] = TWDR;
	case TWI_MRX_ADR_ACK:		// SLA+R has been transmitted and ACK received
		if (twiBufferPointer < (this->bufferLength_ - 1)) {	// Detect the last byte to NACK it
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
		} else {				// Send NACK after next reception
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
		}
		break;
	case TWI_MRX_DATA_NACK:		// Data byte has been received and NACK transmitted
		this->bufferData_[twiBufferPointer] = TWDR;
		this->lastTransOK_ = true;	// Set status bits to completed successfully
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
		break;
	case TWI_ARB_LOST:			// Arbitration lost
		TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWSTA);
		break;
	case TWI_MTX_ADR_NACK:		// SLA+W has been transmitted and NACK received
	case TWI_MRX_ADR_NACK:		// SLA+R has been transmitted and NACK received
	case TWI_MTX_DATA_NACK:		// Data byte has been transmitted and NACK received
	case TWI_BUS_ERROR:			// Bus error due to an illegal START or STOP condition
	default:
		this->state_ = (twiState_e)TWI_REG_STATUS;		// Store TWI_REG_STATUS and automatically sets clears noErrors bit
		TWCR = (1 << TWEN);		// Reset TWI Interface
		break;
	}
}

// =============================================================================
// Class private methods
// =============================================================================

bool TwiMaster::waitWhileIsBusy(void)
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
	} while (isBitSet(TWI_REG_CONTROL, TWI_BIT_INTERRUPT_ENABLE));

	this->lastError_ = ERROR_NONE;
	return true;
}

bool TwiMaster::startTrasmission(void)
{
	this->lastTransOK_ = false;
	this->state_ = TWI_NO_STATE;
	TWI_REG_CONTROL = 	(1 << TWI_BIT_ENABLE) |
						(1 << TWI_BIT_INTERRUPT_ENABLE) |
						(1 << TWI_BIT_INTERRUPT_FLAG) |
						(1 << TWI_BIT_START_CONDITION);

	this->lastError_ = ERROR_NONE;
	return true;
}


bool TwiMaster::readFromBuffer(uint8_t *msg, uint8_t msgSize)
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

ISR(TWI_vect)
{
	twiMaster.interruptHandler();
}
