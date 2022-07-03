/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseAvrLcd.cpp
 * Module:		LCD Interface for GPDSE++ AVR Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseAvrLcd.hpp>
#if __GPDSE_AVR_LCD_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [gpdseAvrLcd.cpp] Build mismatch on header and source code files.
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define LCD_CLEAR_SCREEN				0x01
#define LCD_CURSOR_HOME					0x02
#define LCD_ENTRY_SET					0x04
#define LCD_ENTRY_INCREMENT				0x02
#define LCD_ENTRY_DECREMENT				0x00
#define LCD_ENTRY_SHIFT					0x01
#define LCD_ENTRY_OVERWRITE				0x00
#define LCD_DISPLAY_OFF					0x08
#define LCD_DISPLAY_ON					0x0C
#define LCD_FUNCTION_RESET				0x30
#define LCD_FUNCTION_SET				0x20
#define LCD_FUNCTION_8_BITS				0x10
#define LCD_FUNCTION_4_BITS				0x00
#define LCD_FUNCTION_2_LINES			0x08
#define LCD_FUNCTION_1_LINE				0x00
#define LCD_FUNCTION_5x10_FONT			0x04
#define LCD_FUNCTION_5x8_FONT			0x00
#define LCD_CGRAM_ADRESS				0x40
#define LCD_DDRAM_ADRESS				0x80

// Display and cursor states
#define _OFFSET_CONFIG_STATES			3
#define _OFFSET_DISPLAY_ON				2
#define _OFFSET_BLINK_ON				1
#define _OFFSET_CURSOR_ON				0

// Move pointer
#define _OFFSET_DDRAM_ADDRESS			7
#define _OFFSET_CGRAM_ADDRESS			6

// Display and cursor shifts
#define _OFFSET_CONFIG_SHIFTS			4
#define _OFFSET_1_DISPLAY_0_CURSOR		3
#define _OFFSET_1_RIGHT_0_LEFT			2

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

FILE lcdStream;
Lcd *lcdDefaultHandler;

// =============================================================================
// Static functions declarations
// =============================================================================

int lcdWriteStdWrapper(char character, FILE *stream);
void reportError(error_e error);

// =============================================================================
// Class constructors
// =============================================================================

Lcd::Lcd(void)
{
	// Reset data members
	this->_resetDataMembers();

	// Standard input/output stream
	fdev_setup_stream(&lcdStream, lcdWriteStdWrapper, NULL, _FDEV_SETUP_WRITE);

	// Return successfull
	this->_lastError = ERROR_NONE;
	return;
}

