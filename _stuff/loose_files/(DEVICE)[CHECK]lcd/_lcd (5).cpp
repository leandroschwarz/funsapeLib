/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		lcd.cpp
 * Module:			MAX30102 module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "lcd.hpp"
#ifndef __LCD_HPP
#	error	[lcd.cpp] Error 1 - Header file (lcd.hpp) is missing or corrupted!
#elif __LCD_HPP != __BUILD_TO_REPLACE__
#	error	[lcd.cpp] Error 13 - Build mismatch between source (lcd.cpp) and header (lcd.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define LCD_CLEAR_SCREEN				0x01
#define LCD_CURSOR_HOME					0x02
#define LCD_ENTRY_MODE_SET				0x04
#	define LCD_ENTRY_INCREMENT			(1 << 1)
#	define LCD_ENTRY_DECREMENT			(0 << 1)
#	define LCD_ENTRY_SHIFT				(1 << 0)
#	define LCD_ENTRY_OVERWRITE			(0 << 0)
#define LCD_DISPLAY_CURSOR_CONFIG		0x08
#	define LCD_DISPLAY_ON				(1 << 2)
#	define LCD_DISPLAY_OFF				(0 << 2)
#	define LCD_CURSOR_ON				(1 << 1)
#	define LCD_CURSOR_OFF				(0 << 1)
#	define LCD_CURSOR_BLINK_ON			(1 << 0)
#	define LCD_CURSOR_BLINK_OFF			(0 << 0)
#define LCD_DISPLAY_CURSOR_MOVE			0x10
#	define LCD_DISPLAY_SHIFT			(1 << 3)
#	define LCD_CURSOR_MOVE				(0 << 3)
#	define LCD_DIRECTION_RIGHT			(1 << 2)
#	define LCD_DIRECTION_LEFT			(0 << 2)
#define LCD_FUNCTION_SET				0x20
#	define LCD_INTERFACE_8_BITS			(1 << 4)
#	define LCD_INTERFACE_4_BITS			(0 << 4)
#	define LCD_DISPLAY_2_LINES			(1 << 3)
#	define LCD_DISPLAY_1_LINE			(0 << 3)
#	define LCD_FONT_5X10				(1 << 2)
#	define LCD_FONT_5X8					(0 << 2)
#define LCD_CGRAM_ADRESS				0x40
#define LCD_DDRAM_ADRESS				0x80

// Printf defines
#define PRINTF_NTOA_BUFFER_SIZE    32U
#define PRINTF_FTOA_BUFFER_SIZE    32U
#define PRINTF_SUPPORT_FLOAT
#define PRINTF_SUPPORT_EXPONENTIAL
#define PRINTF_DEFAULT_FLOAT_PRECISION  6U
#define PRINTF_MAX_FLOAT  1e9
#define PRINTF_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_PTRDIFF_T

#define FLAGS_ZEROPAD   (1U <<  0U)
#define FLAGS_LEFT      (1U <<  1U)
#define FLAGS_PLUS      (1U <<  2U)
#define FLAGS_SPACE     (1U <<  3U)
#define FLAGS_HASH      (1U <<  4U)
#define FLAGS_UPPERCASE (1U <<  5U)
#define FLAGS_CHAR      (1U <<  6U)
#define FLAGS_SHORT     (1U <<  7U)
#define FLAGS_LONG      (1U <<  8U)
#define FLAGS_LONG_LONG (1U <<  9U)
#define FLAGS_PRECISION (1U << 10U)
#define FLAGS_ADAPT_EXP (1U << 11U)

// import float.h for DBL_MAX
#if defined(PRINTF_SUPPORT_FLOAT)
#	include <float.h>
#endif

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

#define controlEHigh()	do {																\
        HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinE, GPIO_PIN_SET);	\
    } while(0)
#define controlELow()	do {																\
        HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinE, GPIO_PIN_RESET);	\
    } while(0)
#define controlRsHigh()	do {																\
        HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinRs, GPIO_PIN_SET);	\
    } while(0)
#define controlRsLow()	do {																\
        HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinRs, GPIO_PIN_RESET);	\
    } while(0)
#define controlRwHigh()	do {																\
        if(this->_gpioControlPinRw) {														\
            HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinRw, GPIO_PIN_SET);\
        }																					\
    } while(0)
#define controlRwLow()	do {																\
        if(this->_gpioControlPinRw) {														\
            HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinRw, GPIO_PIN_RESET);\
        }																					\
    } while(0)
#define pulseEnable()	do { 																\
        HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinE, GPIO_PIN_SET);	\
        delayMs(1);																			\
        HAL_GPIO_WritePin(this->_gpioControlPort, this->_gpioControlPinE, GPIO_PIN_RESET);	\
        delayMs(1);																			\
    } while(0)

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

static uint16_t _my_atoi(const char **str);
static inline uint16_t _my_strnlen_s(const char *str, size_t maxsize);

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief		Construct a new Lcd::Lcd object.
 * @details		Construct a new Lcd object and reset all class' internal data
 * 				members to their deafult values.
 */
