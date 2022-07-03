/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseAvrLcdTwi.cpp
 * Module:		Character LCD controller Using PCF8594 over TWI
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseAvrLcdTwi.hpp>
#if __GPDSE_AVR_LCD_TWI_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [gpdseAvrLcdTwi.cpp] Build mismatch on header and source code files.
#endif

#include <gpdseAvrTwi.hpp>

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// static int lcdTwiWriteStd(char c, FILE * stream);
// FILE lcdTwiStream = FDEV_SETUP_STREAM(lcdTwiWriteStd, NULL, _FDEV_SETUP_WRITE);

// =============================================================================
// Static functions declarations
// =============================================================================

// static void lcdTwiFunctionSet8Bits(uint8_t command);
// static void lcdTwiWriteCharacter(uint8_t character);
// static void lcdTwiWriteCommand(uint8_t command);

// =============================================================================
// Class constructors
// =============================================================================

LcdTwi::LcdTwi(Twi0 *twiHandler,
        lcdTwiPinout_e pinout,
        lcdDisplaySize_e displaySize,
        lcdFontSize_e fontSize)
{
    // Reset data members
    this->_resetDataMembers();

    // Update data members - Twi handler
    if (!this->setTwiHandler(twiHandler)) {
        // Error
        return;
    }
    // Update data members - Pinout


}

// =============================================================================
// Class public methods
// =============================================================================

bool LcdTwi::initialize(void)
{

    // Update data members - Display size
    if (!setDisplaySize(displaySize)) {
        // Error
        return;
    }
    // Update data members - Font size
    if (!setFontSize(fontSize)) {
        // Error
        return;
    }

    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}

bool LcdTwi::setTwiHandler(Twi0 *twiHandler)
{
    // Deinitialize
    this->_deinitialize();

    // Check for errors
    if (twiHandler == NULL) {
        this->_lastError = ERROR_HANDLER_POINTER_NULL;
        return false;
    }

    // Update data members
    this->_twiHandler = twiHandler;

    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}

bool LcdTwi::setPinout(lcdTwiPinout_e pinout)
{
    // Deinitialize
    this->_deinitialize();

    // Validate and decode pinout
    if (!this->_validatePinout(pinout)) {
        // Error
        return false;
    }

    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}

bool LcdTwi::setPinout(char *pinout)
{
    // Deinitialize
    this->_deinitialize();

    // Validate and decode pinout
    if (!this->_validatePinout(pinout)) {
        // Error
        return false;
    }

    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}

bool LcdTwi::setDisplaySize(lcdDisplaySize_e displaySize)
{
    // Deinitialize
    this->_deinitialize();

    // Validate and decode display size
    if (!this->_validateLcdDisplaySize(displaySize)) {
        // Error
        return false;
    }

    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}

bool LcdTwi::setFontSize(lcdFontSize_e fontSize)
{
    // Deinitialize
    this->_deinitialize();

    // Validate and decode font size
    if (!this->_validateLcdFontSize(fontSize)) {
        // Error
        return false;
    }

    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}

bool LcdTwi::setEntryMode(lcdEntryMode_e entryMode)
{



    // Return successfully
    this->_lastError = ERROR_NONE;
    return true;
}



// NONE

// =============================================================================
// Class private methods
// =============================================================================

bool LcdTwi::_deinitialize(void)
{
    // Deinitialize data members
    this->_initialized = false;

    // Return successfully
    return true;
}

void LcdTwi::_resetDataMembers(void)
{
    // Reset data members - initialization
    this->_initialized = false;
    // Reset data members - PCF8594 hardware configuration
    this->_twiHandler = NULL;
    this->_pinBl = 0;
    this->_pinD4 = 0;
    this->_pinD5 = 0;
    this->_pinD6 = 0;
    this->_pinD7 = 0;
    this->_pinEn = 0;
    this->_pinRs = 0;
    this->_pinRw = 0;
    // Reset data members - LCD configuration
    this->_cursorColumn = 0;
    this->_cursorLine = 0;
    this->_cursorState = LCD_CURSOR_OFF;
    this->_displayColumns = 0;
    this->_displayLines = 0;
    this->_displayState = false;
    this->_entryMode = LCD_RIGHT_OVERWRITE;
    this->_fontSize = LCD_FONT_SIZE_5X8;
    // Reset data members - Error messages
    this->_lastError = ERROR_NONE;

    // Return successfully
    return;
}

