/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer16.h
 * Module:			Circular Buffer Implementation for 16 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __CIRCULAR_BUFFER_16_H
#define __CIRCULAR_BUFFER_16_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdlib.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef struct circularBuffer16_t {
	uint16_t nextRead;
	uint16_t nextWrite;
	uint16_t size;
	uint16_t occupation;
	uint16_t * data;
} circularBuffer16_t;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define circularBuffer16Create() (circularBuffer16_t){.nextRead = 0, .nextWrite = 0, .size = 0, .occupation = 0, .data = NULL}

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

bool_t	circularBuffer16Init(circularBuffer16_t * buffer, uint16_t bufferSize);
bool_t	circularBuffer16PushData(circularBuffer16_t * buffer, uint16_t data);
bool_t	circularBuffer16PopData(circularBuffer16_t * buffer, uint16_t * data);
bool_t	circularBuffer16IsEmpty(circularBuffer16_t * buffer);

#endif
