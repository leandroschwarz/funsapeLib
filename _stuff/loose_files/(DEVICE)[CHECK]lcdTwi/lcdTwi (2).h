/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			lcdTwi.h
 * Module:			Character I2C LCD controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 19, 2018
 * Purpose:			Character LCD controller using the I2C bus.
 * ---------------------------------------------------------------------------*/

#ifndef __LCD_TWI_H
#define __LCD_TWI_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <stdio.h>
#include "..\twiMaster.h"

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define PCF8574A_ADR		0x3F
#define LCD_DATAFIRST		4
#define LCD_E				2
#define LCD_RW				1	// sempre em baixo
#define LCD_RS				0
#define LCD_i2c_start		0x08

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum lcdTwiDataType_t {
	LCD_TWI_COMMAND		= 0,
	LCD_TWI_DDRAM_CHAR	= 1,
	LCD_TWI_CGRAM_CHAR	= 2
} lcdTwiDataType_t;

typedef struct lcdTwi_t {
	// I/O ports
	uint8_t	twiAddress				: 7;
	uint8_t	controlE				: 3;
	uint8_t	controlRs				: 3;
	uint8_t	controlRw				: 3;
	uint8_t	dataFirst				: 3;
	uint8_t	backlightBit			: 3;
	// Display hardware
	uint8_t	columns					: 6;
	uint8_t	lines					: 2;
	uint8_t	font					: 1;
	// Entry mode set
	uint8_t	entryIncDec				: 1;
	uint8_t	entryShiftDisplay		: 1;
	// Display and cursor configuration
	bool_t	cursorBlink				: 1;	// 0 off, 1 on
	uint8_t	cursorColumn			: 6;	// 0 to 39
	uint8_t	cursorLine				: 2;	// 0 to 3
	bool_t	cursorOn				: 1;	// 0 off, 1 on
	bool_t	displayOn				: 1;	// 0 off, 1 on
	bool_t	backlightOn				: 1;	// 0 off, 1 on
	// Unused bits
	uint8_t	unusedBits				: 6;
} lcdTwi_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

extern FILE lcdTwiStream;
extern lcdTwi_t * defaultDisplay;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define lcdTwiCreate() (lcdTwi_t){.twiAddress = 0, .controlE = 0, .controlRs = 0, .controlRw = 0, .dataFirst = 0, .backlightBit = 0, .columns = 0, .lines = 0, .font = LCD_FONT_5X8, .entryIncDec = LCD_INCREMENT, .entryShiftDisplay = LCD_DISPLAY_OVERWRITE, .cursorBlink = FALSE, .cursorColumn = 0, .cursorLine = 0, .cursorOn = FALSE, .displayOn = FALSE, .backlightOn = FALSE, .unusedBits = 0}

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

void lcdTwiBacklight(lcdTwi_t * lcd, bool_t turnOn);
void lcdTwiClearScreen(lcdTwi_t * lcd);
void lcdTwiCursor(lcdTwi_t * lcd, logic_t state);
void lcdTwiCursorBlink(lcdTwi_t * lcd, logic_t state);
void lcdTwiCursorGoTo(lcdTwi_t * lcd, uint8_t line, uint8_t column);
void lcdTwiCursorHome(lcdTwi_t * lcd);
void lcdTwiCursorMove(lcdTwi_t * lcd, direction_t dir);
void lcdTwiCursorMoveFirstLine(lcdTwi_t * lcd);
void lcdTwiCursorMoveNextLine(lcdTwi_t * lcd);
void lcdTwiCustomCharacterSet(lcdTwi_t * lcd, uint8_t charAddress, uint8_t * charData);
void lcdTwiDisplay(lcdTwi_t * lcd, logic_t state);
void lcdTwiDisplayShift(lcdTwi_t * lcd, direction_t dir);
void lcdTwiEntryModeSet(lcdTwi_t * lcd, lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode);
void lcdTwiInit(lcdTwi_t * lcd, lcdSize_t size, lcdFont_t font, uint8_t twiAddress);
void lcdTwiSetPort(lcdTwi_t * lcd, uint8_t controlE, uint8_t controlRs, uint8_t controlRw, uint8_t dataFirst, uint8_t backlightBit);
void lcdTwiStdio(lcdTwi_t * lcd);

// -----------------------------------------------------------------------------
// Functions for debug purposes ------------------------------------------------

#ifdef __DEBUG_MODE_ON
	#if __DEBUG_MODE_ON == 1

		uint8_t lcdTwiGetLine(lcdTwi_t * lcd);
		uint8_t lcdTwiGetColumn(lcdTwi_t * lcd);

	#endif
#endif

#endif
