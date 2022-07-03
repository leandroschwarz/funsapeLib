/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	circularBuffer16.hpp
 * Module:		16-bits data-wide circular buffer for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 28, 2018
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
	CircularBuffer16(uint16_t bufferSize = 20, bool overwrite = false);
	~CircularBuffer16();

	// Methods
public:
	// -------------------------------------------------------------------------
	// Data manipulation -------------------------------------------------------
	// -------------------------------------------------------------------------

	//		Flushes all circular buffer data
	bool     flush(bool bypassProtection = false);
	//		Pushes data into circular buffer
	bool     push(uint16_t data);
	//		Pops data from circular buffer
	bool     pop(uint16_t *data, bool keepData = false);
	//		Pushes an entire buffer into circular buffer
	bool     pushBuffer(uint16_t *bufData, uint16_t bufSize);
	//		Pops an entire buffer into circular buffer
	bool     popBuffer(uint16_t *bufData, uint16_t bufSize, bool keepData = false);

	// -------------------------------------------------------------------------
	// Buffer information ------------------------------------------------------
	// -------------------------------------------------------------------------

	//		Check if circular buffer is empty
	bool     isEmpty(void);
	//		Check if circular buffer is full
	bool     isFull(void);
	//		Gets the number of occupied elements of the circular buffer
	uint16_t getOccupation(void);
	//		Gets the number of free elements of the circular buffer
	uint16_t getFreeSpace(void);
	//		Returns error state from last operation
	error_e  getLastError(void);

	// -------------------------------------------------------------------------
	// Buffer control ----------------------------------------------------------
	// -------------------------------------------------------------------------

	//		Blocks write access to circular buffer
	void     blockWrite(bool block);
	//		Blocks read access to circular buffer
	void     blockRead(bool block);

private:
	bool     checkLocked_(void);			// Check if buffer is locked
	bool     checkInitialized_(void);		// Check if buffer is initialized
	bool     checkWriteProtected_(void);	// Check if buffer is write protected
	bool     checkReadProtected_(void);		// Check if buffer is read protected
	void     moveReadPointer_(void);		// Moves read pointer one position
	void     moveWritePointer_(void);		// Moves write pointer one position
	void     moveBothPointers_(void);		// Moves read and write pointer one position

protected:

	// Data members
public:

private:
	uint16_t rdIndex_;
	uint16_t wrIndex_;
	uint16_t maxSize_;
	uint16_t occupation_;
	uint16_t *data_;
	// Package status
	error_e  lastError_;
	bool     initialized_		: 1;
	bool     empty_				: 1;
	bool     full_				: 1;
	bool     locked_			: 1;
	bool     rdProtected_		: 1;
	bool     wrProtected_		: 1;
	bool     overwriting_		: 1;

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
