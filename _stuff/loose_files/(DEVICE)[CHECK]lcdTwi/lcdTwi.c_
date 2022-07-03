/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			lcdTwi.c
 * Module:			Character LCD controller Using PCF8594 over TWI
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Build:			1
 * Last edition:	September 15, 2017
 * Purpose:			Character LCD controller over Two Wires Interface without
 *					support to busy flag (delay-driven)
 * ---------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "lcdTwi.h"
#if __LCD_TWI_H_ != 1
	#error Error 101 - Build mismatch on header and source code files (lcdTwi).
#endif

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

static void lcdTwiFunctionSet8Bits(lcdTwiConfiguration_t * lcd, uint8 command);
static void lcdTwiWriteCharacter(lcdTwiConfiguration_t * lcd, uint8 character);
static void lcdTwiWriteCommand(lcdTwiConfiguration_t * lcd, uint8 command);
static int16 lcdTwiWriteStd(int8 c, FILE * stream);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE lcdTwiStream = FDEV_SETUP_STREAM(lcdTwiWriteStd, NULL, _FDEV_SETUP_WRITE);
lcdTwiConfiguration_t * defaultDisplay = NULL;

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiClearScreen
 * Purpose:		Clears the entire screen and moves cursor to home position
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiClearScreen(lcdTwiConfiguration_t * lcd)
{
	lcdTwiWriteCommand(lcd, LCD_CLEAR_SCREEN);
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursor
 * Purpose:		Turns cursor ON/OFF, without changing display or blink option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursor(lcdTwiConfiguration_t * lcd, logic_t state)
{
	uint8 command = LCD_DISPLAY_OFF;

	lcd->cursorOn = (state & 0x01);
	if(lcd->displayOn) {
		command |= 4;
	}
	if(lcd->cursorOn) {
		command |= 2;
	}
	if(lcd->cursorBlink) {
		command |= 1;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorBlink
 * Purpose:		Turns cursor blink ON/OFF, without changing display or cursor
 *				option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorBlink(lcdTwiConfiguration_t * lcd, logic_t state)
{
	uint8 command = LCD_DISPLAY_OFF;

	lcd->cursorBlink = (state & 0x01);
	if(lcd->displayOn) {
		command |= 4;
	}
	if(lcd->cursorOn) {
		command |= 2;
	}
	if(lcd->cursorBlink) {
		command |= 1;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorGoTo
 * Purpose:		Moves the cursor to position
 * Note:		Origin is considered as position (0, 0)
 * Arguments:	lcd			Pointer to the LCD struct
 *				line		line
 *				column		column
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorGoTo(lcdTwiConfiguration_t * lcd, uint8 line, uint8 column)
{
	uint8 address = 0xFF;

	switch(line) {
	case 0:		// Go to line 0
		address = column;
		break;
	case 1:		// Go to line 1
		address = (lcd->lines >= 1) ? (0x40 + column) : 0xFF;
		break;
	case 2:		// Go to line 2
		if((lcd->lines == 3) && (lcd->columns == 11)) {				// Display 12x4
			address = 0x0C + column;
		} else if((lcd->lines == 3) && (lcd->columns == 15)) {		// Display 16x4
			address = 0x10 + column;
		} else if((lcd->lines == 3) && (lcd->columns == 19)) {		// Display 20x4
			address = 0x14 + column;
		}
		break;
	case 3:		// Go to line 3
		if((lcd->lines == 3) && (lcd->columns == 11)) {				// Display 12x4
			address = 0x4C + column;
		} else if((lcd->lines == 3) && (lcd->columns == 15)) {		// Display 16x4
			address = 0x50 + column;
		} else if((lcd->lines == 3) && (lcd->columns == 19)) {		// Display 20x4
			address = 0x54 + column;
		}
		break;
	}

	if(address != 0xFF) {
		lcd->cursorLine = line;
		lcd->cursorColumn = column;
		lcdTwiWriteCommand(lcd, (LCD_DDRAM_ADRESS | address));
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorHome
 * Purpose:		Moves cursor to home position
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorHome(lcdTwiConfiguration_t * lcd)
{
	lcdTwiWriteCommand(lcd, LCD_CURSOR_HOME);
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorMove
 * Purpose:		Moves the cursor one position to the LEFT or RIGHT
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			LEFT or RIGHT (use enumerations defined at
 *							direction_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorMove(lcdTwiConfiguration_t * lcd, direction_t dir)
{
	if(dir == LEFT) {
		lcd->cursorColumn--;
		lcdTwiWriteCommand(lcd, 0x10);
	} else {
		lcd->cursorColumn++;
		lcdTwiWriteCommand(lcd, 0x14);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorMoveFirstLine
 * Purpose:		Moves cursor to the beginning of the first line
 * Note:		The function just calls lcdTwiCursorHome
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorMoveFirstLine(lcdTwiConfiguration_t * lcd)
{
	lcdTwiCursorHome(lcd);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorMoveNextLine
 * Purpose:		Moves cursor to the beginning of the next line
 * Note:		The cursor will wrap automatically from the last line to the
 *				first line
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorMoveNextLine(lcdTwiConfiguration_t * lcd)
{
	uint8 currentLine = (lcd->cursorLine + 1);

	currentLine %= (lcd->lines + 1);
	lcdTwiCursorGoTo(lcd, currentLine, 0);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiDisplay
 * Purpose:		Turns display ON/OFF, without changing cursor or blink option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiDisplay(lcdTwiConfiguration_t * lcd, logic_t state)
{
	uint8 command = LCD_DISPLAY_OFF;

	lcd->displayOn = (state & 0x01);
	if(lcd->displayOn) {
		command |= 4;
	}
	if(lcd->cursorOn) {
		command |= 2;
	}
	if(lcd->cursorBlink) {
		command |= 1;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiDisplayShift
 * Purpose:		Moves the entire display one position to the LEFT or RIGHT
 * Note:		The cursor is also moved in the opposite direction. The use of
 *				this function may cause problems if used before printf() and
 *				lcdTwiCursorGoTo(). The effects of lcdTwiDisplayShift() are cancelled
 *				by the lcdTwiCursorHome() and lcdTwiCursorMoveFirstLine() functions.
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			direction to move (use enumerations defined at
 *							direction_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiDisplayShift(lcdTwiConfiguration_t * lcd, direction_t dir)
{
	if(dir == LEFT) {
		lcd->cursorColumn--;
		lcdTwiWriteCommand(lcd, 0x18);
	} else {
		lcd->cursorColumn++;
		lcdTwiWriteCommand(lcd, 0x1C);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiInit
 * Purpose:		Performs the LCD initialization routine
 * Arguments:	lcd			Pointer to the LCD struct
 *				size		Size of the display (use enumerations at lcdTwiSize_t)
 *				font		Font size (use enumerations at lcdTwiFont_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiInit(lcdTwiConfiguration_t * lcd, lcdTwiSize_t size, lcdTwiFont_t font)
{
	uint8 command = 0;

	// I/O initialization
	clrMask(*(lcd->dataPORT), 0x0F, lcd->dataFirst);
	setMask(*(lcd->dataDDR), 0x0F, lcd->dataFirst);
	clrBit(*(lcd->controlPORT), lcd->controlE);
	setBit(*(lcd->controlDDR), lcd->controlE);
	clrBit(*(lcd->controlPORT), lcd->controlRS);
	setBit(*(lcd->controlDDR), lcd->controlRS);

	// Reset the controller, must be sent 3 times
	_delay_ms(15);
	lcdTwiFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_ms(5);
	lcdTwiFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_us(60);
	lcdTwiFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_us(60);

	// Configures the LCD to 4 bits interface
	lcdTwiFunctionSet8Bits(lcd, LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS);

	// Function set
	command = LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS;
	command |= (font == LCD_FONT_5X8) ? LCD_FUNCTION_5x8_FONT : LCD_FUNCTION_5x10_FONT;
	command |= (size < 200) ? LCD_FUNCTION_1_LINE : LCD_FUNCTION_2_LINES;
	lcdTwiWriteCommand(lcd, command);
	lcdTwiWriteCommand(lcd, LCD_DISPLAY_OFF);
	lcdTwiWriteCommand(lcd, LCD_CLEAR_SCREEN);

	command = LCD_ENTRY_SET;
	if(lcd->entryIncDec == LCD_INCREMENT) {
		command |= LCD_ENTRY_INCREMENT;
	} else {
		command |= LCD_ENTRY_DECREMENT;
	}
	if(lcd->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
		command |= LCD_ENTRY_SHIFT;
	} else {
		command |= LCD_ENTRY_OVERWRITE;
	}
	lcdTwiWriteCommand(lcd, command);

	lcdTwiWriteCommand(lcd, LCD_DISPLAY_ON);

	// Updates the struct variables
	lcd->displayOn = TRUE;
	lcd->cursorOn = FALSE;
	lcd->cursorBlink = FALSE;
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;
	lcd->lines = (size / 100) - 1;
	lcd->columns = (size % 100) - 1;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiSetControlPort
 * Purpose:		Configures the LCD I/O ports for the control bus
 * Arguments:	lcd			Pointer to the LCD struct
 *				controlDDR	Pointer to I/O Data Direction Register
 *				controlPORT	Pointer to I/O Port Output Register
 *				controlE	Position of pin E at the control register
 *				controlRS	Position of pin RS at the control register
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiSetControlPort(lcdTwiConfiguration_t * lcd, vuint8 * controlDDR, vuint8 * controlPORT, uint8 controlE, uint8 controlRS)
{
	lcd->controlDDR = controlDDR;
	lcd->controlPORT = controlPORT;
	lcd->controlE = controlE;
	lcd->controlRS = controlRS;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiSetDataPort
 * Purpose:		Configures the LCD I/O ports for the data bus
 * Arguments:	lcd			Pointer to the LCD struct
 *				dataDDR		Pointer to I/O Data Direction Register
 *				dataPORT	Pointer to I/O Port Output Register
 *				dataFirst	Position of the first bit of the LCD data nibble
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiSetDataPort(lcdTwiConfiguration_t * lcd, vuint8 * dataDDR, vuint8 * dataPORT, uint8 dataFirst)
{
	lcd->dataDDR = dataDDR;
	lcd->dataPORT = dataPORT;
	lcd->dataFirst = dataFirst;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiSetEntryMode
 * Purpose:		Configures how the display will behave when a character is
 *				written
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			write direction (enumerations defined at
 *							lcdTwiIncrementDecrement_t)
 *				mode		insert/overwrite (enumerations defined at
 *							lcdTwiShiftOverwrite_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiSetEntryMode(lcdTwiConfiguration_t * lcd, lcdTwiIncrementDecrement_t dir, lcdTwiShiftOverwrite_t mode)
{
	uint8 command = LCD_ENTRY_SET;

	if(dir != LCD_INCDEC_NO_CHANGE) {
		lcd->entryIncDec = (dir & 0x01);
	}
	if(mode != LCD_DISPLAY_NO_CHANGE) {
		lcd->entryShiftDisplay = (mode & 0x01);
	}

	if(lcd->entryIncDec == LCD_INCREMENT) {
		command |= LCD_ENTRY_INCREMENT;
	} else {
		command |= LCD_ENTRY_DECREMENT;
	}
	if(lcd->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
		command |= LCD_ENTRY_SHIFT;
	} else {
		command |= LCD_ENTRY_OVERWRITE;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiStdio
 * Purpose:		Associate the LCD to the default output for printf function
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiStdio(lcdTwiConfiguration_t * lcd)
{
	stdin = stdout = stderr = &lcdStream;
	defaultDisplay = lcd;

	return;
}

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiFunctionSet8Bits
 * Purpose:		Sends a command to the LCD in 8 bits data interface
 * Arguments:	lcd			Pointer to the LCD struct
 *				command		Command to be sent to LCD
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdTwiFunctionSet8Bits(lcdTwiConfiguration_t * lcd, uint8 command)
{
	clrBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
	clrMask(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data
	*(lcd->dataPORT) |= ((command >> 4) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(40);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiWriteCharacter
 * Purpose:		Sends a character to be written at the LCD
 * Arguments:	lcd			Pointer to the LCD struct
 *				character	Character to be sent to LCD
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdTwiWriteCharacter(lcdTwiConfiguration_t * lcd, uint8 character)
{
	if(lcd->cursorColumn < 40) {
		setBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
		clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
		clrMask(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (higher nibble)
		*(lcd->dataPORT) |= ((character >> 4) << lcd->dataFirst);
		setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
		_delay_us(1);
		clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
		_delay_us(1);
		clrMask(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (lower nibble)
		*(lcd->dataPORT) |= ((character & 0x0F) << lcd->dataFirst);
		setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
		_delay_us(1);
		clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
		_delay_us(40);
		lcd->cursorColumn++;
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiWriteCommand
 * Purpose:		Sends a command to the LCD
 * Arguments:	lcd			Pointer to the LCD struct
 *				command		Command to be sent to LCD
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdTwiWriteCommand(lcdTwiConfiguration_t * lcd, uint8 command)
{
	clrBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
	clrMask(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (higher nibble)
	*(lcd->dataPORT) |= ((command >> 4) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(1);
	clrMask(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (lower nibble)
	*(lcd->dataPORT) |= ((command & 0x0F) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(1);
	if((command == LCD_CLEAR_SCREEN) || (command == LCD_CURSOR_HOME)) {
		_delay_ms(2);
	} else {
		_delay_us(40);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdWriteStd
 * Purpose:		This function is called by the printf() function to write a
 *				character at the display pointed by defaultDisplay.
 * Arguments:	character	Character to be sent to LCD
 *				stream		Pointer to a FILE stream
 * Returns:		0 (This function always returns 0)
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

static int16 lcdWriteStd(int8 character, FILE * stream)
{
	uint8 columns = defaultDisplay->columns + 1;
	uint8 i = 0;

	if(character == '\n') {
		for(i = defaultDisplay->cursorColumn; i < columns; i++) {
			lcdTwiWriteCharacter(defaultDisplay, ' ');
		}
		lcdTwiCursorMoveNextLine(defaultDisplay);
	} else {
		lcdTwiWriteCharacter(defaultDisplay, character);
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Debug function definitions - EXCLUDE BEFORE RELEASE -------------------------

uint8 lcdTwiGetColumn(lcdTwiConfiguration_t * lcd)
{
	return lcd->cursorColumn;
}

uint8 lcdTwiGetLine(lcdTwiConfiguration_t * lcd)
{
	return lcd->cursorLine;
}
