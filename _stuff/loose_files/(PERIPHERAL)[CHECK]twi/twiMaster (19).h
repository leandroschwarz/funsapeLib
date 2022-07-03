/*!
	\file		twiMaster.h
	\brief		TWI interface in Master Mode for the GPDSE AVR8 Integrated Library
	\author		Fábio Cabral Pacheco
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		May 16, 2018
*/

#ifndef __TWI_MASTER_H
#define __TWI_MASTER_H 2

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union twiMasterStatus_t {
	struct {
		bool_t	lastTransOK		: 1;
		bool_t	rxDataInBuf		: 1;
		bool_t	genAddressCall	: 1;
		bool_t	initialized		: 1;
		uint8_t	unusedBits		: 4;
	};
	uint8_t allFlags;
} twiMasterStatus_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

extern twiMasterStatus_t twiMasterStatus;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void		twiMasterInit(uint32_t clockSpeed);
void		twiMasterSendData(uint8_t deviceAddress, twiOperation_t readWrite, uint8_t * message, uint8_t messageSize);
twiState_e	twiMasterErrorHandler(twiState_e twiErrorCode);

#endif
