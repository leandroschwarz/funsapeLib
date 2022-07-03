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

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibLcd.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcd.hpp] Error 11 - Build mismatch between (funsapeLibLcd.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibLcd.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcd.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif

#include "util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibLcd.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcd.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

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

#define attachLcd(object) volatile lcdConfiguration_t object = {.dataDDR = NULL, .dataPORT = NULL, .controlDDR = NULL, .controlPORT = NULL, .entryIncDec = LCD_INCREMENT, .entryShiftDisplay = LCD_DISPLAY_OVERWRITE, .controlE = 0, .controlRS = 0, .displayOn = FALSE, .cursorOn = FALSE, .cursorColumn = 0, .cursorLine = 0, .size = LCD_SIZE_UNDEFINED, .cursorBlink = FALSE, .functionFontSize = LCD_FONT_5X8, .dataPortConfigured = FALSE, .controlPortConfigured = FALSE, .initialized = FALSE}
#define delayMs(x)						HAL_Delay(x)
#define delayUs(x)						do{uint16_t i = x;							\
											while(i) {								\
												for(uint8_t j = 50; j != 0; j--) {	\
													asm volatile("nop");			\
												}									\
												i--;								\
											}										\
										} while(0)


// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// typedef struct lcdConfiguration_t {
// 	// I/O ports

// } lcdConfiguration_t;


// typedef enum lcdFont_t {
// 	LCD_FONT_5X8 = 0,
// 	LCD_FONT_5X10 = 1
// } lcdFont_t;

// typedef enum lcdShiftOverwrite_t {
// 	LCD_DISPLAY_OVERWRITE = 0,
// 	LCD_DISPLAY_SHIFT = 1,
// 	LCD_DISPLAY_NO_CHANGE = 0xFF
// } lcdShiftOverwrite_t;

// typedef enum lcdIncrementDecrement_t {
// 	LCD_DECREMENT = 0,
// 	LCD_INCREMENT = 1,
// 	LCD_INCDEC_NO_CHANGE = 0xFF
// } lcdIncrementDecrement_t;

// typedef enum lcdResult_t {
// 	LCD_OK = 0,
// 	LCD_UNSUPPORTED_FONT_SIZE,
// 	LCD_UNSUPPORTED_INTERFACE_MODE,
// 	LCD_UNSUPPORTED_DISPLAY_SIZE,
// 	LCD_UNSUPPORTED_PARAMETER,
// 	LCD_DATA_PORT_NOT_CONFIGURED,
// 	LCD_CONTROL_PORT_NOT_CONFIGURED,
// 	LCD_NOT_INITIALIZED
// } lcdResult_t;

// typedef enum logic_t {
// 	STATE_ON			= 1,
// 	STATE_OFF			= 0
// } logic_t;

// typedef enum direction_t {
// 	LEFT = 0,
// 	RIGHT = 1
// } direction_t;

// =============================================================================
// Extern global variables
// =============================================================================

// extern FILE lcdStream;
// extern volatile lcdConfiguration_t *defaultDisplay;

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
		LCD_16X1b						= 116,
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
		LCD_16X1						= 999,
	} displaySize_e;

	typedef enum fontSize_b : bool {
		FONT_SIZE_5X8					= false,
		FONT_SIZE_5X10					= true,
	} fontSize_b;

	typedef enum entryMode_t {
		ENTRY_MODE_OVERWRITE_DECREMENT	= 0,
		ENTRY_MODE_SHIFT_DECREMENT		= 1,
		ENTRY_MODE_OVERWRITE_INCREMENT	= 2,
		ENTRY_MODE_SHIFT_INCREMENT		= 3,
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
	bool setHandler(Handler *hdlr_p);
	bool setEntryMode(entryMode_t entryMode_p);
	bool turnDisplayOn(bool state_p);
	bool turnCursorOn(bool state_p);
	bool turnCursorBlinkOn(bool state_p);
	bool clearScreen(void);
	bool cursorHome(void);
	bool init(displaySize_e size_p = LCD_16X2, fontSize_b font_p = FONT_SIZE_5X8);

// lcdResult_t lcdStdio(volatile lcdConfiguration_t *lcd);
// lcdResult_t lcdCursorGoTo(volatile lcdConfiguration_t *lcd, uint8_t line, uint8_t column);
// lcdResult_t lcdCursorMoveNextLine(volatile lcdConfiguration_t *lcd);
// lcdResult_t lcdCursorMoveFirstLine(volatile lcdConfiguration_t *lcd);
// lcdResult_t lcdCursorMove(volatile lcdConfiguration_t *lcd, direction_t dir);
// lcdResult_t lcdDisplayShift(volatile lcdConfiguration_t *lcd, direction_t dir);

private:
	bool _changeDisplayControl(void);
	bool _changeEntryMode(void);
	bool _sendCommand(uint8_t command);
// lcdResult_t lcdWriteCommand(volatile lcdConfiguration_t *lcd, uint8_t command);
// lcdResult_t lcdWriteCharacter(volatile lcdConfiguration_t *lcd, uint8_t character);
// int16_t	lcdWriteStd(int8_t c, FILE *stream);

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
	bool					_cursorBlink		: 1;
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

// NONE

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
