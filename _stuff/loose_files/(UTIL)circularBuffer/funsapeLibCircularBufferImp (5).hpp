/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibCircularBufferImp.hpp
 * Module:			Generic data-wide circular buffer for FunSAPE++ Embedded
 * 					Library project - Template Implementation
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_CIRCULAR_BUFFER_IMP_HPP
#define __FUNSAPE_LIB_CIRCULAR_BUFFER_IMP_HPP	__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibCircularBufferImp.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBufferImp.hpp] Error 10 - Build mismatch between (funsapeLibCircularBufferImp.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif
// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibCircularBufferImp.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBufferImp.hpp] Error 2 - Build mismatch between (funsapeLibCircularBufferImp.hpp) and (funsapeLibDebug.hpp) header files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

void verboseStatus(uint16_t errorCode, char *strMessage);

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
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

template<typename T> CircularBuffer<T>::CircularBuffer(uint16_t bufferSize, bool overwrite)
{
	// Locking procedure
	this->_locked = true;						// Locks circular buffer
	this->_rdProtected = true;					// Protects against read operations
	this->_wrProtected = true;					// Protects against write operations

	// Mark passage for debug
	debugMark();

	// Reset data members
	this->_initialized = false;
	this->_rdIndex = 0;;
	this->_wrIndex = 0;
	this->_maxSize = 0;
	this->_occupation = 0;
	this->_data = NULL;;
	this->_empty = true;
	this->_full = false;
	this->_overwriting = false;

	// Check for errors
	if(bufferSize == 0) {						// Zero-sized buffer
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return;
	}
	if(bufferSize < 2) {						// Size too small
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return;
	}

	// Allocate memory
	this->_data = (T *)calloc(bufferSize, sizeof(T));
	// Was memory allocation successful?
	if(this->_data == NULL) {					// No, trow error
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return;
	}

	// Update data members
	this->_maxSize = bufferSize;
	this->_overwriting = overwrite;
	this->_initialized = true;

	// Unlock procedure
	this->_locked = false;						// Unlocks circular buffer
	this->_rdProtected = false;					// Allows read operations
	this->_wrProtected = false;					// Allows write operations

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

template<typename T> CircularBuffer<T>::~CircularBuffer()
{
	// Locking procedure
	this->_locked = true;						// Locks circular buffer
	this->_rdProtected = true;					// Protects against read operations
	this->_wrProtected = true;					// Protects against write operations

	// Mark passage for debug
	debugMark();

	// Is there some memory allocated to buffer?
	if(this->_data != NULL) {					// Yes, deallocate it!
		free(this->_data);
	}
	this->_data = NULL;							// Clears pointer

	// Reset data members
	this->_initialized = false;
	this->_rdIndex = 0;
	this->_wrIndex = 0;
	this->_maxSize = 0;
	this->_occupation = 0;
	this->_empty = true;
	this->_full = false;
	this->_overwriting = false;

	// Return successfully (but do NOT unlock)
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

template<typename T> Error CircularBuffer<T>::getLastError(void)
{
	return this->_lastError;
}

template<typename T> uint16_t CircularBuffer<T>::getOccupation(void)
{
	this->_lastError = Error::NONE;
	return this->_occupation;
}

template<typename T> uint16_t CircularBuffer<T>::getFreeSpace(void)
{
	this->_lastError = Error::NONE;
	return (this->_maxSize - this->_occupation);
}

template<typename T> bool CircularBuffer<T>::isEmpty(void)
{
	this->_lastError = Error::NONE;
	return this->_empty;
}

template<typename T> bool CircularBuffer<T>::isFull(void)
{
	this->_lastError = Error::NONE;
	return this->_full;
}

template<typename T> void CircularBuffer<T>::blockWrite(bool block)
{
	this->_wrProtected = block;
	this->_lastError = Error::NONE;
	return;
}

template<typename T> void CircularBuffer<T>::blockRead(bool block)
{
	this->_rdProtected = block;
	this->_lastError = Error::NONE;
	return;
}

template<typename T> bool CircularBuffer<T>::flush(bool bypassProtection)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_checkInitialized()) {			// Circular buffer is not initialized
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkLocked()) {					// Circular buffer is locked
		debugMessage(this->_lastError);
		return false;
	}
	// Must bypass read and write protection?
	if(!bypassProtection) {					// No, lets see if any protection is on!
		if(this->_checkReadProtected()) {		// Circular buffer is read protected
			debugMessage(this->_lastError);
			return false;
		}
		if(this->_checkWriteProtected()) {		// Circular buffer is write protected
			debugMessage(this->_lastError);
			return false;
		}
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer
	this->_rdProtected = true;					// Protects against read operations
	this->_wrProtected = true;					// Protects against write operations

	// Flushes data
	this->_rdIndex = 0;;
	this->_wrIndex = 0;
	this->_occupation = 0;
	this->_empty = true;
	this->_full = false;

	// Unlock procedure (also resets protection)
	this->_locked = false;						// Unlocks circular buffer
	this->_rdProtected = false;					// Allows read operations
	this->_wrProtected = false;					// Allows write operations

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

template<typename T> bool CircularBuffer<T>::push(T data)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_checkInitialized()) {			// Circular buffer is not initialized
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkLocked()) {					// Circular buffer is locked
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkWriteProtected()) {			// Circular buffer is write protected
		debugMessage(this->_lastError);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	// Is buffer full?
	if(!this->_full) {							// No, normal operation!
		this->_data[this->_wrIndex] = data;				// Stores data
		this->_moveWritePointer();						// Moves WRITE pointer only
	} else {									// Yes, must check what to do!
		// Is overwriting allowed?
		if(this->_overwriting) {						// Yes, overwrites old data!
			this->_data[this->_wrIndex] = data;					// Stores data
			this->_moveBothPointers();							// Moves BOTH pointers
		} else {										// No, trow error!
			this->_lastError = Error::BUFFER_FULL;
			debugMessage(Error::BUFFER_FULL);
			// Unlocking procedure before exit
			this->_locked = false;
			return false;
		}
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

template<typename T> bool CircularBuffer<T>::pop(T *data, bool keepData)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(data == NULL) {							// Argument is a NULL pointer
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	if(!this->_checkInitialized()) {			// Circular buffer is not initialized
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkLocked()) {					// Circular buffer is locked
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkReadProtected()) {			// Circular buffer is read protected
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_empty) {							// Circular buffer is empty
		this->_lastError = Error::BUFFER_EMPTY;
		debugMessage(Error::BUFFER_EMPTY);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	// Gets data
	*data = this->_data[this->_rdIndex];		// Retrieves data
	// Must move pointer?
	if(!keepData) {							// Yes, pointer must be moved!
		this->_moveReadPointer();						// Moves pointer
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

template<typename T> bool CircularBuffer<T>::pushBuffer(T *bufData, uint16_t bufSize)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(bufData == NULL) {				// Argument is a NULL pointer
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	if(bufSize == 0) {					// Buffer is zero-sized
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	if(!this->_checkInitialized()) {	// Circular buffer is not initialized
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkLocked()) {			// Circular buffer is locked
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkWriteProtected()) {	// Circular buffer is write protected
		debugMessage(this->_lastError);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	uint16_t freeSpace = this->_maxSize - this->_occupation;
	// Is there enough space?
	if(freeSpace >= bufSize) {					// Yes, normal operation!
		for(uint16_t i = 0; i < bufSize; i++) {
			this->_data[this->_wrIndex] = bufData[i];	// Stores data
			this->_moveWritePointer();					// Moves WRITE pointer
		}
	} else {									// No, must check what to do!
		// Is overwriting allowed?
		if(this->_overwriting) {						// Yes, split operation!
			for(uint16_t i = 0; i < bufSize; i++) {
				this->_data[this->_wrIndex] = bufData[i];		// Stores data
				// Is buffer full?
				if(this->_full) {								// Yes, move BOTH pointers!
					this->_moveBothPointers();
				} else {										// No, move WRITE pointer only!
					this->_moveWritePointer();
				}
			}
		} else {										// No, trow error!
			this->_lastError = Error::BUFFER_NOT_ENOUGH_SPACE;
			debugMessage(Error::BUFFER_NOT_ENOUGH_SPACE);
			// Unlocking procedure before exit
			this->_locked = false;
			return false;
		}
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

template<typename T> bool CircularBuffer<T>::popBuffer(T *bufData, uint16_t bufSize, bool keepData)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(bufData == NULL) {						// Argument is a NULL pointer
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	if(bufSize == 0) {							// Buffer is zero-sized
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	if(!this->_checkInitialized()) {			// Circular buffer is not initialized
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkLocked()) {					// Circular buffer is locked
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_checkReadProtected()) {			// Circular buffer is read protected
		debugMessage(this->_lastError);
		return false;
	}
	if(this->_occupation < bufSize) {			// There is not enough elements to get from buffer
		this->_lastError = Error::BUFFER_NOT_ENOUGH_ELEMENTS;
		debugMessage(Error::BUFFER_NOT_ENOUGH_ELEMENTS);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	// Keep record of current buffer status
	uint16_t rdIndexOld = this->_rdIndex;
	uint16_t occupationOld = this->_occupation;
	bool emptyOld = this->_empty;
	bool fullOld = this->_full;

	// Get elements from buffer
	for(uint16_t i = 0; i < bufSize; i++) {
		bufData[i] = this->_data[this->_rdIndex];	// Retrieves data
		this->_moveReadPointer();					// Moves READ pointer
	}

	// Must keep old data?
	if(keepData) {								// Yes, load old status!
		this->_rdIndex = rdIndexOld;
		this->_occupation = occupationOld;
		this->_empty = emptyOld;
		this->_full = fullOld;
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

template<typename T> bool inlined CircularBuffer<T>::_checkLocked(void)
{
	if(this->_locked) {
		this->_lastError = Error::LOCKED;
	}

	return this->_locked;
}

template<typename T> bool inlined CircularBuffer<T>::_checkInitialized(void)
{
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
	}

	return this->_initialized;
}

template<typename T> bool inlined CircularBuffer<T>::_checkWriteProtected(void)
{
	if(this->_wrProtected) {
		this->_lastError = Error::WRITE_PROTECTED;
	}

	return this->_wrProtected;
}

template<typename T> bool inlined CircularBuffer<T>::_checkReadProtected(void)
{
	if(!this->_rdProtected) {
		this->_lastError = Error::READ_PROTECTED;
	}

	return this->_rdProtected;
}

template<typename T> void inlined CircularBuffer<T>::_moveReadPointer(void)
{
	this->_rdIndex++;							// Increments read pointer
	this->_rdIndex %= this->_maxSize;			// Resolves read pointer overflow
	this->_occupation--;						// Decreases occupation number
	this->_full = false;						// Not full anymore
	this->_empty = (this->_occupation == 0);	// Resolves empty status

	return;
}

template<typename T> void inlined CircularBuffer<T>::_moveWritePointer(void)
{
	this->_wrIndex++;							// Increments write pointer
	this->_wrIndex %= this->_maxSize;			// Resolves write pointer overflow
	this->_occupation++;						// Increases occupation number
	this->_full = (this->_occupation == this->_maxSize);	// Resolves full status
	this->_empty = false;						// Not empty anymore

	return;
}

template<typename T> void inlined CircularBuffer<T>::_moveBothPointers(void)
{
	this->_wrIndex++;							// Increments write pointer
	this->_wrIndex %= this->_maxSize;			// Resolves write pointer overflow
	this->_rdIndex++;							// Increments read pointer
	this->_rdIndex %= this->_maxSize;			// Resolves read pointer overflow
	this->_full = (this->_occupation == this->_maxSize);	// Resolves full status
	this->_empty = (this->_occupation == 0);	// Resolves empty status

	return;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_CIRCULAR_BUFFER_IMP_HPP
