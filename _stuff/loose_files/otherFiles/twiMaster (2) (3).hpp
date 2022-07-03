/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	twiMaster.hpp
 * Module:		TWI Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 *				Fábio Cabral Pacheco
 * Build:		1002
 * Date:		June 14, 2018
 * ========================================================================== */

#ifndef __TWI_MASTER_HPP
#define __TWI_MASTER_HPP	1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[twiMaster.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include "system.hpp"

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

#define TWI_BUFFER_SIZE 40

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

typedef enum twiState_e {
	TWI_START					= 0x08,	// START has been transmitted
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been transmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been transmitted and NACK received
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been transmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been transmitted and NACK received
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been transmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been transmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK transmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK transmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = 0
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_e;

typedef enum twiOperation_t {
	TWI_WRITE = 0,
	TWI_READ
} twiOperation_t;

// =============================================================================
// TwiMaster - Class declaration
// =============================================================================

class TwiMaster {
	// New data types - Public
public:

	// New data types - Private
private:
	// New data types - Protected
protected:

	// Constructors
public:
	TwiMaster();

	// Methods - Public
public:
	void setClockSpeed(uint32_t clockSpeed);
	void sendData(uint8_t deviceAddress, twiOperation_t readWrite, uint8_t *message, uint8_t messageSize);
	bool sendData(uint8_t deviceAddress, twiOperation_t readWrite, uint8_t reg, uint8_t *message, uint8_t messageSize);
	void interruptHandler(void);

	// Methods - Private
private:
	bool isBusy(void);
	void readFromBuffer(uint8_t *message, uint8_t messageSize);

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	uint8_t		bufferData_[TWI_BUFFER_SIZE];
	uint8_t		bufferSize_;
	twiState_e	state_;
	bool		lastTransOK_		: 1;
protected:

// Data members - Protected
protected:

}; // class TwiMaster

// =============================================================================
// TwiMaster - Class inline function definitions
// =============================================================================

/* NONE */

// =============================================================================
// Extern global variables
// =============================================================================

extern TwiMaster twiMaster;

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __TWI_MASTER_HPP