Lcd::Lcd(lcdInterfaceMode_e mode)
{
	// Reset data members
	this->_resetDataMembers();

	// Standard input/output stream
	fdev_setup_stream(&lcdStream, lcdWriteStdWrapper, NULL, _FDEV_SETUP_WRITE);


	// Check for errors
	switch (mode) {
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		this->_interfaceMode = mode;
		break;
	case LCD_INTERFACE_UNDEFINED:
	default:
		this->_lastError = ERROR_INTERFACE_MODE_UNSUPPORTED;
		return;
	}

	// Return successfull
	this->_lastError = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool Lcd::deinitialize(void)
{
	// Deactivates USART interface
	this->_initialized = false;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::initialize(void)
{
	uint8_t command = 0;

	// -------------------------------------------------------------------------
	// Deinitialization procedure ----------------------------------------------

	// Must deinitialize first
	if (this->_initialized) {
		if (!this->deinitialize()) {
			this->_lastError = ERROR_UNKNOWN;
			return false;
		}
	}

	// -------------------------------------------------------------------------
	// Checking for errors -----------------------------------------------------

	// Interface mode unsupported
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		break;
	default:
		this->_lastError = ERROR_INTERFACE_MODE_UNSUPPORTED;
		return false;
		break;
	}
	// Control port not configured
	if (!this->_controlPortSet) {
		this->_lastError = ERROR_LCD_CONTROL_PORT_UNDEFINED;
		return false;
	}
	// Data port not configured
	if (!this->_dataPortSet) {
		this->_lastError = ERROR_LCD_DATA_PORT_UNDEFINED;
		return false;
	}
	// Display size unsupported
	switch (this->_displaySize) {
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
		return false;
		this->_lastError = ERROR_LCD_DISPLAY_SIZE_UNSUPPORTED;
		break;
	}

	// -------------------------------------------------------------------------
	// Initialization procedure ------------------------------------------------

	// Hold gpio control
	if (!this->_holdGpio()) {
		return false;
	}

	// Reset the controller, must be sent 3 times
	_delay_ms(15);
	this->_sendCommandForce8Bits(LCD_FUNCTION_RESET);
	_delay_ms(5);
	this->_sendCommandForce8Bits(LCD_FUNCTION_RESET);
	_delay_us(100);
	this->_sendCommandForce8Bits(LCD_FUNCTION_RESET);
	_delay_us(100);
	this->_busyFlagAvailable = true;

	// Configures the LCD to 4 bits interface
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
		this->_sendCommandForce8Bits(LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS);
		_delay_us(100);
		break;
	default:
		break;
	}

	// Function set
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
		command = LCD_FUNCTION_SET | LCD_FUNCTION_4_BITS;
		break;
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		command = LCD_FUNCTION_SET | LCD_FUNCTION_8_BITS;
		break;
	default:
		break;
	}
	command |= (this->_fontSize == LCD_FONT_SIZE_5X8) ? LCD_FUNCTION_5x8_FONT : LCD_FUNCTION_5x10_FONT;
	command |= (this->_displaySize < 200) ? LCD_FUNCTION_1_LINE : LCD_FUNCTION_2_LINES;
	this->_sendCommand(command);
	this->_waitUntilReady();
	// Turn display off
	command = (1 << _OFFSET_CONFIG_STATES) | (0 << _OFFSET_DISPLAY_ON);
	this->_sendCommand(command);
	this->_waitUntilReady();
	// Clear screen
	this->_sendCommand(LCD_CLEAR_SCREEN);
	// Set entry mode
	command = LCD_ENTRY_SET;
	switch (this->_entryMode) {
	case LCD_LEFT_OVERWRITE:	// 00
		command |= LCD_ENTRY_DECREMENT | LCD_ENTRY_OVERWRITE;
		break;
	case LCD_RIGHT_SHIFT:		// 01
		command |= LCD_ENTRY_DECREMENT | LCD_ENTRY_SHIFT;
		break;
	case LCD_RIGHT_OVERWRITE:	// 10
		command |= LCD_ENTRY_INCREMENT | LCD_ENTRY_OVERWRITE;
		break;
	case LCD_LEFT_SHIFT:		// 11
		command |= LCD_ENTRY_INCREMENT | LCD_ENTRY_SHIFT;
		break;
	default:
		break;
	}
	this->_waitUntilReady();
	this->_sendCommand(command);
	this->_waitUntilReady();
	this->_sendCommand(LCD_DISPLAY_ON);

	// Update data members
	this->_cursorColumn = 0;
	this->_cursorLine = 0;
	this->_cursorState = LCD_CURSOR_OFF;
	this->_displayLines = (this->_displaySize / 100) - 1;
	this->_displayColumns = (this->_displaySize % 100) - 1;
	this->_displayState = STATE_ON;
	this->_initialized = true;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

error_e Lcd::getLastError(void)
{
	return this->_lastError;
}

