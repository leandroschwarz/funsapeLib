/* -----------------------------------------------------------------------------
 * Project:		GPSDE++ AVR Library
 * File name:	twiMaster.hpp
 * Module:		TWI Interface Module for GPDSE++ AVR Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 *				Fábio Cabral Pacheco
 * Build:		1001
 * Date:		May 26, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Include guards --------------------------------------------------------------

#ifndef __TWI_MASTER_HPP
#define __TWI_MASTER_HPP	1001

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#if __USE_OWN_DEFINITIONS_FILE == 1
#	include "userDefines.hpp"
#else
#	include "globalDefines.hpp"
#	if __GLOBAL_DEFINES_HPP != 1001
#		error 	[twiMaster.hpp] Error 102 - Build mismatch (globalDefines.hpp and twiMaster.hpp)
#	endif		// __GLOBAL_CONFIGS_HPP != 1001
#endif		// __USE_OWN_DEFINITIONS_FILE == 1


// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Macro-functions definitions -------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Extern global variables -----------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General public function declarations ----------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General inline function declarations ----------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// TwiMaster class - Class Definition ------------------------------------------

class TwiMaster
{
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
	void sendData(uint8_t deviceAddress, twiOperation_e readWrite, uint8_t * message, uint8_t messageSize);
	void interruptHandler(void);

	// Methods - Private
private:
	bool_t isBusy(void);
	void readFromBuffer(uint8_t * message, uint8_t messageSize);

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	uint8_t		bufferData_[TWI_BUFFER_SIZE];
	uint8_t		bufferSize_;
	twiState_e	state_;
	bool_t		lastTransOK_		: 1;
protected:

// Data members - Protected
protected:

};

// -----------------------------------------------------------------------------
// TwiMaster class - Static or inline functions definitions --------------------

/* NONE */

// -----------------------------------------------------------------------------
// External class instantiations -----------------------------------------------

extern TwiMaster twiMaster;

// -----------------------------------------------------------------------------
// General static or inline functions declarations -----------------------------

/* NONE */

#endif	// __TWI_MASTER_HPP
