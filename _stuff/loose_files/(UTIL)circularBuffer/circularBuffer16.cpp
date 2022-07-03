/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	circularBuffer16.cpp
 * Module:		8-bits data-wide circular buffer for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "circularBuffer16.hpp"
#if __CIRCULAR_BUFFER_16_HPP != 1002
#	error [circularBuffer16.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

/* NONE */

// =============================================================================
// Class constructors
// =============================================================================

CircularBuffer16::CircularBuffer16(uint16_t bufferSize)
{
	// Locks circular buffer
	this->_locked = true;
	this->_rdProtected = true;
	this->_wrProtected = true;

	// Reset data members
	this->_initialized = false;
	this->_rdIndex = 0;;
	this->_wrIndex = 0;
	this->_maxSize = 0;
	this->_occupation = 0;
	this->_data = NULL;;
	this->_empty = true;
	this->_full = false;

	// Check for errors - Buffer size
	if (bufferSize == 0) {
//		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return;
	} else if (bufferSize < 2) {
//		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return;
	}

	// Allocate memory
	this->_data = (uint16_t *)calloc(bufferSize, sizeof(uint16_t));
	if (this->_data == NULL) {
//		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}

	// Update data members
	this->_maxSize = bufferSize;
	this->_initialized = true;

	// Unlocks circular buffer
	this->_locked = false;
	this->_rdProtected = false;
	this->_wrProtected = false;
//	this->lastError_ = ERROR_NONE;

	return;
}

CircularBuffer16::~CircularBuffer16()
{
	// Locks circular buffer
	this->_locked = true;
	this->_rdProtected = true;
	this->_wrProtected = true;

	// Deallocate memory if needed
	if (this->_data != NULL) {
		free(this->_data);
	}
	this->_data = NULL;

	// Reset data members
	this->_initialized = false;
	this->_rdIndex = 0;;
	this->_wrIndex = 0;
	this->_maxSize = 0;
	this->_occupation = 0;
	this->_empty = true;
	this->_full = false;

//	this->lastError_ = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

uint16_t CircularBuffer16::getOccupation(void)
{
//	this->lastError_ = ERROR_NONE;
	return this->_occupation;
}

bool CircularBuffer16::isEmpty(void)
{
//	this->lastError_ = ERROR_NONE;
	return this->_empty;
}

bool CircularBuffer16::isFull(void)
{
//	this->lastError_ = ERROR_NONE;
	return this->_full;
}

void CircularBuffer16::blockWrite(bool block)
{
//	this->lastError_ = ERROR_NONE;
	this->_wrProtected = block;
}

void CircularBuffer16::blockRead(bool block)
{
//	this->lastError_ = ERROR_NONE;
	this->_rdProtected = block;
}

bool CircularBuffer16::flush(void)
{
	// Locks circular buffer
	this->_locked = true;
	this->_rdProtected = true;
	this->_wrProtected = true;

	// Flushes data
	this->_rdIndex = 0;;
	this->_wrIndex = 0;
	this->_occupation = 0;
	this->_empty = true;
	this->_full = false;

	// Unlocks circular buffer
	this->_locked = false;
	this->_rdProtected = false;
	this->_wrProtected = false;

//	this->lastError_ = ERROR_NONE;
	return true;
}

bool CircularBuffer16::push(uint16_t data)
{
	// Check for errors - Not initialized
	if (!this->isInitialized()) {
		return false;
	}
	// Check for errors - Write protected
	if (this->isWriteProtected()) {
		return false;
	}
	// Check for errors - Buffer full
	if (this->_full) {
//		this->lastError_ = ERROR_BUFFER_FULL;
		return false;
	}
	// Check for errors - Buffer locked
	if (this->isLocked()) {
//		this->lastError_ = ERROR_LOCKED;
		return false;
	}

	// Locks circular buffer
	this->_locked = true;

	// Stores data
	this->_data[this->_wrIndex] = data;							// Push data into buffer
	this->_wrIndex = ((this->_wrIndex + 1) % this->_maxSize);	// Calculates next write index
	this->_occupation++;										// Updates occupation
	this->_full = (this->_occupation == this->_maxSize);		// Updates full status
	this->_empty = false;										// Updates empty status

	// Unlocks circular buffer
	this->_locked = false;
//	this->lastError_ = ERROR_NONE;
	return true;
}

bool CircularBuffer16::pop(uint16_t *data)
{
	// Check for errors - Not initialized
	if (!this->isInitialized()) {
		return false;
	}
	// Check for errors - Read protected
	if (this->isReadProtected()) {
		return false;
	}
	// Check for errors - Buffer empty
	if (this->_empty) {
//		this->lastError_ = ERROR_BUFFER_EMPTY;
		return false;
	}
	// Check for errors - Buffer locked
	if (this->isLocked()) {
//		this->lastError_ = ERROR_LOCKED;
		return false;
	}

	// Locks circular buffer
	this->_locked = true;

	// Retrieves data
	*data = this->_data[this->_wrIndex];						// Pop data from buffer
	this->_rdIndex = ((this->_rdIndex + 1) % this->_maxSize);	// Calculates next read index
	this->_occupation--;										// Updates occupation
	this->_full = false;										// Updates full status
	this->_empty = (this->_occupation == 0);					// Updates empty status

	// Unlocks circular buffer
	this->_locked = false;
//	this->lastError_ = ERROR_NONE;
	return true;
}

// =============================================================================
// Class private methods
// =============================================================================

bool CircularBuffer16::isLocked(void)
{
	if (this->_locked) {
//		this->lastError_ = ERROR_LOCKED;
	}

	return this->_locked;
}

bool CircularBuffer16::isInitialized(void)
{
	if (!this->_initialized) {
//		this->lastError_ = ERROR_NOT_INITIALIZED;
	}

	return this->_initialized;
}

bool CircularBuffer16::isWriteProtected(void)
{
	if (this->_wrProtected) {
//		this->lastError_ = ERROR_WRITE_PROTECTED;
	}

	return this->_wrProtected;
}

bool CircularBuffer16::isReadProtected(void)
{
	if (!this->_rdProtected) {
//		this->lastError_ = ERROR_READ_PROTECTED;
	}

	return this->_rdProtected;
}

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */

// =============================================================================
// General public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Public functions definitions
// =============================================================================

/* NONE */
