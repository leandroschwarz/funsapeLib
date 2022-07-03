/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		lcd.cpp
 * Module:			Character LCD support for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "lcd.hpp"
#ifndef __FUNSAPE_LCD_HPP
#	error	[lcd.cpp] Error 103 - Header file (lcd.hpp) must be corruted!
#elif __FUNSAPE_LCD_HPP != __BUILD_TO_REPLACE__
#	error	[lcd.cpp] Error 104 - Build mismatch between source (lcd.hpp) and header (lcd.hpp) files!
#endif

// =============================================================================
// File exclusive - Constant definitions
// =============================================================================

#define LCD_CMD_CLEAR_SCREEN							0x01
#define LCD_CMD_CURSOR_RETURN_HOME						0x02
#define LCD_CMD_ENTRY_MODE								0x04
#define LCD_BIT_ENTRY_MODE_INC_DEC_OFFSET						1
#define LCD_BIT_ENTRY_MODE_SHIFT_OVERW_OFFSET					0
#define LCD_CMD_DISPLAY_MODE							0x08
#define LCD_BIT_DISPLAY_MODE_DISPLAY_ON_OFFSET					2
#define LCD_BIT_DISPLAY_MODE_CURSOR_ON_OFFSET					1
#define LCD_BIT_DISPLAY_MODE_BLINK_ON_OFFSET					0
#define LCD_CMD_SHIFT_MODE								0x10
#define LCD_BIT_SHIFT_MODE_DISPLAY_CURSOR_OFFSET				3
#define LCD_BIT_SHIFT_MODE_RIGHT_LEFT_OFFSET					2
#define LCD_CMD_FUNCTION_SET							0x20
#define LCD_BIT_FUNCTION_SET_INTERFACE_MODE_OFFSET				4
#define LCD_BIT_FUNCTION_SET_NUMBER_OF_LINES_OFFSET				3
#define LCD_BIT_FUNCTION_SET_FONT_MODE_OFFSET					2
#define LCD_CMD_CGRAM_ADDRESS_SET						0x40
#define LCD_CMD_DDRAM_ADDRESS_SET						0x80
#define LCD_PIN_UNDEFINED								0x00

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions definitions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Private functions declarations
// =============================================================================

// NONE

// =============================================================================
// Lcd class - Constructors
// =============================================================================

Lcd::Lcd(lcdSize_e size, lcdFont_e font)
{
	// Reset data members
	this->_ioDataDir					= NULL;
	this->_ioDataFirst					= LCD_PIN_UNDEFINED;
	this->_ioDataMask					= 0;
	this->_ioControlDir					= NULL;
	this->_ioControlE					= LCD_PIN_UNDEFINED;
	this->_ioControlRs					= LCD_PIN_UNDEFINED;
	this->_ioControlRw					= LCD_PIN_UNDEFINED;
	this->_cursorBlink					= false;
	this->_cursorColumn					= 0;
	this->_cursorLine					= 0;
	this->_cursorShow					= false;
	this->_displayOn					= false;
	this->_entryIncDec					= LCD_INCREMENT;
	this->_entryShiftDisplay			= LCD_DISPLAY_OVERWRITE;
	this->_columns						= 0;
	this->_lines						= 0;
	this->_interface					= LCD_4_BITS_DELAY;
	this->_ctrlPortSet					= false;
	this->_dataPortSet					= false;
	this->_interfaceSet					= false;
	this->_initialized					= false;

	// Update data members
	this->_font							= font;
	this->_size							= size;

	// Returns successfuly
	this->_lastError = ERROR_NONE;
	return;
}

Lcd::~Lcd(void)
{
	// TODO: Create destructor
	return;
}

// =============================================================================
// Lcd class - Public methods
// =============================================================================

bool Lcd::clearScreen(void)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(LCD_CMD_CLEAR_SCREEN);
}

