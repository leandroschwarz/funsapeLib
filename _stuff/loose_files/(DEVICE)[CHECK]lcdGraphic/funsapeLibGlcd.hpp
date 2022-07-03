/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibGlcd.hpp
 * Module:			Graphical LCD Display interface for FunSAPE++ Embedded
 * 					Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * Notes:			The code is based on U8g2lib Library from XXXXXX, available
 * 					at YYYYYYYYYYYYY.
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_GLCD_HPP
#define __FUNSAPE_LIB_GLCD_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "..\funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibGlcd.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibGlcd.hpp] Error 11 - Build mismatch between (funsapeLibGlcd.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

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
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables => shortening the name of single data types ----------------

// NONE

// -----------------------------------------------------------------------------
// Single variables => enumerations understood as logic values -----------------

// NONE

// -----------------------------------------------------------------------------
// Single variables => small enumerations that can be understood as a single variable

// NONE

// -----------------------------------------------------------------------------
// Enumerations => large enumerations or enumerations with expansible number of element

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet's values

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Flags => enumerations that can be combined in the form of flags

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Structures, Unions and Bit Fields ---------------------------

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Classes -----------------------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// GLCD - Class declaration
// =============================================================================

class GLCD
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:

	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading
	// -------------------------------------------------------------------------

	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------

public:
	GLCD();											// TODO: Implement function
	~GLCD(void);									// TODO: Implement function

	// -------------------------------------------------------------------------
	// Methods
	// -------------------------------------------------------------------------

	// Methods - Public
public:
	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool	initialize(
		void
	);
	bool	setDataPins(
		funsapeIoPin_t rsPin,
		funsapeIoPin_t rdPin,
		funsapeIoPin_t wrPin,
		funsapeIoPin_t rstPin,
		funsapeIoPin_t csPin
	);
	bool	setControlPins(
		funsapeIoPin_t cdPin,
		funsapeIoPin_t cePin,
		funsapeIoPin_t fsPin,
		funsapeIoPin_t rdPin,
		funsapeIoPin_t rstPin,
		funsapeIoPin_t wrPin
	);

	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	error_e	getLastError(void
	);

	//     ///////////////////////     COLORS     ///////////////////////     //

	//     ///////////////////     BASIC DRAWINGS     ///////////////////     //

	//     ////////////////////////     TEXT     ////////////////////////     //

	// Methods - Private
private:

	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

	// Data members - Private
private:

	// NONE

}; // class GLCD

// =============================================================================
// GLCD - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// GLCD - Class inline function definitions
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

#endif // __FUNSAPE_LIB_GLCD_HPP
