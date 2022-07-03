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

#include "myLibCircularBuffer8.h"

#if __MY_LIB_CIRCULAR_BUFFER_8_H != 1
	#error Error 101 - Build mismatch on header and source code files (circularBuffer8).
#endif

// -----------------------------------------------------------------------------
// Initializes the circular buffer, allocating memory.
// -----------------------------------------------------------------------------
myLibCircularBuffer8::myLibCircularBuffer8(uint32_t size)
{
	// Initialize properties
	this->nextRead = 0;
	this->nextWrite = 0;
	this->size = 0;
	this->occupation = 0;
	this->buffer = NULL;
	this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED;
	this->initialized = FALSE;

	// Verify arguments
	if(size == 0) {
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_SIZE_ZERO;
		return;
	}

	// Memory allocation
	this->buffer = (uint8_t *)malloc(size * sizeof(uint8_t));
	if(this->buffer == NULL) {
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_MEM_ALLOC_FAIL;
		return;
	}

	// Update properties values
	this->size = size;
	this->lastError = MYLIB_ERROR_NONE;
	this->initialized = TRUE;

	return;
}

// -----------------------------------------------------------------------------
// Pushes data into circular buffer. If the buffer is full, data will be
// ignored, the internal pointers will not be changed, and the function will
// return FALSE. The isFull() method can be called to check if the buffer is
// full before writting to buffer.
// -----------------------------------------------------------------------------
bool_t myLibCircularBuffer8::pushData(uint8_t data)
{
	// Verify initialization status
	if(!this->initialized){
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED;
		return FALSE;
	}

	// Verify occupation
	if(this->occupation == this->size) {
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_TRY_TO_WRITE_WHEN_FULL;
		return FALSE;
	}

	// Store data
	this->buffer[this->nextWrite] = data;
	this->nextWrite++;
	this->nextWrite %= this->size;
	this->occupation++;

	this->lastError = MYLIB_ERROR_NONE;
	return TRUE;
	}

// -----------------------------------------------------------------------------
// Pops data from the circular buffer. If the buffer is empty, data will be
// unchanged, the function will return FALSE, and the internal pointer will not
// be changed. The isEmpty() method can be called to check if there is data to
// be read in the buffer. Returned data must be immediately copied, since the
// pointed data can now be changed by subsequent pushData() calls.
// -----------------------------------------------------------------------------
bool_t myLibCircularBuffer8::popData(uint8_t * data)
{
	// Verify initialization status
	if(!this->initialized){
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED;
		return FALSE;
	}

	// Verify occupation
	if(this->occupation == 0) {
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_TRY_TO_READ_WHEN_EMPTY;
		return FALSE;
	}
	*data = this->buffer[this->nextRead];
	this->nextRead = (this->nextRead + 1) % this->size;
	this->occupation--;

	this->lastError = MYLIB_ERROR_NONE;
	return TRUE;
}

// -----------------------------------------------------------------------------
// Verifies if there is unread data in the circular buffer. Must be called
// before reading the buffer. Returns TRUE if there is unread data into buffer.
// -----------------------------------------------------------------------------

bool_t myLibCircularBuffer8::isEmpty(void)
{
	// Verify initialization status
	if(!this->initialized){
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED;
		return TRUE;	// If not initialized, then buffer cannot be read, so considered empty
	}

	// Verify occupation
	this->lastError = MYLIB_ERROR_NONE;
	if(this->occupation == 0) {
		return TRUE;
	}

	return FALSE;
}

// -----------------------------------------------------------------------------
// Verifies if circular buffer is full. Can be called before writing to the the
// buffer. Returns TRUE if buffer is full and write operations will be ignored.
// -----------------------------------------------------------------------------
bool_t myLibCircularBuffer8::isFull(void)
{
	// Verify initialization status
	if(!this->initialized){
		this->lastError = MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED;
		return TRUE;	// If not initialized, then buffer cannot be writen, so considered full
	}

	// Verify occupation
	this->lastError = MYLIB_ERROR_NONE;
	if(this->occupation == this->size) {
		return TRUE;
	}

	return FALSE;
}

// -----------------------------------------------------------------------------
// Returns last error. Possible values are:
// 		MYLIB_ERROR_CIRCULAR_BUFFER_NOT_INITIALIZED
// 		MYLIB_ERROR_CIRCULAR_BUFFER_SIZE_ZERO
// 		MYLIB_ERROR_CIRCULAR_BUFFER_MEM_ALLOC_FAIL
// 		MYLIB_ERROR_CIRCULAR_BUFFER_TRY_TO_WRITE_WHEN_FULL
// 		MYLIB_ERROR_CIRCULAR_BUFFER_TRY_TO_READ_WHEN_EMPTY
// -----------------------------------------------------------------------------
myLibErrors_e myLibCircularBuffer8::error(void)
{
	return this->lastError;
}
