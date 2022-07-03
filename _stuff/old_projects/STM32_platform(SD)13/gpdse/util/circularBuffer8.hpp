
/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	circularBuffer8.hpp
 * Module:		8-bits data-wide circular buffer for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

#ifndef __CIRCULAR_BUFFER_8_HPP
#define __CIRCULAR_BUFFER_8_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "..\globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error [packageApi.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
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
// CircularBuffer8 Class
// =============================================================================

class CircularBuffer8
{
	// New data types
public:

	// Constructors
public:
	CircularBuffer8(uint16_t bufferSize);
	~CircularBuffer8();

	// Methods
public:
	bool     push(uint8_t data);		// Add some value to the buffer
	bool     pop(uint8_t * data);		// Add some value to the buffer
	bool     isEmpty(void);				// Check if buffer is empty
	bool     isFull(void);				// Check if buffer is full
	uint16_t getOccupation(void);		// Number of bytes at the buffer
	bool     flush(void);				// Flushes all buffer data
	void     blockWrite(bool block);	// Blocks device to write access
	void     blockRead(bool block);		// Blocks device to read access
	error_e  getLastError(void);		// Returns error state from last operation

private:
	bool     isLocked(void);			// Check if buffer is locked
	bool     isInitialized(void);		// Check if buffer is initialized
	bool     isWriteProtected(void);	// Check if buffer is initialized
	bool     isReadProtected(void);		// Check if buffer is initialized

protected:

	// Data members
public:

private:
	uint16_t rdIndex_;
	uint16_t wrIndex_;
	uint16_t maxSize_;
	uint16_t occupation_;
	uint8_t  * data_;
	// Package status
	error_e  lastError_;
	bool     initialized_		: 1;
	bool     empty_				: 1;
	bool     full_				: 1;
	bool     locked_			: 1;
	bool     rdProtected_       : 1;
	bool     wrProtected_       : 1;

protected:

};  // class CircularBuffer8

// =============================================================================
// Template - Class inline function definitions
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

#endif // __CIRCULAR_BUFFER_8_HPP
