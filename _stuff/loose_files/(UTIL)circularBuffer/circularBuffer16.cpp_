/*!
	\file		circularBuffer16.c
	\brief		Circular buffer of uint16_t variables implementation for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		May 16, 2018
*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "circularBuffer16.hpp"
#if __CIRCULAR_BUFFER_16_HPP != 1000
	#error [circularBuffer16.cpp] Error 101 - Build mismatch on header and source code files.
#endif

/* -----------------------------------------------------------------------------
 * Initializes the circular buffer, allocating memory.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer16Init(circularBuffer16_s * buffer, uint16_t bufferSize)
{
	if(bufferSize == 0) {
		return FALSE;
	}

	// Memory allocation
	buffer->data = (uint16_t *)malloc(bufferSize * sizeof(uint16_t));
	if(buffer->data == NULL) {
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

bool_t circularBuffer16PushData(circularBuffer16_s * buffer, uint16_t data)
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
 * internal pointer will not be changed. The circularBuffer16IsEmpty() function
 * must be called to check if there is data to be read in the buffer. Returned
 * data must be immediately copied, since the pointed data can now be changed
 * by circularBuffer16PushData() calls.
 * -------------------------------------------------------------------------- */

bool_t circularBuffer16PopData(circularBuffer16_s * buffer, uint16_t * data)
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

bool_t circularBuffer16IsEmpty(circularBuffer16_s * buffer)
{
	if(buffer->occupation == 0) {
		return TRUE;
	}

	return FALSE;
}

/* -----------------------------------------------------------------------------
 * Retrieves the number of elements in buffer.
 * -------------------------------------------------------------------------- */

uint16_t circularBuffer16GetOccupation(circularBuffer16_s * buffer)
{
	return buffer->occupation;
}