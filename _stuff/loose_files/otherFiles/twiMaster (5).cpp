/* -----------------------------------------------------------------------------
 * Project:		GPSDE++ AVR Library
 * File name:	twiMaster.cpp
 * Module:		TWI Interface Module for GPDSE++ AVR Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 *				Fábio Cabral Pacheco
 * Build:		1001
 * Date:		May 26, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalConfigs.hpp"
#ifndef __GLOBAL_CONFIGS_HPP
#	error	[twiMaster.cpp] Error 100 - System configuration file (globalConfigs.hpp) is missing
#else
#	if __GLOBAL_CONFIGS_HPP != 1001
#		error 	[twiMaster.cpp] Error 101 - Build mismatch (globalConfigs.hpp and twiMaster.hpp)
#	endif		// __GLOBAL_CONFIGS_HPP != 1001
#endif		// __GLOBAL_CONFIGS_HPP

#if __USE_OWN_DEFINITIONS_FILE == 1
#	include "userDefines.hpp"
#else
#	include "globalDefines.hpp"
#	if __GLOBAL_DEFINES_HPP != 1001
#		error 	[twiMaster.cpp] Error 102 - Build mismatch (globalDefines.hpp and twiMaster.hpp)
#	endif		// __GLOBAL_CONFIGS_HPP != 1001
#endif		// __USE_OWN_DEFINITIONS_FILE == 1

#include "twiMaster.hpp"
#if __TWI_MASTER_HPP != 1001
#	error [twiMaster.cpp] Error 103 - Build mismatch on header and source code files.
#endif

// -----------------------------------------------------------------------------
// File exclusive - Constant definitions ---------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - New data types ---------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - Macro-functions definitions --------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

TwiMaster twiMaster;

// -----------------------------------------------------------------------------
// TwiMaster class - Constructors definitions ----------------------------------

TwiMaster::TwiMaster()
{
	// Clear flags
	this->lastTransOK_ = FALSE;
	this->state_ = TWI_NO_STATE;

	return;
}

// -----------------------------------------------------------------------------
// TwiMaster class - Public methods definitions --------------------------------

void TwiMaster::setClockSpeed(uint32_t clockSpeed)
{
	uint32_t aux32 = 0;
	uint8_t aux8 = 0;

	aux32 = (uint32_t)systemConfiguration.getCpuClock() / (uint32_t)clockSpeed;

	if(aux32 <= 526) {					// Prescaler 1
		aux8 = (uint8_t)((aux32 - 16) / 2);
		clrBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT1);
		clrBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT0);
	} else if(aux32 <= 2056) {			// Prescaler 4
		aux8 = (uint8_t)((aux32 - 16) / 8);
		clrBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT1);
		setBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT0);
	} else if(aux32 <= 8176) {			// Prescaler 16
		aux8 = (uint8_t)((aux32 - 16) / 32);
		setBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT1);
		clrBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT0);
	} else if(aux32 <= 32656) {			// Prescaler 64
		aux8 = (uint8_t)((aux32 - 16) / 128);
		setBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT1);
		setBit(TWI_STATUS_REGISTER, TWI_STATUS_PRESCALER_BIT0);
	} else {
		return;
	}
	TWI_BAUD_RATE_REGISTER = aux8;
	TWI_DATA_REGISTER = 0xFF;									// Release SDA
	TWI_CONTROL_REGISTER = 1 << TWI_CONTROL_ENABLE_BIT;			// Activate TWI interface

	return;
}

void TwiMaster::sendData(uint8_t deviceAddress, twiOperation_e readWrite, uint8_t *message, uint8_t messageSize)
{
	uint8_t i;

	while(this->isBusy())
		;	// Wait until TWI is ready for next transmission.

	this->bufferData_[0] = (deviceAddress << 1) | readWrite;
	this->bufferSize_ = messageSize + 1;
	if(readWrite == TWI_WRITE) {
		for(i = 0; i < messageSize; i++) {
			this->bufferData_[i + 1] = message[i];
		}
	}

	this->lastTransOK_ = FALSE;
	this->state_ = TWI_NO_STATE;
	TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT) | (1 << TWI_CONTROL_START_BIT);
	if(readWrite == TWI_READ) {
		this->readFromBuffer(message, messageSize);
	}

	while(this->isBusy())
		;	// Wait until TWI is ready for next transmission.

	return;
}

void TwiMaster::interruptHandler(void)
{
	static uint8_t twiBufferPointer;

	switch(TWI_STATUS_REGISTER & 0xFC) {
	case TWI_START:				// START has been transmitted
	case TWI_REP_START:			// Repeated START has been transmitted
		twiBufferPointer = 0;	// Set buffer pointer to the TWI Address location
	case TWI_MTX_ADR_ACK:		// SLA+W has been transmitted and ACK received
	case TWI_MTX_DATA_ACK:		// Data byte has been transmitted and ACK received
		if(twiBufferPointer < this->bufferSize_) {
			TWI_DATA_REGISTER = this->bufferData_[twiBufferPointer++];
			TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT);
		} else {			// Send STOP after last byte
			this->lastTransOK_ = TRUE;	// Set status bits to completed successfully
			TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT) | (1 << TWI_CONTROL_STOP_BIT);
		}
		break;
	case TWI_MRX_DATA_ACK:		// Data byte has been received and ACK transmitted
		this->bufferData_[twiBufferPointer++] = TWI_DATA_REGISTER;
	case TWI_MRX_ADR_ACK:		// SLA+R has been transmitted and ACK received
		if(twiBufferPointer < (this->bufferSize_ - 1)) {	// Detect the last byte to NACK it
			TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT) | (1 << TWI_CONTROL_ENABLE_ACK_BIT);
		} else {				// Send NACK after next reception
			TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT);
		}
		break;
	case TWI_MRX_DATA_NACK:		// Data byte has been received and NACK transmitted
		this->bufferData_[twiBufferPointer] = TWI_DATA_REGISTER;
		this->lastTransOK_ = TRUE;	// Set status bits to completed successfully
		TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT) | (1 << TWI_CONTROL_STOP_BIT);
		break;
	case TWI_ARB_LOST:			// Arbitration lost
		TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT) | (1 << TWI_CONTROL_INT_ENABLE_BIT) | (1 << TWI_CONTROL_INT_FLAG_BIT) | (1 << TWI_CONTROL_START_BIT);
		break;
	case TWI_MTX_ADR_NACK:		// SLA+W has been transmitted and NACK received
	case TWI_MRX_ADR_NACK:		// SLA+R has been transmitted and NACK received
	case TWI_MTX_DATA_NACK:		// Data byte has been transmitted and NACK received
	case TWI_BUS_ERROR:			// Bus error due to an illegal START or STOP condition
	default:
		this->state_ = (twiState_e)TWI_STATUS_REGISTER;		// Store TWI_STATUS_REGISTER and automatically sets clears noErrors bit
		TWI_CONTROL_REGISTER = (1 << TWI_CONTROL_ENABLE_BIT);		// Reset TWI Interface
		break;
	}
}

// -----------------------------------------------------------------------------
// TwiMaster class - Private methods definitions -------------------------------

bool_t TwiMaster::isBusy(void)
{
	if(isBitSet(TWI_CONTROL_REGISTER, TWI_CONTROL_INT_ENABLE_BIT)) {
		return TRUE;
	}
	return FALSE;
}

void TwiMaster::readFromBuffer(uint8_t * message, uint8_t messageSize)
{
	uint8_t i;

	while(this->isBusy())
		;	// Wait until TWI is ready for next transmission
	if(this->lastTransOK_) {				// Last transmission competed successfully
		for(i = 0; i < messageSize; i++) {	// Copy data from Transceiver buffer
			message[i] = this->bufferData_[i + 1];
		}
	}

	return;
}

// -----------------------------------------------------------------------------
// TwiMaster class - Protected methods definitions -----------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(TWI_vect)
{
	twiMaster.interruptHandler();
}
