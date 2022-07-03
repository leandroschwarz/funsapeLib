/* =============================================================================
 * Project:		FunSAPE++ Library
 * File name:	funsapeLibTwi.cpp
 * Module:		Twi Peripheral Interface Class for FunSAPE++ Library project
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "funsapeLibTwi.hpp"
#if __FUNSAPE_LIB_TWI_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [funsapeLibTwi.cpp] Build mismatch on header and source code files.
#endif

#include "funsapeLibSystemStatus.hpp"

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		TWI_DEFAULT_TIME_OUT			20

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

// *INDENT-OFF*

#if TWI0_EXISTS
	Twi0 twi0;
#endif

// *INDENT-ON*

// =============================================================================
// Class constructors
// =============================================================================

#if TWI0_EXISTS

Twi0::Twi0()
{
	// Cleaning workspace
	this->_bufferData = NULL;			// Avoid trying to freeing unallocated memory
	this->_resetData();					// Reset data members
	this->_resetRegisters();			// Reset peripheral

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

Twi0::~Twi0()
{
	// Cleaning workspace
	this->_resetData();					// Reset data members
	this->_resetRegisters();			// Reset peripheral

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

#endif

// =============================================================================
// Class public methods
// =============================================================================

#if TWI0_EXISTS

bool Twi0::initialize(uint8_t bufferSize)
{
	// Check for errors (buffer size)
	if (bufferSize < TWI0_MIN_BUFFER_SIZE) {				// If buffer size is too small
		this->_lastError = ERROR_BUFFER_SIZE_TOO_SMALL;		// Report error
		return false;										// Return with error
	} else if (bufferSize > TWI0_MAX_BUFFER_SIZE) {		// If buffer size is too large
		this->_lastError = ERROR_BUFFER_SIZE_TOO_LARGE;		// Report error
		return false;										// Return with error
	}

	// Clear buffer member information
	this->_bufferIndex = 0;
	this->_bufferLength = 0;
	this->_bufferMaxSize = 0;

	// Allocate memory (free memory first if it is already allocated)
	if (this->_bufferData != NULL) {					// If memory is allocated
		free(this->_bufferData);							// Free memory
		this->_bufferData = NULL;							// Erase pointer data
	}
	this->_bufferData = (uint8_t *)calloc(    // Allocate memory
							(bufferSize + 2),				// Include slave address and register
							sizeof(uint8_t)
						);
	if (this->_bufferData == NULL) {					// If memory allocation failed
		this->_lastError = ERROR_MEMORY_ALLOCATION;			// Report error
		return false;										// Return with error
	}
	this->_bufferMaxSize = bufferSize + 2;				// Update buffer member information

	// Initialization procedure was successful
	this->_initialized = true;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Twi0::setMaster(uint32_t bitRate)
{
	// Local variables
	uint32_t aux32 = 0;

	// Check for errors (bit rate)
	if (bitRate < TWI0_MIN_BIT_RATE) {				// If bit rate is too slow
		this->_lastError = ERROR_CLOCK_SPEED_TOO_LOW;		// Report error
		return false;										// Return with error
	} else if (bitRate > TWI0_MAX_BIT_RATE) {			// If bit rate is too fast
		this->_lastError = ERROR_CLOCK_SPEED_TOO_HIGH;		// Report error
		return false;										// Return with error
	}

	// Evaluate bit rate and prescaler
	aux32 = bitRateRegisterValue(systemStatus.getCpuClock(), bitRate);
	if (aux32 <= 255) {									// Set prescaler 1
		this->_bitRateRegister		= (uint8_t)aux32;
		this->_bitRatePrescaler		= TWI0_PRESCALER_1;
	} else if (aux32 <= 1020) {							// Set prescaler 4
		this->_bitRateRegister		= (uint8_t)(aux32 / 4);
		this->_bitRatePrescaler		= TWI0_PRESCALER_4;
	} else if (aux32 <= 4080) {							// Set prescaler 16
		this->_bitRateRegister		= (uint8_t)(aux32 / 16);
		this->_bitRatePrescaler		= TWI0_PRESCALER_16;
	} else if (aux32 <= 16320) {						// Set prescaler 64
		this->_bitRateRegister		= (uint8_t)(aux32 / 64);
		this->_bitRatePrescaler		= TWI0_PRESCALER_64;
	} else {											// No valid prescaler found
		this->_lastError = ERROR_UNKNOWN;					// Report error (TODO)
		return false;										// Return with error
	}

	// Stop transmissions
	this->_disable();

	// Update peripheral registers
	TWI0_BIT_RATE_REG = this->_bitRateRegister;
	clrMaskOffset(TWI0_CLOCK_PRESCALER_REG, 0x03, TWI0_CLOCK_PRESCALER_BIT);
	setMaskOffset(TWI0_CLOCK_PRESCALER_REG, this->_bitRatePrescaler, TWI0_CLOCK_PRESCALER_BIT);
	setBit(TWI0_ENABLE_REG, TWI0_ENABLE_BIT);			// Mark master mode as enabled
	this->_masterEnabled		= true;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

error_e Twi0::getLastError(void)
{
	return this->_lastError;
}

void Twi0::interruptHandler(void)
{
	static uint8_t twiBufferPointer;

	switch (TWI0_STATUS_REG & TWI0_STATUS_MASK) {
	case TWI_START:				// START has been transmitted
	case TWI_REP_START:			// Repeated START has been transmitted
		twiBufferPointer = 0;	// Set buffer pointer to the TWI Address location
	case TWI_MTX_ADR_ACK:		// SLA+W has been transmitted and ACK received
	case TWI_MTX_DATA_ACK:		// Data byte has been transmitted and ACK received
		if (twiBufferPointer < this->_bufferLength) {
			TWI0_DATA_REG = this->_bufferData[twiBufferPointer++];
			TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
								(1 << TWI0_INT_ENABLE_BIT) |
								(1 << TWI0_INT_FLAG_BIT);
		} else {			// Send STOP after last byte
			this->_lastTransOk = true;	// Set status bits to completed successfully
			TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
								(1 << TWI0_INT_FLAG_BIT) |
								(1 << TWI0_SEND_STOP_BIT);
		}
		break;
	case TWI_MRX_DATA_ACK:		// Data byte has been received and ACK transmitted
		this->_bufferData[twiBufferPointer++] = TWI0_DATA_REG;
	case TWI_MRX_ADR_ACK:		// SLA+R has been transmitted and ACK received
		if (twiBufferPointer < (this->_bufferLength - 1)) {	// Detect the last byte to NACK it
			TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
								(1 << TWI0_INT_ENABLE_BIT) |
								(1 << TWI0_INT_FLAG_BIT) |
								(1 << TWI0_SEND_ACK_BIT);			// ACK
		} else {				// Send NACK after next reception
			TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
								(1 << TWI0_INT_ENABLE_BIT) |
								(1 << TWI0_INT_FLAG_BIT) |
								(0 << TWI0_SEND_ACK_BIT);			// NACK
		}
		break;
	case TWI_MRX_DATA_NACK:		// Data byte has been received and NACK transmitted
		this->_bufferData[twiBufferPointer] = TWI0_DATA_REG;
		this->_lastTransOk = true;	// Set status bits to completed successfully
		TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
							(1 << TWI0_INT_FLAG_BIT) |
							(1 << TWI0_SEND_STOP_BIT);
		break;
	case TWI_ARB_LOST:			// Arbitration lost
		TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
							(1 << TWI0_INT_ENABLE_BIT) |
							(1 << TWI0_INT_FLAG_BIT) |
							(1 << TWI0_SEND_START_BIT);
		break;
	case TWI_MTX_ADR_NACK:		// SLA+W has been transmitted and NACK received
	case TWI_MRX_ADR_NACK:		// SLA+R has been transmitted and NACK received
	case TWI_MTX_DATA_NACK:		// Data byte has been transmitted and NACK received
	case TWI_BUS_ERROR:			// Bus error due to an illegal START or STOP condition
	default:
		this->_state = (twiState_e)TWI0_STATUS_REG;		// Store TWI0_STATUS_REG and automatically sets clears noErrors bit
		TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
							(1 << TWI0_SEND_STOP_BIT);
		break;
	}
}

bool Twi0::sendData(uint8_t devAddress, twiOperation_t readWrite, uint8_t regAddress, uint8_t *msg, uint8_t msgSize)
{
	// Check for errors (not initialized)
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	// Check for errors (master not enabled)
	if (!this->_masterEnabled) {
		this->_lastError = ERROR_MASTER_NOT_ENABLED;
		return false;
	}
	// Check for errors (Null buffer pointer)
	if ((msgSize > 0) && (msg == NULL)) {
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}

	// Wait until last transmission ends
	if (!this->_waitWhileIsBusy()) {
		return false;
	}

	// Prepare to send data
	if (readWrite == TWI_WRITE) {		// Write operation
		this->_bufferData[0] = (devAddress << 1) | TWI_WRITE;	// [0] => Device address + WR
		this->_bufferData[1] = regAddress;						// [1] => Register address
		for (uint8_t i = 1; i < msgSize; i++) {
			this->_bufferData[i + 1] = msg[i];					// [2-] => Message
		}
		this->_bufferLength = msgSize + 2;
		this->_startTrasmission();
	} else {							// Read operation
		// Set pointer first
		this->_bufferData[0] = (devAddress << 1) | TWI_WRITE;	// [0] => Device address + WR
		this->_bufferData[1] = regAddress;						// [1] => Register address
		this->_bufferLength = 2;
		this->_startTrasmission();
		// Wait last transmission ends
		if (!this->_waitWhileIsBusy()) {
			return false;
		}
		// Read data
		this->_bufferData[0] = (devAddress << 1) | TWI_READ;	// [0] => Device address + RD
		this->_bufferLength = msgSize + 1;
		this->_startTrasmission();
		if (!this->_waitWhileIsBusy()) {
			return false;
		}
		if (!this->_readFromBuffer(msg, msgSize)) {
			return false;
		}
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Twi0::setTimeOut(uint8_t timeOut)
{
	this->_timeOut = timeOut;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}
#endif

// =============================================================================
// Class private methods
// =============================================================================

#if TWI0_EXISTS

void Twi0::_resetData(void)
{
	// Deallocate memory
	if (this->_bufferData != NULL) {					// If memory is allocated
		free(this->_bufferData);							// Free memory
		this->_bufferData = NULL;							// Erase pointer data
	}

	// Reset data members
	this->_initialized			= false;
	this->_masterEnabled		= false;
	this->_slaveEnabled			= false;
	this->_lastTransOk			= false;
	this->_bufferMaxSize		= 0;
	this->_bufferIndex			= 0;
	this->_bufferLength			= 0;
	this->_timeOut				= TWI_DEFAULT_TIME_OUT;
	this->_state				= TWI_NO_STATE;
	this->_tempBytesToSend		= 0;
	this->_bitRateRegister		= 0;
	this->_bitRatePrescaler		= TWI0_PRESCALER_1;

	// Return successfully
	return;
}

void Twi0::_resetRegisters(void)
{
	// Reset all registers
	TWI0_CONTROL_REG			= 0x00;
	this->_masterEnabled		= false;
	TWI0_STATUS_REG				= 0x00;
	TWI0_BIT_RATE_REG			= 0x00;
	TWI0_SLAVE_ADDRESS_REG		= 0x00;
	TWI0_SLAVE_MASK_REG			= 0x00;

	// Return successfully
	return;
}

void Twi0::_disable(void)
{
	// Disabling peripheral
	clrBit(TWI0_ENABLE_REG, TWI0_ENABLE_BIT);			// Stop peripheral
	this->_masterEnabled		= false;					// Mark master mode as disabled
	this->_lastTransOk			= false;					// Mark last transmission as error
	this->_state				= TWI_NO_STATE;				// Reset states machine
	this->_bufferIndex			= 0;						// Reset internal buffer index
	this->_bufferLength			= 0;						// Mark internal buffer as empty
	this->_tempBytesToSend		= 0;						// No more bytes to send

	// Return successfully
	return;
}

bool Twi0::_waitWhileIsBusy(void)
{
	// Local variables
	uint32_t stopwatchMark;
	uint32_t stopwatchDeadline;

	// Calculate stopwatch deadline
	if (this->_timeOut == 0) {
		stopwatchDeadline = 0xFFFFFFFF;
	} else {
		stopwatchDeadline = systemStatus.readStopwatch() + this->_timeOut;
	}

	// Wait until TWI is ready for next transmission
	do {		// If TWI Interrupt is enabled then the transceiver is busy
		stopwatchMark = systemStatus.readStopwatch();
		if (stopwatchMark > stopwatchDeadline) {
			this->_lastError = ERROR_COMMUNICATION_TIME_OUT;
			return false;
		}
	} while (isBitSet(TWI0_CONTROL_REG, TWI0_INT_ENABLE_BIT));

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Twi0::_startTrasmission(void)
{
	this->_lastTransOk = false;
	this->_state = TWI_NO_STATE;
	TWI0_CONTROL_REG =	(1 << TWI0_ENABLE_BIT) |
						(1 << TWI0_INT_ENABLE_BIT) |
						(1 << TWI0_INT_FLAG_BIT) |
						(1 << TWI0_SEND_START_BIT);

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Twi0::_readFromBuffer(uint8_t *msg, uint8_t msgSize)
{
	// Check for errors
	if (!this->_lastTransOk) {
		this->_lastError = ERROR_COMMUNICATION_FAILED;
		return false;
	}

	// Transfer buffers
	for (uint8_t i = 0; i < msgSize; i++) {
		msg[i] = this->_bufferData[i + 1];
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

#endif

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

#if TWI0_EXISTS

ISR(TWI_vect)
{
	twi0.interruptHandler();
}

#endif
