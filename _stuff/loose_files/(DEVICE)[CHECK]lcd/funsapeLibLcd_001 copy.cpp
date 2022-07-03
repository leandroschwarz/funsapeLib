
int lcdWriteStdWrapper(char character, FILE *stream);
void reportError(error_e error);

// =============================================================================
// Class own methods - public
// =============================================================================

bool Lcd::clearScreen(void)
{
	// Local variables
	uint8_t command = LCD_CLEAR_SCREEN_CMD;

	// Check for errors
	if(this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}

	// Send command
	if(!this->_handler->sendData(&command, 1)) {
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Updates data members
	this->_cursorLine = 0;
	this->_cursorColumn = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::cursorHome(void)
{
	// Local variables
	uint8_t command = LCD_CURSOR_HOME_CMD;

	// Check for errors
	if(this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		debugMessage(ERROR_NOT_INITIALIZED);
		return false;
	}

	// Send command
	if(!this->_handler->sendData(&command, 1)) {
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Updates data members
	this->_cursorLine = 0;
	this->_cursorColumn = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::init(displaySize_e size_p, fontSize_b font_p)
{
	// Returns sucessfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setEntryMode(entryMode_t entryMode_p)
{
	// Update data members
	this->_entryMode = entryMode_p;

	// Check if already initialized
	if(this->_initialized) {
		if(!this->_changeEntryMode()) {
			// Returns with error
			debugMessage(this->_lastError);
			return false;
		}
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::setHandler(Handler *hdlr_p)
{
	// Local variables
	Handler::handlerType_e hdlrType = Handler::HANDLER_TYPE_NONE;

	// Check for errors - Handler pointer
	if(!isPointerValid(hdlr_p)) {						// ERROR - Handler pointer is null
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		debugMessage(ERROR_HANDLER_POINTER_NULL);
		return false;
	}

	// Update data members
	this->_initialized = false;
	this->_handlerOk = false;
	hdlrType = hdlr_p->getHandlerType();
	switch(hdlrType) {
	case Handler::HANDLER_TYPE_LCD_PARALLEL:			// 4 or 8 bits LCD parallel interface
	case Handler::HANDLER_TYPE_LCD_SERIAL:				// Simple serial interface using external shift register
	case Handler::HANDLER_TYPE_LCD_TWI:					// TWI interface using PCF8574 chip
		this->_interfaceMode = hdlrType;
		this->_handler = hdlr_p;
		this->_handlerOk = true;
		break;
	default:
		// Returns error
		this->_lastError = ERROR_HANDLER_UNSUPPORTED;
		debugMessage(ERROR_HANDLER_UNSUPPORTED);
		return false;
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::turnCursorBlinkOn(bool state_p)
{
	// Update data members
	this->_cursorBlinkOn = state_p;

	// Check if already initialized
	if(this->_initialized) {
		if(!this->_changeDisplayControl()) {
			// Returns with error
			debugMessage(this->_lastError);
			return false;
		}
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::turnCursorOn(bool state_p)
{
	// Update data members
	this->_cursorOn = state_p;

	// Check if already initialized
	if(this->_initialized) {
		if(!this->_changeDisplayControl()) {
			// Returns with error
			debugMessage(this->_lastError);
			return false;
		}
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::turnDisplayOn(bool state_p)
{
	// Update data members
	this->_displayOn = state_p;

	// Check if already initialized
	if(this->_initialized) {
		if(!this->_changeDisplayControl()) {
			// Returns with error
			debugMessage(this->_lastError);
			return false;
		}
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}







bool Lcd::init(displaySize_e size_p, fontSize_b font_p)
{
	// Local variables
	uint8_t command = 0;

	// Update display size
	switch(size_p) {
	case LCD_16X1:						// Special case
		this->_displayMaxLine = 2;
		this->_displayMaxColumn = 7;
		break;
	default:
		this->_displayMaxLine = ((size_p % 100) - 1);
		this->_displayMaxColumn = ((size_p / 100) - 1);
		break;
	}

	// Update font type
	this->_fontSize = font_p;

	// Reset the controller, must be sent 3 times
	delayMs(15);
	if(!this->_sendCommand(LCD_FUNCTION_RESET_CMD)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayMs(5);
	if(!this->_sendCommand(LCD_FUNCTION_RESET_CMD)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayUs(60);
	if(!this->_sendCommand(LCD_FUNCTION_RESET_CMD)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayUs(60);

	// Function set
	command = LCD_FUNCTION_SET_CMD | LCD_FUNCTION_8_BITS;
	if(!this->_sendCommand(command)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayUs(60);
	command |= (this->_fontSize == FONT_SIZE_5X8) ? LCD_FUNCTION_5x8_FONT : LCD_FUNCTION_5x10_FONT;
	command |= (this->_displayMaxLine == 1) ? LCD_FUNCTION_2_LINES : LCD_FUNCTION_1_LINE;	// MaxLine = Lines -1
	if(!this->_sendCommand(command)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayUs(60);
	if(!this->_sendCommand(LCD_DISPLAY_CMD | LCD_DISPLAY_OFF)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayUs(60);
	if(!this->_sendCommand(LCD_CLEAR_SCREEN_CMD)) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	delayMs(2);
	// Entry mode set
	if(!this->_changeEntryMode()) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}
	// Updates display state
	if(!this->_changeDisplayControl()) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}

	// Updates the struct variables
	this->_initialized = true;
	this->_cursorColumn = 0;
	this->_cursorLine = 0;

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

// -----------------------------------------------------------------------------
// Function:	lcdCursorGoTo
// Purpose:		Moves the cursor to position
// Note:		Origin is considered as position (1, 1)
// Arguments:	lcd			Pointer to the LCD struct
//				line		line
//				column		column
// Returns:		LCD_NOT_INITIALIZED
//				LCD_UNSUPPORTED_DISPLAY_SIZE
//				LCD_UNSUPPORTED_PARAMETER
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdCursorGoTo(volatile lcdConfiguration_t *lcd, uint8_t line, uint8_t column)
// {
// 	uint8_t address = 0x00;

// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	if(line == 1) {
// 		switch(lcd->size) {
// 		case LCD_8X1:
// 		case LCD_16X1:
// 		case LCD_16X1b:
// 		case LCD_20X1:
// 		case LCD_24X1:
// 		case LCD_40X1:
// 		case LCD_8X2:
// 		case LCD_10x2:
// 		case LCD_12X2:
// 		case LCD_16X2:
// 		case LCD_20X2:
// 		case LCD_24X2:
// 		case LCD_30X2:
// 		case LCD_32X2:
// 		case LCD_40X2:
// 		case LCD_12X4:
// 		case LCD_16X4:
// 		case LCD_20X4:
// 			address += (column - 1);
// 			break;
// 		default:
// 			return LCD_UNSUPPORTED_DISPLAY_SIZE;
// 		}
// 	} else if(line == 2) {
// 		switch(lcd->size) {
// 		case LCD_8X1:
// 		case LCD_16X1:
// 		case LCD_16X1b:
// 		case LCD_20X1:
// 		case LCD_24X1:
// 		case LCD_40X1:
// 			return LCD_UNSUPPORTED_PARAMETER;
// 		case LCD_8X2:
// 		case LCD_10x2:
// 		case LCD_12X2:
// 		case LCD_16X2:
// 		case LCD_20X2:
// 		case LCD_24X2:
// 		case LCD_30X2:
// 		case LCD_32X2:
// 		case LCD_40X2:
// 		case LCD_12X4:
// 		case LCD_16X4:
// 		case LCD_20X4:
// 			address += (0x40 + column - 1);
// 			break;
// 		default:
// 			return LCD_UNSUPPORTED_DISPLAY_SIZE;
// 		}
// 	} else if(line == 3) {
// 		switch(lcd->size) {
// 		case LCD_8X1:
// 		case LCD_16X1:
// 		case LCD_16X1b:
// 		case LCD_20X1:
// 		case LCD_24X1:
// 		case LCD_40X1:
// 		case LCD_8X2:
// 		case LCD_10x2:
// 		case LCD_12X2:
// 		case LCD_16X2:
// 		case LCD_20X2:
// 		case LCD_24X2:
// 		case LCD_30X2:
// 		case LCD_32X2:
// 		case LCD_40X2:
// 			return LCD_UNSUPPORTED_PARAMETER;
// 		case LCD_12X4:
// 			address += (0x0C + column - 1);
// 			break;
// 		case LCD_16X4:
// 			address += (0x10 + column - 1);
// 			break;
// 		case LCD_20X4:
// 			address += (0x14 + column - 1);
// 			break;
// 		default:
// 			return LCD_UNSUPPORTED_DISPLAY_SIZE;
// 		}
// 	} else {
// 		switch(lcd->size) {
// 		case LCD_8X1:
// 		case LCD_16X1:
// 		case LCD_16X1b:
// 		case LCD_20X1:
// 		case LCD_24X1:
// 		case LCD_40X1:
// 		case LCD_8X2:
// 		case LCD_10x2:
// 		case LCD_12X2:
// 		case LCD_16X2:
// 		case LCD_20X2:
// 		case LCD_24X2:
// 		case LCD_30X2:
// 		case LCD_32X2:
// 		case LCD_40X2:
// 			return LCD_UNSUPPORTED_PARAMETER;
// 		case LCD_12X4:
// 			address += (0x4C + column - 1);
// 			break;
// 		case LCD_16X4:
// 			address += (0x50 + column - 1);
// 			break;
// 		case LCD_20X4:
// 			address += (0x54 + column - 1);
// 			break;
// 		default:
// 			return LCD_UNSUPPORTED_DISPLAY_SIZE;
// 		}
// 	}

// 	lcd->cursorLine = line - 1;
// 	lcd->cursorColumn = column - 1;

// 	return this->_sendCommand((LCD_DDRAM_ADRESS | address));
// }

// -----------------------------------------------------------------------------
// Function:	lcdStdio
// Purpose:		Associate the LCD to the default output for printf function
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		LCD_NOT_INITIALIZED
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdStdio(volatile lcdConfiguration_t *lcd)
// {
// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	stdin = stdout = stderr = &lcdStream;
// 	defaultDisplay = lcd;

// 	return LCD_OK;
// }

// -----------------------------------------------------------------------------
// Function:	lcdCursorMoveNextLine
// Purpose:		Moves cursor to the beginning of the next line
// Note:		The cursor will wrap automatically from the last line to the
//				first line
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		LCD_NOT_INITIALIZED
//				LCD_UNSUPPORTED_DISPLAY_SIZE
//				LCD_UNSUPPORTED_PARAMETER
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdCursorMoveNextLine(volatile lcdConfiguration_t *lcd)
// {
// 	uint8_t line = 0;

// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	switch(lcd->size) {
// 	case LCD_8X1:
// 	case LCD_16X1:
// 	case LCD_16X1b:
// 	case LCD_20X1:
// 	case LCD_24X1:
// 	case LCD_40X1:
// 		return lcdCursorGoTo(lcd, (line + 1), 1);
// 	case LCD_8X2:
// 	case LCD_10x2:
// 	case LCD_12X2:
// 	case LCD_16X2:
// 	case LCD_20X2:
// 	case LCD_24X2:
// 	case LCD_30X2:
// 	case LCD_32X2:
// 	case LCD_40X2:
// 		line = (lcd->cursorLine == 1) ? 0 : 1;
// 		return lcdCursorGoTo(lcd, (line + 1), 1);
// 	case LCD_12X4:
// 	case LCD_16X4:
// 	case LCD_20X4:
// 		line = (lcd->cursorLine == 3) ? 0 : (lcd->cursorLine + 1);
// 		return lcdCursorGoTo(lcd, (line + 1), 1);
// 	default:
// 		return LCD_UNSUPPORTED_DISPLAY_SIZE;
// 	}
// }

// -----------------------------------------------------------------------------
// Function:	lcdCursorMoveFirstLine
// Purpose:		Moves cursor to the beginning of the first line
// Note:		The function just calls lcdCursorHome
// Arguments:	lcd			Pointer to the LCD struct
// Returns:		LCD_NOT_INITIALIZED
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdCursorMoveFirstLine(volatile lcdConfiguration_t *lcd)
// {
// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	return lcdCursorHome(lcd);
// }

// -----------------------------------------------------------------------------
// Function:	lcdCursorMove
// Purpose:		Moves the cursor one position to the LEFT or RIGHT
// Arguments:	lcd			Pointer to the LCD struct
//				dir			LEFT or RIGHT (use enumerations defined at
//							direction_t)
// Returns:		LCD_NOT_INITIALIZED
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdCursorMove(volatile lcdConfiguration_t *lcd, direction_t dir)
// {
// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	if(dir == LEFT) {
// 		lcd->cursorColumn--;
// 		return this->_sendCommand(0x10);
// 	}
// 	lcd->cursorColumn++;
// 	return this->_sendCommand(0x14);
// }

// -----------------------------------------------------------------------------
// Function:	lcdDisplayShift
// Purpose:		Moves the entire display one position to the LEFT or RIGHT
// Note:		The cursor is also moved in the opposite direction. The use of
//				this function may cause problems if used before printf() and
//				lcdCursorGoTo(). The effects of lcdDisplayShift() are cancelled
//				by the lcdCursorHome() and lcdCursorMoveFirstLine() functions.
// Arguments:	lcd			Pointer to the LCD struct
//				dir			direction to move (use enumerations defined at
//							direction_t)
// Returns:		LCD_NOT_INITIALIZED
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdDisplayShift(volatile lcdConfiguration_t *lcd, direction_t dir)
// {
// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	if(dir == LEFT) {
// 		lcd->cursorColumn--;
// 		return this->_sendCommand(0x18);
// 	}
// 	lcd->cursorColumn++;
// 	return this->_sendCommand(0x1C);
// }

// -----------------------------------------------------------------------------
// Private function definitions ------------------------------------------------

// -----------------------------------------------------------------------------
// Function:	this->_sendCommand
// Purpose:		Sends a command to the LCD
// Arguments:	lcd			Pointer to the LCD struct
//				command		Command to be sent to LCD
// Returns:		LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t this->_sendCommand(cdConfiguration_t *lcd, uint8_t command)
// {
// 	clrBit(*(lcd->controlPORT), lcd->controlRS);	// LCD in command mode
// 	clrBit(*(lcd->controlPORT), lcd->controlE);		// Makes sure enable is LOW
// 	*(lcd->dataPORT) = command;						// Writes data
// 	setBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse start
// 	delayUs(1);
// 	clrBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse end
// 	if((command == LCD_CLEAR_SCREEN) || (command == LCD_CURSOR_HOME)) {
// 		delayMs(2);
// 		lcd->cursorLine = 0;
// 		lcd->cursorColumn = 0;
// 	} else {
// 		delayUs(40);
// 	}

// 	return LCD_OK;
// }

// -----------------------------------------------------------------------------
// Function:	lcdWriteCharacter
// Purpose:		Sends a character to be written at the LCD
// Arguments:	lcd			Pointer to the LCD struct
//				character	Character to be sent to LCD
// Returns:		LCD_NOT_INITIALIZED
//				LCD_OK
// -----------------------------------------------------------------------------

// lcdResult_t lcdWriteCharacter(volatile lcdConfiguration_t *lcd, uint8_t character)
// {
// 	if(lcd->initialized == false) {
// 		return LCD_NOT_INITIALIZED;
// 	}

// 	if(lcd->cursorColumn < 40) {
// 		setBit(*(lcd->controlPORT), lcd->controlRS);	// LCD in command mode
// 		clrBit(*(lcd->controlPORT), lcd->controlE);		// Makes sure enable is LOW
// 		*(lcd->dataPORT) = character;						// Writes data
// 		setBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse start
// 		delayUs(1);
// 		clrBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse end
// 		delayUs(40);
// 		lcd->cursorColumn++;
// 	}

// 	return LCD_OK;
// }

// -----------------------------------------------------------------------------
// Function:	lcdWriteStd
// Purpose:		This function is called by the printf() function to write a
//				character at the display pointed by defaultDisplay.
// Arguments:	character	Character to be sent to LCD
//				stream		Pointer to a FILE stream
// Returns:		0 (This function always returns 0)
// Note:		This function must not be used outside this file
// -----------------------------------------------------------------------------

// int16_t lcdWriteStd(int8_t character, FILE *stream)
// {
// 	uint8_t columns = 0;
// 	uint8_t i = 0;

// 	if(defaultDisplay->initialized == false) {
// 		return false;
// 	}

// 	switch(defaultDisplay->size) {
// 	case LCD_8X1:
// 	case LCD_8X2:
// 		columns = 8;
// 		break;
// 	case LCD_10x2:
// 		columns = 10;
// 		break;
// 	case LCD_12X2:
// 	case LCD_12X4:
// 		columns = 12;
// 		break;
// 	case LCD_16X1:
// 	case LCD_16X1b:
// 	case LCD_16X2:
// 	case LCD_16X4:
// 		columns = 16;
// 		break;
// 	case LCD_20X1:
// 	case LCD_20X2:
// 	case LCD_20X4:
// 		columns = 20;
// 		break;
// 	case LCD_24X1:
// 	case LCD_24X2:
// 		columns = 24;
// 		break;
// 	case LCD_30X2:
// 		columns = 30;
// 		break;
// 	case LCD_32X2:
// 		columns = 32;
// 		break;
// 	case LCD_40X1:
// 	case LCD_40X2:
// 		columns = 40;
// 		break;
// 	}

// 	if(character == '\n') {
// 		for(i = defaultDisplay->cursorColumn; i < columns; i++) {
// 			lcdWriteCharacter(defaultDisplay, ' ');
// 		}
// 		lcdCursorMoveNextLine(defaultDisplay);
// 	} else {
// 		lcdWriteCharacter(defaultDisplay, character);
// 	}

// 	return 0;
// }

// =============================================================================
// Class own methods - private
// =============================================================================

void Lcd::_reset(void)
{
	// Reset data members
	this->_cursorBlinkOn				= false;
	this->_cursorColumn					= 0;
	this->_cursorLine					= 0;
	this->_cursorOn						= false;
	this->_displayMaxColumn				= 0;
	this->_displayMaxLine				= 0;
	this->_displayOn					= true;
	this->_entryMode					= ENTRY_MODE_OVERWRITE_INCREMENT;
	this->_fontSize						= FONT_SIZE_5X8;
	this->_handler						= nullptr;
	this->_handlerOk					= false;
	this->_initialized					= false;
	this->_interfaceMode				= Handler::HANDLER_TYPE_NONE;

	// Returns successfully
	return;
}




bool Lcd::_changeDisplayControl(void)
{
	// Local variables
	uint8_t command = LCD_DISPLAY_CMD;

	// Process command
	command |= (this->_displayOn) ? (LCD_DISPLAY_ON) : (LCD_DISPLAY_OFF);
	command |= (this->_cursorOn) ? (LCD_CURSOR_ON) : (LCD_CURSOR_OFF);
	command |= (this->_cursorBlink) ? (LCD_CURSOR_BLINK_ON) : (LCD_CURSOR_BLINK_OFF);

	if(!this->_handler->sendData(&command, 1)) {
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}

bool Lcd::_changeEntryMode(void)
{
	// Local variables
	uint8_t command = LCD_ENTRY_MODE_CMD;

	// Process command
	command |= this->_entryMode;

	if(!this->_handler->sendData(&command, 1)) {
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = ERROR_NONE;
	debugMessage(ERROR_NONE);
	return true;
}
*/

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
