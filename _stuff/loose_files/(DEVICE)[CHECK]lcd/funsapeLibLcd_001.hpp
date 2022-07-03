/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibLcd.hpp
 * Module:			Character LCD Module Interface for FunSAPE++ Embedded
 * 					Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_LCD_HPP
#define __FUNSAPE_LIB_LCD_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibLcd.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcd.hpp] Error 11 - Build mismatch between (funsapeLibLcd.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibLcd.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcd.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif

#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibLcd.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcd.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
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

#define delayMs(x)						HAL_Delay(x)
#define delayUs(x)						do{uint16_t_t i = x;							\
											while(i) {								\
												for(uint8_t_t j = 50; j != 0; j--) {	\
													asm volatile("nop");			\
												}									\
												i--;								\
											}										\
										} while(0)


// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

typedef enum lcdResult_t {
	LCD_OK = 0,
	LCD_UNSUPPORTED_FONT_SIZE,
	LCD_UNSUPPORTED_INTERFACE_MODE,
	LCD_UNSUPPORTED_DISPLAY_SIZE,
	LCD_UNSUPPORTED_PARAMETER,
	LCD_DATA_PORT_NOT_CONFIGURED,
	LCD_CONTROL_PORT_NOT_CONFIGURED,
	LCD_NOT_INITIALIZED
} lcdResult_t;

typedef enum direction_t {
	LEFT				= 0,
	RIGHT				= 1,
} direction_t;

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
	// New data types ----------------------------------------------------------

public:
	typedef enum displaySize_e {
		LCD_SIZE_UNDEFINED				= 0,
		LCD_8X1							= 108,
		LCD_16X1						= 116,
		LCD_20X1						= 120,
		LCD_24X1						= 124,
		LCD_40X1						= 140,
		LCD_8X2							= 208,
		LCD_10x2						= 210,
		LCD_12X2						= 212,
		LCD_16X2						= 216,
		LCD_20X2						= 220,
		LCD_24X2						= 224,
		LCD_30X2						= 230,
		LCD_32X2						= 232,
		LCD_40X2						= 240,
		LCD_12X4						= 412,
		LCD_16X4						= 416,
		LCD_20X4						= 420,
	} displaySize_e;

	typedef enum fontSize_b : bool {
		LCD_FONT_SIZE_5X8				= false,
		LCD_FONT_SIZE_5X10				= true,
	} fontSize_b;

	typedef enum entryMode_t : uint8_t {
		LCD_LEFT_OVERWRITE				= 0,
		LCD_RIGHT_SHIFT					= 1,
		LCD_RIGHT_OVERWRITE				= 2,
		LCD_LEFT_SHIFT					= 3
	} entryMode_t;

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
	Lcd(void);
	Lcd(Handler *hdlr_p);				// Pin config and interface mode
	~Lcd(void);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     ///////////////////     CONFIGURATION     ////////////////////     //
	bool init(displaySize_e size_p = LCD_16X2, fontSize_b font_p = LCD_FONT_SIZE_5X8);
	bool setEntryMode(entryMode_t entryMode_p = LCD_RIGHT_OVERWRITE);
	bool setHandler(Handler *hdlr_p);

	//     ///////////////////     TEXT COMMANDS     ////////////////////     //
	bool sendCharacter(uint8_t character_p);
	int16_t writeCharacterStd(char character_p, FILE *stream_p);

	//     /////////////////    INTERFACE COMMANDS     //////////////////     //
	bool clearScreen(void);
	bool turnCursorBlinkOn(bool state_p);
	bool turnCursorOn(bool state_p);
	bool turnDisplayOn(bool state_p);
	bool moveCursor(direction_t dir_p);
	bool moveCursor(uint8_t line_p, uint8_t column_p);
	bool moveCursorFirstLine(void);
	bool moveCursorHome(void);
	bool moveCursorNextLine(void);
	bool shiftDisplay(direction_t dir_p);

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	error_e getLastError(void);
	uint8_t getLine(void);
	uint8_t getColumn(void);

// lcdResult_t lcdStdio(volatile lcdConfiguration_t * lcd);
// lcdResult_t lcdCursorGoTo(volatile lcdConfiguration_t * lcd, uint8_t line, uint8_t column);
// lcdResult_t lcdCursorMoveNextLine(volatile lcdConfiguration_t * lcd);
// lcdResult_t lcdCursorMoveFirstLine(volatile lcdConfiguration_t * lcd);
// lcdResult_t lcdCursorMove(volatile lcdConfiguration_t * lcd, direction_t dir);
// lcdResult_t lcdDisplayShift(volatile lcdConfiguration_t * lcd, direction_t dir);
// lcdResult_t lcdWriteCommand(volatile lcdConfiguration_t * lcd, uint8_t command);
// lcdResult_t lcdWriteCharacter(volatile lcdConfiguration_t * lcd, uint8_t character);
// int16_t	lcdWriteStd(int8_t c, FILE * stream);

private:
	void _reset(void);

	bool _changeDisplayControl(void);
	bool _changeEntryMode(void);
	bool _sendCommand(uint8_t command_p);

	bool _holdGpio(void);
	bool _releaseGpio(void);
	bool _sendCommandForce8Bits(uint8_t command);
	bool _sendCharacter(uint8_t character);
	bool _waitUntilReady(void);
	bool _configCursorDisplayState(void);
	bool _configCursorDisplayShift(void);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
	Handler					*_handler;

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool					_cursorBlinkOn		: 1;
	uint8_t					_cursorColumn		: 6;	// 0 to 39
	uint8_t					_cursorLine			: 2;	// 0 to 3
	bool					_cursorOn			: 1;
	bool					_displayOn			: 1;
	bool					_initialized		: 1;
	error_e					_lastError;

	//     ///////////////////     CONFIGURATION     ////////////////////     //
	bool					_handlerOk			: 1;
	Handler::handlerType_e	_interfaceMode;
	uint8_t					_displayMaxColumn	: 6;	// 0 to 39
	uint8_t					_displayMaxLine		: 2;	// 0 to 3
	fontSize_b				_fontSize			: 1;	// 0 5x8, 1 5x10
	entryMode_t				_entryMode			: 2;

protected:
	// NONE

};	// class Lcd

// =============================================================================
// Lcd - Class overloading operators
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

extern Lcd *lcdDefaultHandler;

// =============================================================================
// Lcd - Class inline function definitions
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

#endif	// __FUNSAPE_LIB_LCD_HPP
