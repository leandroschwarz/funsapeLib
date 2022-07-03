/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		circularBuffer32.hpp
 * Module:			32-bits data-wide circular buffer for FunSAPE++ Embedded
 *					Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_CIRCULAR_BUFFER_32_HPP
#define __FUNSAPE_CIRCULAR_BUFFER_32_HPP		__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "globalDefines.hpp"
#ifndef __FUNSAPE_GLOBAL_DEFINES_HPP
#	error	[circularBuffer32.hpp] Error 105 - Global definitions file (globalDefines.hpp) is missing!
#elif __FUNSAPE_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[circularBuffer32.hpp] Error 106 - Build mismatch between header file (circularBuffer32.hpp) and global definitions file (globalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

#include <stdlib.h>

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
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// CircularBuffer32 Class
// =============================================================================

class CircularBuffer32
{
	// New data types - Public
public:

	// New data types - Private
private:

	// New data types - Protected
protected:

	// Constructors
public:
	CircularBuffer32(uint16_t bufferSize = 20, bool overwrite = false);
	~CircularBuffer32(void);

	// Methods - Public
public:
	// -------------------------------------------------------------------------
	// Data manipulation -------------------------------------------------------
	// -------------------------------------------------------------------------

	//				Flushes all circular buffer data
	bool		flush(bool bypassProtection = false);
	//				Pushes data into circular buffer
	bool		push(uint32_t data);
	//				Pops data from circular buffer
	bool		pop(uint32_t *data, bool keepData = false);
	//				Pushes an entire buffer into circular buffer
	bool		pushBuffer(uint32_t *bufData, uint16_t bufSize);
	//				Pops an entire buffer into circular buffer
	bool		popBuffer(uint32_t *bufData, uint16_t bufSize, bool keepData = false);

	// -------------------------------------------------------------------------
	// Buffer information ------------------------------------------------------
	// -------------------------------------------------------------------------

	//				Check if circular buffer is empty
	bool		isEmpty(void);
	//				Check if circular buffer is full
	bool		isFull(void);
	//				Gets the number of occupied elements of the circular buffer
	uint16_t	getOccupation(void);
	//				Gets the number of free elements of the circular buffer
	uint16_t	getFreeSpace(void);
	//				Returns error state from last operation
	error_e		getLastError(void);

	// -------------------------------------------------------------------------
	// Buffer control ----------------------------------------------------------
	// -------------------------------------------------------------------------

	//				Blocks write access to circular buffer
	bool		blockWrite(bool block);
	//				Blocks read access to circular buffer
	bool		blockRead(bool block);

	// Methods - Private
private:
	void		_moveReadPointer(void);			// Moves read pointer one position
	void		_moveWritePointer(void);		// Moves write pointer one position
	void		_moveBothPointers(void);		// Moves read and write pointer one position

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	uint16_t	_rdIndex;						// TODO: Describe data member
	uint16_t	_wrIndex;						// TODO: Describe data member
	uint16_t	_maxSize;						// TODO: Describe data member
	uint16_t	_occupation;					// TODO: Describe data member
	uint32_t	*_data;							// TODO: Describe data member
	error_e		_lastError;						// TODO: Describe data member
	bool		_initialized			: 1;	// TODO: Describe data member
	bool		_empty					: 1;	// TODO: Describe data member
	bool		_full					: 1;	// TODO: Describe data member
	bool		_locked					: 1;	// TODO: Describe data member
	bool		_rdProtected			: 1;	// TODO: Describe data member
	bool		_wrProtected			: 1;	// TODO: Describe data member
	bool		_overwriting			: 1;	// TODO: Describe data member

	// Data members - Protected
protected:

}; // class CircularBuffer32

// =============================================================================
// CircularBuffer32 - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
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

#endif // __FUNSAPE_CIRCULAR_BUFFER_32_HPP
