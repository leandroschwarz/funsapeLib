/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	circularBuffer64.cpp
 * Module:		64-bits data-wide circular buffer for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "circularBuffer64.h"
#if __CIRCULAR_BUFFER_64_H != __BUILD_MACRO_TO_BE_CHANGED__
#	error Build mismatch on header and source code files (circularBuffer64).
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

CircularBuffer64::CircularBuffer64(uint16_t bufferSize, bool overwrite)
{
	// Locking procedure
	this->locked_ = true;						// Locks circular buffer
	this->rdProtected_ = true;					// Protects against read operations
	this->wrProtected_ = true;					// Protects against write operations

	// Reset data members
	this->initialized_ = false;
	this->rdIndex_ = 0;;
	this->wrIndex_ = 0;
	this->maxSize_ = 0;
	this->occupation_ = 0;
	this->data_ = NULL;;
	this->empty_ = true;
	this->full_ = false;
	this->overwriting_ = false;

	// Check for errors
	if (bufferSize == 0) {						// Zero-sized buffer
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return;
	}
	if (bufferSize < 2) {						// Size too small
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return;
	}

	// Allocate memory
	this->data_ = (uint64_t *)calloc(bufferSize, sizeof(uint64_t));
	// Was memory allocation successful?
	if (this->data_ == NULL) {					// No, trow error
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}

	// Update data members
	this->maxSize_ = bufferSize;
	this->overwriting_ = overwrite;
	this->initialized_ = true;

	// Unlock procedure
	this->locked_ = false;						// Unlocks circular buffer
	this->rdProtected_ = false;					// Allows read operations
	this->wrProtected_ = false;					// Allows write operations

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return;
}

