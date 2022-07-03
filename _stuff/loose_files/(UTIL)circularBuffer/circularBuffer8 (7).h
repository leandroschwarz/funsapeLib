/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer8.h
 * Module:			Circular Buffer Implementation for 8 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __CIRCULAR_BUFFER_8_H
#define __CIRCULAR_BUFFER_8_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdlib.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef struct circularBuffer8_t {
	uint16_t nextRead;
	uint16_t nextWrite;
	uint16_t size;
	uint16_t occupation;
	uint8_t * data;
} circularBuffer8_t;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define circularBuffer8Create() (circularBuffer8_t){.nextRead = 0, .nextWrite = 0, .size = 0, .occupation = 0, .data = NULL}

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

bool_t	circularBuffer8Init(circularBuffer8_t * buffer, uint16_t bufferSize);
bool_t	circularBuffer8PushData(circularBuffer8_t * buffer, uint8_t data);
bool_t	circularBuffer8PopData(circularBuffer8_t * buffer, uint8_t * data);
bool_t	circularBuffer8IsEmpty(circularBuffer8_t * buffer);

#endif