Lcd::Lcd(void)
{
    // Mark passage for debugging purpose
    debugMark("Lcd::Lcd(void)", 1);

    // Reset all class members
    this->_clearMembers();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

//     //////////////////     DEVICE CONFIGURATION     //////////////////     //

/**
 * @brief		Initializes the device.
 * @details		Initializes the handler and communicates with the device to
 * 				check it's part ID.
 * @param[in]	displaySize_p
 * @param[in]	fontSize_p
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Lcd::init(DisplaySize displaySize_p, FontSize fontSize_p)
{
    // Local variables
    uint8_t auxReg = 0;

    // Mark passage for debugging purpose
    debugMark("Lcd::init(DisplaySize, FontSize)", 1);

    // Check if data and control ports are set

    // Reset class members
    this->_initialized					= false;
    this->_lastError					= Error::NONE;

    // Set control lines to default value
    controlELow();
    controlRsLow();
    controlRwLow();

    // LCD initialization procedure
    delayMs(15);
    if (this->_dataPortWide4Bits) {
        // First, must configure the data bus to 8 bits wide for three times
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_8_BITS, true);
        delayMs(5);
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_8_BITS, true);
        delayMs(1);
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_8_BITS, true);
        // Now, configure the data bus to 4 bits wide
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_4_BITS, true);
    } else {
        // Just configure the data bus to 8 bits wide, must be sent three times
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_8_BITS);
        delayMs(5);
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_8_BITS);
        delayMs(1);
        this->_sendCommand(LCD_FUNCTION_SET | LCD_INTERFACE_8_BITS);
    }

    // Configure display size and font type
    switch (displaySize_p) {			// Number of lines on the display
    case DisplaySize::DISPLAY_8X1:
    case DisplaySize::DISPLAY_16X1:
    case DisplaySize::DISPLAY_20X1:
    case DisplaySize::DISPLAY_24X1:
    case DisplaySize::DISPLAY_40X1:
        this->_displaySizeLines = 1;
        break;
    case DisplaySize::DISPLAY_8X2:
    case DisplaySize::DISPLAY_10x2:
    case DisplaySize::DISPLAY_12X2:
    case DisplaySize::DISPLAY_12X4:
    case DisplaySize::DISPLAY_16X2:
    case DisplaySize::DISPLAY_16X4:
    case DisplaySize::DISPLAY_20X2:
    case DisplaySize::DISPLAY_20X4:
    case DisplaySize::DISPLAY_24X2:
    case DisplaySize::DISPLAY_30X2:
    case DisplaySize::DISPLAY_32X2:
    case DisplaySize::DISPLAY_40X2:
        this->_displaySizeLines = 2;
        break;
    default:
        // Returns error
        this->_displaySizeLines = 0;
        this->_lastError = Error::DEVICE_UNSUPPORTED;
        debugMessage(Error::DEVICE_UNSUPPORTED);
        return false;
        break;
    }
    switch (displaySize_p) {			// Number of columns on the display
    case DisplaySize::DISPLAY_8X1:
    case DisplaySize::DISPLAY_8X2:
        this->_displaySizeColumns = 8;
        break;
    case DisplaySize::DISPLAY_10x2:
        this->_displaySizeColumns = 10;
        break;
    case DisplaySize::DISPLAY_12X2:
    case DisplaySize::DISPLAY_12X4:
        this->_displaySizeColumns = 12;
        break;
    case DisplaySize::DISPLAY_16X1:
    case DisplaySize::DISPLAY_16X2:
    case DisplaySize::DISPLAY_16X4:
        this->_displaySizeColumns = 16;
        break;
    case DisplaySize::DISPLAY_20X1:
    case DisplaySize::DISPLAY_20X2:
    case DisplaySize::DISPLAY_20X4:
        this->_displaySizeColumns = 20;
        break;
    case DisplaySize::DISPLAY_24X1:
    case DisplaySize::DISPLAY_24X2:
        this->_displaySizeColumns = 24;
        break;
    case DisplaySize::DISPLAY_30X2:
        this->_displaySizeColumns = 30;
        break;
    case DisplaySize::DISPLAY_32X2:
        this->_displaySizeColumns = 32;
        break;
    case DisplaySize::DISPLAY_40X1:
    case DisplaySize::DISPLAY_40X2:
        this->_displaySizeColumns = 40;
        break;
    default:
        // Returns error
        this->_displaySizeColumns = 0;
        this->_lastError = Error::DEVICE_UNSUPPORTED;
        debugMessage(Error::DEVICE_UNSUPPORTED);
        return false;
        break;
    }
    auxReg = LCD_FUNCTION_SET;
    if (this->_dataPortWide4Bits) {
        auxReg |= LCD_INTERFACE_4_BITS;
    } else {
        auxReg |= LCD_INTERFACE_8_BITS;
    }
    if (this->_displaySizeLines == 1) {
        auxReg |= LCD_DISPLAY_1_LINE;
    } else {
        auxReg |= LCD_DISPLAY_2_LINES;
    }
    if (fontSize_p == FontSize::FONT_5X8) {
        auxReg |= LCD_FONT_5X8;
    } else {
        auxReg |= LCD_FONT_5X10;
    }
    this->_sendCommand(auxReg);
    this->_displaySize = displaySize_p;
    this->_fontSize = fontSize_p;

    // Turns display off and clears screen
    this->_sendCommand(LCD_DISPLAY_CURSOR_CONFIG | LCD_DISPLAY_OFF |
            LCD_CURSOR_OFF | LCD_CURSOR_BLINK_OFF);		// Turns display off
    this->_sendCommand(LCD_CLEAR_SCREEN);				// Clears screen
    this->_displayState = State::OFF;
    this->_cursorState = CursorState::HIDING;
    this->_cursorPositionColumn = 0;
    this->_cursorPositionLine = 0;

    // Configure entry mode to RIGHT_OVERWRITE
    this->_sendCommand(LCD_ENTRY_MODE_SET | LCD_ENTRY_INCREMENT | LCD_ENTRY_OVERWRITE);
    this->_entryMode = EntryMode::RIGHT_OVERWRITE;

    // Turns display on and cursor off

    this->_sendCommand(LCD_DISPLAY_CURSOR_CONFIG | LCD_DISPLAY_ON |
            LCD_CURSOR_OFF | LCD_CURSOR_BLINK_OFF);
    this->_displayState = State::ON;
    this->_cursorState = CursorState::HIDING;

    // Update class members
    this->_initialized					= true;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param gpioPort_p
 * @param pinE_p
 * @param pinRs_p
 * @param pinRw_p
 * @return true
 * @return false
 */
bool Lcd::setControlPort(GPIO_TypeDef *gpioPort_p, uint16_t pinE_p, uint16_t pinRs_p, uint16_t pinRw_p)
{
    // Mark passage for debugging purpose
    debugMark("setControlPort(GPIO_TypeDef *, uint16_t, uint16_t, uint16_t)", 1);

    // Check function arguments for errors
    if (!isPointerValid(gpioPort_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Update class members
    this->_gpioControlPort				= gpioPort_p;
    this->_gpioControlPinE				= pinE_p;
    this->_gpioControlPinRs				= pinRs_p;
    if (pinRw_p) {
        this->_useBusyFlag				= true;
        this->_gpioControlPinRw = pinRw_p;
    } else {
        this->_useBusyFlag				= false;
    }
    this->_controlPortSet				= true;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param gpioData_p
 * @param pinFist_p
 * @param interface4Bits_p
 * @return true
 * @return false
 */
bool Lcd::setDataPort(GPIO_TypeDef *gpioData_p, uint16_t pinFist_p, bool interface4Bits_p)
{
    // Local variables
    uint8_t auxPin = 0xFF;

    // Mark passage for debugging purpose
    debugMark("setDataPort(GPIO_TypeDef *, uint16_t, bool)", 1);

    // Check function arguments for errors
    if (!isPointerValid(gpioData_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    if (pinFist_p == 0x00) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        debugMessage(Error::ARGUMENT_VALUE_INVALID);
        return false;
    } else {
        auxPin = 0xFF;
        for (uint8_t i = 0; i < 16; i++)	{
            if (isBitSet(pinFist_p, i)) {
                auxPin = i;
                break;
            }
        }
        if (auxPin > 15) {
            // Returns error
            this->_lastError = Error::UNKNOWN;
            debugMessage(Error::UNKNOWN);
            return false;
        }
    }

    // Update class members
    this->_gpioDataPinFirstIndex		= auxPin;
    this->_gpioDataPort					= gpioData_p;
    this->_dataPortWide4Bits			= interface4Bits_p;
    this->_dataPortSet					= true;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param entryMode_p
 * @return true
 * @return false
 */
bool Lcd::setEntryMode(EntryMode entryMode_p)
{
    // Mark passage for debugging purpose
    debugMark("setEntryMode(EntryMode)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Send command
    switch (entryMode_p) {
    case EntryMode::LEFT_OVERWRITE:
        this->_sendCommand(LCD_ENTRY_MODE_SET | LCD_ENTRY_DECREMENT | LCD_ENTRY_OVERWRITE);
        break;
    case EntryMode::RIGHT_OVERWRITE:
        this->_sendCommand(LCD_ENTRY_MODE_SET | LCD_ENTRY_INCREMENT | LCD_ENTRY_OVERWRITE);
        break;
    case EntryMode::LEFT_SHIFT:
        this->_sendCommand(LCD_ENTRY_MODE_SET | LCD_ENTRY_INCREMENT | LCD_ENTRY_SHIFT);
        break;
    case EntryMode::RIGHT_SHIFT:
        this->_sendCommand(LCD_ENTRY_MODE_SET | LCD_ENTRY_DECREMENT | LCD_ENTRY_SHIFT);
        break;
    default:
        // Returns error
        this->_lastError = Error::MODE_UNSUPPORTED;
        debugMessage(Error::MODE_UNSUPPORTED);
        return false;
        break;
    }

    // Update class members
    this->_entryMode					= entryMode_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Lcd::clearScreen(void)
{
    // Mark passage for debugging purpose
    debugMark("clearScreen(void)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Send command
    this->_sendCommand(LCD_CLEAR_SCREEN);

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param displayState_p
 * @return true
 * @return false
 */
bool Lcd::turnDisplay(State displayState_p)
{
    // Local variables
    uint8_t auxReg = 0;

    // Mark passage for debugging purpose
    debugMark("turnDisplay(State)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Send command
    auxReg = LCD_DISPLAY_CURSOR_CONFIG;
    if (displayState_p == State::ON) {
        auxReg |= LCD_DISPLAY_ON;
    } else {
        auxReg |= LCD_DISPLAY_OFF;
    }
    switch (this->_cursorState) {
    case CursorState::HIDING:
        auxReg |= LCD_CURSOR_OFF;
        auxReg |= LCD_CURSOR_BLINK_OFF;
        break;
    case CursorState::SHOWING:
        auxReg |= LCD_CURSOR_ON;
        auxReg |= LCD_CURSOR_BLINK_OFF;
        break;
    case CursorState::BLINKING:
        auxReg |= LCD_CURSOR_ON;
        auxReg |= LCD_CURSOR_BLINK_ON;
        break;
    }
    this->_sendCommand(auxReg);

    // Update class members
    this->_displayState					= displayState_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param cursorState_p
 * @return true
 * @return false
 */
bool Lcd::turnCursor(CursorState cursorState_p)
{
    // Local variables
    uint8_t auxReg = 0;

    // Mark passage for debugging purpose
    debugMark("turnCursor(CursorState)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Send command
    auxReg = LCD_DISPLAY_CURSOR_CONFIG;
    if (this->_displayState == State::ON) {
        auxReg |= LCD_DISPLAY_ON;
    } else {
        auxReg |= LCD_DISPLAY_OFF;
    }
    switch (cursorState_p) {
    case CursorState::HIDING:
        auxReg |= LCD_CURSOR_OFF;
        auxReg |= LCD_CURSOR_BLINK_OFF;
        break;
    case CursorState::SHOWING:
        auxReg |= LCD_CURSOR_ON;
        auxReg |= LCD_CURSOR_BLINK_OFF;
        break;
    case CursorState::BLINKING:
        auxReg |= LCD_CURSOR_ON;
        auxReg |= LCD_CURSOR_BLINK_ON;
        break;
    }
    this->_sendCommand(auxReg);

    // Update class members
    this->_cursorState					= cursorState_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param cursorPosition_p
 * @return true
 * @return false
 */
bool Lcd::moveCursor(CursorPosition cursorPosition_p)
{
    // Mark passage for debugging purpose
    debugMark("moveCursor(CursorPosition)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Send command
    switch (cursorPosition_p) {
    case CursorPosition::HOME:
        this->_sendCommand(LCD_CURSOR_HOME);
        break;

    case CursorPosition::NEXT_LINE:
        if (this->_displaySizeLines == 1) {				// Display has only 1 line
            this->_sendCommand(LCD_CURSOR_HOME);
        } else if (this->_displaySizeLines == 2) {		// Display has 2 lines
            if (this->_cursorPositionLine == 0) {
                this->cursorGoTo(2, 1);
            } else {
                this->cursorGoTo(1, 1);
            }
        } else if (this->_displaySizeLines == 4) {		// Display has 4 lines
            if (this->_cursorPositionLine == 3) {
                this->cursorGoTo(1, 1);
            } else {
                this->cursorGoTo((this->_cursorPositionLine + 1), 0);
            }
        }
        break;

    case CursorPosition::FORWARD:
        this->_sendCommand(LCD_DISPLAY_CURSOR_MOVE | LCD_DIRECTION_RIGHT);
        break;

    case CursorPosition::BACKWARD:
        this->_sendCommand(LCD_DISPLAY_CURSOR_MOVE | LCD_DIRECTION_LEFT);
        break;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param cursorLine_p
 * @param cursorColumn_p
 * @return true
 * @return false
 */
bool Lcd::moveCursor(uint8_t cursorLine_p, uint8_t cursorColumn_p)
{
    // Mark passage for debugging purpose
    debugMark("moveCursor(uint8_t, uint8_t)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // TODO: Finish this

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param shiftDirection_p
 * @return true
 * @return false
 */
bool Lcd::shiftDisplay(ShiftDirection shiftDirection_p)
{
    // Mark passage for debugging purpose
    debugMark("shiftDisplay(ShiftDirection)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Send command
    switch (shiftDirection_p) {
    case ShiftDirection::LEFT:
        this->_sendCommand(LCD_DISPLAY_CURSOR_MOVE | LCD_DIRECTION_LEFT);
        break;

    case ShiftDirection::RIGHT:
        this->_sendCommand(LCD_DISPLAY_CURSOR_MOVE | LCD_DIRECTION_RIGHT);
        break;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param strData_p
 * @param strSize_p
 * @return true
 * @return false
 */
bool Lcd::print(const char *strData_p, uint16_t strSize_p)
{
    // Mark passage for debugging purpose
    debugMark("print(const char *, uint16_t)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(strData_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }
    if (!strSize_p) {
        // Returns error
        this->_lastError = Error::ARGUMENT_CANNOT_BE_ZERO;
        debugMessage(Error::ARGUMENT_CANNOT_BE_ZERO);
        return false;
    }

    // Print string
    for (uint16_t i = 0; i < strSize_p; i++) {
        this->_processCharacter(strData_p[i]);
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

/**
 * @brief
 *
 * @param strData_p
 * @param ...
 * @return true
 * @return false
 */
int Lcd::printf(const char *format, ...)
{
    // Local variables
    va_list va;
    char buffer[1];

    // Mark passage for debugging purpose
    debugMark("printf(const char *, ...)", 1);

    // Check if device is initialized
    if (!this->_isInitialized()) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(format)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    va_start(va, format);
    const int ret = this->_my_vsnprintf(buffer, (size_t) -1, format, va);
    va_end(va);
    return ret;
}

void Lcd::cursorGoTo(uint8_t line_p, uint8_t column_p)
{
    uint8_t address = 0x00;

    if (line_p == 1) {
        switch (this->_displaySize) {
        case DisplaySize::DISPLAY_8X1:
        case DisplaySize::DISPLAY_16X1:
        case DisplaySize::DISPLAY_20X1:
        case DisplaySize::DISPLAY_24X1:
        case DisplaySize::DISPLAY_40X1:
        case DisplaySize::DISPLAY_8X2:
        case DisplaySize::DISPLAY_10x2:
        case DisplaySize::DISPLAY_12X2:
        case DisplaySize::DISPLAY_16X2:
        case DisplaySize::DISPLAY_20X2:
        case DisplaySize::DISPLAY_24X2:
        case DisplaySize::DISPLAY_30X2:
        case DisplaySize::DISPLAY_32X2:
        case DisplaySize::DISPLAY_40X2:
        case DisplaySize::DISPLAY_12X4:
        case DisplaySize::DISPLAY_16X4:
        case DisplaySize::DISPLAY_20X4:
            address += (column_p - 1);
            break;
        default:
            // FIXME: Unsupported
            return;
        }
    } else if (line_p == 2) {
        switch (this->_displaySize) {
        case DisplaySize::DISPLAY_8X1:
        case DisplaySize::DISPLAY_16X1:
        case DisplaySize::DISPLAY_20X1:
        case DisplaySize::DISPLAY_24X1:
        case DisplaySize::DISPLAY_40X1:
            // FIXME: Unsupported
            return;
        case DisplaySize::DISPLAY_8X2:
        case DisplaySize::DISPLAY_10x2:
        case DisplaySize::DISPLAY_12X2:
        case DisplaySize::DISPLAY_16X2:
        case DisplaySize::DISPLAY_20X2:
        case DisplaySize::DISPLAY_24X2:
        case DisplaySize::DISPLAY_30X2:
        case DisplaySize::DISPLAY_32X2:
        case DisplaySize::DISPLAY_40X2:
        case DisplaySize::DISPLAY_12X4:
        case DisplaySize::DISPLAY_16X4:
        case DisplaySize::DISPLAY_20X4:
            address += (0x40 + column_p - 1);
            break;
        default:
            // FIXME: Unsupported
            return;
        }
    } else if (line_p == 3) {
        switch (this->_displaySize) {
        case DisplaySize::DISPLAY_8X1:
        case DisplaySize::DISPLAY_16X1:
        case DisplaySize::DISPLAY_20X1:
        case DisplaySize::DISPLAY_24X1:
        case DisplaySize::DISPLAY_40X1:
        case DisplaySize::DISPLAY_8X2:
        case DisplaySize::DISPLAY_10x2:
        case DisplaySize::DISPLAY_12X2:
        case DisplaySize::DISPLAY_16X2:
        case DisplaySize::DISPLAY_20X2:
        case DisplaySize::DISPLAY_24X2:
        case DisplaySize::DISPLAY_30X2:
        case DisplaySize::DISPLAY_32X2:
        case DisplaySize::DISPLAY_40X2:
            // FIXME: Unsupported
            return;
        case DisplaySize::DISPLAY_12X4:
            address += (0x0C + column_p - 1);
            break;
        case DisplaySize::DISPLAY_16X4:
            address += (0x10 + column_p - 1);
            break;
        case DisplaySize::DISPLAY_20X4:
            address += (0x14 + column_p - 1);
            break;
        default:
            // FIXME: Unsupported
            return;
        }
    } else {
        switch (this->_displaySize) {
        case DisplaySize::DISPLAY_8X1:
        case DisplaySize::DISPLAY_16X1:
        case DisplaySize::DISPLAY_20X1:
        case DisplaySize::DISPLAY_24X1:
        case DisplaySize::DISPLAY_40X1:
        case DisplaySize::DISPLAY_8X2:
        case DisplaySize::DISPLAY_10x2:
        case DisplaySize::DISPLAY_12X2:
        case DisplaySize::DISPLAY_16X2:
        case DisplaySize::DISPLAY_20X2:
        case DisplaySize::DISPLAY_24X2:
        case DisplaySize::DISPLAY_30X2:
        case DisplaySize::DISPLAY_32X2:
        case DisplaySize::DISPLAY_40X2:
            // FIXME: Unsupported
            return;
        case DisplaySize::DISPLAY_12X4:
            address += (0x4C + column_p - 1);
            break;
        case DisplaySize::DISPLAY_16X4:
            address += (0x50 + column_p - 1);
            break;
        case DisplaySize::DISPLAY_20X4:
            address += (0x54 + column_p - 1);
            break;
        default:
            // FIXME: Unsupported
            return;
        }
    }

    this->_sendCommand(LCD_DDRAM_ADRESS | address);
    this->_cursorPositionColumn = (column_p - 1);
    this->_cursorPositionLine = (line_p - 1);

    return;
}

//     //////////////////////    DEVICE STATUS     //////////////////////     //

/**
 * @brief		Returns the code of the last error.
 * @details		Returns the FunSAPE++ Embedded Library error code of the last
 * 				operation. If the operation was successfull, the return code
 * 				will be Error::NONE.
 * @return		Error	Error:: error code.
 */
Error Lcd::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ////////////////    CLASS CONTROL AND STATUS     ////////////////     //

/**
 * @brief		Checks if the device ports were configurated.
 * @details		Checks if the device control and data ports were configurated.
 * @note		This function is private, so before calling it, the
 * 				programmer must ensure that all parameters are checked.
 * @attention	This function only verifies the device ports configuration, it
 * 				does not verifies if the device is configured properly.
 * @return true
 * @return false
 */
bool Lcd::_arePortsSet(void)
{
    if (!((this->_dataPortSet) && (this->_controlPortSet))) {
        // Returns error
        this->_lastError = Error::LCD_PORTS_NOT_SET;
        debugMessage(Error::LCD_PORTS_NOT_SET);
        return false;
    }

    // Returns successfully
    return true;
}

/**
 * @brief		Reset internal class members.
 * @details		Reset internal class members related to the device's registers
 * 				and the internal class members used to control the device. The
 * 				function is used after a device physical reset, when the
 * 				internal registers are reset to their default values.
 */
void Lcd::_clearMembers(void)
{
    // Reset class members
    this->_cursorPositionColumn			= 0;
    this->_cursorPositionLine			= 0;
    this->_cursorState					= CursorState::HIDING;
    this->_displaySize					= DisplaySize::DISPLAY_16X2;
    this->_displaySizeColumns			= 0;
    this->_displaySizeLines				= 0;
    this->_displayState					= State::OFF;
    this->_entryMode					= EntryMode::LEFT_OVERWRITE;
    this->_fontSize						= FontSize::FONT_5X8;
    this->_initialized					= false;
    this->_lastError					= Error::NONE;

    // Port configuration
    this->_controlPortSet				= false;
    this->_dataPortWide4Bits			= false;
    this->_dataPortSet					= false;
    this->_gpioControlPinE				= 0;
    this->_gpioControlPinRs				= 0;
    this->_gpioControlPinRw				= 0;
    this->_gpioControlPort				= nullptr;
    this->_gpioDataPinFirstIndex				= 0;
    this->_gpioDataPort					= nullptr;
    this->_useBusyFlag					= false;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief		Checks if the device was initialized.
 * @details		Checks if the device was initialized.
 * @note		This function is private, so before calling it, the
 * 				programmer must ensure that all parameters are checked.
 * @attention	This function only verifies the device initialization, it does
 * 				not verifies if the device is configured properly.
 * @return		true		Device is initialized.
 * @return		false		Device is not initialized.
 */
bool Lcd::_isInitialized(void)
{
    // Checks if device is initialized
    if (!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
    }

    // Returns successfully
    return true;
}

//     ////////////////////    DATA MANIPULATION     ////////////////////     //

void Lcd::_sendCommand(uint8_t command_p, bool onlyHigherNibble_p)
{
    // Local variables
    uint8_t auxCommand = 0;
    bool auxDelay = false;

    // Decode command (must be evaluated from MSB to LSB)
    if (command_p & LCD_DDRAM_ADRESS) {
        // TODO: Send DDRAM ADDRESS
        auxCommand = command_p;
        auxDelay = false;
        // this->_cursorPositionColumn = 0;
        // this->_cursorPositionLine = 0;

    } else if (command_p & LCD_CGRAM_ADRESS) {
        // TODO: Send CGRAM ADDRESS
        auxCommand = command_p;
        auxDelay = false;

    } else if (command_p & LCD_FUNCTION_SET) {
        auxCommand = command_p;
        auxDelay = false;

    } else if (command_p & LCD_DISPLAY_CURSOR_MOVE) {
        auxCommand = command_p;
        auxDelay = false;

        if (command_p & LCD_CURSOR_MOVE) {
            this->_cursorPositionColumn++;
        } else {
            this->_cursorPositionColumn--;
        }

    } else if (command_p & LCD_DISPLAY_CURSOR_CONFIG) {
        auxCommand = command_p;
        auxDelay = false;

    } else if (command_p & LCD_ENTRY_MODE_SET) {
        auxCommand = command_p;
        auxDelay = false;

    } else if (command_p & LCD_CURSOR_HOME) {
        auxCommand = command_p;
        auxDelay = true;
        this->_cursorPositionColumn = 0;

    } else if (command_p & LCD_CLEAR_SCREEN) {
        auxCommand = command_p;
        auxDelay = true;
        this->_cursorPositionColumn = 0;
        this->_cursorPositionLine = 0;

    }

// Send the command
// Note: During initialization in 4 bits interface, only the higher nibble
// must be sent.
    controlRwLow();
    controlRsLow();
    if (onlyHigherNibble_p) {
        clrMaskOffset(this->_gpioDataPort->ODR, 0x000F, this->_gpioDataPinFirstIndex);
        setMaskOffset(this->_gpioDataPort->ODR, (auxCommand >> 4), this->_gpioDataPinFirstIndex);
    } else {
        if (this->_dataPortWide4Bits) {
            clrMaskOffset(this->_gpioDataPort->ODR, 0x000F, this->_gpioDataPinFirstIndex);
            setMaskOffset(this->_gpioDataPort->ODR, (auxCommand >> 4), this->_gpioDataPinFirstIndex);
            pulseEnable();
            clrMaskOffset(this->_gpioDataPort->ODR, 0x000F, this->_gpioDataPinFirstIndex);
            setMaskOffset(this->_gpioDataPort->ODR, (auxCommand & 0x0F), this->_gpioDataPinFirstIndex);
        } else {
            clrMaskOffset(this->_gpioDataPort->ODR, 0x00FF, this->_gpioDataPinFirstIndex);
            setMaskOffset(this->_gpioDataPort->ODR, auxCommand, this->_gpioDataPinFirstIndex);
        }
    }
    pulseEnable();
    if (auxDelay) {
        delayMs(2);
    }

    return;
}

void Lcd::_sendCharacter(uint8_t character_p)
{
    // Set controlm lines
    controlRwLow();
    controlRsHigh();

    // Send command
    if (this->_dataPortWide4Bits) {
        clrMaskOffset(this->_gpioDataPort->ODR, 0x000F, this->_gpioDataPinFirstIndex);
        setMaskOffset(this->_gpioDataPort->ODR, (character_p >> 4), this->_gpioDataPinFirstIndex);
        pulseEnable();
        clrMaskOffset(this->_gpioDataPort->ODR, 0x000F, this->_gpioDataPinFirstIndex);
        setMaskOffset(this->_gpioDataPort->ODR, (character_p & 0x0F), this->_gpioDataPinFirstIndex);
    } else {
        clrMaskOffset(this->_gpioDataPort->ODR, 0x00FF, this->_gpioDataPinFirstIndex);
        setMaskOffset(this->_gpioDataPort->ODR, character_p, this->_gpioDataPinFirstIndex);
    }
    pulseEnable();

    // Update class members
    this->_cursorPositionColumn++;
}

void Lcd::_processCharacter(uint8_t character_p)
{
    // Decode special characters
    switch (character_p) {
    case '\0':			// Null character		=> ignore
    case '\a':			// Alert				=> ignore
    case '\b':			// Backspace			=> ignore
    case '\e':			// Escape character		=> ignore
    case '\f':			// Formfeed page break	=> ignore
    case '\r':			// Carriage return		=> ignore
    case '\v':			// Vertical tab			=> ignore
        break;

    case '\t': {		// Horizontal tab		=> complete tabulation with spaces

            uint8_t aux8 = (4 - (this->_cursorPositionColumn % 4));
            for (uint8_t i = 0; i < aux8; i++) {
                this->_sendCharacter(0x20);
            }
            break;
        }

    case '\n': {			// New line			=> complete line with spaces
            uint8_t aux8 = (this->_displaySizeColumns - this->_cursorPositionColumn);
            for (uint8_t i = 0; i < aux8; i++) {
                this->_sendCharacter(0x20);
            }
            this->moveCursor(CursorPosition::NEXT_LINE);
            break;
        }

    case '\\': {		// Backslash			=> change to a similar character
            this->_sendCharacter(0xA4);
            break;
        }

    default:
        this->_sendCharacter(character_p);
        break;
    }
}

//     ////////////////////     PRINTF FUNCTIONS     ////////////////////     //

inline void Lcd::_my_out_char(char character, void *buffer, size_t idx, size_t maxlen)
{
    (void)buffer;
    (void)idx;
    (void)maxlen;

    if (!this->_makeFunctionNull) {
        if (character) {
            this->_processCharacter(character);
        }
    }
}

int Lcd::_my_vsnprintf(char *buffer, const size_t maxlen, const char *format, va_list va)
{
    unsigned int flags, width, precision, n;
    size_t idx = 0U;

    if (!buffer) {
        // use null output function
        this->_makeFunctionNull = true;
    } else {
        this->_makeFunctionNull = false;
    }

    while (*format) {
        // format specifier?  %[flags][width][.precision][length]
        if (*format != '%') {
            // no
            this->_my_out_char(*format, buffer, idx++, maxlen);
            format++;
            continue;
        } else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        flags = 0U;
        do {
            switch (*format) {
            case '0': flags |= FLAGS_ZEROPAD; format++; n = 1U; break;
            case '-': flags |= FLAGS_LEFT;    format++; n = 1U; break;
            case '+': flags |= FLAGS_PLUS;    format++; n = 1U; break;
            case ' ': flags |= FLAGS_SPACE;   format++; n = 1U; break;
            case '#': flags |= FLAGS_HASH;    format++; n = 1U; break;
            default :                                   n = 0U; break;
            }
        } while (n);

        // evaluate width field
        width = 0U;
        if (isAsciiNumber(*format)) {
            width = _my_atoi(&format);
        } else if (*format == '*') {
            const int w = va_arg(va, int);
            if (w < 0) {
                flags |= FLAGS_LEFT;    // reverse padding
                width = (unsigned int) - w;
            } else {
                width = (unsigned int)w;
            }
            format++;
        }

        // evaluate precision field
        precision = 0U;
        if (*format == '.') {
            flags |= FLAGS_PRECISION;
            format++;
            if (isAsciiNumber(*format)) {
                precision = _my_atoi(&format);
            } else if (*format == '*') {
                const int prec = (int)va_arg(va, int);
                precision = prec > 0 ? (unsigned int)prec : 0U;
                format++;
            }
        }

        // evaluate length field
        switch (*format) {
        case 'l' :
            flags |= FLAGS_LONG;
            format++;
            if (*format == 'l') {
                flags |= FLAGS_LONG_LONG;
                format++;
            }
            break;
        case 'h' :
            flags |= FLAGS_SHORT;
            format++;
            if (*format == 'h') {
                flags |= FLAGS_CHAR;
                format++;
            }
            break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
        case 't' :
            flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
            format++;
            break;
#endif
        case 'j' :
            flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
            format++;
            break;
        case 'z' :
            flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
            format++;
            break;
        default :
            break;
        }

        // evaluate specifier
        switch (*format) {
        case 'd' :
        case 'i' :
        case 'u' :
        case 'x' :
        case 'X' :
        case 'o' :
        case 'b' : {
                // set the base
                unsigned int base;
                if (*format == 'x' || *format == 'X') {
                    base = 16U;
                } else if (*format == 'o') {
                    base =  8U;
                } else if (*format == 'b') {
                    base =  2U;
                } else {
                    base = 10U;
                    flags &= ~FLAGS_HASH;   // no hash for dec format
                }
                // uppercase
                if (*format == 'X') {
                    flags |= FLAGS_UPPERCASE;
                }

                // no plus or space flag for u, x, X, o, b
                if ((*format != 'i') && (*format != 'd')) {
                    flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
                }

                // ignore '0' flag when precision is given
                if (flags & FLAGS_PRECISION) {
                    flags &= ~FLAGS_ZEROPAD;
                }

                // convert the integer
                if ((*format == 'i') || (*format == 'd')) {
                    // signed
                    if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                        const long long value = va_arg(va, long long);
                        idx = this->_my_ntoa_long_long(buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0,
                                        base, precision, width, flags);
#endif
                    } else if (flags & FLAGS_LONG) {
                        const long value = va_arg(va, long);
                        idx = this->_my_ntoa_long(buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base,
                                        precision, width, flags);
                    } else {
                        const int value = (flags & FLAGS_CHAR) ? (char)va_arg(va, int) : (flags & FLAGS_SHORT) ? (short int)va_arg(va,
                                        int) : va_arg(va, int);
                        idx = this->_my_ntoa_long(buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base,
                                        precision, width, flags);
                    }
                } else {
                    // unsigned
                    if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                        idx = this->_my_ntoa_long_long(buffer, idx, maxlen, va_arg(va, unsigned long long), false, base, precision, width,
                                        flags);
#endif
                    } else if (flags & FLAGS_LONG) {
                        idx = this->_my_ntoa_long(buffer, idx, maxlen, va_arg(va, unsigned long), false, base, precision, width, flags);
                    } else {
                        const unsigned int value = (flags & FLAGS_CHAR) ? (unsigned char)va_arg(va,
                                        unsigned int) : (flags & FLAGS_SHORT) ? (unsigned short int)va_arg(va, unsigned int) : va_arg(va, unsigned int);
                        idx = this->_my_ntoa_long(buffer, idx, maxlen, value, false, base, precision, width, flags);
                    }
                }
                format++;
                break;
            }
#if defined(PRINTF_SUPPORT_FLOAT)
        case 'f' :
        case 'F' :
            if (*format == 'F') {
                flags |= FLAGS_UPPERCASE;
            }
            idx = this->_my_ftoa(buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
            format++;
            break;
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if ((*format == 'g') || (*format == 'G')) {
                flags |= FLAGS_ADAPT_EXP;
            }
            if ((*format == 'E') || (*format == 'G')) {
                flags |= FLAGS_UPPERCASE;
            }
            idx = this->_my_etoa(buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
            format++;
            break;
#endif  // PRINTF_SUPPORT_EXPONENTIAL
#endif  // PRINTF_SUPPORT_FLOAT
        case 'c' : {
                unsigned int l = 1U;
                // pre padding
                if (!(flags & FLAGS_LEFT)) {
                    while (l++ < width) {
                        this->_my_out_char(' ', buffer, idx++, maxlen);
                    }
                }
                // char output
                this->_my_out_char((char)va_arg(va, int), buffer, idx++, maxlen);
                // post padding
                if (flags & FLAGS_LEFT) {
                    while (l++ < width) {
                        this->_my_out_char(' ', buffer, idx++, maxlen);
                    }
                }
                format++;
                break;
            }

        case 's' : {
                const char *p = va_arg(va, char *);
                unsigned int l = _my_strnlen_s(p, precision ? precision : (size_t) -1);
                // pre padding
                if (flags & FLAGS_PRECISION) {
                    l = (l < precision ? l : precision);
                }
                if (!(flags & FLAGS_LEFT)) {
                    while (l++ < width) {
                        this->_my_out_char(' ', buffer, idx++, maxlen);
                    }
                }
                // string output
                while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
                    this->_my_out_char(*(p++), buffer, idx++, maxlen);
                }
                // post padding
                if (flags & FLAGS_LEFT) {
                    while (l++ < width) {
                        this->_my_out_char(' ', buffer, idx++, maxlen);
                    }
                }
                format++;
                break;
            }

        case 'p' : {
                width = sizeof(void *) * 2U;
                flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
                const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
                if (is_ll) {
                    idx = this->_my_ntoa_long_long(buffer, idx, maxlen, (uintptr_t)va_arg(va, void *), false, 16U, precision, width, flags);
                } else {
#endif
                    idx = this->_my_ntoa_long(buffer, idx, maxlen, (unsigned long)((uintptr_t)va_arg(va, void *)), false, 16U, precision,
                                    width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
                }
#endif
                format++;
                break;
            }

        case '%' :
            this->_my_out_char('%', buffer, idx++, maxlen);
            format++;
            break;

        default :
            this->_my_out_char(*format, buffer, idx++, maxlen);
            format++;
            break;
        }
    }

    // termination
    this->_my_out_char((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

    // return written chars without terminating \0
    return (int)idx;
}

size_t Lcd::_my_ntoa_format(char *buffer, size_t idx, size_t maxlen, char *buf, size_t len, bool negative,
        unsigned int base, unsigned int prec, unsigned int width, unsigned int flags)
{
    // pad leading zeros
    if (!(flags & FLAGS_LEFT)) {
        if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
            width--;
        }
        while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
        while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
    }

    // handle hash
    if (flags & FLAGS_HASH) {
        if (!(flags & FLAGS_PRECISION) && len && ((len == prec) || (len == width))) {
            len--;
            if (len && (base == 16U)) {
                len--;
            }
        }
        if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'x';
        } else if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'X';
        } else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
            buf[len++] = 'b';
        }
        if (len < PRINTF_NTOA_BUFFER_SIZE) {
            buf[len++] = '0';
        }
    }

    if (len < PRINTF_NTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+';  // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }

    return this->_my_out_rev(buffer, idx, maxlen, buf, len, width, flags);
}

size_t Lcd::_my_out_rev(char *buffer, size_t idx, size_t maxlen, const char *buf, size_t len, unsigned int width,
        unsigned int flags)
{
    const size_t start_idx = idx;

    // pad spaces up to given width
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
        for (size_t i = len; i < width; i++) {
            this->_my_out_char(' ', buffer, idx++, maxlen);
        }
    }

    // reverse string
    while (len) {
        this->_my_out_char(buf[--len], buffer, idx++, maxlen);
    }

    // append pad spaces up to given width
    if (flags & FLAGS_LEFT) {
        while (idx - start_idx < width) {
            this->_my_out_char(' ', buffer, idx++, maxlen);
        }
    }

    return idx;
}

size_t Lcd::_my_ntoa_long(char *buffer, size_t idx, size_t maxlen, unsigned long value, bool negative,
        unsigned long base, unsigned int prec, unsigned int width, unsigned int flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return this->_my_ntoa_format(buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}

// internal itoa for 'long long' type
#if defined(PRINTF_SUPPORT_LONG_LONG)
size_t Lcd::_my_ntoa_long_long(char *buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative,
        unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    size_t len = 0U;

    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
    }

    return this->_my_ntoa_format(buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}
#endif  // PRINTF_SUPPORT_LONG_LONG

#if defined(PRINTF_SUPPORT_FLOAT)
size_t Lcd::_my_ftoa(char *buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width,
        unsigned int flags)
{
    char buf[PRINTF_FTOA_BUFFER_SIZE];
    size_t len  = 0U;
    double diff = 0.0;

    // powers of 10
    static const double pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

    // test for special values
    if (value != value) {
        return this->_my_out_rev(buffer, idx, maxlen, "nan", 3, width, flags);
    }
    if (value < -DBL_MAX) {
        return this->_my_out_rev(buffer, idx, maxlen, "fni-", 4, width, flags);
    }
    if (value > DBL_MAX) {
        return this->_my_out_rev(buffer, idx, maxlen, (flags & FLAGS_PLUS) ? "fni+" : "fni", (flags & FLAGS_PLUS) ? 4U : 3U,
                        width, flags);
    }

    // test for very large values
    // standard printf behavior is to print EVERY whole number digit -- which could be 100s of characters overflowing your buffers == bad
    if ((value > PRINTF_MAX_FLOAT) || (value < -PRINTF_MAX_FLOAT)) {
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
        return this->_my_etoa(buffer, idx, maxlen, value, prec, width, flags);
#else
        return 0U;
#endif
    }

    // test for negative
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = 0 - value;
    }

    // set default precision, if not set explicitly
    if (!(flags & FLAGS_PRECISION)) {
        prec = PRINTF_DEFAULT_FLOAT_PRECISION;
    }
    // limit precision to 9, cause a prec >= 10 can lead to overflow errors
    while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U)) {
        buf[len++] = '0';
        prec--;
    }

    int whole = (int)value;
    double tmp = (value - whole) * pow10[prec];
    unsigned long frac = (unsigned long)tmp;
    diff = tmp - frac;

    if (diff > 0.5) {
        ++frac;
        // handle rollover, e.g. case 0.99 with prec 1 is 1.0
        if (frac >= pow10[prec]) {
            frac = 0;
            ++whole;
        }
    } else if (diff < 0.5) {
    } else if ((frac == 0U) || (frac & 1U)) {
        // if halfway, round up if odd OR if last digit is 0
        ++frac;
    }

    if (prec == 0U) {
        diff = value - (double)whole;
        if ((!(diff < 0.5) || (diff > 0.5)) && (whole & 1)) {
            // exactly 0.5 and ODD, then round up
            // 1.5 -> 2, but 2.5 -> 2
            ++whole;
        }
    } else {
        unsigned int count = prec;
        // now do fractional part, as an unsigned number
        while (len < PRINTF_FTOA_BUFFER_SIZE) {
            --count;
            buf[len++] = (char)(48U + (frac % 10U));
            if (!(frac /= 10U)) {
                break;
            }
        }
        // add extra 0s
        while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U)) {
            buf[len++] = '0';
        }
        if (len < PRINTF_FTOA_BUFFER_SIZE) {
            // add decimal
            buf[len++] = '.';
        }
    }

    // do whole part, number is reversed
    while (len < PRINTF_FTOA_BUFFER_SIZE) {
        buf[len++] = (char)(48 + (whole % 10));
        if (!(whole /= 10)) {
            break;
        }
    }

    // pad leading zeros
    if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD)) {
        if (width && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
            width--;
        }
        while ((len < width) && (len < PRINTF_FTOA_BUFFER_SIZE)) {
            buf[len++] = '0';
        }
    }

    if (len < PRINTF_FTOA_BUFFER_SIZE) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+';  // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }

    return this->_my_out_rev(buffer, idx, maxlen, buf, len, width, flags);
}
#endif

#if defined(PRINTF_SUPPORT_FLOAT)
#	if defined(PRINTF_SUPPORT_EXPONENTIAL)
size_t Lcd::_my_etoa(char *buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width,
        unsigned int flags)
{
    // check for NaN and special values
    if ((value != value) || (value > DBL_MAX) || (value < -DBL_MAX)) {
        return this->_my_ftoa(buffer, idx, maxlen, value, prec, width, flags);
    }

    // determine the sign
    const bool negative = value < 0;
    if (negative) {
        value = -value;
    }

    // default precision
    if (!(flags & FLAGS_PRECISION)) {
        prec = PRINTF_DEFAULT_FLOAT_PRECISION;
    }

    // determine the decimal exponent
    // based on the algorithm by David Gay (https://www.ampl.com/netlib/fp/dtoa.c)
    union {
        uint64_t U;
        double   F;
    } conv;

    conv.F = value;
    int exp2 = (int)((conv.U >> 52U) & 0x07FFU) - 1023;           // effectively log2
    conv.U = (conv.U & ((1ULL << 52U) - 1U)) | (1023ULL << 52U);  // drop the exponent so conv.F is now in [1,2)
    // now approximate log10 from the log2 integer part and an expansion of ln around 1.5
    int expval = (int)(0.1760912590558 + exp2 * 0.301029995663981 + (conv.F - 1.5) * 0.289529654602168);
    // now we want to compute 10^expval but we want to be sure it won't overflow
    exp2 = (int)(expval * 3.321928094887362 + 0.5);
    const double z  = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
    const double z2 = z * z;
    conv.U = (uint64_t)(exp2 + 1023) << 52U;
    // compute exp(z) using continued fractions, see https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
    conv.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
    // correct for rounding errors
    if (value < conv.F) {
        expval--;
        conv.F /= 10;
    }

    // the exponent format is "%+03d" and largest value is "307", so set aside 4-5 characters
    unsigned int minwidth = ((expval < 100) && (expval > -100)) ? 4U : 5U;

    // in "%g" mode, "prec" is the number of *significant figures* not decimals
    if (flags & FLAGS_ADAPT_EXP) {
        // do we want to fall-back to "%f" mode?
        if ((value >= 1e-4) && (value < 1e6)) {
            if ((int)prec > expval) {
                prec = (unsigned)((int)prec - expval - 1);
            } else {
                prec = 0;
            }
            flags |= FLAGS_PRECISION;   // make sure _ftoa respects precision
            // no characters in exponent
            minwidth = 0U;
            expval   = 0;
        } else {
            // we use one sigfig for the whole part
            if ((prec > 0) && (flags & FLAGS_PRECISION)) {
                --prec;
            }
        }
    }

    // will everything fit?
    unsigned int fwidth = width;
    if (width > minwidth) {
        // we didn't fall-back so subtract the characters required for the exponent
        fwidth -= minwidth;
    } else {
        // not enough characters, so go back to default sizing
        fwidth = 0U;
    }
    if ((flags & FLAGS_LEFT) && minwidth) {
        // if we're padding on the right, DON'T pad the floating part
        fwidth = 0U;
    }

    // rescale the float value
    if (expval) {
        value /= conv.F;
    }

    // output the floating part
    const size_t start_idx = idx;
    idx = this->_my_ftoa(buffer, idx, maxlen, negative ? -value : value, prec, fwidth, flags & ~FLAGS_ADAPT_EXP);

    // output the exponent part
    if (minwidth) {
        // output the exponential symbol
        this->_my_out_char((flags & FLAGS_UPPERCASE) ? 'E' : 'e', buffer, idx++, maxlen);
        // output the exponent value
        idx = this->_my_ntoa_long(buffer, idx, maxlen, (expval < 0) ? -expval : expval, expval < 0, 10, 0, minwidth - 1,
                        FLAGS_ZEROPAD | FLAGS_PLUS);
        // might need to right-pad spaces
        if (flags & FLAGS_LEFT) {
            while (idx - start_idx < width) {
                this->_my_out_char(' ', buffer, idx++, maxlen);
            }
        }
    }
    return idx;
}
#	endif  // PRINTF_SUPPORT_EXPONENTIAL
#endif  // PRINTF_SUPPORT_FLOAT

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

static uint16_t _my_atoi(const char **str)
{
    uint16_t i = 0U;
    while (isAsciiNumber(**str)) {
        i = i * 10U + (uint16_t)(*((*str)++) - '0');
    }
    return i;
}

static inline uint16_t _my_strnlen_s(const char *str, size_t maxsize)
{
    const char *s;
    for (s = str; *s && maxsize--; ++s);
    return (uint16_t)(s - str);
}

///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2019, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////
