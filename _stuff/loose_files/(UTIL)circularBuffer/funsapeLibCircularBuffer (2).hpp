/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibCircularBuffer.hpp
 * Module:			Generic data-wide circular buffer module for FunSAPE++
 * 						Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_CIRCULAR_BUFFER_HPP
#define __FUNSAPE_LIB_CIRCULAR_BUFFER_HPP		__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibCircularBuffer.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBuffer.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibCircularBuffer.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibCircularBuffer.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBuffer.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibCircularBuffer.hpp)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

// NONE

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// NONE

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// CircularBuffer - Class declaration
// =============================================================================

template<typename T>
class CircularBuffer
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading ---------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	CircularBuffer(
			uint16_t	bufferSize_p	= 20,
			bool		overwrite_p		= false
	);
	// TODO: Implement destructor function
	~CircularBuffer(
			void
	);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	uint16_t getFreeSpace(
			void
	);
	Error getLastError(
			void
	);
	uint16_t getOccupation(
			void
	);
	bool isEmpty(
			void
	);
	bool isFull(
			void
	);

	//     //////////////////    DATA MANIPULATION     //////////////////     //
	bool flush(
			bool bypassProtection_p = false
	);
	bool pop(
			T *data_p,
			bool keepData_p = false
	);
	bool popBuffer(
			T *bufData_p,
			uint16_t bufSize_p,
			bool keepData_p = false
	);
	bool push(
			T data_p
	);
	bool pushBuffer(
			T *bufData_p,
			uint16_t bufSize_p
	);

	//     ///////////////////     BUFFER CONTROL     ///////////////////     //
	void setBlockRead(
			bool block_p
	);
	void setBlockWrite(
			bool block_p
	);

private:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	bool _isInitialized(
			void
	);
	bool _isLocked(
			void
	);
	bool _isReadProtected(
			void
	);
	bool _isWriteProtected(
			void
	);

	//     /////////////    BUFFER POINTER MANIPULATION     /////////////     //
	void _moveBothPointers(
			void
	);
	void _moveReadPointer(
			void
	);
	void _moveWritePointer(
			void
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	bool		_empty					: 1;
	bool		_full					: 1;
	bool		_initialized			: 1;
	bool		_locked					: 1;
	bool		_overwriting			: 1;
	bool		_rdProtected			: 1;
	bool		_wrProtected			: 1;

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	uint16_t	_maxSize;

	//     /////////////////////    BUFFER DATA     /////////////////////     //
	T			*_data;
	uint16_t	_rdIndex;
	uint16_t	_wrIndex;
	uint16_t	_occupation;

	//     ///////////////////     ERROR MESSAGES     ///////////////////     //
	Error		_lastError;

protected:
	// NONE

};  // class CircularBuffer

// =============================================================================
// CircularBuffer - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// CircularBuffer - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Circular Buffer<T>::Circular Buffer object
 * @details
 * @tparam		T
 * @param[in]	bufferSize_p
 * @param[in]	overwrite_p
 */
