/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer16.h
 * Module:			Circular Buffer Implementation for 16 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __CIRCULAR_BUFFER_32_H
#define __CIRCULAR_BUFFER_32_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdlib.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef struct circularBuffer32_t {
	uint16_t nextRead;
	uint16_t nextWrite;
	uint16_t size;
	uint16_t occupation;
	uint32_t * data;
} circularBuffer32_t;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define circularBuffer32Create() (circularBuffer32_t){.nextRead = 0, .nextWrite = 0, .size = 0, .occupation = 0, .data = NULL}

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

bool_t	circularBuffer32Init(circularBuffer32_t * buffer, uint16_t bufferSize);
bool_t	circularBuffer32PushData(circularBuffer32_t * buffer, uint32_t data);
bool_t	circularBuffer32PopData(circularBuffer32_t * buffer, uint32_t * data);
bool_t	circularBuffer32IsEmpty(circularBuffer32_t * buffer);

#endif
