/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	twi.hpp
 * Module:		TWI Module Interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 14, 2018
 * ========================================================================== */

#ifndef __TWI_HPP
#define __TWI_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[twi.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include <stdint.h>
#include "system.hpp"

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

typedef enum twiOperation_e {
	TWI_WRITE		= 0,
	TWI_READ
} twiOperation_e;

// =============================================================================
// Twi - Class declaration
// =============================================================================

class Twi {
	// New data types
public:

	// Constructors
public:
	Twi(uint32_t clockSpeed = 10000, bool master = true, uint8_t bufferSize = 20);

	// Methods
public:
	bool sendData(uint8_t deviceAddress, twiOperation_e operation, uint8_t *message, uint8_t messageSize);

private:
	void waitUntilReady(void);

protected:

	// Data members
public:

private:
	// Status
	bool		initialized_	: 1;
	bool		lastTransOK_	: 1;
	error_e		lastError_;

	// Configuration
	uint8_t		maxSize_;

	// Master mode
	bool		masterMode_		: 1;
	uint32_t	speed_			: 22;

	// Slave mode
//	uint8_t		slaveAddress_	: 7;
//	bool		genAddressCall_	: 1;

//	bool		rxDataInBuf_	: 1;

protected:

}; // class Template

// =============================================================================
// Template - Class inline function definitions
// =============================================================================

/* NONE */

// =============================================================================
// Extern global variables
// =============================================================================

extern SystemStatus systemStatus;

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __TWI_HPP
