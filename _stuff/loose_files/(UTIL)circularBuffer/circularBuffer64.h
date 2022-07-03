/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	circularBuffer64.h
 * Module:		64-bits data-wide circular buffer for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __CIRCULAR_BUFFER_64_H
#define __CIRCULAR_BUFFER_64_H					__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "..\gpdseArmGlobalDefines.h"
#if __GPDSE_ARM_GLOBAL_DEFINES_H != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (gpdseArmGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constants
// =============================================================================

// NONE

// =============================================================================
// Macro-functions
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// CircularBuffer64 Class
// =============================================================================

class CircularBuffer64
{
	// New data types
public:

	// Constructors
public:
	CircularBuffer64(uint16_t bufferSize = 20, bool overwrite = false);
	~CircularBuffer64();

	// Methods
public:
	// -------------------------------------------------------------------------
	// Data manipulation -------------------------------------------------------
	// -------------------------------------------------------------------------

	//		Flushes all circular buffer data
	bool     flush(bool bypassProtection = false);
	//		Pushes data into circular buffer
	bool     push(uint64_t data);
	//		Pops data from circular buffer
	bool     pop(uint64_t *data, bool keepData = false);
	//		Pushes an entire buffer into circular buffer
	bool     pushBuffer(uint64_t *bufData, uint16_t bufSize);
	//		Pops an entire buffer into circular buffer
	bool     popBuffer(uint64_t *bufData, uint16_t bufSize, bool keepData = false);

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
	uint64_t *data_;
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

};  // class CircularBuffer64

// =============================================================================
// CircularBuffer64 - Class inline function definitions
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

#endif // __CIRCULAR_BUFFER_64_H
