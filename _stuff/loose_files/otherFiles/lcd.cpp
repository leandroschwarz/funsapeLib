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

#define LCD_CGRAM_ADRESS				0x40
#define LCD_CLEAR_SCREEN				0x01
#define LCD_CURSOR_HOME					0x02
#define LCD_DDRAM_ADRESS				0x80
#define LCD_DISPLAY_OFF					0x08
#define LCD_DISPLAY_ON					0x0C
#define LCD_ENTRY_DECREMENT				0x00
#define LCD_ENTRY_INCREMENT				0x02
#define LCD_ENTRY_OVERWRITE				0x00
#define LCD_ENTRY_SET					0x04
#define LCD_ENTRY_SHIFT					0x01
#define LCD_FUNCTION_1_LINE				0x00
#define LCD_FUNCTION_2_LINES			0x08
#define LCD_FUNCTION_4_BITS				0x00
#define LCD_FUNCTION_5x10_FONT			0x04
#define LCD_FUNCTION_5x8_FONT			0x00
#define LCD_FUNCTION_8_BITS				0x10
#define LCD_FUNCTION_RESET				0x30
#define LCD_FUNCTION_SET				0x20

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

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

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
	debugMark("Lcd(void)");

	// Reset data members
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
 * @param[in]	handler_p	class handler associated with the device.
 * @param[in]	size_p
 * @param[in]	font_p
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Lcd::init(Handler *handler_p, DisplaySize size_p, FontSize font_p)
{
	// Local variables
	Handler::HandlerType handlerType = Handler::HandlerType::NONE;

	// Mark passage for debugging purpose
	debugMark("init(Handler *, DisplaySize, FontSize)");

	// Reset data members
	this->_clearMembers();

	// Check function arguments for errors
	if(!isPointerValid(handler_p)) {
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	} else {
		handlerType = handler_p->getHandlerType();
		switch(handlerType) {
		case Handler::HandlerType::PARALLEL:
		case Handler::HandlerType::TWI:
			break;
		default:
			// Returns error
			this->_lastError = Error::HANDLER_UNSUPPORTED;
			debugMessage(Error::HANDLER_UNSUPPORTED);
			return false;
			break;
		}
	}

	// Update data members
	this->_handler = handler_p;
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
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

//     ////////////////    DEVICE CONTROL AND STATUS     ////////////////     //

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
	this->_cursorState					= CursorState::HIDE;
	this->_displaySize					= DisplaySize::DISPLAY_16X2;
	this->_displayState					= State::OFF;
	this->_fontSize						= FontSize::FONT_5X8;
	this->_handler						= nullptr;
	this->_initialized					= false;
	this->_lastError					= Error::NONE;

	// Port configuration
	this->_controlE						= 0;
	this->_controlRs					= 0;
	this->_controlRw					= 0;
	this->_dataBusLength				= 0;
	this->_dataFirstPin					= 0;
	this->_gpioControl					= nullptr;
	this->_gpioData						= nullptr;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

//     //////////////////    REGISTER MANIPULATION     //////////////////     //

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
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		debugMessage(Error::NOT_INITIALIZED);
	}

	// Returns successfully
	return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

// NONE