bool LcdTwi::_validatePinout(lcdTwiPinout_e pinout)
{
    // Check error - verify if pinout exists
    switch (pinout) {
    case LCD_TWI_PINOUT_D7_D6_D5_D4_BL_EN_RW_RS ...
            LCD_TWI_PINOUT_D7_D6_D5_D4_RS_RW_EN_BL:
        break;
    default:
        // Return error
        this->_lastError = ERROR_ARGUMENTS_PINOUT_UNSUPPORTED;
        return false;
    }

    // Return successfully
    return true;
}

bool LcdTwi::_validateLcdDisplaySize(lcdDisplaySize_e displaySize)
{
    // Check error - verify if size exists
    switch (displaySize) {
    case LCD_DISPLAY_SIZE_8X1:
    case LCD_DISPLAY_SIZE_8X2:
    case LCD_DISPLAY_SIZE_10x2:
    case LCD_DISPLAY_SIZE_12X2:
    case LCD_DISPLAY_SIZE_12X4:
    case LCD_DISPLAY_SIZE_16X1:
    case LCD_DISPLAY_SIZE_16X2:
    case LCD_DISPLAY_SIZE_16X4:
    case LCD_DISPLAY_SIZE_20X1:
    case LCD_DISPLAY_SIZE_20X2:
    case LCD_DISPLAY_SIZE_20X4:
    case LCD_DISPLAY_SIZE_24X1:
    case LCD_DISPLAY_SIZE_24X2:
    case LCD_DISPLAY_SIZE_30X2:
    case LCD_DISPLAY_SIZE_32X2:
    case LCD_DISPLAY_SIZE_40X1:
    case LCD_DISPLAY_SIZE_40X2:
        break;
    default:
        // Return error
        this->_lastError = ERROR_LCD_DISPLAY_SIZE_UNSUPPORTED;
        return false;
    }

    // Return successfully
    return true;
}

bool LcdTwi::_validateLcdFontSize(lcdFontSize_e fontSize)
{
    // Check error - verify if LCD font size exists
    switch (fontSize) {
    case LCD_FONT_SIZE_5X8:
    case LCD_FONT_SIZE_5X10:
        break;
    default:
        // Return error
        this->_lastError = ERROR_LCD_FONT_SIZE_UNSUPPORTED;
        return false;
    }

    // Return successfully
    return true;
}











// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Interruption handlers
// =============================================================================

// NONE





