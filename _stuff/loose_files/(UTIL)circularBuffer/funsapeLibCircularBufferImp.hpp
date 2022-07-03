/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibCircularBufferImp.hpp
 * Module:			Generic data-wide circular buffer module for FunSAPE++
 * 					Embedded Library project - Template Implementation
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
#	error	[funsapeLibCircularBufferImp.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBufferImp.hpp] Error 14 - Build mismatch between (funsapeLibCircularBufferImp.hpp) header file and (funsapeLibDebug.hpp) module!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

// void verboseStatus(uint16_t errorCode, char *strMessage);

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

/**
 * @brief Construct a new Circular Buffer<T>::Circular Buffer object
 *
 * @tparam T
 * @param bufferSize_p
 * @param overwrite_p
 */
template<typename T> CircularBuffer<T>::CircularBuffer(uint16_t bufferSize_p, bool overwrite_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::CircularBuffer(uint16_t, bool)", 4);

	// Locking procedure
	this->_locked = true;						// Locks circular buffer
	this->_rdProtected = true;					// Protects against read operations
	this->_wrProtected = true;					// Protects against write operations

	// Reset data members
	this->_data = nullptr;
	this->_empty = true;
	this->_full = false;
	this->_initialized = false;
	this->_maxSize = 0;
	this->_occupation = 0;
	this->_overwriting = false;
	this->_rdIndex = 0;;
	this->_wrIndex = 0;

	// CHECK FOR ERROR - Zero-sized buffer
	if(bufferSize_p == 0) {
		// Return error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return;
	}
	// CHECK FOR ERROR - Size too small
	if(bufferSize_p < 2) {
		// Return error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return;
	}

	// Allocate memory
	this->_data = (T *)calloc(bufferSize_p, sizeof(T));
	if(!isPointerValid(this->_data)) {
		// Return error
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return;
	}

	// Update data members
	this->_initialized = true;
	this->_maxSize = bufferSize_p;
	this->_overwriting = overwrite_p;

	// Unlock procedure
	this->_locked = false;						// Unlocks circular buffer
	this->_rdProtected = false;					// Allows read operations
	this->_wrProtected = false;					// Allows write operations

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Destroy the Circular Buffer<T>::Circular Buffer object
 *
 * @tparam T
 */
template<typename T> CircularBuffer<T>::~CircularBuffer(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::~CircularBuffer(void)", 4);

	// Locking procedure
	this->_locked = true;						// Locks circular buffer
	this->_rdProtected = true;					// Protects against read operations
	this->_wrProtected = true;					// Protects against write operations

	// Is there some memory allocated to buffer?
	if(isPointerValid(this->_data)) {
		free(this->_data);
	}
	this->_data = nullptr;							// Clears pointer

	// Reset data members
	this->_empty = true;
	this->_full = false;
	this->_initialized = false;
	this->_maxSize = 0;
	this->_occupation = 0;
	this->_overwriting = false;
	this->_rdIndex = 0;
	this->_wrIndex = 0;

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

/**
 * @brief
 *
 * @tparam T
 * @return Error
 */
template<typename T> Error CircularBuffer<T>::getLastError(void)
{
	// Returns successfully
	return this->_lastError;
}

/**
 * @brief
 *
 * @tparam T
 * @return uint16_t
 */
template<typename T> uint16_t CircularBuffer<T>::getOccupation(void)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return this->_occupation;
}

/**
 * @brief
 *
 * @tparam T
 * @return uint16_t
 */
template<typename T> uint16_t CircularBuffer<T>::getFreeSpace(void)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return (this->_maxSize - this->_occupation);
}

/**
 * @brief
 *
 * @tparam T
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::isEmpty(void)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return this->_empty;
}

/**
 * @brief
 *
 * @tparam T
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::isFull(void)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return this->_full;
}

/**
 * @brief
 *
 * @tparam T
 * @param block_p
 */
template<typename T> void CircularBuffer<T>::blockWrite(bool block_p)
{
	// Returns successfully
	this->_wrProtected = block_p;
	this->_lastError = Error::NONE;
	return;
}