bool Lcd::setControlPort(vuint8_t * controlReg, uint8_t pinEn, uint8_t pinRs, uint8_t pinRw)
{
	// Check for errors - Invalid controlReg
	if (controlReg == NULL) {
		this->_lastError = ERROR_REGISTER_ADDRESS_INVALID;
		return false;
	}
	// Check for errors - Invalid pinEn
	if (pinEn > 7) {
		this->_lastError = ERROR_LCD_CONTROL_EN_PIN_INVALID;
		return false;
	}
	// Check for errors - Invalid pinRs
	if (pinRs > 7) {
		this->_lastError = ERROR_LCD_CONTROL_RS_PIN_INVALID;
		return false;
	}
	// Check for errors - Invalid pinRw
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_DELAYED:
		if (pinRw != 0xFF) {			// Try to use pinRw in delayed mode
			this->_lastError = ERROR_ARGUMENTS_MISMATCH;
			return false;
		}
		break;
	case LCD_INTERFACE_8_BITS_FLAGGED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
		if (pinRw == 0xFF) {			// Try to not use pinRw in flagged mode
			this->_lastError = ERROR_ARGUMENTS_MISMATCH;
			return false;
		} else if (pinRw > 7) {			// Try to use invalid pinRw number
			this->_lastError = ERROR_LCD_CONTROL_RW_PIN_INVALID;
			return false;
		}
		break;
	default:
		this->_lastError = ERROR_INTERFACE_MODE_UNSUPPORTED;
		return false;
	}
	// Check for errors - Pins not unique
	if ((pinEn == pinRs) || (pinEn == pinRw) || (pinRs == pinRw)) {
		this->_lastError = ERROR_LCD_CONTROL_PIN_CONTENTION;
		return false;
	}

	// Decode port address
	controlReg -= __SFR_OFFSET;
	switch ((uint16_t)controlReg) {
	case 0x00 ... 0x02:		// GPIO_A
		controlReg = (vuint8_t *)(0x00 + __SFR_OFFSET);
		break;
	case 0x03 ... 0x05:		// GPIO_B
		controlReg = (vuint8_t *)(0x03 + __SFR_OFFSET);
		break;
	case 0x06 ... 0x08:		// GPIO_C
		controlReg = (vuint8_t *)(0x06 + __SFR_OFFSET);
		break;
	case 0x09 ... 0x0B:		// GPIO_D
		controlReg = (vuint8_t *)(0x09 + __SFR_OFFSET);
		break;
	case 0x0C ... 0x0E:		// GPIO_E
		controlReg = (vuint8_t *)(0x0C + __SFR_OFFSET);
		break;
	case 0x0F ... 0x11:		// GPIO_F
		controlReg = (vuint8_t *)(0x0E + __SFR_OFFSET);
		break;
	case 0x12 ... 0x14:		// GPIO_G
		controlReg = (vuint8_t *)(0x12 + __SFR_OFFSET);
		break;
	case 0x15 ... 0x17:		// GPIO_H
		controlReg = (vuint8_t *)(0x15 + __SFR_OFFSET);
		break;
	case 0x18 ... 0x1A:		// GPIO_J
		controlReg = (vuint8_t *)(0x18 + __SFR_OFFSET);
		break;
	default:
		this->_lastError = ERROR_REGISTER_ADDRESS_INVALID;
		return false;
	}

	// Update data members
	this->_controlDdr = controlReg + GPIO_DDDR_REG_OFFSET;
	this->_controlPort = controlReg + GPIO_PORT_REG_OFFSET;
	this->_controlEn = pinEn;
	this->_controlRs = pinRs;
	if (this->_controlRw <= 7) {
		this->_controlRw = pinRw;
	} else {
		this->_controlRw = 0;
	}

	// Return successfull
	this->_controlPortSet = true;
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setDataPort(vuint8_t * dataReg, uint8_t firstBit)
{
	// Check for errors - Invalid dataReg
	if (dataReg == NULL) {
		this->_lastError = ERROR_REGISTER_ADDRESS_INVALID;
		return false;
	}
	// Check for errors - Invalid firstBit
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
		if (firstBit == 0xFF) {			// Try to not use firstBit in 4 bits mode
			this->_lastError = ERROR_ARGUMENTS_MISMATCH;
			return false;
		} else if (firstBit > 0x04) {	// Fist bit is invalid
			this->_lastError = ERROR_LCD_DATA_FIRST_PIN_INVALID;
			return false;
		}
		break;
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		if (firstBit != 0xFF) {			// Try to use fistBit in 8 bits mode
			this->_lastError = ERROR_ARGUMENTS_MISMATCH;
			return false;
		}
		break;
	default:
		this->_lastError = ERROR_INTERFACE_MODE_UNSUPPORTED;
		return false;
	}

	// Decode port address
	dataReg -= __SFR_OFFSET;
	switch ((uint16_t)dataReg) {
	case 0x00 ... 0x02:		// GPIO_A
		dataReg = (vuint8_t *)(0x00 + __SFR_OFFSET);
		break;
	case 0x03 ... 0x05:		// GPIO_B
		dataReg = (vuint8_t *)(0x03 + __SFR_OFFSET);
		break;
	case 0x06 ... 0x08:		// GPIO_C
		dataReg = (vuint8_t *)(0x06 + __SFR_OFFSET);
		break;
	case 0x09 ... 0x0B:		// GPIO_D
		dataReg = (vuint8_t *)(0x09 + __SFR_OFFSET);
		break;
	case 0x0C ... 0x0E:		// GPIO_E
		dataReg = (vuint8_t *)(0x0C + __SFR_OFFSET);
		break;
	case 0x0F ... 0x11:		// GPIO_F
		dataReg = (vuint8_t *)(0x0E + __SFR_OFFSET);
		break;
	case 0x12 ... 0x14:		// GPIO_G
		dataReg = (vuint8_t *)(0x12 + __SFR_OFFSET);
		break;
	case 0x15 ... 0x17:		// GPIO_H
		dataReg = (vuint8_t *)(0x15 + __SFR_OFFSET);
		break;
	case 0x18 ... 0x1A:		// GPIO_J
		dataReg = (vuint8_t *)(0x18 + __SFR_OFFSET);
		break;
	default:
		this->_lastError = ERROR_REGISTER_ADDRESS_INVALID;
		return false;
	}

	// Update data members
	this->_dataPin = dataReg + GPIO_PIN_REG_OFFSET;
	this->_dataDdr = dataReg + GPIO_DDDR_REG_OFFSET;
	this->_dataPort = dataReg + GPIO_PORT_REG_OFFSET;
	if (firstBit <= 4) {
		this->_dataFirst = firstBit;
	} else {
		this->_dataFirst = 0;
	}

	// Return successfull
	this->_dataPortSet = true;
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setDisplaySize(lcdDisplaySize_e displaySize)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setEntryMode(lcdEntryMode_e entryMode)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setFontSize(lcdFontSize_e fontSize)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setBacklightState(stateOffOn_t offOn)
{
	// TO DO

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::clearScreen(void)
{
	this->_waitUntilReady();
	this->_sendCommand(LCD_CLEAR_SCREEN);
	this->_cursorLine = 0;
	this->_cursorColumn = 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::moveCursor(direction_t dir)
{
	uint8_t command = ((1 << _OFFSET_CONFIG_SHIFTS) | (0 << _OFFSET_1_DISPLAY_0_CURSOR));

	// Decode direction
	switch (dir) {
	case DIRECTION_LEFT:
		command |= (0 << _OFFSET_1_RIGHT_0_LEFT);
		if (this->_cursorColumn != 0) {
			this->_cursorColumn--;
			this->_waitUntilReady();
			if (!this->_sendCommand(command)) {
				return false;
			}
		}
		break;
	case DIRECTION_RIGHT:
		command |= (1 << _OFFSET_1_RIGHT_0_LEFT);
		if (this->_cursorColumn != 39) {
			this->_cursorColumn++;
			this->_waitUntilReady();
			if (!this->_sendCommand(command)) {
				return false;
			}
		}
		break;
	default:
		this->_lastError = ERROR_DIRECTION_INVALID;
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::moveCursor(uint8_t line, uint8_t column)
{
	uint8_t address = 0xFF;

	if (line > 0) {
		line--;
	}
	if (column > 0) {
		column--;
	}

	// Decode line
	switch (line) {
	case 0:		// Go to line 0
		address = column;
		break;
	case 1:		// Go to line 1
		address = (this->_displayLines >= 1) ? (0x40 + column) : 0xFF;
		break;
	case 2:		// Go to line 2
		if ((this->_displayLines == 3) && (this->_displayColumns == 11)) {				// Display 12x4
			address = 0x0C + column;
		} else if ((this->_displayLines == 3) && (this->_displayColumns == 15)) {		// Display 16x4
			address = 0x10 + column;
		} else if ((this->_displayLines == 3) && (this->_displayColumns == 19)) {		// Display 20x4
			address = 0x14 + column;
		}
		break;
	case 3:		// Go to line 3
		if ((this->_displayLines == 3) && (this->_displayColumns == 11)) {				// Display 12x4
			address = 0x4C + column;
		} else if ((this->_displayLines == 3) && (this->_displayColumns == 15)) {		// Display 16x4
			address = 0x50 + column;
		} else if ((this->_displayLines == 3) && (this->_displayColumns == 19)) {		// Display 20x4
			address = 0x54 + column;
		}
		break;
	}

	if (address == 0xFF) {
		this->_lastError = ERROR_COORDINATE_INVALID;
		return false;
	}

	// Make changes
	this->_waitUntilReady();
	if (!this->_sendCommand((1 << _OFFSET_DDRAM_ADDRESS) | address)) {
		return false;
	}

	// Update data members
	this->_cursorLine = line;
	this->_cursorColumn = column;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::moveCursorHome(void)
{
	// Make changes
	this->_waitUntilReady();
	if (!this->_sendCommand(LCD_CURSOR_HOME)) {
		return false;
	}

	// Update data members
	this->_cursorLine = 0;
	this->_cursorColumn = 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::moveCursorNextLine(void)
{
	uint8_t currentLine = (this->_cursorLine + 1);

	currentLine %= (this->_displayLines + 1);
	return this->moveCursor(currentLine + 1, 1);
}

bool Lcd::setCursorState(lcdCursorState_e cursorState)
{
	// Update data members
	this->_cursorState = cursorState;

	// Make changes
	if (!this->_configCursorDisplayState()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::setDisplayState(stateOffOn_t offOn)
{
	// Update data members
	this->_displayState = offOn;

	// Make changes
	if (!this->_configCursorDisplayState()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::shiftDisplay(direction_t dir)
{
	uint8_t command = ((1 << _OFFSET_CONFIG_SHIFTS) | (1 << _OFFSET_1_DISPLAY_0_CURSOR));

	// Decode direction
	switch (dir) {
	case DIRECTION_LEFT:
		command |= (0 << _OFFSET_1_RIGHT_0_LEFT);
		this->_waitUntilReady();
		if (!this->_sendCommand(command)) {
			return false;
		}
		break;
	case DIRECTION_RIGHT:
		command |= (1 << _OFFSET_1_RIGHT_0_LEFT);
		this->_waitUntilReady();
		if (!this->_sendCommand(command)) {
			return false;
		}
		break;
	default:
		this->_lastError = ERROR_DIRECTION_INVALID;
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

int16_t Lcd::writeCharacterStd(char character, FILE * stream)
{
	uint8_t i = 0;

	if (character == '\n') {
		for (i = this->_cursorColumn; i < (this->_displayColumns + 1); i++) {
			this->_sendCharacter(' ');
		}
		this->moveCursorNextLine();
	} else {
		this->_sendCharacter(character);
	}

	return 0;
}

bool Lcd::sendCharacter(uint8_t character)
{
	if (this->_cursorColumn < 40) {		// TO DO - VERIFY THIS CONDITION
		this->_waitUntilReady();
		// LCD in write mode
		switch (this->_interfaceMode) {
		case LCD_INTERFACE_4_BITS_FLAGGED:
		case LCD_INTERFACE_8_BITS_FLAGGED:
			clrBit(*(this->_controlPort), this->_controlRw);
			break;
		default:
			break;
		}
		// LCD in text mode
		setBit(*(this->_controlPort), this->_controlRs);
		// Makes sure enable is LOW
		clrBit(*(this->_controlPort), this->_controlEn);
		// Prepare data
		switch (this->_interfaceMode) {
		case LCD_INTERFACE_8_BITS_DELAYED:
		case LCD_INTERFACE_8_BITS_FLAGGED:
			// Writes the whole byte
			*(this->_dataPort) = character;
			break;
		case LCD_INTERFACE_4_BITS_DELAYED:
		case LCD_INTERFACE_4_BITS_FLAGGED:
			// Writes the higher nibble
			clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
			*(this->_dataPort) |= ((character >> 4) << this->_dataFirst);
			// Pulse enable HIGH
			_delay_us(1);
			setBit(*(this->_controlPort), this->_controlEn);
			_delay_us(1);
			clrBit(*(this->_controlPort), this->_controlEn);
			// Writes the lower nibble
			clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
			*(this->_dataPort) |= ((character & 0x0F) << this->_dataFirst);
			break;
		default:
			break;
		}
		// Pulse enable HIGH
		_delay_us(1);
		setBit(*(this->_controlPort), this->_controlEn);
		_delay_us(1);
		clrBit(*(this->_controlPort), this->_controlEn);
		// Wait until ready
		_delay_us(40);
		this->_cursorColumn++;
	}

	return true;
}

void Lcd::remapStdio(void)
{
	stdin = stdout = stderr = &lcdStream;
	lcdDefaultHandler = this;
	return;
}

// =============================================================================
// Class private methods
// =============================================================================

void Lcd::_resetDataMembers(void)
{
	// Default Data Port for LCD Keypad Shield
	this->_interfaceMode				= LCD_INTERFACE_4_BITS_DELAYED;
	this->_displaySize					= LCD_DISPLAY_SIZE_16X2;
	this->_initialized 					= false;
#if PORT_B_EXISTS && PORT_D_EXISTS
	this->_controlDdr					= &DDRB;
	this->_controlEn					= PB1;
	this->_controlPort					= &PORTB;
	this->_controlPortSet				= true;
	this->_controlRs					= PB0;
	this->_controlRw					= 0;
	this->_dataDdr						= &DDRD;
	this->_dataFirst					= PD4;
	this->_dataPin						= &PIND;
	this->_dataPort						= &PORTD;
	this->_dataPortSet					= true;
#else
	this->_controlDdr					= NULL;
	this->_controlEn					= 0;
	this->_controlPort					= NULL;
	this->_controlPortSet				= false;
	this->_controlRs					= 0;
	this->_controlRw					= 0;
	this->_dataDdr						= NULL;
	this->_dataFirst					= 0;
	this->_dataPin						= NULL;
	this->_dataPort						= NULL;
	this->_dataPortSet					= false;
#endif
	this->_backlightState				= STATE_OFF;
	this->_cursorColumn					= 0;
	this->_cursorLine					= 0;
	this->_cursorState					= LCD_CURSOR_OFF;
	this->_displayColumns				= 0;
	this->_displayLines					= 0;
	this->_displayState					= STATE_OFF;
	this->_entryMode					= LCD_RIGHT_OVERWRITE;
	this->_fontSize						= LCD_FONT_SIZE_5X8;

	// Return successfully
	this->_lastError					= ERROR_NONE;
	return;
}

bool Lcd::_holdGpio(void)
{
	// Check for errors
	if (!this->_dataPortSet) {			// Data port not set
		this->_lastError = ERROR_LCD_DATA_PORT_UNDEFINED;
		return false;
	}
	if (!this->_controlPortSet) {		// Control port not set
		this->_lastError = ERROR_LCD_CONTROL_PORT_UNDEFINED;
		return false;
	}

	// Hold data port
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:			// Use 8 bits from port
		clrMask(*(this->_dataPort), 0xFF, 0);
		setMask(*(this->_dataDdr), 0xFF, 0);
		break;
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:			// Use 4 bits from port
		clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
		setMask(*(this->_dataDdr), 0x0F, this->_dataFirst);
		break;
	default:
		this->_lastError = ERROR_INTERFACE_MODE_UNSUPPORTED;
		return false;
	}

	// Hold control port
	clrBit(*(this->_controlPort), this->_controlEn);	// EN pin
	setBit(*(this->_controlDdr), this->_controlEn);
	clrBit(*(this->_controlPort), this->_controlRs);	// RS pin
	setBit(*(this->_controlDdr), this->_controlRs);
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_FLAGGED:			// Use flagged mode
	case LCD_INTERFACE_8_BITS_FLAGGED:
		clrBit(*(this->_controlPort), this->_controlRw);	// EN pin
		setBit(*(this->_controlDdr), this->_controlRw);
		break;
	default:
		break;
	}

	// Return successfully
	this->_holdingGpio = true;
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::_releaseGpio(void)
{
	// Nothing to do, just leave
	if (!this->_holdingGpio) {
		this->_lastError = ERROR_NONE;
		return true;
	}

	// Check for errors
	if (!this->_dataPortSet) {			// Data port not set
		this->_lastError = ERROR_LCD_DATA_PORT_UNDEFINED;
		return false;
	}
	if (!this->_controlPortSet) {		// Control port not set
		this->_lastError = ERROR_LCD_CONTROL_PORT_UNDEFINED;
		return false;
	}

	// Release data port
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:			// Use 8 bits from port
		clrMask(*(this->_dataPort), 0xFF, 0);
		clrMask(*(this->_dataDdr), 0xFF, 0);
		break;
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:			// Use 4 bits from port
		clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
		clrMask(*(this->_dataDdr), 0x0F, this->_dataFirst);
		break;
	default:
		this->_lastError = ERROR_INTERFACE_MODE_UNSUPPORTED;
		return false;
	}

	// Configure control port
	clrBit(*(this->_controlPort), this->_controlEn);	// EN pin
	clrBit(*(this->_controlDdr), this->_controlEn);
	clrBit(*(this->_controlPort), this->_controlRs);	// RS pin
	clrBit(*(this->_controlDdr), this->_controlRs);
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_FLAGGED:			// Use flagged mode
	case LCD_INTERFACE_8_BITS_FLAGGED:
		clrBit(*(this->_controlPort), this->_controlRw);	// EN pin
		clrBit(*(this->_controlDdr), this->_controlRw);
		break;
	default:
		break;
	}

	// Return successfully
	this->_holdingGpio = false;
	this->_lastError = ERROR_NONE;
	return true;
}

bool Lcd::_sendCommandForce8Bits(uint8_t command)
{
	// LCD in write mode
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_FLAGGED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		clrBit(*(this->_controlPort), this->_controlRw);
		break;
	default:
		break;
	}
	// LCD in command mode
	clrBit(*(this->_controlPort), this->_controlRs);
	// Makes sure enable is LOW
	clrBit(*(this->_controlPort), this->_controlEn);
	// Prepare data
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		*(this->_dataPort) = command;
		break;
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
		clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
		*(this->_dataPort) |= ((command >> 4) << this->_dataFirst);
		break;
	default:
		break;
	}
	// Pulse enable HIGH
	setBit(*(this->_controlPort), this->_controlEn);
	_delay_us(1);
	clrBit(*(this->_controlPort), this->_controlEn);
	_delay_us(1);

	return true;
}

bool Lcd::_sendCommand(uint8_t command)
{
	// LCD in write mode
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_4_BITS_FLAGGED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		clrBit(*(this->_controlPort), this->_controlRw);
		break;
	default:
		break;
	}
	// LCD in command mode
	clrBit(*(this->_controlPort), this->_controlRs);
	// Makes sure enable is LOW
	clrBit(*(this->_controlPort), this->_controlEn);
	// Prepare data
	switch (this->_interfaceMode) {
	case LCD_INTERFACE_8_BITS_DELAYED:
	case LCD_INTERFACE_8_BITS_FLAGGED:
		// Writes the whole byte
		*(this->_dataPort) = command;
		break;
	case LCD_INTERFACE_4_BITS_DELAYED:
	case LCD_INTERFACE_4_BITS_FLAGGED:
		// Writes the higher nibble
		clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
		*(this->_dataPort) |= ((command >> 4) << this->_dataFirst);
		// Pulse enable HIGH
		_delay_us(1);																		// ONLY - 4 BITS MODE
		setBit(*(this->_controlPort), this->_controlEn);
		_delay_us(1);
		clrBit(*(this->_controlPort), this->_controlEn);
		// Writes the lower nibble
		clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
		*(this->_dataPort) |= ((command & 0x0F) << this->_dataFirst);
		break;
	default:
		break;
	}
	// Pulse enable HIGH
	_delay_us(1);
	setBit(*(this->_controlPort), this->_controlEn);
	_delay_us(1);
	clrBit(*(this->_controlPort), this->_controlEn);
	// Wait until ready
	if ((command == LCD_CLEAR_SCREEN) || (command == LCD_CURSOR_HOME)) {
		_delay_ms(2);
	} else {
		_delay_us(40);
	}

	return true;
}

bool Lcd::_sendCharacter(uint8_t character)
{
	if (this->_cursorColumn < 40) {		// TO DO - VERIFY THIS CONDITION
		this->_waitUntilReady();
		// LCD in write mode
		switch (this->_interfaceMode) {
		case LCD_INTERFACE_4_BITS_FLAGGED:
		case LCD_INTERFACE_8_BITS_FLAGGED:
			clrBit(*(this->_controlPort), this->_controlRw);
			break;
		default:
			break;
		}
		// LCD in text mode
		setBit(*(this->_controlPort), this->_controlRs);
		// Makes sure enable is LOW
		clrBit(*(this->_controlPort), this->_controlEn);
		// Prepare data
		switch (this->_interfaceMode) {
		case LCD_INTERFACE_8_BITS_DELAYED:
		case LCD_INTERFACE_8_BITS_FLAGGED:
			// Writes the whole byte
			*(this->_dataPort) = character;
			break;
		case LCD_INTERFACE_4_BITS_DELAYED:
		case LCD_INTERFACE_4_BITS_FLAGGED:
			// Writes the higher nibble
			clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
			*(this->_dataPort) |= ((character >> 4) << this->_dataFirst);
			// Pulse enable HIGH
			_delay_us(1);
			setBit(*(this->_controlPort), this->_controlEn);
			_delay_us(1);
			clrBit(*(this->_controlPort), this->_controlEn);
			// Writes the lower nibble
			clrMask(*(this->_dataPort), 0x0F, this->_dataFirst);
			*(this->_dataPort) |= ((character & 0x0F) << this->_dataFirst);
			break;
		default:
			break;
		}
		// Pulse enable HIGH
		_delay_us(1);
		setBit(*(this->_controlPort), this->_controlEn);
		_delay_us(1);
		clrBit(*(this->_controlPort), this->_controlEn);
		// Wait until ready
		_delay_us(40);
		this->_cursorColumn++;
	}

	return true;
}

bool Lcd::_configCursorDisplayState(void)
{
	uint8_t command = (1 << _OFFSET_CONFIG_STATES);

	// Decode display state
	command |= (this->_displayState << _OFFSET_DISPLAY_ON);
	// Decode cursorState
	switch (this->_cursorState) {
	case LCD_CURSOR_OFF:
		command |= (0 << _OFFSET_CURSOR_ON) | (0 << _OFFSET_BLINK_ON);
		break;
	case LCD_CURSOR_ON:
		command |= (1 << _OFFSET_CURSOR_ON) | (0 << _OFFSET_BLINK_ON);
		break;
	case LCD_CURSOR_BLINK:
		command |= (1 << _OFFSET_CURSOR_ON) | (1 << _OFFSET_BLINK_ON);
		break;
	}

	// Send command
	this->_waitUntilReady();
	this->_sendCommand(command);

	return true;
}

bool Lcd::_waitUntilReady(void)
{
	return true;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

int lcdWriteStdWrapper(char data, FILE *stream)
{
	return lcdDefaultHandler->writeCharacterStd(data, stream);
}



uint8_t Lcd::lcdGetLine(void)
{
	return this->_cursorLine;
}
uint8_t Lcd::lcdGetColumn(void)
{
	return this->_cursorColumn;
}