/*


// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

// -----------------------------------------------------------------------------
// Function:	lcdTwiClearScreen
// Purpose:		Clears the entire screen and moves cursor to home position
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiClearScreen(lcdTwiConfiguration_t * lcd)
{
	lcdTwiWriteCommand(lcd, LCD_CLEAR_SCREEN);
	lcd->_cursorLine = 0;
	lcd->_cursorColumn = 0;

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursor
// Purpose:		Turns cursor ON/OFF, without changing display or blink option
// Arguments:	lcd			Pointer to the LCD struct
// 				state		ON or OFF (enumerations defined at logic_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursor(lcdTwiConfiguration_t * lcd, logic_t state)
{
	uint8_t command = LCD_DISPLAY_OFF;

	lcd->_cursorOn = (state & 0x01);
	if (lcd->_displayOn) {
		command |= 4;
	}
	if (lcd->_cursorOn) {
		command |= 2;
	}
	if (lcd->_cursorBlink) {
		command |= 1;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursorBlink
// Purpose:		Turns cursor blink ON/OFF, without changing display or cursor
//				option
// Arguments:	lcd			Pointer to the LCD struct
//				state		ON or OFF (enumerations defined at logic_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursorBlink(lcdTwiConfiguration_t * lcd, logic_t state)
{
	uint8_t command = LCD_DISPLAY_OFF;

	lcd->_cursorBlink = (state & 0x01);
	if (lcd->_displayOn) {
		command |= 4;
	}
	if (lcd->_cursorOn) {
		command |= 2;
	}
	if (lcd->_cursorBlink) {
		command |= 1;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursorGoTo
// Purpose:		Moves the cursor to position
// Note:		Origin is considered as position (0, 0)
// Arguments:	lcd			Pointer to the LCD struct
//				line		line
//				column		column
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursorGoTo(lcdTwiConfiguration_t * lcd, uint8_t line, uint8_t column)
{
	uint8_t address = 0xFF;

	switch (line) {
	case 0:		// Go to line 0
		address = column;
		break;
	case 1:		// Go to line 1
		address = (lcd->_displayLines >= 1) ? (0x40 + column) : 0xFF;
		break;
	case 2:		// Go to line 2
		if ((lcd->_displayLines == 3) && (lcd->_displayColumns == 11)) {				// Display 12x4
			address = 0x0C + column;
		} else if ((lcd->_displayLines == 3) && (lcd->_displayColumns == 15)) {		// Display 16x4
			address = 0x10 + column;
		} else if ((lcd->_displayLines == 3) && (lcd->_displayColumns == 19)) {		// Display 20x4
			address = 0x14 + column;
		}
		break;
	case 3:		// Go to line 3
		if ((lcd->_displayLines == 3) && (lcd->_displayColumns == 11)) {				// Display 12x4
			address = 0x4C + column;
		} else if ((lcd->_displayLines == 3) && (lcd->_displayColumns == 15)) {		// Display 16x4
			address = 0x50 + column;
		} else if ((lcd->_displayLines == 3) && (lcd->_displayColumns == 19)) {		// Display 20x4
			address = 0x54 + column;
		}
		break;
	}

	if (address != 0xFF) {
		lcd->_cursorLine = line;
		lcd->_cursorColumn = column;
		lcdTwiWriteCommand(lcd, (LCD_DDRAM_ADRESS | address));
	}

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursorHome
// Purpose:		Moves cursor to home position
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursorHome(lcdTwiConfiguration_t * lcd)
{
	lcdTwiWriteCommand(lcd, LCD_CURSOR_HOME);
	lcd->_cursorLine = 0;
	lcd->_cursorColumn = 0;

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursorMove
// Purpose:		Moves the cursor one position to the LEFT or RIGHT
// Arguments:	lcd			Pointer to the LCD struct
//				dir			LEFT or RIGHT (use enumerations defined at
//							direction_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursorMove(lcdTwiConfiguration_t * lcd, direction_t dir)
{
	if (dir == LEFT) {
		lcd->_cursorColumn--;
		lcdTwiWriteCommand(lcd, 0x10);
	} else {
		lcd->_cursorColumn++;
		lcdTwiWriteCommand(lcd, 0x14);
	}

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursorMoveFirstLine
// Purpose:		Moves cursor to the beginning of the first line
// Note:		The function just calls lcdTwiCursorHome
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursorMoveFirstLine(lcdTwiConfiguration_t * lcd)
{
	lcdTwiCursorHome(lcd);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiCursorMoveNextLine
// Purpose:		Moves cursor to the beginning of the next line
// Note:		The cursor will wrap automatically from the last line to the
//				first line
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiCursorMoveNextLine(lcdTwiConfiguration_t * lcd)
{
	uint8_t currentLine = (lcd->_cursorLine + 1);

	currentLine %= (lcd->_displayLines + 1);
	lcdTwiCursorGoTo(lcd, currentLine, 0);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiDisplay
// Purpose:		Turns display ON/OFF, without changing cursor or blink option
// Arguments:	lcd			Pointer to the LCD struct
//				state		ON or OFF (enumerations defined at logic_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiDisplay(lcdTwiConfiguration_t * lcd, logic_t state)
{
	uint8_t command = LCD_DISPLAY_OFF;

	lcd->_displayOn = (state & 0x01);
	if (lcd->_displayOn) {
		command |= 4;
	}
	if (lcd->_cursorOn) {
		command |= 2;
	}
	if (lcd->_cursorBlink) {
		command |= 1;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiDisplayShift
// Purpose:		Moves the entire display one position to the LEFT or RIGHT
// Note:		The cursor is also moved in the opposite direction. The use of
//				this function may cause problems if used before printf() and
//				lcdTwiCursorGoTo(). The effects of lcdTwiDisplayShift() are cancelled
//				by the lcdTwiCursorHome() and lcdTwiCursorMoveFirstLine() functions.
// Arguments:	lcd			Pointer to the LCD struct
//				dir			direction to move (use enumerations defined at
//							direction_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiDisplayShift(lcdTwiConfiguration_t * lcd, direction_t dir)
{
	if (dir == LEFT) {
		lcd->_cursorColumn--;
		lcdTwiWriteCommand(lcd, 0x18);
	} else {
		lcd->_cursorColumn++;
		lcdTwiWriteCommand(lcd, 0x1C);
	}

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiInit
// Purpose:		Performs the LCD initialization routine
// Arguments:	lcd			Pointer to the LCD struct
//				size		Size of the display (use enumerations at lcdTwiSize_t)
//				font		Font size (use enumerations at lcdTwiFont_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiInit(lcdTwiConfiguration_t * lcd, lcdTwiSize_t size, lcdTwiFont_t font)
{
	uint8_t command = 0;

	// I/O initialization
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);
	setMaskOffset(*(lcd->dataDDR), 0x0F, lcd->dataFirst);
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
	if (lcd->entryIncDec == LCD_INCREMENT) {
		command |= LCD_ENTRY_INCREMENT;
	} else {
		command |= LCD_ENTRY_DECREMENT;
	}
	if (lcd->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
		command |= LCD_ENTRY_SHIFT;
	} else {
		command |= LCD_ENTRY_OVERWRITE;
	}
	lcdTwiWriteCommand(lcd, command);

	lcdTwiWriteCommand(lcd, LCD_DISPLAY_ON);

	// Updates the struct variables
	lcd->_displayOn = TRUE;
	lcd->_cursorOn = FALSE;
	lcd->_cursorBlink = FALSE;
	lcd->_cursorLine = 0;
	lcd->_cursorColumn = 0;
	lcd->_displayLines = (size / 100) - 1;
	lcd->_displayColumns = (size % 100) - 1;

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiSetControlPort
// Purpose:		Configures the LCD I/O ports for the control bus
// Arguments:	lcd			Pointer to the LCD struct
//				controlDDR	Pointer to I / O Data Direction Register
//				controlPORT	Pointer to I / O Port Output Register
//				controlE	Position of pin E at the control register
//				controlRS	Position of pin RS at the control register
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiSetControlPort(lcdTwiConfiguration_t * lcd, vuint8_t * controlDDR, vuint8_t * controlPORT, uint8_t controlE, uint8_t controlRS)
{
	lcd->controlDDR = controlDDR;
	lcd->controlPORT = controlPORT;
	lcd->controlE = controlE;
	lcd->controlRS = controlRS;

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiSetDataPort
// Purpose:		Configures the LCD I/O ports for the data bus
// Arguments:	lcd			Pointer to the LCD struct
//				dataDDR		Pointer to I / O Data Direction Register
//				dataPORT	Pointer to I / O Port Output Register
//				dataFirst	Position of the first bit of the LCD data nibble
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiSetDataPort(lcdTwiConfiguration_t * lcd, vuint8_t * dataDDR, vuint8_t * dataPORT, uint8_t dataFirst)
{
	lcd->dataDDR = dataDDR;
	lcd->dataPORT = dataPORT;
	lcd->dataFirst = dataFirst;

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiSetEntryMode
// Purpose:		Configures how the display will behave when a character is
//				written
// Arguments:	lcd			Pointer to the LCD struct
//				dir			write direction (enumerations defined at
//							lcdTwiIncrementDecrement_t)
//				mode		insert / overwrite (enumerations defined at
//							lcdTwiShiftOverwrite_t)
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiSetEntryMode(lcdTwiConfiguration_t * lcd, lcdTwiIncrementDecrement_t dir, lcdTwiShiftOverwrite_t mode)
{
	uint8_t command = LCD_ENTRY_SET;

	if (dir != LCD_INCDEC_NO_CHANGE) {
		lcd->entryIncDec = (dir & 0x01);
	}
	if (mode != LCD_DISPLAY_NO_CHANGE) {
		lcd->entryShiftDisplay = (mode & 0x01);
	}

	if (lcd->entryIncDec == LCD_INCREMENT) {
		command |= LCD_ENTRY_INCREMENT;
	} else {
		command |= LCD_ENTRY_DECREMENT;
	}
	if (lcd->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
		command |= LCD_ENTRY_SHIFT;
	} else {
		command |= LCD_ENTRY_OVERWRITE;
	}
	lcdTwiWriteCommand(lcd, command);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiStdio
// Purpose:		Associate the LCD to the default output for printf function
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		-
// -----------------------------------------------------------------------------

void lcdTwiStdio(lcdTwiConfiguration_t * lcd)
{
	stdin = stdout = stderr = &lcdStream;
	defaultDisplay = lcd;

	return;
}

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

// -----------------------------------------------------------------------------
// Function:	lcdTwiFunctionSet8Bits
// Purpose:		Sends a command to the LCD in 8 bits data interface
// Arguments:	lcd			Pointer to the LCD struct
//				command		Command to be sent to LCD
// Returns:		-
// Note:		This function must not be used outside this file
// -----------------------------------------------------------------------------

void lcdTwiFunctionSet8Bits(lcdTwiConfiguration_t * lcd, uint8_t command)
{
	clrBit(*(lcd->controlPORT), lcd->controlRS);		// LCD in command mode
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Makes sure enable is LOW
	clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);	// Writes data
	*(lcd->dataPORT) |= ((command >> 4) << lcd->dataFirst);
	setBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(lcd->controlPORT), lcd->controlE);			// Enable pulse end
	_delay_us(40);

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiWriteCharacter
// Purpose:		Sends a character to be written at the LCD
// Arguments:	lcd			Pointer to the LCD struct
//				character	Character to be sent to LCD
// Returns:		-
// Note:		This function must not be used outside this file
// -----------------------------------------------------------------------------

void lcdTwiWriteCharacter(lcdTwiConfiguration_t * lcd, uint8_t character)
{
	if (lcd->_cursorColumn < 40) {
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
		_delay_us(40);
		lcd->_cursorColumn++;
	}

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdTwiWriteCommand
// Purpose:		Sends a command to the LCD
// Arguments:	lcd			Pointer to the LCD struct
//				command		Command to be sent to LCD
// Returns:		-
// Note:		This function must not be used outside this file
// -----------------------------------------------------------------------------

void lcdTwiWriteCommand(lcdTwiConfiguration_t * lcd, uint8_t command)
{
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
	if ((command == LCD_CLEAR_SCREEN) || (command == LCD_CURSOR_HOME)) {
		_delay_ms(2);
	} else {
		_delay_us(40);
	}

	return;
}

// -----------------------------------------------------------------------------
// Function:	lcdWriteStd
// Purpose:		This function is called by the printf() function to write a
//				character at the display pointed by defaultDisplay.
// Arguments:	character	Character to be sent to LCD
//				stream		Pointer to a FILE stream
// Returns:		0 (This function always returns 0)
// Note:		This function must not be used outside this file
// -----------------------------------------------------------------------------

static int16_t lcdWriteStd(int8_t character, FILE * stream)
{
	uint8_t _displayColumns = defaultDisplay->_displayColumns + 1;
	uint8_t i = 0;

	if (character == '\n') {
		for (i = defaultDisplay->_cursorColumn; i < _displayColumns; i++) {
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

uint8_t lcdTwiGetColumn(lcdTwiConfiguration_t * lcd)
{
	return lcd->_cursorColumn;
}

uint8_t lcdTwiGetLine(lcdTwiConfiguration_t * lcd)
{
	return lcd->_cursorLine;
}
*/
