
/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	circularBuffer16.hpp
 * Module:		16-bits data-wide circular buffer for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 16, 2018
 * ========================================================================== */

#ifndef __CIRCULAR_BUFFER_16_HPP
#define __CIRCULAR_BUFFER_16_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "..\globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error [circularBuffer16.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
/* NONE */

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

/* NONE */

// =============================================================================
// CircularBuffer16 Class
// =============================================================================

class CircularBuffer16 {
	// New data types
public:

	// Constructors
public:
	CircularBuffer16(uint16_t bufferSize);
	~CircularBuffer16();

	// Methods
public:
	bool     push(uint16_t data);		// Pushes data into buffer
	bool     pop(uint16_t *data);		// Pops data from buffer
	bool     isEmpty(void);				// Check if buffer is empty
	bool     isFull(void);				// Check if buffer is full
	uint16_t getOccupation(void);		// Number of occupied elements at the buffer
	uint16_t getFreeSpace(void);		// Number of free elements at the buffer
	bool     flush(void);				// Flushes all buffer data
	void     blockWrite(bool block);	// Controls write access to device
	void     blockRead(bool block);		// Controls read access to device
	error_e  getLastError(void);		// Returns error state from last operation

private:
	bool     isLocked(void);			// Check if buffer is locked
	bool     isInitialized(void);		// Check if buffer is initialized
	bool     isWriteProtected(void);	// Check if buffer is write protected
	bool     isReadProtected(void);		// Check if buffer is read protected

protected:

	// Data members
public:

private:
	uint16_t rdIndex_;
	uint16_t wrIndex_;
	uint16_t maxSize_;
	uint16_t occupation_;
	uint16_t   *data_;
	// Package status
	error_e  lastError_;
	bool     initialized_		: 1;
	bool     empty_				: 1;
	bool     full_				: 1;
	bool     locked_			: 1;
	bool     rdProtected_       : 1;
	bool     wrProtected_       : 1;

protected:

};  // class CircularBuffer16

// =============================================================================
// CircularBuffer16 - Class inline function definitions
// =============================================================================

/* NONE */

// =============================================================================
// Extern global variables
// =============================================================================

/* NONE */

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __CIRCULAR_BUFFER_16_HPP
