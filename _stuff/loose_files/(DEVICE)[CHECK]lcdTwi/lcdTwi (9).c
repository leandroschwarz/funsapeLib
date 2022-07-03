/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			lcdTwi.c
 * Module:			Character I2C LCD controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 19, 2018
 * Purpose:			Character LCD controller using the I2C bus.
 * ---------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "lcdTwi.h"
#if __LCD_TWI_H != 1
	#error Error 101 - Build mismatch on header and source code files (lcdTwi).
#endif

// -----------------------------------------------------------------------------
// New data types (used only in this file) -------------------------------------

typedef enum lcdTwiInstructions_t {
	LCD_CLEAR_DISPLAY			= 0x01,
	LCD_RETURN_HOME				= 0x02,
	LCD_ENTRY_MODE_SET			= 0x04,
	LCD_DISPLAY_CONTROL			= 0x08,
	LCD_MOVE_CURSOR_OR_DISPLAY	= 0x10,
	LCD_FUNCTION_SET			= 0x20,
	LCD_SET_CGRAM_ADDRESS		= 0x40,
	LCD_SET_DDRAM_ADDRESS		= 0x80
} lcdTwiInstructions_t;

typedef enum lcdTwiEntryModeSetOptions_t {
	LCD_ENTRY_MODE_SET_INCREMENT	= 0x02,
	LCD_ENTRY_MODE_SET_DECREMENT	= 0x00,
	LCD_ENTRY_MODE_SET_SHIFT		= 0x01,
	LCD_ENTRY_MODE_SET_OVERWRITE	= 0x00
} lcdTwiEntryModeSetOptions_t;

typedef enum lcdTwiDisplayControlOptions_t {
	LCD_DISPLAY_CONTROL_DISPLAY_ON			= 0x04,
	LCD_DISPLAY_CONTROL_DISPLAY_OFF			= 0x00,
	LCD_DISPLAY_CONTROL_CURSOR_ON			= 0x02,
	LCD_DISPLAY_CONTROL_CURSOR_OFF			= 0x00,
	LCD_DISPLAY_CONTROL_CURSOR_BLINK_ON		= 0x01,
	LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF	= 0x00
} lcdTwiDisplayControlOptions_t;

typedef enum lcdTwiMoveCursorOrDisplayOptions_t {
	LCD_MOVE_CURSOR_OR_DISPLAY_SHIFT_DISPLAY	= 0x80,
	LCD_MOVE_CURSOR_OR_DISPLAY_MOVE_CURSOR		= 0x00,
	LCD_MOVE_CURSOR_OR_DISPLAY_RIGHT			= 0x40,
	LCD_MOVE_CURSOR_OR_DISPLAY_LEFT				= 0x00
} lcdTwiMoveCursorOrDisplayOptions_t;

typedef enum lcdTwiFunctionSetOptions_t {
	LCD_FUNCTION_SET_8_BITS_INTERFACE	= 0x10,
	LCD_FUNCTION_SET_4_BITS_INTERFACE	= 0x00,
	LCD_FUNCTION_SET_2_LINES			= 0x08,
	LCD_FUNCTION_SET_1_LINE				= 0x00,
	LCD_FUNCTION_SET_5x10_FONT			= 0x04,
	LCD_FUNCTION_SET_5x8_FONT			= 0x00
} lcdTwiFunctionSetOptions_t;

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

static void lcdTwiFunctionSet8Bits(lcdTwi_s * lcd, uint8_t command);
static void lcdTwiSendData(lcdTwi_s * lcd, uint8_t data, lcdTwiDataType_e type);
static int lcdTwiWriteStd(char c, FILE * stream);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE lcdTwiStream = FDEV_SETUP_STREAM(lcdTwiWriteStd, NULL, _FDEV_SETUP_WRITE);
lcdTwi_s * defaultDisplay = NULL;

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiBacklight
 * Purpose:
 * Arguments:
 * Returns:
 * -------------------------------------------------------------------------- */

