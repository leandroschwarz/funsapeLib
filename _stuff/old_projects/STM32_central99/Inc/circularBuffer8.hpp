/* =============================================================================
 * Project:		FunSAPE++ Library
 * File name:	circularBuffer8.hpp
 * Module:		8-bits data-wide circular buffer for FunSAPE++ Library project
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __CIRCULAR_BUFFER_8_HPP
#define __CIRCULAR_BUFFER_8_HPP					__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "globalDefines.hpp"
#if __ARM_GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (globalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Undefining previous definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Defining new constants
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Defining macro-functions
// -----------------------------------------------------------------------------

// NONE

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Logic values
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Finite lists
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Expansible lists
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be algebraically manipulated
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be manipulated using bitwise operations
// -----------------------------------------------------------------------------

// NONE

// =============================================================================
// Global variable declaration
// =============================================================================

// NONE

// =============================================================================
// Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Class CircularBuffer8 - Start
// -----------------------------------------------------------------------------

// =============================================================================
// CircularBuffer8 Class
// =============================================================================

class CircularBuffer8
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------
	// NONE

	// -------------------------------------------------------------------------
	// New data declaration ----------------------------------------------------
private:
	// NONE

	// -------------------------------------------------------------------------
	// New data overloading operators ------------------------------------------
private:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------
public:

	CircularBuffer8(					// Automatic constructor
		uint16_t bufferSize = 20,
		bool overwrite = false
	);
	~CircularBuffer8(void);				// Automatic destructor

	// -------------------------------------------------------------------------
	// Public methods
	// -------------------------------------------------------------------------
public:

	//     /////////////////     DATA  MANIPULATION     /////////////////     //

	bool flush(							// Flushes all circular buffer data
		bool bypassProtection = false
	);
	bool push(							// Pushes data into circular buffer
		uint8_t data
	);
	bool pop(							// Pops data from circular buffer
		uint8_t *data,
		bool keepData = false
	);
	bool pushBuffer(					// Pushes an entire buffer into circular buffer
		uint8_t *bufData,
		uint16_t bufSize
	);
	bool popBuffer(						// Pops an entire buffer into circular buffer
		uint8_t *bufData,
		uint16_t bufSize,
		bool keepData = false
	);

	//     /////////////////     BUFFER INFORMATION     /////////////////     //

	bool isEmpty(void);					// Check if circular buffer is empty
	bool isFull(void);					// Check if circular buffer is full
	uint16_t getOccupation(void);		// Gets the number of occupied elements of the circular buffer
	uint16_t getFreeSpace(void);		// Gets the number of free elements of the circular buffer
	error_e getLastError(void);			// Returns error state from last operation

	//     ///////////////////     BUFFER CONTROL     ///////////////////     //

	void blockWrite(					// Blocks write access to circular buffer
		bool block
	);
	void blockRead(						// Blocks read access to circular buffer
		bool block
	);

	// -------------------------------------------------------------------------
	// Private methods
	// -------------------------------------------------------------------------
private:

	bool inlined _checkLocked(void);			// Check if buffer is locked
	bool inlined _checkInitialized(void);		// Check if buffer is initialized
	bool inlined _checkWriteProtected(void);	// Check if buffer is write protected
	bool inlined _checkReadProtected(void);		// Check if buffer is read protected
	void inlined _moveReadPointer(void);		// Moves read pointer one position
	void inlined _moveWritePointer(void);		// Moves write pointer one position
	void inlined _moveBothPointers(void);		// Moves read and write pointer one position

	// -------------------------------------------------------------------------
	// Protected methods
	// -------------------------------------------------------------------------
protected:

	//     ///////////////////////     TO  DO     ///////////////////////     //

	// NONE

	// -------------------------------------------------------------------------
	// Private properties
	// -------------------------------------------------------------------------
private:

	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool			_initialized		: 1;

	//     ///////////////////////     STATUS     ///////////////////////     //
	uint16_t		_occupation;
	bool			_empty				: 1;
	bool			_full				: 1;
	bool			_locked				: 1;
	bool			_rdProtected		: 1;
	bool			_wrProtected		: 1;
	bool			_overwriting		: 1;
	error_e			_lastError;

	//     ///////////////////////     BUFFER     ///////////////////////     //
	uint8_t			* _data;
	uint16_t		_rdIndex;
	uint16_t		_wrIndex;
	uint16_t		_maxSize;

	// -------------------------------------------------------------------------
	// Protected properties
	// -------------------------------------------------------------------------
protected:

}; // class CircularBuffer8

// -----------------------------------------------------------------------------
// Class CircularBuffer8 - Class overloading operators
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class CircularBuffer8 - Class inline functions definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class CircularBuffer8 - End
// -----------------------------------------------------------------------------

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

#endif // __CIRCULAR_BUFFER_8_HPP
