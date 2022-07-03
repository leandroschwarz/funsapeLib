/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseAvrLcdTwi.hpp
 * Module:		Character LCD controller Using PCF8594 over TWI
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_AVR_LCD_TWI_HPP
#define __GPDSE_AVR_LCD_TWI_HPP			__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseAvrGlobalDefines.hpp>
#if __GPDSE_AVR_GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (gpdseAvrGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

#include <gpdseAvrTwi.hpp>

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define LCD_CLEAR_SCREEN		0x01
#define LCD_CURSOR_HOME			0x02
#define LCD_ENTRY_SET			0x04
#define LCD_ENTRY_INCREMENT		0x02
#define LCD_ENTRY_DECREMENT		0x00
#define LCD_ENTRY_SHIFT			0x01
#define LCD_ENTRY_OVERWRITE		0x00
#define LCD_DISPLAY_OFF			0x08
#define LCD_DISPLAY_ON			0x0C
#define LCD_FUNCTION_RESET		0x30
#define LCD_FUNCTION_SET		0x20
#define LCD_FUNCTION_8_BITS		0x10
#define LCD_FUNCTION_4_BITS		0x00
#define LCD_FUNCTION_2_LINES	0x08
#define LCD_FUNCTION_1_LINE		0x00
#define LCD_FUNCTION_5x10_FONT	0x04
#define LCD_FUNCTION_5x8_FONT	0x00
#define LCD_CGRAM_ADRESS		0x40
#define LCD_DDRAM_ADRESS		0x80

// =============================================================================
// Macro-functions
// =============================================================================

// NONE

// =============================================================================
// New data types - Single variables
// =============================================================================

// NONE

// =============================================================================
// New data types - Enumerations (logic values)
// =============================================================================

// NONE

// =============================================================================
// New data types - Enumerations (small and finite lists)
// =============================================================================

typedef enum lcdCursorState_e {
	LCD_CURSOR_OFF						= 0,
	LCD_CURSOR_ON						= 1,
	LCD_CURSOR_BLINK					= 2
} lcdCursorState_e;

typedef enum lcdEntryMode_e {
	LCD_LEFT_OVERWRITE					= 0,
	LCD_RIGHT_SHIFT						= 1,
	LCD_RIGHT_OVERWRITE					= 2,
	LCD_LEFT_SHIFT						= 3
} lcdEntryMode_e;

typedef enum lcdFontSize_e {
	LCD_FONT_SIZE_5X8					= 0,
	LCD_FONT_SIZE_5X10					= 1
} lcdFontSize_e;

// =============================================================================
// New data types - Enumerations (large or expansible lists)
// =============================================================================

typedef enum lcdDisplaySize_e {
	LCD_DISPLAY_SIZE_UNDEFINED					= 0,
	LCD_DISPLAY_SIZE_8X1						= 108,
	LCD_DISPLAY_SIZE_8X2						= 208,
	LCD_DISPLAY_SIZE_10x2						= 210,
	LCD_DISPLAY_SIZE_12X2						= 212,
	LCD_DISPLAY_SIZE_12X4						= 412,
	LCD_DISPLAY_SIZE_16X1						= 116,
	LCD_DISPLAY_SIZE_16X2						= 216,
	LCD_DISPLAY_SIZE_16X4						= 416,
	LCD_DISPLAY_SIZE_20X1						= 120,
	LCD_DISPLAY_SIZE_20X2						= 220,
	LCD_DISPLAY_SIZE_20X4						= 420,
	LCD_DISPLAY_SIZE_24X1						= 124,
	LCD_DISPLAY_SIZE_24X2						= 224,
	LCD_DISPLAY_SIZE_30X2						= 230,
	LCD_DISPLAY_SIZE_32X2						= 232,
	LCD_DISPLAY_SIZE_40X1						= 140,
	LCD_DISPLAY_SIZE_40X2						= 240
} lcdDisplaySize_e;