template<typename T> CircularBuffer<T>::CircularBuffer(uint16_t bufferSize_p, bool overwrite_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::CircularBuffer(uint16_t, bool)", DEBUG_CODE_CIRCULAR_BUFFER);

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
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return;
	}
	// CHECK FOR ERROR - Size too small
	if(bufferSize_p < 2) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
		debugMessage(Error::BUFFER_SIZE_TOO_SMALL);
		return;
	}

	// Allocate memory
	this->_data = (T *)calloc(bufferSize_p, sizeof(T));
	if(!isPointerValid(this->_data)) {
		// Returns error
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

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Destroy the Circular Buffer<T>::Circular Buffer object
 * @details
 * @tparam		T
 */
template<typename T> CircularBuffer<T>::~CircularBuffer(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::~CircularBuffer(void)", DEBUG_CODE_CIRCULAR_BUFFER);

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

	// Returns successfully (but do NOT unlock)
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 * @details
 * @tparam		T
 * @return
 */
template<typename T> uint16_t CircularBuffer<T>::getFreeSpace(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::getFreeSpace(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Returns amount of free space
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return (this->_maxSize - this->_occupation);
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return
 */
template<typename T> Error CircularBuffer<T>::getLastError(void)
{
	// Returns successfully
	return this->_lastError;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return
 */
template<typename T> uint16_t CircularBuffer<T>::getOccupation(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::getOccupation(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Returns amount of occupied space
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return this->_occupation;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::isEmpty(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::isEmpty(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Returns whether buffer is empty or not
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return this->_empty;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::isFull(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::isFull(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Returns whether buffer is full or not
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return this->_full;
}

//     ////////////////////    DATA MANIPULATION     ////////////////////     //

/**
 * @brief
 * @details
 * @tparam		T
 * @param[in]	bypassProtection_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::flush(bool bypassProtection_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::flush(bool)", DEBUG_CODE_CIRCULAR_BUFFER);

	// CHECK FOR ERROR - Not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_isLocked()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// Must bypass read and write protection?
	if(!bypassProtection_p) {					// No, lets see if any protection is on!
		// CHECK FOR ERROR - Read protected
		if(this->_isReadProtected()) {
			// Returns error
			debugMessage(this->_lastError);
			return false;
		}
		// CHECK FOR ERROR - Write protected
		if(this->_isWriteProtected()) {
			// Returns error
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

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @param[out]	data_p
 * @param[in]	keepData_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::pop(T *data_p, bool keepData_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::pop(T *, bool)", DEBUG_CODE_CIRCULAR_BUFFER);

	// CHECK FOR ERROR - Argument is a NULL pointer
	if(!isPointerValid(data_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - Not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_isLocked()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Read protected
	if(this->_isReadProtected()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Buffer empty
	if(this->_empty) {
		// Returns error
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

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @param[out]	bufData_p
 * @param[in]	bufSize_p
 * @param[in]	keepData_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::popBuffer(T *bufData_p, uint16_t bufSize_p, bool keepData_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::popBuffer(T *, uint16_t, bool)", DEBUG_CODE_CIRCULAR_BUFFER);

	// CHECK FOR ERROR - Argument is a NULL pointer
	if(!isPointerValid(bufData_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - Buffer is zero-sized
	if(bufSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - Not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_isLocked()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Read protected
	if(this->_isReadProtected()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Not enough elements to get from buffer
	if(this->_occupation < bufSize_p) {
		// Returns error
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

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @param[in]	data_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::push(T data_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::push(T)", DEBUG_CODE_CIRCULAR_BUFFER);

	// CHECK FOR ERROR - Not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Locked
	if(this->_isLocked()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Write protected
	if(this->_isWriteProtected()) {
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
			// Returns error
			this->_locked = false;				// Unlocking procedure before exit
			this->_lastError = Error::BUFFER_FULL;
			debugMessage(Error::BUFFER_FULL);
			return false;
		}
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @param[in]	bufData_p
 * @param[in]	bufSize_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::pushBuffer(T *bufData_p, uint16_t bufSize_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::pushBuffer(T *, uint16_t)", DEBUG_CODE_CIRCULAR_BUFFER);

	// CHECK FOR ERROR - Argument is a NULL pointer
	if(!isPointerValid(bufData_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	// CHECK FOR ERROR - Buffer is zero-sized
	if(bufSize_p == 0) {
		// Returns error
		this->_lastError = Error::BUFFER_SIZE_ZERO;
		debugMessage(Error::BUFFER_SIZE_ZERO);
		return false;
	}
	// CHECK FOR ERROR - Not initialized
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Buffer is locked
	if(this->_isLocked()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	// CHECK FOR ERROR - Write protected
	if(this->_isWriteProtected()) {
		// Returns error
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
			// Returns error
			this->_locked = false;				// Unlocking procedure before exit
			this->_lastError = Error::BUFFER_NOT_ENOUGH_SPACE;
			debugMessage(Error::BUFFER_NOT_ENOUGH_SPACE);
			return false;
		}
	}

	// Unlocking procedure
	this->_locked = false;						// Unlocks circular buffer

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     /////////////////////     BUFFER CONTROL     /////////////////////     //

/**
 * @brief
 * @details
 * @tparam		T
 * @param[in]	block_p
 */
template<typename T> void CircularBuffer<T>::setBlockRead(bool block_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::setBlockRead(bool)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Update data members
	this->_rdProtected = block_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @param[in]	block_p
 */
template<typename T> void CircularBuffer<T>::setBlockWrite(bool block_p)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::setBlockWrite(bool)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Update data members
	this->_wrProtected = block_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 * @details
 * @tparam		T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isInitialized(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::_isInitialized(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// CHECK FOR ERROR - peripheral not initialized
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isLocked(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::_isLocked(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Check locking status
	if(this->_locked) {
		// Returns error
		this->_lastError = Error::LOCKED;
		debugMessage(Error::LOCKED);
		return true;
	}

	// Returns successfully (false - not locked)
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return false;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isReadProtected(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::_isReadProtected(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Check protection status
	if(this->_rdProtected) {
		// Returns error
		this->_lastError = Error::READ_PROTECTED;
		debugMessage(Error::READ_PROTECTED);
		return true;
	}

	// Returns successfully (false - not protected)
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return false;
}

/**
 * @brief
 * @details
 * @tparam		T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isWriteProtected(void)
{
	// Mark passage for debugging purpose
	debugMark("CircularBuffer::_isWriteProtected(void)", DEBUG_CODE_CIRCULAR_BUFFER);

	// Check protection status
	if(this->_wrProtected) {
		// Returns error
		this->_lastError = Error::WRITE_PROTECTED;
		debugMessage(Error::WRITE_PROTECTED);
		return true;
	}

	// Returns successfully (false - not protected)
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return false;
}

//     ///////////////    BUFFER POINTER MANIPULATION     ///////////////     //

/**
 * @brief
 * @details
 * @tparam		T
 */
template<typename T> void inlined CircularBuffer<T>::_moveBothPointers(void)
{
	// Move buffer pointers
	this->_wrIndex++;							// Increments write pointer
	this->_wrIndex %= this->_maxSize;			// Resolves write pointer overflow
	this->_rdIndex++;							// Increments read pointer
	this->_rdIndex %= this->_maxSize;			// Resolves read pointer overflow
	this->_full = (this->_occupation == this->_maxSize);	// Resolves full status
	this->_empty = (this->_occupation == 0);	// Resolves empty status

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief
 * @details
 * @tparam		T
 */
template<typename T> void inlined CircularBuffer<T>::_moveReadPointer(void)
{
	// Move buffer pointers
	this->_rdIndex++;							// Increments read pointer
	this->_rdIndex %= this->_maxSize;			// Resolves read pointer overflow
	this->_occupation--;						// Decreases occupation number
	this->_full = false;						// Not full anymore
	this->_empty = (this->_occupation == 0);	// Resolves empty status

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief
 * @details
 * @tparam		T
 */
template<typename T> void inlined CircularBuffer<T>::_moveWritePointer(void)
{
	// Move buffer pointers
	this->_wrIndex++;							// Increments write pointer
	this->_wrIndex %= this->_maxSize;			// Resolves write pointer overflow
	this->_occupation++;						// Increases occupation number
	this->_full = (this->_occupation == this->_maxSize);	// Resolves full status
	this->_empty = false;						// Not empty anymore

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_CIRCULAR_BUFFER_HPP

// =============================================================================
// END OF FILE
// =============================================================================