CircularBuffer64::~CircularBuffer64()
{
	// Locking procedure
	this->locked_ = true;						// Locks circular buffer
	this->rdProtected_ = true;					// Protects against read operations
	this->wrProtected_ = true;					// Protects against write operations

	// Is there some memory allocated to buffer?
	if (this->data_ != NULL) {					// Yes, deallocate it!
		free(this->data_);
	}
	this->data_ = NULL;							// Clears pointer

	// Reset data members
	this->initialized_ = false;
	this->rdIndex_ = 0;
	this->wrIndex_ = 0;
	this->maxSize_ = 0;
	this->occupation_ = 0;
	this->empty_ = true;
	this->full_ = false;
	this->overwriting_ = false;

	// Return successfully (but do NOT unlock)
	this->lastError_ = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

error_e  CircularBuffer64::getLastError(void)
{
	return this->lastError_;
}

uint16_t CircularBuffer64::getOccupation(void)
{
	this->lastError_ = ERROR_NONE;
	return this->occupation_;
}

uint16_t CircularBuffer64::getFreeSpace(void)
{
	this->lastError_ = ERROR_NONE;
	return (this->maxSize_ - this->occupation_);
}

bool CircularBuffer64::isEmpty(void)
{
	this->lastError_ = ERROR_NONE;
	return this->empty_;
}

bool CircularBuffer64::isFull(void)
{
	this->lastError_ = ERROR_NONE;
	return this->full_;
}

void CircularBuffer64::blockWrite(bool block)
{
	this->lastError_ = ERROR_NONE;
	this->wrProtected_ = block;
}

void CircularBuffer64::blockRead(bool block)
{
	this->lastError_ = ERROR_NONE;
	this->rdProtected_ = block;
}

bool CircularBuffer64::flush(bool bypassProtection)
{
	// Check for errors
	if (!this->checkInitialized_()) {			// Circular buffer is not initialized
		return false;
	}
	if (this->checkLocked_()) {					// Circular buffer is locked
		return false;
	}
	// Must bypass read and write protection?
	if (!bypassProtection) {					// No, lets see if any protection is on!
		if (this->checkReadProtected_()) {		// Circular buffer is read protected
			return false;
		}
		if (this->checkWriteProtected_()) {		// Circular buffer is write protected
			return false;
		}
	}

	// Locking procedure
	this->locked_ = true;						// Locks circular buffer
	this->rdProtected_ = true;					// Protects against read operations
	this->wrProtected_ = true;					// Protects against write operations

	// Flushes data
	this->rdIndex_ = 0;;
	this->wrIndex_ = 0;
	this->occupation_ = 0;
	this->empty_ = true;
	this->full_ = false;

	// Unlock procedure (also resets protection)
	this->locked_ = false;						// Unlocks circular buffer
	this->rdProtected_ = false;					// Allows read operations
	this->wrProtected_ = false;					// Allows write operations

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool CircularBuffer64::push(uint64_t data)
{
	// Check for errors
	if (!this->checkInitialized_()) {			// Circular buffer is not initialized
		return false;
	}
	if (this->checkLocked_()) {					// Circular buffer is locked
		return false;
	}
	if (this->checkWriteProtected_()) {			// Circular buffer is write protected
		return false;
	}

	// Locking procedure
	this->locked_ = true;						// Locks circular buffer

	// Is buffer full?
	if (!this->full_) {							// No, normal operation!
		this->data_[this->wrIndex_] = data;				// Stores data
		this->moveWritePointer_();						// Moves WRITE pointer only
	} else {									// Yes, must check what to do!
		// Is overwriting allowed?
		if (this->overwriting_) {						// Yes, overwrites old data!
			this->data_[this->wrIndex_] = data;					// Stores data
			this->moveBothPointers_();							// Moves BOTH pointers
		} else {										// No, trow error!
			this->lastError_ = ERROR_BUFFER_FULL;
			// Unlocking procedure before exit
			this->locked_ = false;
			return false;
		}
	}

	// Unlocking procedure
	this->locked_ = false;						// Unlocks circular buffer

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool CircularBuffer64::pop(uint64_t *data, bool keepData)
{
	// Check for errors
	if (data == NULL) {							// Argument is a NULL pointer
		this->lastError_ = ERROR_ARGUMENT_POINTER_NULL;
		return false;
	}
	if (!this->checkInitialized_()) {			// Circular buffer is not initialized
		return false;
	}
	if (this->checkLocked_()) {					// Circular buffer is locked
		return false;
	}
	if (this->checkReadProtected_()) {			// Circular buffer is read protected
		return false;
	}
	if (this->empty_) {							// Circular buffer is empty
		this->lastError_ = ERROR_BUFFER_EMPTY;
		return false;
	}

	// Locking procedure
	this->locked_ = true;						// Locks circular buffer

	// Gets data
	*data = this->data_[this->rdIndex_];		// Retrieves data
	// Must move pointer?
	if (!keepData) {							// Yes, pointer must be moved!
		this->moveReadPointer_();						// Moves pointer
	}

	// Unlocking procedure
	this->locked_ = false;						// Unlocks circular buffer

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool CircularBuffer64::pushBuffer(uint64_t *bufData, uint16_t bufSize)
{
	// Check for errors
	if (bufData == NULL) {				// Argument is a NULL pointer
		this->lastError_ = ERROR_ARGUMENT_POINTER_NULL;
		return false;
	}
	if (bufSize == 0) {					// Buffer is zero-sized
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if (!this->checkInitialized_()) {	// Circular buffer is not initialized
		return false;
	}
	if (this->checkLocked_()) {			// Circular buffer is locked
		return false;
	}
	if (this->checkWriteProtected_()) {	// Circular buffer is write protected
		return false;
	}

	// Locking procedure
	this->locked_ = true;						// Locks circular buffer

	uint16_t freeSpace = this->maxSize_ - this->occupation_;
	// Is there enough space?
	if (freeSpace >= bufSize) {					// Yes, normal operation!
		for (uint16_t i = 0; i < bufSize; i++) {
			this->data_[this->wrIndex_] = bufData[i];	// Stores data
			this->moveWritePointer_();					// Moves WRITE pointer
		}
	} else {									// No, must check what to do!
		// Is overwriting allowed?
		if (this->overwriting_) {						// Yes, split operation!
			for (uint16_t i = 0; i < bufSize; i++) {
				this->data_[this->wrIndex_] = bufData[i];		// Stores data
				// Is buffer full?
				if (this->full_) {								// Yes, move BOTH pointers!
					this->moveBothPointers_();
				} else {										// No, move WRITE pointer only!
					this->moveWritePointer_();
				}
			}
		} else {										// No, trow error!
			this->lastError_ = ERROR_BUFFER_NOT_ENOUGH_SPACE;
			// Unlocking procedure before exit
			this->locked_ = false;
			return false;
		}
	}

	// Unlocking procedure
	this->locked_ = false;						// Unlocks circular buffer

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool CircularBuffer64::popBuffer(uint64_t *bufData, uint16_t bufSize, bool keepData)
{
	// Check for errors
	if (bufData == NULL) {						// Argument is a NULL pointer
		this->lastError_ = ERROR_ARGUMENT_POINTER_NULL;
		return false;
	}
	if (bufSize == 0) {							// Buffer is zero-sized
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if (!this->checkInitialized_()) {			// Circular buffer is not initialized
		return false;
	}
	if (this->checkLocked_()) {					// Circular buffer is locked
		return false;
	}
	if (this->checkReadProtected_()) {			// Circular buffer is read protected
		return false;
	}
	if (this->occupation_ < bufSize) {			// There is not enough elements to get from buffer
		this->lastError_ = ERROR_BUFFER_NOT_ENOUGH_ELEMENTS;
		return false;
	}

	// Locking procedure
	this->locked_ = true;						// Locks circular buffer

	// Keep record of current buffer status
	uint16_t rdIndexOld = this->rdIndex_;
	uint16_t occupationOld = this->occupation_;
	bool emptyOld = this->empty_;
	bool fullOld = this->full_;

	// Get elements from buffer
	for (uint16_t i = 0; i < bufSize; i++) {
		bufData[i] = this->data_[this->rdIndex_];	// Retrieves data
		this->moveReadPointer_();					// Moves READ pointer
	}

	// Must keep old data?
	if (keepData) {								// Yes, load old status!
		this->rdIndex_ = rdIndexOld;
		this->occupation_ = occupationOld;
		this->empty_ = emptyOld;
		this->full_ = fullOld;
	}

	// Unlocking procedure
	this->locked_ = false;						// Unlocks circular buffer

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

// =============================================================================
// Class private methods
// =============================================================================

bool inline __attribute__((always_inline)) CircularBuffer64::checkLocked_(void)
{
	if (this->locked_) {
		this->lastError_ = ERROR_LOCKED;
	}

	return this->locked_;
}

bool inline __attribute__((always_inline)) CircularBuffer64::checkInitialized_(void)
{
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
	}

	return this->initialized_;
}

bool inline __attribute__((always_inline)) CircularBuffer64::checkWriteProtected_(void)
{
	if (this->wrProtected_) {
		this->lastError_ = ERROR_WRITE_PROTECTED;
	}

	return this->wrProtected_;
}

bool inline __attribute__((always_inline)) CircularBuffer64::checkReadProtected_(void)
{
	if (!this->rdProtected_) {
		this->lastError_ = ERROR_READ_PROTECTED;
	}

	return this->rdProtected_;
}

void inline __attribute__((always_inline)) CircularBuffer64::moveReadPointer_(void)
{
	this->rdIndex_++;							// Increments read pointer
	this->rdIndex_ %= this->maxSize_;			// Resolves read pointer overflow
	this->occupation_--;						// Decreases occupation number
	this->full_ = false;						// Not full anymore
	this->empty_ = (this->occupation_ == 0);	// Resolves empty status

	return;
}

void inline __attribute__((always_inline)) CircularBuffer64::moveWritePointer_(void)
{
	this->wrIndex_++;							// Increments write pointer
	this->wrIndex_ %= this->maxSize_;			// Resolves write pointer overflow
	this->occupation_++;						// Increases occupation number
	this->full_ = (this->occupation_ == this->maxSize_);	// Resolves full status
	this->empty_ = false;						// Not empty anymore

	return;
}

void inline __attribute__((always_inline)) CircularBuffer64::moveBothPointers_(void)
{
	this->wrIndex_++;							// Increments write pointer
	this->wrIndex_ %= this->maxSize_;			// Resolves write pointer overflow
	this->rdIndex_++;							// Increments read pointer
	this->rdIndex_ %= this->maxSize_;			// Resolves read pointer overflow
	this->full_ = (this->occupation_ == this->maxSize_);	// Resolves full status
	this->empty_ = (this->occupation_ == 0);	// Resolves empty status

	return;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE
