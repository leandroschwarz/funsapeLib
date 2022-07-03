/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibCircularBuffer64.hpp
 * Module:			64-bits data-wide circular buffer for FunSAPE++ Embedded
 * 					Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_CIRCULAR_BUFFER_64_HPP
#define __FUNSAPE_LIB_CIRCULAR_BUFFER_64_HPP	__BUILD_TO_REPLACE__

// =============================================================================
// System file dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "..\funsapeLibGlobalDefines.hpp"
#if __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error [funsapeLibCircularBuffer64.hpp] Error 101 - Global definitions file (funsapeLibGlobalDefines.hpp) must be build __BUILD_TO_REPLACE__.
#endif

// -----------------------------------------------------------------------------
// Other header files ----------------------------------------------------------

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

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (logic values) -------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (small and finite lists) ---------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (large or expansible lists) ------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (algebraic operated list elements) -----------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (bitwise operated flags) ---------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations (bitwise operated flags) ---------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// CircularBuffer64 - Class declaration
// =============================================================================

class CircularBuffer64
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading ---------------------------------------------------

	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	CircularBuffer64(
		uint16_t	bufferSize			= 20,
		bool		overwrite			= false);
	~CircularBuffer64(void);

	// -------------------------------------------------------------------------
	// Methods -----------------------------------------------------------------

public:
	//     //////////////////    DATA MANIPULATION     //////////////////     //
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

	//     /////////////////     BUFFER INFORMATION     /////////////////     //
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

	//     ///////////////////     BUFFER CONTROL     ///////////////////     //
	//		Blocks write access to circular buffer
	void     blockWrite(bool block);
	//		Blocks read access to circular buffer
	void     blockRead(bool block);

private:
	//     ///////////////////     ??????????????     ///////////////////     //
	bool     _checkLocked(void);			// Check if buffer is locked
	bool     _checkInitialized(void);		// Check if buffer is initialized
	bool     _checkWriteProtected(void);	// Check if buffer is write protected
	bool     _checkReadProtected(void);		// Check if buffer is read protected
	void     _moveReadPointer(void);		// Moves read pointer one position
	void     _moveWritePointer(void);		// Moves write pointer one position
	void     _moveBothPointers(void);		// Moves read and write pointer one position

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

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
	uint64_t	*_data;
	uint16_t	_rdIndex;
	uint16_t	_wrIndex;
	uint16_t	_occupation;

	//     ///////////////////     ERROR MESSAGES     ///////////////////     //
	error_e		_lastError;

protected:

};  // class CircularBuffer64

// =============================================================================
// CircularBuffer64 - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
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

#endif // __FUNSAPE_LIB_CIRCULAR_BUFFER_64_HPP
