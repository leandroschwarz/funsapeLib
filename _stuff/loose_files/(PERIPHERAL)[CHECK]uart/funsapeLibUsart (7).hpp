/* 
 * Project:		FunSAPE++ Library
 * File name:	funsapeLibUsart.hpp
 * Module:		USART Peripheral Interface Class for FunSAPE++ Library project
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// 
// File build
// 

#ifndef __FUNSAPE_LIB_USART_HPP
#define __FUNSAPE_LIB_USART_HPP				__BUILD_MACRO_TO_BE_CHANGED__

// 
// System file dependencies
// 

#include "funsapeLibGlobalDefines.hpp"
#if __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (funsapeLibGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// 
// Constants
// 

#define USART0_EXISTS 1

// -----------------------------------------------------------------------------
// Undefining previous definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Defining new constants
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Defining macro-functions
// -----------------------------------------------------------------------------

// NONE

// 
// New data types
// 

// -----------------------------------------------------------------------------
// Single variables
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Logic values
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Finite lists
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Expansible lists
// -----------------------------------------------------------------------------

typedef enum usartBaudRate_e {
	USART_BAUD_600			= 600UL,
	USART_BAUD_1200			= 1200UL,
	USART_BAUD_1800			= 1800UL,
	USART_BAUD_2400			= 2400UL,
	USART_BAUD_4800			= 4800UL,
	USART_BAUD_9600			= 9600UL,
	USART_BAUD_14400		= 14400UL,
	USART_BAUD_19200		= 19200UL,
	USART_BAUD_28800		= 28800UL,
	USART_BAUD_38400		= 38400UL,
	USART_BAUD_56000		= 56000UL,
	USART_BAUD_57600		= 57600UL,
	USART_BAUD_115200		= 115200UL,
	USART_BAUD_128000		= 128000UL,
	USART_BAUD_230400		= 230400UL,
	USART_BAUD_256000		= 256000UL
} usartBaudRate_e;

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be algebraically manipulated
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be manipulated using bitwise operations
// -----------------------------------------------------------------------------

typedef enum usartDataFormat_e {
	//									= 0bDDDDPPSS
	// DDDD -> Number of data bits (5-8)
	// PP   -> Parity (NONE / ODD / EVEN)
	// SS   -> Number of stop bits (1-2)
	USART_DATA_8_N_1					= 0b10000001,
	USART_DATA_8_O_1					= 0b10000101,
	USART_DATA_8_E_1					= 0b10001001,
	USART_DATA_8_N_2					= 0b10000010,
	USART_DATA_8_O_2					= 0b10000110,
	USART_DATA_8_E_2					= 0b10001010,
	USART_DATA_9_N_1					= 0b10010001,
	USART_DATA_9_O_1					= 0b10010101,
	USART_DATA_9_E_1					= 0b10011001,
	USART_DATA_9_N_2					= 0b10010010,
	USART_DATA_9_O_2					= 0b10010110,
	USART_DATA_9_E_2					= 0b10011010,
	USART_DATA_5_N_1					= 0b01010001,
	USART_DATA_5_O_1					= 0b01010101,
	USART_DATA_5_E_1					= 0b01011001,
	USART_DATA_5_N_2					= 0b01010010,
	USART_DATA_5_O_2					= 0b01010110,
	USART_DATA_5_E_2					= 0b01011010,
	USART_DATA_6_N_1					= 0b01100001,
	USART_DATA_6_O_1					= 0b01100101,
	USART_DATA_6_E_1					= 0b01101001,
	USART_DATA_6_N_2					= 0b01100010,
	USART_DATA_6_O_2					= 0b01100110,
	USART_DATA_6_E_2					= 0b01101010,
	USART_DATA_7_N_1					= 0b01110001,
	USART_DATA_7_O_1					= 0b01110101,
	USART_DATA_7_E_1					= 0b01111001,
	USART_DATA_7_N_2					= 0b01110010,
	USART_DATA_7_O_2					= 0b01110110,
	USART_DATA_7_E_2					= 0b01111010
} usartDataFormat_e;

// 
// Global variable declaration
// 

// NONE

// 
// Class declaration
// 

// -----------------------------------------------------------------------------
// Class Usart0 - Start
// -----------------------------------------------------------------------------

#if USART0_EXISTS

class Usart0
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------
	// NONE

	// -------------------------------------------------------------------------
	// New data declaration ----------------------------------------------------
private:
	// NONE

	// -------------------------------------------------------------------------
	// New data overloading operators ------------------------------------------
private:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------
public:

	Usart0(void);										// Automatic constructor
	~Usart0(void);										// Automatic destructor

	// -------------------------------------------------------------------------
	// Public methods
	// -------------------------------------------------------------------------
public:

	//     ///////////////////     INITIALIZATION     ///////////////////     //

	bool initialize(	uint8_t txBufferSize = 50,
						uint8_t rxBufferSize = 50
				   );

	// NONE

	//     ////////////////////     CHECK STATUS     ////////////////////     //

	// NONE

	//     ///////////////////     DATA STREAMING     ///////////////////     //

	// NONE

	//     /////////////////////     INTERRUPTS     /////////////////////     //

	// NONE

	// -------------------------------------------------------------------------
	// Private methods
	// -------------------------------------------------------------------------
private:

	bool _changeDataFormat(void);
	bool _changeBaudRate(void);
	// NONE

	// -------------------------------------------------------------------------
	// Protected methods
	// -------------------------------------------------------------------------
protected:

	//     ///////////////////////     TO  DO     ///////////////////////     //

	// NONE

	// -------------------------------------------------------------------------
	// Private properties
	// -------------------------------------------------------------------------
private:

	usartDataFormat_e		_dataFormat;
	usartBaudRate_e			_baudRate;
	bool					_synchronousMode;
	error_e					_lastError;


	//     ///////////////////     INITIALIZATION     ///////////////////     //

	// NONE

	// -------------------------------------------------------------------------
	// Protected properties
	// -------------------------------------------------------------------------
protected:

}; // class Usart0

// -----------------------------------------------------------------------------
// Class Usart0 - Class overloading operators
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class Usart0 - Class inline functions definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class Usart0 - End
// -----------------------------------------------------------------------------

#endif // USART0_EXISTS

// 
// General public functions declarations
// 

// NONE

// 
// General inline functions definitions
// 

// NONE

// 
// External default objects
// 

// NONE

#endif // __FUNSAPE_LIB_USART_HPP