void lcdTwiBacklight(lcdTwi_s * lcd, bool_t turnOn)
{
	uint8_t message = 0;

	if(turnOn) {
		lcd->backlightOn = TRUE;
	} else {
		lcd->backlightOn = FALSE;
	}

	// Backlight configuration
	setMask(message, (lcd->backlightOn), lcd->backlightBit);

	// Send data
	twiMasterSendData(lcd->twiAddress, TWI_WRITE, &message, 1);

	return;
}


/* -----------------------------------------------------------------------------
 * Function:	lcdTwiClearScreen
 * Purpose:		Clears the entire screen and moves cursor to home position
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiClearScreen(lcdTwi_s * lcd)
{
	lcdTwiSendData(lcd, LCD_CLEAR_DISPLAY, LCD_TWI_COMMAND);
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

void lcdTwiCursor(lcdTwi_s * lcd, logicLevel_t state)
{
	uint8_t command;

	lcd->cursorOn = state;
	command = LCD_DISPLAY_CONTROL;
	command |= (lcd->displayOn) ? LCD_DISPLAY_CONTROL_DISPLAY_ON : LCD_DISPLAY_CONTROL_DISPLAY_OFF;
	command |= (lcd->cursorOn) ? LCD_DISPLAY_CONTROL_CURSOR_ON : LCD_DISPLAY_CONTROL_CURSOR_OFF;
	command |= (lcd->cursorBlink) ? LCD_DISPLAY_CONTROL_CURSOR_BLINK_ON : LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF;
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

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

void lcdTwiCursorBlink(lcdTwi_s * lcd, logicLevel_t state)
{
	uint8_t command;

	lcd->cursorBlink = state;
	command = LCD_DISPLAY_CONTROL;
	command |= (lcd->displayOn) ? LCD_DISPLAY_CONTROL_DISPLAY_ON : LCD_DISPLAY_CONTROL_DISPLAY_OFF;
	command |= (lcd->cursorOn) ? LCD_DISPLAY_CONTROL_CURSOR_ON : LCD_DISPLAY_CONTROL_CURSOR_OFF;
	command |= (lcd->cursorBlink) ? LCD_DISPLAY_CONTROL_CURSOR_BLINK_ON : LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF;
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorGoTo
 * Purpose:		Moves the cursor to position
 * Note:		Origin is considered as position (1, 1)
 * Arguments:	lcd			Pointer to the LCD struct
 *				line		line
 *				column		column
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorGoTo(lcdTwi_s * lcd, uint8_t line, uint8_t column)
{
	uint8_t address = 0xFF;

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
		lcdTwiSendData(lcd, (LCD_SET_DDRAM_ADDRESS | address), LCD_TWI_COMMAND);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorHome
 * Purpose:		Moves cursor to home position
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorHome(lcdTwi_s * lcd)
{
	lcdTwiSendData(lcd, LCD_RETURN_HOME, LCD_TWI_COMMAND);
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorMove
 * Purpose:		Moves the cursor one position to the LEFT or RIGHT
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			LEFT or RIGHT (use enumerations defined at
 *							planarDirection_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorMove(lcdTwi_s * lcd, planarDirection_t dir)
{
	uint8_t command = LCD_MOVE_CURSOR_OR_DISPLAY | LCD_MOVE_CURSOR_OR_DISPLAY_MOVE_CURSOR;

	command |= (dir == LEFT) ? LCD_MOVE_CURSOR_OR_DISPLAY_LEFT : LCD_MOVE_CURSOR_OR_DISPLAY_RIGHT;
	if(dir == LEFT) {
		lcd->cursorColumn--;
	} else {
		lcd->cursorColumn++;
	}
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCursorMoveFirstLine
 * Purpose:		Moves cursor to the beginning of the first line
 * Note:		The function just calls lcdTwiCursorHome
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiCursorMoveFirstLine(lcdTwi_s * lcd)
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

void lcdTwiCursorMoveNextLine(lcdTwi_s * lcd)
{
	uint8_t currentLine = (lcd->cursorLine + 1);

	currentLine %= (lcd->lines + 1);
	lcdTwiCursorGoTo(lcd, currentLine, 0);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiCustomCharacterSet
 * Purpose:		Stores an entire character in CGRAM
 * Arguments:	lcd				Pointer to the LCD struct
 *				charAddress		Character address
 *				charData		Character data
 * Returns:		-
 * Note:		The charData argument must be a vector of 8 bytes for a 5x8 font
 *				display and 10 bytes for a 5x10 font display
 * -------------------------------------------------------------------------- */

