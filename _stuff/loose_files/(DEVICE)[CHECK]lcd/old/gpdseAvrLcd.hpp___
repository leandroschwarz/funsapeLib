/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseAvrLcd.hpp
 * Module:		LCD Interface for GPDSE++ AVR Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_AVR_LCD_HPP
#define __GPDSE_AVR_LCD_HPP				__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseAvrGlobalDefines.hpp>
#if __GPDSE_AVR_GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (gpdseAvrGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
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
	LCD_DISPLAY_SIZE_UNDEFINED			= 0,
	LCD_DISPLAY_SIZE_8X1				= 108,
	LCD_DISPLAY_SIZE_8X2				= 208,
	LCD_DISPLAY_SIZE_10x2				= 210,
	LCD_DISPLAY_SIZE_12X2				= 212,
	LCD_DISPLAY_SIZE_12X4				= 412,
	LCD_DISPLAY_SIZE_16X1				= 116,
	LCD_DISPLAY_SIZE_16X2				= 216,
	LCD_DISPLAY_SIZE_16X4				= 416,
	LCD_DISPLAY_SIZE_20X1				= 120,
	LCD_DISPLAY_SIZE_20X2				= 220,
	LCD_DISPLAY_SIZE_20X4				= 420,
	LCD_DISPLAY_SIZE_24X1				= 124,
	LCD_DISPLAY_SIZE_24X2				= 224,
	LCD_DISPLAY_SIZE_30X2				= 230,
	LCD_DISPLAY_SIZE_32X2				= 232,
	LCD_DISPLAY_SIZE_40X1				= 140,
	LCD_DISPLAY_SIZE_40X2				= 240
} lcdDisplaySize_e;

typedef enum lcdInterfaceMode_e {
	LCD_INTERFACE_UNDEFINED				= 0,
	LCD_INTERFACE_4_BITS_DELAYED		= 1,
	LCD_INTERFACE_4_BITS_FLAGGED		= 2,
	LCD_INTERFACE_8_BITS_DELAYED		= 3,
	LCD_INTERFACE_8_BITS_FLAGGED		= 4
} lcdInterfaceMode_e;

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

extern FILE lcdStream;

// =============================================================================
// Lcd - Class declaration
// =============================================================================

class Lcd
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
	Lcd(	void
	   );
	Lcd(	lcdInterfaceMode_e interfaceMode
	   );

	// -------------------------------------------------------------------------
	// Methods
	// -------------------------------------------------------------------------

public:
	//     ///////////////////////     STATUS     ///////////////////////     //
	bool deinitialize(void);
	bool initialize(void);

	//     ///////////////     HARDWARE CONFIGURATION     ///////////////     //
	bool setControlPort(	vuint8_t * controlReg,
							uint8_t pinEn,
							uint8_t pinRs,
							uint8_t pinRw = 0xFF
					   );
	bool setDataPort(	vuint8_t * dataReg,
						uint8_t firstBit = 0xFF
					);
	bool setDisplaySize(lcdDisplaySize_e displaySize);
	bool setEntryMode(lcdEntryMode_e entryMode);
	bool setFontSize(lcdFontSize_e fontSize);
	bool setBacklightState(stateOffOn_t offOn);

	//     /////////////////     DISPLAY AND CURSOR     /////////////////     //
	bool clearScreen(void);
	bool moveCursor(direction_t dir);
	bool moveCursor(uint8_t line, uint8_t column);
	inlined bool moveCursorFirstLine(void);
	bool moveCursorHome(void);
	bool moveCursorNextLine(void);
	bool setCursorState(lcdCursorState_e cursorState);
	bool setDisplayState(stateOffOn_t offOn);
	bool shiftDisplay(direction_t dir);

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	error_e getLastError(void);

	void remapStdio(void);
	int16_t writeCharacterStd(char character, FILE * stream);


	bool sendCharacter(uint8_t character);

private:
	//     ////////////////     DATA MEMBER UPDATING     ////////////////     //
	void _resetDataMembers(void);
	bool _holdGpio(void);
	bool _releaseGpio(void);
	bool _sendCommandForce8Bits(uint8_t command);
	bool _sendCommand(uint8_t command);
	bool _sendCharacter(uint8_t character);
	bool _waitUntilReady(void);
	bool _configCursorDisplayState(void);
	bool _configCursorDisplayShift(void);

	//     //////////////////     DATA  VALIDATION     //////////////////     //

	//     ////////////////////     CHECK STATUS     ////////////////////     //


	uint8_t lcdGetLine(void);
	uint8_t lcdGetColumn(void);



	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	bool				_initialized					: 1;
	bool				_controlPortSet					: 1;
	bool				_dataPortSet					: 1;
	bool				_holdingGpio					: 1;
	bool				_busyFlagAvailable				: 1;

	//     //////////////     INTERFACE  CONFIGURATION     //////////////     //
	vuint8_t *			_controlDdr;
	vuint8_t *			_controlPort;
	uint8_t				_controlEn						: 3;
	uint8_t				_controlRs						: 3;
	uint8_t				_controlRw						: 3;
	vuint8_t *			_dataDdr;
	uint8_t				_dataFirst						: 3;
	vuint8_t *			_dataPort;
	vuint8_t *			_dataPin;
	lcdInterfaceMode_e	_interfaceMode					: 3;

	//     ///////////////     HARDWARE CONFIGURATION     ///////////////     //
	stateOffOn_t		_backlightState					: 1;
	uint8_t				_cursorColumn					: 6;
	uint8_t				_cursorLine						: 2;
	lcdCursorState_e	_cursorState					: 2;
	uint8_t				_displayColumns					: 6;
	uint8_t				_displayLines					: 2;
	lcdDisplaySize_e	_displaySize;
	stateOffOn_t		_displayState					: 1;
	lcdEntryMode_e		_entryMode						: 2;
	lcdFontSize_e		_fontSize						: 1;

	//     ///////////////////     ERROR MESSAGES     ///////////////////     //
	error_e				_lastError;

}; // class Lcd

// =============================================================================
// Lcd - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Lcd - Class inline function definitions
// =============================================================================

inlined bool Lcd::moveCursorFirstLine(void)
{
	return this->moveCursorHome();
}

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

extern Lcd *lcdDefaultHandler;

#endif // __GPDSE_AVR_LCD_HPP