typedef enum lcdTwiPinout_e {
	LCD_TWI_PINOUT_D7_D6_D5_D4_BL_EN_RW_RS		= 0,
	LCD_TWI_PINOUT_D7_D6_D5_D4_BL_EN_RS_RW		= 1,
	LCD_TWI_PINOUT_D7_D6_D5_D4_BL_RW_EN_RS		= 2,
	LCD_TWI_PINOUT_D7_D6_D5_D4_BL_RW_RS_EN		= 3,
	LCD_TWI_PINOUT_D7_D6_D5_D4_BL_RS_EN_RW		= 4,
	LCD_TWI_PINOUT_D7_D6_D5_D4_BL_RS_RW_EN		= 5,
	LCD_TWI_PINOUT_D7_D6_D5_D4_EN_BL_RW_RS		= 6,
	LCD_TWI_PINOUT_D7_D6_D5_D4_EN_BL_RS_RW		= 7,
	LCD_TWI_PINOUT_D7_D6_D5_D4_EN_RW_BL_RS		= 8,
	LCD_TWI_PINOUT_D7_D6_D5_D4_EN_RW_RS_BL		= 9,
	LCD_TWI_PINOUT_D7_D6_D5_D4_EN_RS_BL_RW		= 10,
	LCD_TWI_PINOUT_D7_D6_D5_D4_EN_RS_RW_BL		= 11,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RW_BL_EN_RS		= 12,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RW_BL_RS_EN		= 13,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RW_EN_BL_RS		= 14,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RW_EN_RS_BL		= 15,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RW_RS_BL_EN		= 16,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RW_RS_EN_BL		= 17,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RS_BL_EN_RW		= 18,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RS_BL_RW_EN		= 19,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RS_EN_BL_RW		= 20,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RS_EN_RW_BL		= 21,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RS_RW_BL_EN		= 22,
	LCD_TWI_PINOUT_D7_D6_D5_D4_RS_RW_EN_BL		= 23
} lcdTwiPinout_e;

// =============================================================================
// New data types - Enumerations (algebraic operated list elements)
// =============================================================================

// NONE

// =============================================================================
// New data types - Enumerations (bitwise operated flags)
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// LcdTwi - Class declaration
// =============================================================================

class LcdTwi
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading
	// -------------------------------------------------------------------------

	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------

public:
	LcdTwi(	Twi0 * twiHandler,
			lcdTwiPinout_e pinout = LCD_TWI_PINOUT_D7_D6_D5_D4_BL_EN_RW_RS,
			lcdDisplaySize_e displaySize = LCD_DISPLAY_SIZE_16X2,
			lcdFontSize_e fontSize = LCD_FONT_SIZE_5X8
		  );
	LcdTwi(	Twi0 * twiHandler,
			char * pinout,
			lcdDisplaySize_e displaySize = LCD_DISPLAY_SIZE_16X2,
			lcdFontSize_e fontSize = LCD_FONT_SIZE_5X8
		  );
	// -------------------------------------------------------------------------
	// Methods
	// -------------------------------------------------------------------------

public:
	bool initialize(void);

	bool setTwiHandler(Twi0 * twiHandler);
	bool setPinout(lcdTwiPinout_e pinout);
	bool setDisplaySize(lcdDisplaySize_e displaySize);
	bool setFontSize(lcdFontSize_e fontSize);
	bool setEntryMode(lcdEntryMode_e entryMode);



	bool clearScreen(void);
	bool setCursor(bool state);
	bool setDisplay(bool state);
	bool setCursorBlink(bool state);

//	bool setDisplayShift(horizontalDirection_t dir);
//	bool moveCursor(horizontalDirection_t dir);
	bool moveCursorXY(uint8_t line, uint8_t column);
	bool moveCursorHome(void);
	bool moveCursorFirstLine(void);
	bool moveCursorNextLine(void);
	bool remapStdio(void);
	error_e getLastError(void);

private:
	bool _deinitialize(void);
	void _resetDataMembers(void);
	bool _validatePinout(lcdTwiPinout_e pinout);						// To Do
	bool _validateLcdDisplaySize(lcdDisplaySize_e displaySize);
	bool _validateLcdFontSize(lcdFontSize_e fontSize);


	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

private:
	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool				_initialized					: 1;

	//     ///////////     PCF8594 HARDWARE CONFIGURATION     ///////////     //
	Twi0				* _twiHandler;
	uint8_t				_pinBl							: 3;
	uint8_t				_pinD4							: 3;
	uint8_t				_pinD5							: 3;
	uint8_t				_pinD6							: 3;
	uint8_t				_pinD7							: 3;
	uint8_t				_pinEn							: 3;
	uint8_t				_pinRs							: 3;
	uint8_t				_pinRw							: 3;

	//     /////////////////     LCD  CONFIGURATION     /////////////////     //
	uint8_t				_cursorColumn					: 6;
	uint8_t				_cursorLine						: 2;
	lcdCursorState_e	_cursorState					: 2;
	uint8_t				_displayColumns					: 6;
	uint8_t				_displayLines					: 2;
	bool				_displayState					: 1;
	lcdEntryMode_e		_entryMode						: 2;
	lcdFontSize_e		_fontSize						: 1;

	//     ///////////////////     ERROR MESSAGES     ///////////////////     //
	error_e				_lastError;
}; // class LcdTwi


// =============================================================================
// Extern global variables
// =============================================================================

extern FILE lcdTwiStream;

// =============================================================================
// General public functions declarations
// =============================================================================



// -----------------------------------------------------------------------------
// Functions for debug purposes ------------------------------------------------

uint8_t lcdGetLine(void);
uint8_t lcdGetColumn(void);

#endif
