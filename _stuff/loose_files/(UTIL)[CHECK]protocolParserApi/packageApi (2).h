// *INDENT-OFF*

/* =============================================================================
 * Project:
 * File name:		packageApi.h
 * Module:			API Package Frame decoder module
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard - Begin
// =============================================================================

#ifndef __PACKAGE_API_H
#define __PACKAGE_API_H							__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "globalDefines.h"
#ifndef __GLOBAL_DEFINES_H
#	error	[packageApi.h] Error 12 - Global definitions file (globalDefines.h) is missing or corrupted!
#elif __GLOBAL_DEFINES_H != __BUILD_TO_REPLACE__
#	error	[packageApi.h] Error 13 - Build mismatch between global definitions file (globalDefines.h) and current header file (packageApi.h)!
#endif

// -----------------------------------------------------------------------------
// Header files - Library header files -----------------------------------------

#include "circularBuffer.h"
#ifndef __CIRCULAR_BUFFER_H
#	error	[packageApi.h] Error 14 - Required module (circularBuffer.h) is missing or corrupted!
#elif __CIRCULAR_BUFFER_H != __BUILD_TO_REPLACE__
#	error	[packageApi.h] Error 15 - Build mismatch between required module (circularBuffer.h) and current header file (packageApi.h)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

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
// INDENT-OFF*
// =============================================================================

// NONE

// INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// PackageApi - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class PackageApi
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	// NONE

private:
	enum class FrameState : uint8_t {
		GET_START                               = 0,
		GET_ID                                  = 1,
		GET_SIZE                                = 2,
		GET_TYPE                                = 3,
		GET_DATA                                = 4,
		GET_CHECKSUM                            = 5,
		READY                                   = 6,
	};

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
	PackageApi(
		void
	);
	PackageApi(
		cuint16_t       circularBufferSize_p,
		cuint16_t       frameBufferSize_p,
		cbool_t         overwrite_p
	);
	// TODO: Implement destructor function
	~PackageApi(
		void
	);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	Error getLastError(
		void
	);
	bool init(
		cuint16_t       circularBufferSize_p    = 200,
		cuint16_t       frameBufferSize_p       = 35,
		cbool_t         overwrite_p             = false
	);
	bool isFrameValid(
		void
	);
	bool isInitialized(
		void
	);
	bool isNewFrameAvailable(
		void
	);

	//     //////////////    FRAME BUFFER MANIPULATION     //////////////     //
	bool feedBuffer(
		uint8_t        data_p
	);
	bool feedBuffer(
		uint8_t        *data_p,
		cuint16_t       size_p
	);
	bool flushBuffer(
		void
	);
	uint16_t getBufferFreeSpace(
		void
	);
	uint16_t getBufferOccupation(
		void
	);
	bool isBufferEmpty(
		void
	);
	bool isBufferFull(
		void
	);

	//     ///////////////    FRAME DATA MANIPULATION     ///////////////     //
	bool getFrameData(
		uint8_t         *frameId_p,
		uint8_t         *frameType_p,
		uint8_t         *frameData_p,
		uint16_t        *frameDataSize_p,
		cuint16_t       maxDataSize_p,
		bool_t         *isFrameValid_p
	);

private:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	bool _deinitialize(
		void
	);
	bool _initialize(
		cuint16_t       circularBufferSize_p,
		cuint16_t       frameBufferSize_p,
		cbool_t         overwrite_p
	);
	bool _processInternalBuffer(
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
	bool_t                      _initialized            : 1;
	bool_t                      _newFrameReady          : 1;
	bool_t                      _frameValidity          : 1;

	//     //////////////////     FRAME PROCESSING     //////////////////     //
	FrameState                  _frameState;
	bool                        _escapeNextChar         : 1;
	uint16_t                    _frameDataIndex;

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	uint16_t                    _frameDataMaxSize;

	//     //////////////////    FRAME BUFFER DATA     //////////////////     //
	CircularBuffer<uint8_t>     _circularBuffer;

	//     /////////////////     FRAME DECODED DATA     /////////////////     //
	uint8_t                     *_frameDataBuffer;
	uint16_t                    _frameDataSize;
	uint8_t                     _frameType;
	uint8_t                     _frameId;

	//     ///////////////////     ERROR MESSAGES     ///////////////////     //
	Error                       _lastError;

protected:
	// NONE

};  // class PackageApi

// =============================================================================
// PackageApi - Class overloading operators
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
// PackageApi - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE
void myErrorHandler(char * message);

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

// =============================================================================
// Include guard - End
// =============================================================================

#endif // __PACKAGE_API_H

// =============================================================================
// END OF FILE
// =============================================================================

// *INDENT-ON*
