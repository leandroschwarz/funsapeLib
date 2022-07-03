/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		lcd.hpp
 * Module:			Character LCD support for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LCD_HPP
#define __FUNSAPE_LCD_HPP						__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "globalDefines.hpp"
#ifndef __FUNSAPE_GLOBAL_DEFINES_HPP
#	error	[lcd.hpp] Error 105 - Global definitions file (globalDefines.hpp) is missing!
#elif __FUNSAPE_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[lcd.hpp] Error 106 - Build mismatch between header file (lcd.hpp) and global definitions file (globalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Standard header files -------------------------------------------------------

#include <stdio.h>

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

typedef enum lcdShiftOverwrite_t {
	LCD_DISPLAY_OVERWRITE = 0,
	LCD_DISPLAY_SHIFT = 1
} lcdShiftOverwrite_t;

typedef enum lcdIncrementDecrement_t {
	LCD_DECREMENT = 0,
	LCD_INCREMENT = 1
} lcdIncrementDecrement_t;

// -----------------------------------------------------------------------------
// Single variables => small enumerations that can be understood as a single variable

// NONE

// -----------------------------------------------------------------------------
// Enumerations => large enumerations or enumerations with expansible number of elements

typedef enum lcdFont_e {
	LCD_FONT_5X8 = 0,
	LCD_FONT_5X10 = 1
} lcdFont_e;

typedef enum lcdInterface_e {
	LCD_8_BITS_DELAY					= 0,
	LCD_8_BITS_POOLING					= 1,
	LCD_4_BITS_DELAY					= 2,
	LCD_4_BITS_POOLING					= 3
} lcdInterface_e;

typedef enum lcdSize_e {
	LCD_SIZE_UNDEFINED = 0,
	LCD_8X1		= 108,
	LCD_8X2		= 208,
	LCD_10x2	= 210,
	LCD_12X2	= 212,
	LCD_12X4	= 412,
	LCD_16X1	= 116,
	LCD_16X2	= 216,
	LCD_16X4	= 416,
	LCD_20X1	= 120,
	LCD_20X2	= 220,
	LCD_20X4	= 420,
	LCD_24X1	= 124,
	LCD_24X2	= 224,
	LCD_30X2	= 230,
	LCD_32X2	= 232,
	LCD_40X1	= 140,
	LCD_40X2	= 240
} lcdSize_e;

// -----------------------------------------------------------------------------
// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet's values

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Flags => enumerations that can be combined in the form of flags

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Structures, Unions and Bit Fields ---------------------------

// NONE

// =============================================================================
// Lcd Class
// =============================================================================

// TODO: Implement support to AVR
class Lcd
{
	// New data types - Public
public:

	// New data types - Private
private:

	// New data types - Protected
protected:

	// Constructors
public:
	Lcd(lcdSize_e size = LCD_16X2, lcdFont_e font = LCD_FONT_5X8);
	~Lcd(void);

	// Methods - Public
public:
	// -------------------------------------------------------------------------
	// Hardware configuration --------------------------------------------------
	// -------------------------------------------------------------------------

	//				TODO: Describe function
	bool setInterface(lcdInterface_e interface = LCD_4_BITS_DELAY);
	//				TODO: Describe function
	bool setDataPort(ioReg_t ioDataDir, ioPin_t ioDataFirst = GPIO_PIN_0);
	//				TODO: Describe function
	bool setControlPort(ioReg_t ioControlDir, ioPin_t ioControlE, ioPin_t ioControlRs, ioPin_t ioControlRw);
	//				TODO: Describe function
	bool setControlPort(ioReg_t ioControlDir, ioPin_t ioControlE, ioPin_t ioControlRs);
	//				TODO: Describe function
	bool displayShift(bool leftRight);
	//				TODO: Describe function
	bool setEntryMode(lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode);
	//				TODO: Describe function
	bool setEntryMode(lcdIncrementDecrement_t dir);
	//				TODO: Describe function
	bool setEntryMode(lcdShiftOverwrite_t mode);
	//				TODO: Describe function
	bool initialize(void);

	// -------------------------------------------------------------------------
	// Cursor and display management -------------------------------------------
	// -------------------------------------------------------------------------

	//				TODO: Describe function
	bool cursorBlink(bool blink);
	//				TODO: Describe function
	bool cursorGoTo(uint8_t line, uint8_t column);
	//				TODO: Describe function
	bool cursorHome(void);
	//				TODO: Describe function
	bool cursorMove(bool leftRight);
	//				TODO: Describe function
	bool cursorMoveFirstLine(void);
	//				TODO: Describe function
	bool cursorMoveNextLine(void);
	//				TODO: Describe function
	bool cursorShow(bool show);
	//				TODO: Describe function
	bool displayOn(bool turnOn);

	// -------------------------------------------------------------------------
	// Edition -----------------------------------------------------------------
	// -------------------------------------------------------------------------

	//				TODO: Describe function
	bool clearScreen(void);
	//				TODO: Describe function
	bool remapStdio(void);

	// -------------------------------------------------------------------------
	// Error handling ----------------------------------------------------------
	// -------------------------------------------------------------------------

	//				Returns error state from last operation
	error_e		getLastError(void);

	// Methods - Private
private:
	uint8_t		_getLine(void);					// TODO: Describe function
	uint8_t		_getColumn(void);				// TODO: Describe function
	void _functionSet8Bits(uint8_t command);	// TODO: Describe function

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	// Hardware configuration
	lcdSize_e				_size;						// TODO: Describe data member
	lcdFont_e				_font;						// TODO: Describe data member
	lcdInterface_e			_interface;					// TODO: Describe data member
	// I/O ports
	ioReg_t					_ioDataDir;					// TODO: Describe data member
	ioPin_t					_ioDataFirst		: 4;	// TODO: Describe data member
	ioPin_t					_ioDataMask;				// TODO: Describe data member
	ioReg_t					_ioControlDir;				// TODO: Describe data member
	ioPin_t					_ioControlE			: 4;	// TODO: Describe data member
	ioPin_t					_ioControlRs		: 4;	// TODO: Describe data member
	ioPin_t					_ioControlRw		: 4;	// TODO: Describe data member
	// Display and cursor configuration
	bool					_cursorBlink		: 1;	// 0 off, 1 on
	uint8_t					_cursorColumn		: 6;	// 0 to 39
	uint8_t					_cursorLine			: 2;	// 0 to 3
	bool					_cursorShow			: 1;	// 0 off, 1 on
	bool					_displayOn			: 1;	// 0 off, 1 on
	lcdIncrementDecrement_t	_entryIncDec		: 1;	// TODO: Describe data member
	lcdShiftOverwrite_t		_entryShiftDisplay	: 1;	// TODO: Describe data member
	// Status and control
	uint8_t					_columns			: 6;	// TODO: Describe data member
	uint8_t					_lines				: 2;	// TODO: Describe data member
	bool					_ctrlPortSet		: 1;	// TODO: Describe data member
	bool					_dataPortSet		: 1;	// TODO: Describe data member
	bool					_interfaceSet		: 1;	// TODO: Describe data member
	bool					_initialized		: 1;	// TODO: Describe data member
	error_e					_lastError;					// TODO: Describe data member

	// Data members - Protected
protected:

}; // class Lcd

// =============================================================================
// Lcd - Class inline function definitions
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

#endif // __FUNSAPE_LCD_HPP
