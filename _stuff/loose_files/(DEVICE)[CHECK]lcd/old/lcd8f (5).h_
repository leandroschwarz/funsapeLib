/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			lcd8f.h
  * Module:			Character LCD controller
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Build:			1
 * Last edition:	September 18, 2017
 * Purpose:			Character LCD controller with 8 bits interface and support
 *					to busy flag
 * ---------------------------------------------------------------------------*/

#ifndef __LCD8F_H
#define __LCD8F_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBALDEFINES_H != 1
	#error Error 100 - The defintion file is outdated (globalDefines must be build 1).
#endif
#include <stdio.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

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

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile struct lcdConfiguration_t {
	// I/O ports
	vuint8 *	dataDDR;
	vuint8 *	dataPORT;
	vuint8 *	dataPIN;
	vuint8 *	controlDDR;
	vuint8 *	controlPORT;
	uint8		controlE			: 3;
	uint8		controlRS			: 3;
	uint8		controlRW			: 3;
	// Display hardware
	uint8		columns				: 6;
	uint8		lines				: 2;
	// Entry mode set
	uint8		entryIncDec			: 1;
	uint8		entryShiftDisplay	: 1;
	// Display and cursor configuration
	bool_t		cursorBlink			: 1;	// 0 off, 1 on
	uint8		cursorColumn		: 6;	// 0 to 39
	uint8		cursorLine			: 2;	// 0 to 3
	bool_t		cursorOn			: 1;	// 0 off, 1 on
	bool_t		displayOn			: 1;	// 0 off, 1 on
	// Unused bits
	uint8		unusedBits			: 2;
} lcdConfiguration_t;

typedef enum lcdSize_t {
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
} lcdSize_t;

typedef enum lcdFont_t {
	LCD_FONT_5X8 = 0,
	LCD_FONT_5X10 = 1
} lcdFont_t;

typedef enum lcdShiftOverwrite_t {
	LCD_DISPLAY_OVERWRITE = 0,
	LCD_DISPLAY_SHIFT = 1,
	LCD_DISPLAY_NO_CHANGE = 0xFF
} lcdShiftOverwrite_t;

typedef enum lcdIncrementDecrement_t {
	LCD_DECREMENT = 0,
	LCD_INCREMENT = 1,
	LCD_INCDEC_NO_CHANGE = 0xFF
} lcdIncrementDecrement_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

extern FILE lcdStream;
extern lcdConfiguration_t * defaultDisplay;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define createLcd() (lcdConfiguration_t){.dataDDR = NULL, .dataPORT = NULL, .dataPIN = NULL, .controlDDR = NULL, .controlPORT = NULL, .controlE = 0, .controlRS = 0, .controlRW = 0, .columns = 0, .lines = 0, .entryIncDec = LCD_INCREMENT, .entryShiftDisplay = LCD_DISPLAY_OVERWRITE, .cursorBlink = FALSE, .cursorColumn = 0, .cursorLine = 0, .cursorOn = FALSE, .displayOn = FALSE, .unused = 0}

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

void lcdClearScreen(lcdConfiguration_t * lcd);
void lcdCursor(lcdConfiguration_t * lcd, logic_t state);
void lcdCursorBlink(lcdConfiguration_t * lcd, logic_t state);
void lcdCursorGoTo(lcdConfiguration_t * lcd, uint8 line, uint8 column);
void lcdCursorHome(lcdConfiguration_t * lcd);
void lcdCursorMove(lcdConfiguration_t * lcd, direction_t dir);
void lcdCursorMoveFirstLine(lcdConfiguration_t * lcd);
void lcdCursorMoveNextLine(lcdConfiguration_t * lcd);
void lcdDisplay(lcdConfiguration_t * lcd, logic_t state);
void lcdDisplayShift(lcdConfiguration_t * lcd, direction_t dir);
void lcdInit(lcdConfiguration_t * lcd, lcdSize_t size, lcdFont_t font);
void lcdSetControlPort(lcdConfiguration_t * lcd, vuint8 * controlDDR, vuint8 * controlPORT, uint8 controlE, uint8 controlRS, uint8 controlRW);
void lcdSetDataPort(lcdConfiguration_t * lcd, vuint8 * dataDDR, vuint8 * dataPORT, vuint8 * dataPIN);
void lcdSetEntryMode(lcdConfiguration_t * lcd, lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode);
void lcdStdio(lcdConfiguration_t * lcd);

// -----------------------------------------------------------------------------
// Functions for debug purposes ------------------------------------------------

uint8 lcdGetLine(lcdConfiguration_t * lcd);
uint8 lcdGetColumn(lcdConfiguration_t * lcd);

#endif
