/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibCircularBuffer.hpp
 * Module:			Generic data-wide circular buffer module for FunSAPE++
 * 					Embedded Library project
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
#	error	[funsapeLibCircularBuffer.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBuffer.hpp] Error 10 - Build mismatch between (funsapeLibCircularBuffer.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

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
	// Methods - Inherited methods ---------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     //////////////////    DATA MANIPULATION     //////////////////     //
	bool flush(
			bool bypassProtection_p = false
	);
	bool push(
			T data_p
	);
	bool pop(
			T *data_p,
			bool keepData_p = false
	);
	bool pushBuffer(
			T *bufData_p,
			uint16_t bufSize_p
	);
	bool popBuffer(
			T *bufData_p,
			uint16_t bufSize_p,
			bool keepData_p = false
	);

	//     /////////////////     BUFFER INFORMATION     /////////////////     //
	bool isEmpty(
			void
	);
	bool isFull(
			void
	);
	uint16_t getOccupation(
			void
	);
	uint16_t getFreeSpace(
			void
	);
	Error getLastError(
			void
	);

	//     ///////////////////     BUFFER CONTROL     ///////////////////     //
	void blockWrite(
			bool block_p
	);
	void blockRead(
			bool block_p
	);

private:
	//     ///////////////////     ??????????????     ///////////////////     //
	bool _checkLocked(
			void
	);
	bool _checkInitialized(
			void
	);
	bool _checkWriteProtected(
			void
	);
	bool _checkReadProtected(
			void
	);
	void _moveReadPointer(
			void
	);
	void _moveWritePointer(
			void
	);
	void _moveBothPointers(
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
// Include template implementation
// =============================================================================

#include "funsapeLibCircularBufferImp.hpp"
#ifndef __FUNSAPE_LIB_CIRCULAR_BUFFER_IMP_HPP
#	error	[funsapeLibCircularBuffer.hpp] Error 15 - Template implementation (funsapeLibCircularBufferImp.hpp) header file is corrupted or missing!
#elif __FUNSAPE_LIB_CIRCULAR_BUFFER_IMP_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibCircularBuffer.hpp] Error 16 - Build mismatch between template implementation (funsapeLibCircularBufferImp.hpp) and (funsapeLibCircularBuffer.hpp) header files!
#endif

#endif // __FUNSAPE_LIB_CIRCULAR_BUFFER_HPP
