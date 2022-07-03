/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			lcd4f.c
 * Module:			Character LCD controller
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Build:			1
 * Last edition:	September 18, 2017
 * Purpose:			Character LCD controller with 4 bits interface and support
 *					to busy flag
 * ---------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "lcd4f.h"
#if __LCD4F_H != 1
	#error Error 101 - Build mismatch on header and source code files (lcd4f).
#endif

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

static void lcdFunctionSet8Bits(lcdConfiguration_t * lcd, uint8 command);
static void lcdWaitUntilReady(lcdConfiguration_t * lcd);
static void lcdWriteCharacter(lcdConfiguration_t * lcd, uint8 character);
static void lcdWriteCommand(lcdConfiguration_t * lcd, uint8 command);
static int16 lcdWriteStd(int8 c, FILE * stream);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE lcdStream = FDEV_SETUP_STREAM(lcdWriteStd, NULL, _FDEV_SETUP_WRITE);
lcdConfiguration_t * defaultDisplay = NULL;

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	lcdClearScreen
 * Purpose:		Clears the entire screen and moves cursor to home position
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdClearScreen(lcdConfiguration_t * lcd)
{
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, LCD_CLEAR_SCREEN);
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursor
 * Purpose:		Turns cursor ON/OFF, without changing display or blink option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursor(lcdConfiguration_t * lcd, logic_t state)
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
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursorBlink
 * Purpose:		Turns cursor blink ON/OFF, without changing display or cursor
 *				option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursorBlink(lcdConfiguration_t * lcd, logic_t state)
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
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursorGoTo
 * Purpose:		Moves the cursor to position
 * Note:		Origin is considered as position (0, 0)
 * Arguments:	lcd			Pointer to the LCD struct
 *				line		line
 *				column		column
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursorGoTo(lcdConfiguration_t * lcd, uint8 line, uint8 column)
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
		lcdWaitUntilReady(lcd);
		lcdWriteCommand(lcd, (LCD_DDRAM_ADRESS | address));
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursorHome
 * Purpose:		Moves cursor to home position
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursorHome(lcdConfiguration_t * lcd)
{
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, LCD_CURSOR_HOME);
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursorMove
 * Purpose:		Moves the cursor one position to the LEFT or RIGHT
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			LEFT or RIGHT (use enumerations defined at
 *							direction_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursorMove(lcdConfiguration_t * lcd, direction_t dir)
{
	if(dir == LEFT) {
		lcd->cursorColumn--;
		lcdWaitUntilReady(lcd);
		lcdWriteCommand(lcd, 0x10);
	} else {
		lcd->cursorColumn++;
		lcdWaitUntilReady(lcd);
		lcdWriteCommand(lcd, 0x14);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursorMoveFirstLine
 * Purpose:		Moves cursor to the beginning of the first line
 * Note:		The function just calls lcdCursorHome
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursorMoveFirstLine(lcdConfiguration_t * lcd)
{
	lcdCursorHome(lcd);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdCursorMoveNextLine
 * Purpose:		Moves cursor to the beginning of the next line
 * Note:		The cursor will wrap automatically from the last line to the
 *				first line
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdCursorMoveNextLine(lcdConfiguration_t * lcd)
{
	uint8 currentLine = (lcd->cursorLine + 1);

	currentLine %= (lcd->lines + 1);
	lcdCursorGoTo(lcd, currentLine, 0);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdDisplay
 * Purpose:		Turns display ON/OFF, without changing cursor or blink option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdDisplay(lcdConfiguration_t * lcd, logic_t state)
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
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdDisplayShift
 * Purpose:		Moves the entire display one position to the LEFT or RIGHT
 * Note:		The cursor is also moved in the opposite direction. The use of
 *				this function may cause problems if used before printf() and
 *				lcdCursorGoTo(). The effects of lcdDisplayShift() are cancelled
 *				by the lcdCursorHome() and lcdCursorMoveFirstLine() functions.
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			direction to move (use enumerations defined at
 *							direction_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdDisplayShift(lcdConfiguration_t * lcd, direction_t dir)
{
	if(dir == LEFT) {
		lcd->cursorColumn--;
		lcdWaitUntilReady(lcd);
		lcdWriteCommand(lcd, 0x18);
	} else {
		lcd->cursorColumn++;
		lcdWaitUntilReady(lcd);
		lcdWriteCommand(lcd, 0x1C);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdInit
 * Purpose:		Performs the LCD initialization routine
 * Arguments:	lcd			Pointer to the LCD struct
 *				size		Size of the display (use enumerations at lcdSize_t)
 *				font		Font size (use enumerations at lcdFont_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdInit(lcdConfiguration_t * lcd, lcdSize_t size, lcdFont_t font)
{
	uint8 command = 0;

	// I/O initialization
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);
	setMaskOffset(*(lcd->dataDDR), 0x0F, lcd->dataFirst);
	clrBit(*(lcd->controlPORT), lcd->controlE);
	setBit(*(lcd->controlDDR), lcd->controlE);
	clrBit(*(lcd->controlPORT), lcd->controlRS);
	setBit(*(lcd->controlDDR), lcd->controlRS);
	clrBit(*(lcd->controlPORT), lcd->controlRW);
	setBit(*(lcd->controlDDR), lcd->controlRW);

	// Reset the controller, must be sent 3 times
	_delay_ms(15);
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_ms(5);
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_us(100);
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_us(100);

	// Configures the LCD to 4 bits interface
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS);
	_delay_us(100);

	// Function set
	command = LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS;
	command |= (font == LCD_FONT_5X8) ? LCD_FUNCTION_5x8_FONT : LCD_FUNCTION_5x10_FONT;
	command |= (size < 200) ? LCD_FUNCTION_1_LINE : LCD_FUNCTION_2_LINES;
	lcdWriteCommand(lcd, command);
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, LCD_DISPLAY_OFF);
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, LCD_CLEAR_SCREEN);
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
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, command);
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, LCD_DISPLAY_ON);

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
 * Function:	lcdSetControlPort
 * Purpose:		Configures the LCD I/O ports for the control bus
 * Arguments:	lcd			Pointer to the LCD struct
 *				controlDDR	Pointer to I/O Data Direction Register
 *				controlPORT	Pointer to I/O Port Output Register
 *				controlE	Position of pin E at the control register
 *				controlRS	Position of pin RS at the control register
 *				controlRW	Position of pin RW at the control register
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdSetControlPort(lcdConfiguration_t * lcd, vuint8 * controlDDR, vuint8 * controlPORT, uint8 controlE, uint8 controlRS, uint8 controlRW)
{
	lcd->controlDDR = controlDDR;
	lcd->controlPORT = controlPORT;
	lcd->controlE = controlE;
	lcd->controlRS = controlRS;
	lcd->controlRW = controlRW;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdSetDataPort
 * Purpose:		Configures the LCD I/O ports for the data bus
 * Arguments:	lcd			Pointer to the LCD struct
 *				dataDDR		Pointer to I/O Data Direction Register
 *				dataPORT	Pointer to I/O Port Output Register
 *				dataPIN		Pointer to I/O Port Input Register
 *				dataFirst	Position of the first bit of the LCD data nibble
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdSetDataPort(lcdConfiguration_t * lcd, vuint8 * dataDDR, vuint8 * dataPORT, vuint8 * dataPIN, uint8 dataFirst)
{
	lcd->dataDDR = dataDDR;
	lcd->dataPORT = dataPORT;
	lcd->dataPIN = dataPIN;
	lcd->dataFirst = dataFirst;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdSetEntryMode
 * Purpose:		Configures how the display will behave when a character is
 *				written
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			write direction (enumerations defined at
 *							lcdIncrementDecrement_t)
 *				mode		insert/overwrite (enumerations defined at
 *							lcdShiftOverwrite_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdSetEntryMode(lcdConfiguration_t * lcd, lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode)
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
	lcdWaitUntilReady(lcd);
	lcdWriteCommand(lcd, command);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdStdio
 * Purpose:		Associate the LCD to the default output for printf function
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdStdio(lcdConfiguration_t * lcd)
{
	stdin = stdout = stderr = &lcdStream;
	defaultDisplay = lcd;

	return;
}

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	lcdFunctionSet8Bits
 * Purpose:		Sends a command to the LCD in 8 bits data interface
 * Arguments:	lcd			Pointer to the LCD struct
 *				command		Command to be sent to LCD
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdFunctionSet8Bits(lcdConfiguration_t * lcd, uint8 command)
{
	clrBit(*(lcd->controlPORT), lcd->controlRW);	// LCD in write mode
	clrBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data
	*(lcd->dataPORT) |= ((command >> 4) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(1);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdWaitUntilReady
 * Purpose:		Checks busy flag until LCD is ready to receive commands.
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdWaitUntilReady(lcdConfiguration_t * lcd)
{
	uint8 busyFlag = 0;

	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);
	clrMaskOffset(*(lcd->dataDDR), 0x0F, lcd->dataFirst);
	clrBit(*(lcd->controlPORT), lcd->controlRS);	// LCD in command mode
	setBit(*(lcd->controlPORT), lcd->controlRW);	// LCD in write mode
	clrBit(*(lcd->controlPORT), lcd->controlE);		// Makes sure enable is LOW

	do {
		busyFlag = 0;
		setBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse start
		_delay_us(1);
		busyFlag = *(lcd->dataPIN);
		busyFlag &= (0x08 << lcd->dataFirst);
		clrBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse end
		_delay_us(1);									// Discarding lower nibble
		setBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse start
		_delay_us(1);
		clrBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse end
		_delay_us(1);
	} while(busyFlag);

	clrBit(*(lcd->controlPORT), lcd->controlRW);	// LCD in write mode
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);
	setMaskOffset(*(lcd->dataDDR), 0x0F, lcd->dataFirst);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdWriteCharacter
 * Purpose:		Sends a character to be written at the LCD
 * Arguments:	lcd			Pointer to the LCD struct
 *				character	Character to be sent to LCD
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdWriteCharacter(lcdConfiguration_t * lcd, uint8 character)
{
	if(lcd->cursorColumn < 40) {
		lcdWaitUntilReady(lcd);
		clrBit(*(lcd->controlPORT), lcd->controlRW);		// LCD in write mode
		setBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
		clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
		clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (higher nibble)
		*(lcd->dataPORT) |= ((character >> 4) << lcd->dataFirst);
		setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
		_delay_us(1);
		clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
		_delay_us(1);
		clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (lower nibble)
		*(lcd->dataPORT) |= ((character & 0x0F) << lcd->dataFirst);
		setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
		_delay_us(1);
		clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
		lcd->cursorColumn++;
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdWriteCommand
 * Purpose:		Sends a command to the LCD
 * Arguments:	lcd			Pointer to the LCD struct
 *				command		Command to be sent to LCD
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void lcdWriteCommand(lcdConfiguration_t * lcd, uint8 command)
{
	clrBit(*(lcd->controlPORT), lcd->controlRW);	// LCD in write mode
	clrBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (higher nibble)
	*(lcd->dataPORT) |= ((command >> 4) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(1);
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data (lower nibble)
	*(lcd->dataPORT) |= ((command & 0x0F) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(1);

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

int16 lcdWriteStd(int8 character, FILE * stream)
{
	uint8 columns = defaultDisplay->columns + 1;
	uint8 i = 0;

	if(character == '\n') {
		for(i = defaultDisplay->cursorColumn; i < columns; i++) {
			lcdWriteCharacter(defaultDisplay, ' ');
		}
		lcdCursorMoveNextLine(defaultDisplay);
	} else {
		lcdWriteCharacter(defaultDisplay, character);
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Debug function definitions - EXCLUDE BEFORE RELEASE -------------------------

uint8 lcdGetColumn(lcdConfiguration_t * lcd)
{
	return lcd->cursorColumn;
}

uint8 lcdGetLine(lcdConfiguration_t * lcd)
{
	return lcd->cursorLine;
}