void lcdTwiCustomCharacterSet(lcdTwi_s * lcd, uint8_t charAddress, uint8_t * charData)
{
	uint8_t i = 0;

	if(lcd->font == LCD_FONT_5X8) {
		if(charAddress > 7) {
			return;
		}
		lcdTwiSendData(lcd, (LCD_SET_CGRAM_ADDRESS | (charAddress * 8)), LCD_TWI_COMMAND);
		for(i = 0; i < 8; i++) {
			lcdTwiSendData(lcd, charData[i], LCD_TWI_CGRAM_CHAR);
		}
	} else {
		if(charAddress > 3) {
			return;
		}
		lcdTwiSendData(lcd, (LCD_SET_CGRAM_ADDRESS | (charAddress * 10)), LCD_TWI_COMMAND);
		for(i = 0; i < 10; i++) {
			lcdTwiSendData(lcd, charData[i], LCD_TWI_CGRAM_CHAR);
		}
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiDisplay
 * Purpose:		Turns display ON/OFF, without changing cursor or blink option
 * Arguments:	lcd			Pointer to the LCD struct
 *				state		ON or OFF (enumerations defined at logic_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiDisplay(lcdTwi_s * lcd, logicLevel_t state)
{
	uint8_t command;

	lcd->displayOn = state;

	command = LCD_DISPLAY_CONTROL;
	command |= (lcd->displayOn) ? LCD_DISPLAY_CONTROL_DISPLAY_ON : LCD_DISPLAY_CONTROL_DISPLAY_OFF;
	command |= (lcd->cursorOn) ? LCD_DISPLAY_CONTROL_CURSOR_ON : LCD_DISPLAY_CONTROL_CURSOR_OFF;
	command |= (lcd->cursorBlink) ? LCD_DISPLAY_CONTROL_CURSOR_BLINK_ON : LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF;
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

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

void lcdTwiDisplayShift(lcdTwi_s * lcd, planarDirection_t dir)
{
	uint8_t command = LCD_MOVE_CURSOR_OR_DISPLAY | LCD_MOVE_CURSOR_OR_DISPLAY_SHIFT_DISPLAY;

	command |= (dir == LEFT) ? LCD_MOVE_CURSOR_OR_DISPLAY_LEFT : LCD_MOVE_CURSOR_OR_DISPLAY_RIGHT;
	if(dir == LEFT) {
		lcd->cursorColumn--;
	} else {
		lcd->cursorColumn++;
	}
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiEntryModeSet
 * Purpose:		Configures how the display will behave when a character is
 *				written
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			write direction (enumerations defined at
 *							lcdIncrementDecrement_t)
 *				mode		insert/overwrite (enumerations defined at
 *							lcdShiftOverwrite_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiEntryModeSet(lcdTwi_s * lcd, lcdDisplayDirection_e dir, lcdDisplayMode_e mode)
{
	uint8_t command = LCD_ENTRY_MODE_SET;

	if(dir != LCD_DISPLAY_DIRECTION_NO_CHANGE) {
		lcd->entryIncDec = (dir & 0x01);
	}
	if(mode != LCD_DISPLAY_MODE_NO_CHANGE) {
		lcd->entryShiftDisplay = (mode & 0x01);
	}

	command |= (lcd->entryIncDec == LCD_DISPLAY_DIRECTION_INCREMENT) ? LCD_ENTRY_MODE_SET_INCREMENT : LCD_ENTRY_MODE_SET_DECREMENT;
	command |= (lcd->entryShiftDisplay == LCD_DISPLAY_MODE_SHIFT) ? LCD_ENTRY_MODE_SET_SHIFT : LCD_ENTRY_MODE_SET_OVERWRITE;
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiInit
 * Purpose:		Performs the LCD initialization routine
 * Arguments:	lcd			Pointer to the LCD struct
 *				size		Size of the display (use enumerations at lcdSize_t)
 *				font		Font size (use enumerations at lcdFont_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiInit(lcdTwi_s * lcd, lcdSize_e size, lcdFont_e font, uint8_t twiAddress)
{
	uint8_t command  = 0;

	// Update some struct variables
	lcd->twiAddress = twiAddress;

	// Reset the controller, must be sent 3 times
	_delay_ms(15);
	lcdTwiFunctionSet8Bits(lcd, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_8_BITS_INTERFACE));
	_delay_ms(5);
	lcdTwiFunctionSet8Bits(lcd, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_8_BITS_INTERFACE));
	_delay_us(100);
	lcdTwiFunctionSet8Bits(lcd, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_8_BITS_INTERFACE));
	_delay_us(100);

	// Configures the LCD to 4-bits interface
	lcdTwiFunctionSet8Bits(lcd, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_4_BITS_INTERFACE));
	_delay_us(100);

	// Function set
	command = LCD_FUNCTION_SET | LCD_FUNCTION_SET_4_BITS_INTERFACE;
	command |= (font == LCD_FONT_5X8) ? LCD_FUNCTION_SET_5x8_FONT : LCD_FUNCTION_SET_5x10_FONT;
	lcd->font = font;
	command |= (size < 200) ? LCD_FUNCTION_SET_1_LINE : LCD_FUNCTION_SET_2_LINES;
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);
	lcdTwiSendData(lcd, (LCD_DISPLAY_CONTROL | LCD_DISPLAY_CONTROL_DISPLAY_OFF), LCD_TWI_COMMAND);
	lcdTwiSendData(lcd, LCD_CLEAR_DISPLAY, LCD_TWI_COMMAND);

	command = LCD_ENTRY_MODE_SET;
	if(lcd->entryIncDec == LCD_DISPLAY_DIRECTION_INCREMENT) {
		command |= LCD_ENTRY_MODE_SET_INCREMENT;
	} else {
		command |= LCD_ENTRY_MODE_SET_DECREMENT;
	}
	if(lcd->entryShiftDisplay == LCD_DISPLAY_MODE_SHIFT) {
		command |= LCD_ENTRY_MODE_SET_SHIFT;
	} else {
		command |= LCD_ENTRY_MODE_SET_OVERWRITE;
	}
	lcdTwiSendData(lcd, command, LCD_TWI_COMMAND);

	lcdTwiSendData(lcd, (LCD_DISPLAY_CONTROL | LCD_DISPLAY_CONTROL_DISPLAY_ON), LCD_TWI_COMMAND);

	// Updates the rest of the struct variables
	lcd->displayOn = TRUE;
	lcd->cursorOn = FALSE;
	lcd->cursorBlink = FALSE;
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;
	lcd->font = LCD_FONT_5X8;
	lcd->lines = (size / 100) - 1;
	lcd->columns = (size % 100) - 1;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiSetPort
 * Purpose:
 * Arguments:
 * Returns:
 * -------------------------------------------------------------------------- */

void lcdTwiSetPort(lcdTwi_s * lcd, uint8_t controlE, uint8_t controlRs, uint8_t controlRw, uint8_t dataFirst, uint8_t backlightBit)
{
	lcd->controlE		= controlE;
	lcd->controlRs		= controlRs;
	lcd->controlRw		= controlRw;
	lcd->dataFirst		= dataFirst;
	lcd->backlightBit	= backlightBit;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiStdio
 * Purpose:		Associate the LCD to the default output for printf function
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdTwiStdio(lcdTwi_s * lcd)
{
	stdin = stdout = stderr = &lcdTwiStream;
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

void lcdTwiFunctionSet8Bits(lcdTwi_s * lcd, uint8_t command)
{
	uint8_t message[2] = {0, 0};

	// Backlight configuration
	setMask(message[0], (lcd->backlightOn), lcd->backlightBit);
	setMask(message[1], (lcd->backlightOn), lcd->backlightBit);

	// Higher nibble
	setMask(message[0], (command >> 4), lcd->dataFirst);	// Writes data
	setBit(message[0], lcd->controlE);						// Enable pulse
	setMask(message[1], (command >> 4), lcd->dataFirst);	// Writes data

	// Send data
	twiMasterSendData(lcd->twiAddress, TWI_WRITE, message, 2);

	// Instruction execution time
	_delay_us(40);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiSendData
 * Purpose:
 * Arguments:
 *
 * Returns:
 * Note:
 * -------------------------------------------------------------------------- */

void lcdTwiSendData(lcdTwi_s * lcd, uint8_t data, lcdTwiDataType_e type)
{
	uint8_t message[4] = {0, 0, 0, 0};

	// Backlight configuration
	setMask(message[0], (lcd->backlightOn), lcd->backlightBit);
	setMask(message[1], (lcd->backlightOn), lcd->backlightBit);
	setMask(message[2], (lcd->backlightOn), lcd->backlightBit);
	setMask(message[3], (lcd->backlightOn), lcd->backlightBit);

	// Higher nibble
	{
		setMask(message[0], (data >> 4), lcd->dataFirst);    // Writes data
	}
	setBit(message[0], lcd->controlE);					// Enable pulse
	setMask(message[1], (data >> 4), lcd->dataFirst);	// Writes data
	// Lower nibble
	setMask(message[2], (data & 0x0F), lcd->dataFirst);	// Writes data
	setBit(message[2], lcd->controlE);					// Enable pulse
	setMask(message[3], (data & 0x0F), lcd->dataFirst);	// Writes data

	if(type != LCD_TWI_COMMAND) {
		setBit(message[0], lcd->controlRs);				// LCD in data mode
		setBit(message[1], lcd->controlRs);				// LCD in data mode
		setBit(message[2], lcd->controlRs);				// LCD in data mode
		setBit(message[3], lcd->controlRs);				// LCD in data mode
	}

	// Send data
	twiMasterSendData(lcd->twiAddress, TWI_WRITE, message, 4);

	// Instruction execution time
	switch(type) {
	case LCD_TWI_COMMAND:
		if((data == LCD_CLEAR_DISPLAY) || (data == LCD_RETURN_HOME)) {
			_delay_ms(2);
		} else {
			_delay_us(40);
		}
		break;
	default:
		_delay_us(40);
		break;
	}
	if(type == LCD_TWI_DDRAM_CHAR) {
		lcd->cursorColumn++;
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdTwiWriteStd
 * Purpose:		This function is called by the printf() function to write a
 *				character at the display pointed by defaultDisplay.
 * Arguments:	character	Character to be sent to LCD
 *				stream		Pointer to a FILE stream
 * Returns:		0 (This function always returns 0)
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

int lcdTwiWriteStd(char character, FILE * stream)
{
	uint8_t columns = defaultDisplay->columns + 1;
	uint8_t i = 0;

	if(character == '\n') {
		for(i = defaultDisplay->cursorColumn; i < columns; i++) {
			lcdTwiSendData(defaultDisplay, ' ', LCD_TWI_DDRAM_CHAR);
		}
		lcdTwiCursorMoveNextLine(defaultDisplay);
	} else {
		lcdTwiSendData(defaultDisplay, character, LCD_TWI_DDRAM_CHAR);
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Debug function definitions - EXCLUDE BEFORE RELEASE -------------------------

#ifdef __DEBUG_MODE_ON
#if __DEBUG_MODE_ON == 1

uint8_t lcdTwiGetColumn(lcdTwi_s * lcd)
{
	return lcd->cursorColumn;
}

uint8_t lcdTwiGetLine(lcdTwi_s * lcd)
{
	return lcd->cursorLine;
}

#endif
#endif
