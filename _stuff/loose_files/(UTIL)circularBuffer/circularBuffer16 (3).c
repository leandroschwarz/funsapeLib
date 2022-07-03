/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer16.c
 * Module:			Circular Buffer Implementation for 16 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "circularBuffer16.h"
#if __CIRCULAR_BUFFER_16_H != 1
	#error Error 101 - Build mismatch on header and source code files (circularBuffer16).
#endif

/* -----------------------------------------------------------------------------
 * Initializes the circular buffer, allocating memory.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer16Init(circularBuffer16_t * buffer, uint16_t bufferSize)
{
	if(bufferSize == 0){
		return FALSE;
	}

	// Memory allocation
	buffer->data = (uint16_t *)malloc(bufferSize * sizeof(uint16_t));
	if(buffer->data == NULL){
		return FALSE;
	}

	buffer->size = bufferSize;
	buffer->nextRead = 0;
	buffer->nextWrite = 0;
	buffer->occupation = 0;

	return TRUE;
}

/* -----------------------------------------------------------------------------
 * Pushes data into circular buffer. The function returns FALSE if buffer is full.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer16PushData(circularBuffer16_t * buffer, uint16_t data)
{
	if((buffer->occupation == 0) || (buffer->nextWrite != buffer->nextRead)){
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
 * internal pointer will not be changed. The circularBuffer16IsEmpty() function
 * must be called to check if there is data to be read in the buffer. Returned
 * data must be immediately copied, since the pointed data can now be changed
 * by circularBuffer16PushData() calls.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer16PopData(circularBuffer16_t * buffer, uint16_t * data)
{
	
	*data = buffer->data[buffer->nextRead];

	if(buffer->occupation > 0){
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

bool_t circularBuffer16IsEmpty(circularBuffer16_t * buffer)
{
	if(buffer->occupation == 0){
		return TRUE;
	}

	return FALSE;
}
