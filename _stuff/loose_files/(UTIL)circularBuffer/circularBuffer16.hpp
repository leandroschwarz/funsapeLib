
/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	circularBuffer16.hpp
 * Module:		8-bits data-wide circular buffer for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

#ifndef __CIRCULAR_BUFFER_16_HPP
#define __CIRCULAR_BUFFER_16_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Header files
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

class CircularBuffer16
{
	// New data types
public:

	// Constructors
public:
	CircularBuffer16(uint16_t bufferSize);
	~CircularBuffer16();

	// Methods
public:
	bool     push(uint16_t data);		// Add some value to the buffer
	bool     pop(uint16_t *data);		// Add some value to the buffer
	bool     isEmpty(void);				// Check if buffer is empty
	bool     isFull(void);				// Check if buffer is full
	uint16_t getOccupation(void);		// Number of bytes at the buffer
	bool     flush(void);				// Flushes all buffer data
	void     blockWrite(bool block);	// Blocks device to write access
	void     blockRead(bool block);		// Blocks device to read access

private:
	bool     isLocked(void);			// Check if buffer is locked
	bool     isInitialized(void);		// Check if buffer is initialized
	bool     isWriteProtected(void);	// Check if buffer is initialized
	bool     isReadProtected(void);		// Check if buffer is initialized

protected:

	// Data members
public:

private:
	uint16_t _rdIndex;
	uint16_t _wrIndex;
	uint16_t _maxSize;
	uint16_t _occupation;
	uint16_t   *_data;
	// Package status
//	error_e lastError_;
	bool     _initialized		: 1;
	bool     _empty				: 1;
	bool     _full				: 1;
	bool     _locked			: 1;
	bool     _rdProtected       : 1;
	bool     _wrProtected       : 1;

protected:

};  // class CircularBuffer16

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

#endif // __CIRCULAR_BUFFER_16_HPP
