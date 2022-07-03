/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer8.h
 * Module:			Circular Buffer Implementation for 8 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __MY_LIB_CIRCULAR_BUFFER_8_H
#define __MY_LIB_CIRCULAR_BUFFER_8_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "myLibGlobalDefinitions.h"
#if __MY_LIB_GLOBAL_DEFINITIONS_H != 1
	#error Error 100 - myLibGlobalDefinitions.h - wrong build (globalDefinitions must be build 1).
#endif
#include <stdlib.h>

class myLibCircularBuffer8
{
public:
	myLibCircularBuffer8(uint32_t size);
	bool_t pushData(uint8_t data);
	bool_t popData(uint8_t * data);
	bool_t isEmpty(void);
	bool_t isFull(void);
	myLibErrors_e error(void);
	uint16_t		occupation;

private:
	uint16_t		nextRead;
	uint16_t		nextWrite;
	uint16_t		size;
	uint8_t *		buffer;
	myLibErrors_e	lastError;
	bool_t			initialized;
};

#endif