bool Lcd::cursorBlink(bool blink)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Prepare command
	uint8_t command = LCD_CMD_DISPLAY_MODE;
	command |= (this->_displayOn) << LCD_BIT_DISPLAY_MODE_DISPLAY_ON_OFFSET;
	command |= (this->_cursorShow) << LCD_BIT_DISPLAY_MODE_CURSOR_ON_OFFSET;
	command |= blink << LCD_BIT_DISPLAY_MODE_BLINK_ON_OFFSET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(command);
}

bool Lcd::cursorGoTo(uint8_t line, uint8_t column)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	// Check for errors - arguments
	if ((line > this->_lines) || (column > this->_columns)) {
		this->_lastError = ERROR_ARGUMENTS_WRONG_NUMBER;	// FIXME:
	}

	// Evaluate line
	uint8_t address = 0;
	switch (line) {
	case 1:
		address += (column - 1);
		break;
	case 2:
		address += (0x40 + column - 1);
		break;
	case 3:
		if (this->_size == LCD_12X4) {
			address += (0x0C + column - 1);
		} else if (LCD_16X4) {
			address += (0x10 + column - 1);
		} else if (LCD_20X4) {
			address += (0x14 + column - 1);
		} else {
			// Returns with error
			this->_lastError = ERROR_UNKNOWN;			// FIXME: CHANGE ERROR NAME
			return false;
		}
		break;
	case 4:
		if (this->_size == LCD_12X4) {
			address += (0x4C + column - 1);
		} else if (LCD_16X4) {
			address += (0x50 + column - 1);
		} else if (LCD_20X4) {
			address += (0x54 + column - 1);
		} else {
			// Returns with error
			this->_lastError = ERROR_UNKNOWN;			// FIXME: CHANGE ERROR NAME
			return false;
		}
		break;
	default:
		// Returns with error
		this->_lastError = ERROR_UNKNOWN;			// FIXME: CHANGE ERROR NAME
		return false;
	}
	address | LCD_CMD_DDRAM_ADDRESS_SET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(address);
}

bool Lcd::cursorHome(void)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(LCD_CMD_CURSOR_RETURN_HOME);
}

bool Lcd::cursorMove(bool leftRight)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Prepare command
	uint8_t command = LCD_CMD_SHIFT_MODE;
	command |= leftRight << LCD_BIT_SHIFT_MODE_RIGHT_LEFT_OFFSET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return lcdWriteCommand(lcd, 0x10);
}

bool Lcd::cursorMoveFirstLine(void)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(LCD_CMD_CURSOR_RETURN_HOME);
}

bool Lcd::cursorMoveNextLine(void)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	if (this->_lines == 1) {
		// Returns operation result
		return this->cursorGoTo(1, 1);
	} else if (this->_lines == 2) {
		// Returns operation result
		uint8_t line = (this->_cursorLine == 1) ? 0 : 1;
		return this->cursorGoTo((line + 1), 1);
	} else if (this->_lines == 4) {
		// Returns operation result
		uint8_t line = (this->_cursorLine == 3) ? 0 : (this->_cursorLine + 1);
		return this->cursorGoTo((line + 1), 1);
	} else {
		// Returns with error
		this->_lastError = ERROR_UNKNOWN;			// FIXME: CHANGE ERROR NAME
		return false;
	}

	// Returns with error
	this->_lastError = ERROR_UNKNOWN;			// FIXME: You shouldn't arrive here!!!
	return false;
}

bool Lcd::cursorShow(bool show)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Prepare command
	uint8_t command = LCD_CMD_DISPLAY_MODE;
	command |= (this->_displayOn) << LCD_BIT_DISPLAY_MODE_DISPLAY_ON_OFFSET;
	command |= show << LCD_BIT_DISPLAY_MODE_CURSOR_ON_OFFSET;
	command |= (this->_cursorBlink) << LCD_BIT_DISPLAY_MODE_BLINK_ON_OFFSET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(command);
}

bool Lcd::displayOn(bool turnOn)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Prepare command
	uint8_t command = LCD_CMD_DISPLAY_MODE;
	command |= turnOn << LCD_BIT_DISPLAY_MODE_DISPLAY_ON_OFFSET;
	command |= (this->_cursorShow) << LCD_BIT_DISPLAY_MODE_CURSOR_ON_OFFSET;
	command |= (this->_cursorBlink) << LCD_BIT_DISPLAY_MODE_BLINK_ON_OFFSET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(command);
}

