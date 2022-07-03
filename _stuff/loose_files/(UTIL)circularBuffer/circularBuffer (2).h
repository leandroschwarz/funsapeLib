/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer.h
 * Module:			Circular Buffer Implementation for generic size variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdlib.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef struct circularBuffer_t {
	uint16_t nextRead;
	uint16_t nextWrite;
	uint16_t size;
	uint8_t varSize;
	uint16_t occupation;
	uint8_t * data;
} circularBuffer_t;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define circularBufferCreate() (circularBuffer_t){.nextRead = 0, .nextWrite = 0, .size = 0, .occupation = 0, .data = NULL}

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

bool_t	circularBufferInit(circularBuffer_t * buffer, uint16_t bufferSize, uint8_t variableSize);
bool_t	circularBufferPushData(circularBuffer_t * buffer, void * data);
bool_t	circularBufferPopData(circularBuffer_t * buffer, void * data);
bool_t	circularBufferIsEmpty(circularBuffer_t * buffer);

#endif
