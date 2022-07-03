/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twi.h
 * Module:			Two Wire Interface module in master mode
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 *					Fábio Cabral Pacheco
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __TWI_MASTER_H
#define __TWI_MASTER_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union twiStatus_t {
	struct {
		bool_t	lastTransOK		: 1;
		bool_t	rxDataInBuf		: 1;
		bool_t	genAddressCall	: 1;
		bool_t	initialized		: 1;
		uint8_t	unusedBits		: 4;
	};
	uint8_t allFlags;
} twiStatus_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

extern twiStatus_t twiStatus;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void		twiInit(uint32_t clockSpeed);
void		twiSendData(uint8_t deviceAddress, twiOperation_t readWrite, uint8_t * message, uint8_t messageSize);
twiState_t	twiErrorHandler(twiState_t twiErrorCode);

#endif
