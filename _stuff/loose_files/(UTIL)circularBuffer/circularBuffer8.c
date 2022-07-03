/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer8.c
 * Module:			Circular Buffer Implementation for 8 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "circularBuffer8.h"
#if __CIRCULAR_BUFFER_8_H != 1
	#error Error 101 - Build mismatch on header and source code files (circularBuffer8).
#endif

/* -----------------------------------------------------------------------------
 * Initializes the circular buffer, allocating memory.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer8Init(circularBuffer8_t * buffer, uint16_t bufferSize)
{
	if(bufferSize == 0) {
		return FALSE;
	}

	// Memory allocation
//	buffer->data = (uint8_t *)malloc(bufferSize);
// 	if(buffer->data == NULL) {
// 		return FALSE;
// 	}

	buffer->size = bufferSize;
	buffer->nextRead = 0;
	buffer->nextWrite = 0;
	buffer->occupation = 0;

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * Pushes data into circular buffer. The function returns FALSE if buffer is full.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer8PushData(circularBuffer8_t * buffer, uint8_t data)
{
	if((buffer->occupation == 0) || (buffer->nextWrite != buffer->nextRead)) {
		buffer->data[buffer->nextWrite] = data;
		buffer->nextWrite++;
		buffer->nextWrite %= buffer->size;
		buffer->occupation++;

		return TRUE;
	}

	return FALSE;
}

/* -----------------------------------------------------------------------------
 * Pops data from the circular buffer. If the buffer is empty, the last
 * retrieved data will be stored into data and the function return FALSE. The
 * internal pointer will not be changed. The circularBuffer8IsEmpty() function
 * must be called to check if there is data to be read in the buffer. Returned
 * data must be immediately copied, since the pointed data can now be changed
 * by circularBuffer8PushData() calls.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer8PopData(circularBuffer8_t * buffer, uint8_t * data)
{

	*data = buffer->data[buffer->nextRead];

	if(buffer->occupation > 0) {
		buffer->nextRead = (buffer->nextRead + 1) % buffer->size;
		buffer->occupation--;

		return TRUE;
	}

	return FALSE;
}

/* -----------------------------------------------------------------------------
 * Verifies if there is unread data in the circular buffer. Must be called
 * before reading the buffer. Returns TRUE if there is unread data into buffer.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer8IsEmpty(circularBuffer8_t * buffer)
{
	if(buffer->occupation == 0) {
		return TRUE;
	}

	return FALSE;
}