/**
 * @brief
 *
 * @tparam T
 * @param block_p
 */
template<typename T> void CircularBuffer<T>::blockRead(bool block_p)
{
	// Returns successfully
	this->_rdProtected = block_p;
	this->_lastError = Error::NONE;
	return;
}

/**
 * @brief
 *
 * @tparam T
 * @param bypassProtection_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::flush(bool bypassProtection_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::flush(bool)", 4);

	// CHECK FOR ERROR - Not initialized
	if(!this->_checkInitialized()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_checkLocked()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// Must bypass read and write protection?
	if(!bypassProtection_p) {					// No, lets see if any protection is on!
		// CHECK FOR ERROR - Read protected
		if(this->_checkReadProtected()) {
			// Return error
			debugMessage(this->_lastError);
			return false;
		}
		// CHECK FOR ERROR - Write protected
		if(this->_checkWriteProtected()) {
			// Return error
			debugMessage(this->_lastError);
			return false;
		}
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer
	this->_rdProtected = true;					// Protects against read operations
	this->_wrProtected = true;					// Protects against write operations

	// Flushes data
	this->_empty = true;
	this->_full = false;
	this->_occupation = 0;
	this->_rdIndex = 0;;
	this->_wrIndex = 0;

	// Unlock procedure (also resets protection)
	this->_locked = false;						// Unlocks circular buffer
	this->_rdProtected = false;					// Allows read operations
	this->_wrProtected = false;					// Allows write operations

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * @tparam T
 * @param data_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::push(T data_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::push(T)", 4);

	// CHECK FOR ERROR - Not initialized
	if(!this->_checkInitialized()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_checkLocked()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Write protected
	if(this->_checkWriteProtected()) {
		debugMessage(this->_lastError);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	// Is buffer full?
	if(!this->_full) {							// No, normal operation!
		this->_data[this->_wrIndex] = data_p;			// Stores data
		this->_moveWritePointer();						// Moves WRITE pointer only
	} else {									// Yes, must check what to do!
		// Is overwriting allowed?
		if(this->_overwriting) {						// Yes, overwrites old data!
			this->_data[this->_wrIndex] = data_p;				// Stores data
			this->_moveBothPointers();							// Moves BOTH pointers
		} else {										// No, trow error!
			// Return error
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

/**
 * @brief
 *
 * @tparam T
 * @param data_p
 * @param keepData_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::pop(T *data_p, bool keepData_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::pop(T, bool)", 4);

	// CHECK FOR ERROR - Argument is a NULL pointer
	if(!isPointerValid(data_p)) {
		// Return error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - Not initialized
	if(!this->_checkInitialized()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_checkLocked()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Read protected
	if(this->_checkReadProtected()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Buffer empty
	if(this->_empty) {
		// Return error
		this->_lastError = Error::BUFFER_EMPTY;
		debugMessage(Error::BUFFER_EMPTY);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	// Gets data
	*data_p = this->_data[this->_rdIndex];		// Retrieves data
	// Must move pointer?
	if(!keepData_p) {							// Yes, pointer must be moved!
		this->_moveReadPointer();						// Moves pointer
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Return successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 *
 * @tparam T
 * @param bufData_p
 * @param bufSize_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::pushBuffer(T *bufData_p, uint16_t bufSize_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::pop(T, bool)", 4);

	// CHECK FOR ERROR - Argument is a NULL pointer
	if(bufData_p == NULL) {
		// Return error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - Buffer is zero-sized
	if(bufSize_p == 0) {
		// Return error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - Not initialized
	if(!this->_checkInitialized()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Buffer is locked
	if(this->_checkLocked()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Write protected
	if(this->_checkWriteProtected()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	uint16_t freeSpace = this->_maxSize - this->_occupation;
	// Is there enough space?
	if(freeSpace >= bufSize_p) {					// Yes, normal operation!
		for(uint16_t i = 0; i < bufSize_p; i++) {
			this->_data[this->_wrIndex] = bufData_p[i];	// Stores data
			this->_moveWritePointer();					// Moves WRITE pointer
		}
	} else {									// No, must check what to do!
		// Is overwriting allowed?
		if(this->_overwriting) {						// Yes, split operation!
			for(uint16_t i = 0; i < bufSize_p; i++) {
				this->_data[this->_wrIndex] = bufData_p[i];		// Stores data
				// Is buffer full?
				if(this->_full) {								// Yes, move BOTH pointers!
					this->_moveBothPointers();
				} else {										// No, move WRITE pointer only!
					this->_moveWritePointer();
				}
			}
		} else {										// No, trow error!
			// Return error
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

/**
 * @brief
 *
 * @tparam T
 * @param bufData_p
 * @param bufSize_p
 * @param keepData_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::popBuffer(T *bufData_p, uint16_t bufSize_p, bool keepData_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::pop(T, bool)", 4);

	// CHECK FOR ERROR - Argument is a NULL pointer
	if(bufData_p == NULL) {
		// Return error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - Buffer is zero-sized
	if(bufSize_p == 0) {
		// Return error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - Not initialized
	if(!this->_checkInitialized()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_checkLocked()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Read protected
	if(this->_checkReadProtected()) {
		// Return error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Not enough elements to get from buffer
	if(this->_occupation < bufSize_p) {
		// Return error
		this->_lastError = Error::BUFFER_NOT_ENOUGH_ELEMENTS;
		debugMessage(Error::BUFFER_NOT_ENOUGH_ELEMENTS);
		return false;
	}

	// Locking procedure
	this->_locked = true;						// Locks circular buffer

	// Keep record of current buffer status
	bool		emptyOld				= this->_empty;
	bool		fullOld					= this->_full;
	uint16_t	occupationOld			= this->_occupation;
	uint16_t	rdIndexOld				= this->_rdIndex;

	// Get elements from buffer
	for(uint16_t i = 0; i < bufSize_p; i++) {
		bufData_p[i] = this->_data[this->_rdIndex];	// Retrieves data
		this->_moveReadPointer();					// Moves READ pointer
	}

	// Must keep old data?
	if(keepData_p) {								// Yes, load old status!
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

/**
 * @brief
 *
 * @tparam T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_checkLocked(void)
{
	if(this->_locked) {
		this->_lastError = Error::LOCKED;
	}

	return this->_locked;
}

/**
 * @brief
 *
 * @tparam T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_checkInitialized(void)
{
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
	}

	return this->_initialized;
}

/**
 * @brief
 *
 * @tparam T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_checkWriteProtected(void)
{
	if(this->_wrProtected) {
		this->_lastError = Error::WRITE_PROTECTED;
	}

	return this->_wrProtected;
}

/**
 * @brief
 *
 * @tparam T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_checkReadProtected(void)
{
	if(!this->_rdProtected) {
		this->_lastError = Error::READ_PROTECTED;
	}

	return this->_rdProtected;
}

/**
 * @brief
 *
 * @tparam T
 */
template<typename T> void inlined CircularBuffer<T>::_moveReadPointer(void)
{
	this->_rdIndex++;							// Increments read pointer
	this->_rdIndex %= this->_maxSize;			// Resolves read pointer overflow
	this->_occupation--;						// Decreases occupation number
	this->_full = false;						// Not full anymore
	this->_empty = (this->_occupation == 0);	// Resolves empty status

	return;
}

/**
 * @brief
 *
 * @tparam T
 */
template<typename T> void inlined CircularBuffer<T>::_moveWritePointer(void)
{
	this->_wrIndex++;							// Increments write pointer
	this->_wrIndex %= this->_maxSize;			// Resolves write pointer overflow
	this->_occupation++;						// Increases occupation number
	this->_full = (this->_occupation == this->_maxSize);	// Resolves full status
	this->_empty = false;						// Not empty anymore

	return;
}

/**
 * @brief
 *
 * @tparam T
 */
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
