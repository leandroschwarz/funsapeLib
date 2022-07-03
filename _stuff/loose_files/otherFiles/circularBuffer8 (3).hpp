

#ifndef __CIRCULAR_BUFFER_8_HPP
#define __CIRCULAR_BUFFER_8_HPP 1001

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1001
#	error [packageApi.hpp] Error 100 - Build mismatch (globalDefines must be build 1001).
#endif
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum circularBufferError_e {
} circularBufferError_e;
// -----------------------------------------------------------------------------
// Macro-functions definitions -------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Extern Global Variables -----------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General public functions declarations ---------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General inline functions declarations ---------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// CircularBuffer8 class - Class Definition ------------------------------------

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
	error_e lastError_;
	bool     initialized_		: 1;
	bool     empty_				: 1;
	bool     full_				: 1;
	bool     locked_			: 1;
	bool     rdProtected_       : 1;
	bool     wrProtected_       : 1;

protected:

};  // class PackageApi

// -----------------------------------------------------------------------------
// PackageApi class - Inline functions definitions -----------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General inline functions declarations ---------------------------------------

/* NONE */

#endif // __CIRCULAR_BUFFER_8_HPP