bool Lcd::initialize(void)
{
	// Check for errors - Initilization stuff
	if ((!this->_ctrlPortSet) || (!this->_dataPortSet) || (!this->_interfaceSet)) {
		// Returns with error
		this->_lastError = ERROR_UNKNOWN;	// FIXME: Create real error code
		return false;
	}

	// Get display size
	this->_lines = (this->_size / 100);
	this->_columns = (this->_size % 100);

	// Generate Data Mask
	do {
		uint8_t aux8 = 0;
		switch (this->_interface) {
		case LCD_8_BITS_DELAY:
		case LCD_8_BITS_POOLING:
			aux8 = 8;
			break;
		case LCD_4_BITS_DELAY:
		case LCD_4_BITS_POOLING:
			aux8 = 4;
			break;
		}
		uint16_t auxPin = this->_ioDataFirst;
		for (uint8_t i = 0; i < aux8; ++i) {
			this->_ioDataMask |= auxPin;
			auxPin <<= 1;
		}
	} while (0);

	// Configure Data Port
	do {
		// Local variables
		GPIO_InitTypeDef gpioConfig = {0};
		// Config pins
		gpioConfig.Pin |= this->_ioDataMask;
		gpioConfig.Mode = GPIO_MODE_AF_PP;
		gpioConfig.Pull  = GPIO_PULLUP;
		gpioConfig.Speed = GPIO_SPEED_FREQ_HIGH;
		// Changes pins configuration
		HAL_GPIO_Init(this->_ioDataDir, &gpioConfig);
		// Changes pins logic level
		HAL_GPIO_WritePin(this->_ioDataDir, this->_ioDataMask, GPIO_PIN_RESET);
	} while (0);

	// Configure Control Port
	do {
		// Local variables
		GPIO_InitTypeDef gpioConfig = {0};
		// Config pins
		switch (this->_interface) {
		case LCD_8_BITS_POOLING:
		case LCD_4_BITS_POOLING:
			gpioConfig.Pin |= this->_ioControlRw;		// DO NOT BREAK, JUST CONTINUE
		case LCD_8_BITS_DELAY:
		case LCD_4_BITS_DELAY:
			gpioConfig.Pin |= this->_ioControlRs;
			gpioConfig.Pin |= this->_ioControlE;
			break;
		}
		gpioConfig.Mode = GPIO_MODE_AF_PP; o
		gpioConfig.Pull  = GPIO_PULLUP;
		gpioConfig.Speed = GPIO_SPEED_FREQ_HIGH;
		// Execute configuration
		HAL_GPIO_Init(this->_ioControlDir, &gpioConfig);
		// Changes pins logic level
		HAL_GPIO_WritePin(this->_ioDataDir,
						  (this->_ioControlRw | this->_ioControlRs | this->_ioControlE),
						  GPIO_PIN_RESET);
	} while (0);

	// Reset the controller, must be sent 3 times
	HAL_Delay(15);
	this->functionSet8Bits(LCD_FUNCTION_RESET);
	HAL_Delay(5);
	this->functionSet8Bits(LCD_FUNCTION_RESET);
	HAL_Delay(1);
	this->functionSet8Bits(LCD_FUNCTION_RESET);
	HAL_Delay(1);

	// Configures the LCD to 4 bits interface
	this->functionSet8Bits(LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS);
	HAL_Delay(1);

	// Function set
	command = LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS;
	if (lcd->functionFontSize == LCD_FONT_5X8) {
		command |= LCD_FUNCTION_5x8_FONT;
	}
	if (lcd->functionFontSize == LCD_FONT_5X10) {
		command |= LCD_FUNCTION_5x10_FONT;
	}
	if (lines == 1) {
		command |= LCD_FUNCTION_1_LINE;
	}
	if ((lines == 2) || (lines == 4)) {
		command |= LCD_FUNCTION_2_LINES;
	}
	lcdWriteCommand(lcd, command);

// Turns display off
	lcdCheckBusyFlag(lcd);
	lcdWriteCommand(lcd, LCD_DISPLAY_OFF);

// Clears the screen
	lcdCheckBusyFlag(lcd);
	lcdWriteCommand(lcd, LCD_CLEAR_SCREEN);

// Entry set
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
	lcdCheckBusyFlag(lcd);
	lcdWriteCommand(lcd, command);

// Turns display on
	lcdCheckBusyFlag(lcd);
	lcdWriteCommand(lcd, LCD_DISPLAY_ON);

// Updates the struct variables
	lcd->initialized = TRUE;
	lcd->displayOn = TRUE;
	lcd->cursorOn = FALSE;
	lcd->cursorBlink = FALSE;
	lcd->cursorLine = 0;
	lcd->cursorColumn = 0;

	return LCD_OK;








// Returns successfuly
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::remapStdio(void)
{
	// TODO: Implement function
	// if (this->initialized == FALSE) {
	// 	return LCD_NOT_INITIALIZED;
	// }

	// stdin = stdout = stderr = &lcdStream;
	// defaultDisplay = lcd;

	// Returns successfuly
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setControlPort(ioReg_t ioControlDir, ioPin_t ioControlE, ioPin_t ioControlRs, ioPin_t ioControlRw)
{
	// TODO: check for argument range

	// Update data members
	this->_ioControlDir = ioControlDir;
	this->_ioControlE = ioControlE;
	this->_ioControlRs = ioControlRs;
	this->_ioControlRw = ioControlRw;
	this->_dataPortSet = true;
	this->_initialized = false;

	// Returns successfuly
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setControlPort(ioReg_t ioControlDir, ioPin_t ioControlE, ioPin_t ioControlRs)
{
	// TODO: check for argument range

	// Update data members
	this->_ioControlDir = ioControlDir;
	this->_ioControlE = ioControlE;
	this->_ioControlRs = ioControlRs;
	this->_ioControlRw = PIN_UNDEFINED;
	this->_dataPortSet = true;
	this->_initialized = false;

	// Returns successfuly
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setDataPort(ioReg_t ioDataDir, ioPin_t ioDataFirst)
{
	// TODO: check for argument range

	// Update data members
	this->_ioDataDir = ioDataDir;
	this->_ioDataFirst = ioDataFirst;
	this->_dataPortSet = true;
	this->_initialized = false;

	// Returns successfuly
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::displayShift(bool leftRight)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Prepare command
	uint8_t command = LCD_CMD_SHIFT_MODE;
	command |= 1 << LCD_BIT_SHIFT_MODE_DISPLAY_CURSOR_OFFSET;
	command |= leftRight << LCD_BIT_SHIFT_MODE_RIGHT_LEFT_OFFSET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(command);
}

bool Lcd::setEntryMode(lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode)
{
	// Check for errors - initialization
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Prepare command
	uint8_t command = LCD_CMD_ENTRY_MODE;
	command |= dir << LCD_BIT_ENTRY_MODE_INC_DEC_OFFSET;
	command |= mode << LCD_BIT_ENTRY_MODE_SHIFT_OVERW_OFFSET;

	this->_waitBusyFlag();				// FIXME: BUSY FLAG -> MOVE TO _writeCommand
	// Returns operation result
	return this->_writeCommand(command);
}

bool Lcd::setEntryMode(lcdIncrementDecrement_t dir)
{
	// Returns operation result
	return this->setEntryMode(dir, this->_entryShiftDisplay);
}

bool Lcd::setEntryMode(lcdShiftOverwrite_t mode)
{
	// Returns operation result
	return this->setEntryMode(this->_entryIncDec, mode);
}

bool Lcd::setInterface(lcdInterface_e interface)
{
	// Update data members
	this->_interface = interface;
	this->_interfaceSet = true;
	this->_initialized = false;

	// Returns successfuly
	this->_lastError = ERROR_NONE;
	return true;
}

error_e Lcd::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

// =============================================================================
// Lcd class - Private methods
// =============================================================================

uint8_t Lcd::_getLine(void)
{
	// Returns number of lines
	return this->_lines;
}
uint8_t Lcd::_getColumn(void)
{
	// Returns number of columns
	return this->_columns;
}

// =============================================================================
// Lcd class - Protected methods
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Private functions definitions
// =============================================================================

// NONE





/*

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

static void lcdFunctionSet8Bits(lcdConfiguration_t * lcd, uint8 command);
static void lcdWriteCharacter(lcdConfiguration_t * lcd, uint8 character);
static void lcdWriteCommand(lcdConfiguration_t * lcd, uint8 command);
static int16 lcdWriteStd(int8 c, FILE * stream);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

FILE lcdStream = FDEV_SETUP_STREAM(lcdWriteStd, NULL, _FDEV_SETUP_WRITE);
lcdConfiguration_t * defaultDisplay = NULL;

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

void lcdClearScreen(lcdConfiguration_t * lcd)
{
	lcdWriteCommand(lcd, LCD_CMD_CLEAR_SCREEN);
	this->cursorLine = 0;
	this->cursorColumn = 0;

	return;
}

void lcdCursor(lcdConfiguration_t * lcd, logic_t state)
{
	uint8 command = LCD_DISPLAY_OFF;

	this->cursorOn = (state & 0x01);
	if(this->displayOn) {
		command |= 4;
	}
	if(this->cursorOn) {
		command |= 2;
	}
	if(this->cursorBlink) {
		command |= 1;
	}
	lcdWriteCommand(lcd, command);

	return;
}

void lcdCursorBlink(lcdConfiguration_t * lcd, logic_t state)
{
	uint8 command = LCD_DISPLAY_OFF;

	this->cursorBlink = (state & 0x01);
	if(this->displayOn) {
		command |= 4;
	}
	if(this->cursorOn) {
		command |= 2;
	}
	if(this->cursorBlink) {
		command |= 1;
	}
	lcdWriteCommand(lcd, command);

	return;
}

void lcdCursorGoTo(lcdConfiguration_t * lcd, uint8 line, uint8 column)
{
	uint8 address = 0xFF;

	switch(line) {
	case 0:		// Go to line 0
		address = column;
		break;
	case 1:		// Go to line 1
		address = (this->lines >= 1) ? (0x40 + column) : 0xFF;
		break;
	case 2:		// Go to line 2
		if((this->lines == 3) && (this->columns == 11)) {				// Display 12x4
			address = 0x0C + column;
		} else if((this->lines == 3) && (this->columns == 15)) {		// Display 16x4
			address = 0x10 + column;
		} else if((this->lines == 3) && (this->columns == 19)) {		// Display 20x4
			address = 0x14 + column;
		}
		break;
	case 3:		// Go to line 3
		if((this->lines == 3) && (this->columns == 11)) {				// Display 12x4
			address = 0x4C + column;
		} else if((this->lines == 3) && (this->columns == 15)) {		// Display 16x4
			address = 0x50 + column;
		} else if((this->lines == 3) && (this->columns == 19)) {		// Display 20x4
			address = 0x54 + column;
		}
		break;
	}

	if(address != 0xFF) {
		this->cursorLine = line;
		this->cursorColumn = column;
		lcdWriteCommand(lcd, (LCD_DDRAM_ADRESS | address));
	}

	return;
}

void lcdCursorHome(lcdConfiguration_t * lcd)
{
	lcdWriteCommand(lcd, LCD_CURSOR_HOME);
	this->cursorLine = 0;
	this->cursorColumn = 0;

	return;
}

void lcdCursorMove(lcdConfiguration_t * lcd, direction_t dir)
{
	if(dir == LEFT) {
		this->cursorColumn--;
		lcdWriteCommand(lcd, 0x10);
	} else {
		this->cursorColumn++;
		lcdWriteCommand(lcd, 0x14);
	}

	return;
}

void lcdCursorMoveFirstLine(lcdConfiguration_t * lcd)
{
	lcdCursorHome(lcd);

	return;
}

void lcdCursorMoveNextLine(lcdConfiguration_t * lcd)
{
	uint8 currentLine = (this->cursorLine + 1);

	currentLine %= (this->lines + 1);
	lcdCursorGoTo(lcd, currentLine, 0);

	return;
}

void lcdDisplay(lcdConfiguration_t * lcd, logic_t state)
{
	uint8 command = LCD_DISPLAY_OFF;

	this->displayOn = (state & 0x01);
	if(this->displayOn) {
		command |= 4;
	}
	if(this->cursorOn) {
		command |= 2;
	}
	if(this->cursorBlink) {
		command |= 1;
	}
	lcdWriteCommand(lcd, command);

	return;
}

void lcdDisplayShift(lcdConfiguration_t * lcd, direction_t dir)
{
	if(dir == LEFT) {
		this->cursorColumn--;
		lcdWriteCommand(lcd, 0x18);
	} else {
		this->cursorColumn++;
		lcdWriteCommand(lcd, 0x1C);
	}

	return;
}

void lcdInit(lcdConfiguration_t * lcd, lcdSize_t size, lcdFont_t font)
{
	uint8 command = 0;

	// I/O initialization
	clrMask(*(this->dataPORT), 0x0F, this->dataFirst);
	setMask(*(this->dataDDR), 0x0F, this->dataFirst);
	clrBit(*(this->controlPORT), this->controlE);
	setBit(*(this->controlDDR), this->controlE);
	clrBit(*(this->controlPORT), this->controlRS);
	setBit(*(this->controlDDR), this->controlRS);

	// Reset the controller, must be sent 3 times
	_delay_ms(15);
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_ms(5);
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_us(60);
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_RESET);
	_delay_us(60);

	// Configures the LCD to 4 bits interface
	lcdFunctionSet8Bits(lcd, LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS);

	// Function set
	command = LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS;
	command |= (font == LCD_FONT_5X8) ? LCD_FUNCTION_5x8_FONT : LCD_FUNCTION_5x10_FONT;
	command |= (size < 200) ? LCD_FUNCTION_1_LINE : LCD_FUNCTION_2_LINES;
	lcdWriteCommand(lcd, command);
	lcdWriteCommand(lcd, LCD_DISPLAY_OFF);
	lcdWriteCommand(lcd, LCD_CMD_CLEAR_SCREEN);

	command = LCD_ENTRY_SET;
	if(this->entryIncDec == LCD_INCREMENT) {
		command |= LCD_ENTRY_INCREMENT;
	} else {
		command |= LCD_ENTRY_DECREMENT;
	}
	if(this->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
		command |= LCD_ENTRY_SHIFT;
	} else {
		command |= LCD_ENTRY_OVERWRITE;
	}
	lcdWriteCommand(lcd, command);

	lcdWriteCommand(lcd, LCD_DISPLAY_ON);

	// Updates the struct variables
	this->displayOn = TRUE;
	this->cursorOn = FALSE;
	this->cursorBlink = FALSE;
	this->cursorLine = 0;
	this->cursorColumn = 0;
	this->lines = (size / 100) - 1;
	this->columns = (size % 100) - 1;

	return;
}

void lcdSetControlPort(lcdConfiguration_t * lcd, vuint8 * controlDDR, vuint8 * controlPORT, uint8 controlE, uint8 controlRS)
{
	this->controlDDR = controlDDR;
	this->controlPORT = controlPORT;
	this->controlE = controlE;
	this->controlRS = controlRS;

	return;
}

void lcdSetDataPort(lcdConfiguration_t * lcd, vuint8 * dataDDR, vuint8 * dataPORT, uint8 dataFirst)
{
	this->dataDDR = dataDDR;
	this->dataPORT = dataPORT;
	this->dataFirst = dataFirst;

	return;
}

void lcdSetEntryMode(lcdConfiguration_t * lcd, lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode)
{
	uint8 command = LCD_ENTRY_SET;

	if(dir != LCD_INCDEC_NO_CHANGE) {
		this->entryIncDec = (dir & 0x01);
	}
	if(mode != LCD_DISPLAY_NO_CHANGE) {
		this->entryShiftDisplay = (mode & 0x01);
	}

	if(this->entryIncDec == LCD_INCREMENT) {
		command |= LCD_ENTRY_INCREMENT;
	} else {
		command |= LCD_ENTRY_DECREMENT;
	}
	if(this->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
		command |= LCD_ENTRY_SHIFT;
	} else {
		command |= LCD_ENTRY_OVERWRITE;
	}
	lcdWriteCommand(lcd, command);

	return;
}

void lcdStdio(lcdConfiguration_t * lcd)
{
	stdin = stdout = stderr = &lcdStream;
	defaultDisplay = lcd;

	return;
}

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

void lcdFunctionSet8Bits(lcdConfiguration_t * lcd, uint8 command)
{
	clrBit(*(this->controlPORT), this->controlRS);		// LCD in command mode
	clrBit(*(this->controlPORT), this->controlE);			// Makes sure enable is LOW
	clrMask(*(this->dataPORT), 0x0F, this->dataFirst);	// Writes data
	*(this->dataPORT) |= ((command << 4) << this->dataFirst);
	setBit(*(this->controlPORT), this->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(this->controlPORT), this->controlE);			// Enable pulse end
	_delay_us(40);

	return;
}

void lcdWriteCharacter(lcdConfiguration_t * lcd, uint8 character)
{
	if(this->cursorColumn < 40) {
		setBit(*(this->controlPORT), this->controlRS);		// LCD in command mode
		clrBit(*(this->controlPORT), this->controlE);			// Makes sure enable is LOW
		clrMask(*(this->dataPORT), 0x0F, this->dataFirst);	// Writes data (higher nibble)
		*(this->dataPORT) |= ((character << 4) << this->dataFirst);
		setBit(*(this->controlPORT), this->controlE);			// Enable pulse start
		_delay_us(1);
		clrBit(*(this->controlPORT), this->controlE);			// Enable pulse end
		_delay_us(1);
		clrMask(*(this->dataPORT), 0x0F, this->dataFirst);	// Writes data (lower nibble)
		*(this->dataPORT) |= ((character & 0x0F) << this->dataFirst);
		setBit(*(this->controlPORT), this->controlE);			// Enable pulse start
		_delay_us(1);
		clrBit(*(this->controlPORT), this->controlE);			// Enable pulse end
		_delay_us(40);
		this->cursorColumn++;
	}

	return;
}

void lcdWriteCommand(lcdConfiguration_t * lcd, uint8 command)
{
	clrBit(*(this->controlPORT), this->controlRS);		// LCD in command mode
	clrBit(*(this->controlPORT), this->controlE);			// Makes sure enable is LOW
	clrMask(*(this->dataPORT), 0x0F, this->dataFirst);	// Writes data (higher nibble)
	*(this->dataPORT) |= ((command << 4) << this->dataFirst);
	setBit(*(this->controlPORT), this->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(this->controlPORT), this->controlE);			// Enable pulse end
	_delay_us(1);
	clrMask(*(this->dataPORT), 0x0F, this->dataFirst);	// Writes data (lower nibble)
	*(this->dataPORT) |= ((command & 0x0F) << this->dataFirst);
	setBit(*(this->controlPORT), this->controlE);			// Enable pulse start
	_delay_us(1);
	clrBit(*(this->controlPORT), this->controlE);			// Enable pulse end
	_delay_us(1);
	if((command == LCD_CMD_CLEAR_SCREEN) || (command == LCD_CURSOR_HOME)) {
		_delay_ms(2);
	} else {
		_delay_us(40);
	}

	return;
}

static int16 lcdWriteStd(int8 character, FILE * stream)
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
	return this->cursorColumn;
}

uint8 lcdGetLine(lcdConfiguration_t * lcd)
{
	return this->cursorLine;
}
*/
