/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twiSlave.h
 * Module:			TWI Slave controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * Purpose:			Interfaces a TWI Slave data bus
 * -------------------------------------------------------------------------- */

#ifndef __TWI_SLAVE_H
#define __TWI_SLAVE_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdlib.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef vuint8_t twiSlaveBuffer_t;

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

twiSlaveBuffer_t * twiSlaveInit(uint8_t twiSlaveAddr, uint8_t bufferSize, bool_t genCallAcceptance);

#endif
