/* =============================================================================
 * Project:		FunSAPE++ Library
 * File name:	funsapeLibTwi.hpp
 * Module:		Twi Peripheral Interface Class for FunSAPE++ Library project
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __FUNSAPE_LIB_TWI_HPP
#define __FUNSAPE_LIB_TWI_HPP					__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "funsapeLibGlobalDefines.hpp"
#if __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (funsapeLibGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Constants
// =============================================================================

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

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Logic values
// -----------------------------------------------------------------------------

typedef enum twiOperation_t {
	TWI_WRITE = 0,
	TWI_READ
} twiOperation_t;

// -----------------------------------------------------------------------------
// Enumerations - Finite lists
// -----------------------------------------------------------------------------

typedef enum twiState_e {
	TWI_START					= 0x08,	// START has been transmitted
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been transmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been transmitted and NACK received
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been transmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been transmitted and NACK received
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been transmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been transmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK transmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK transmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = 0
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_e;

// -----------------------------------------------------------------------------
// Enumerations - Expansible lists
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be algebraically manipulated
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be manipulated using bitwise operations
// -----------------------------------------------------------------------------

// NONE

// =============================================================================
// Global variable declaration
// =============================================================================

// NONE

// =============================================================================
// Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Class Twi0 - Start
// -----------------------------------------------------------------------------

#if TWI0_EXISTS
class Twi0
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

	Twi0(void);											// Automatic constructor

	~Twi0(void);										// Automatic destructor

	// -------------------------------------------------------------------------
	// Public methods
	// -------------------------------------------------------------------------
public:

	//     ///////////////////     INITIALIZATION     ///////////////////     //

	bool initialize(									// Initialize peripheral
		uint8_t bufferSize = 20								// Internal buffer size
	);

	bool setMaster(										// Configure TWI to master role
		uint32_t bitRate = TWI0_MIN_BIT_RATE				// Master SCL frequency
	);

	//     ////////////////////     CHECK STATUS     ////////////////////     //

	error_e getLastError(void);							// Returns last error

	//     ///////////////////     DATA STREAMING     ///////////////////     //

	bool sendData(										// Send data to slave device (explicit device register address)
		uint8_t devAddress,									// Slave device 7-bit address
		twiOperation_t readWrite,							// Operation mode
		uint8_t regAddress,									// Device register address
		uint8_t * msg = NULL,								// Pointer to message (without device register address)
		uint8_t msgSize = 0									// Message size
	);

	// bool sendData(										// Send data to slave device (implicit device register address)
	// 	uint8_t devAddress,									// Slave device 7-bit address
	// 	twiOperation_t readWrite,							// Operation mode
	// 	uint8_t * msg,										// Pointer to message (with device register address)
	// 	uint8_t msgSize										// Message size
	// );

	//     /////////////////////     INTERRUPTS     /////////////////////     //

	void interruptHandler(void);

	//     ////////////////     PROTOCOL  PARAMETERS     ////////////////     //
	bool setTimeOut(
		uint8_t timeOut
	);

	// -------------------------------------------------------------------------
	// Private methods
	// -------------------------------------------------------------------------
private:

	void _resetData(void);								// Reset internal data members
	void _resetRegisters(void);							// Stop peripheral and reset its registers
	void _disable(void);								// Disables peripheral
	bool _waitWhileIsBusy(void);
	bool _startTrasmission(void);
	bool _readFromBuffer(
		uint8_t * msg,
		uint8_t msgSize
	);

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

	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool				_initialized					: 1;
	bool				_masterEnabled					: 1;
	bool				_slaveEnabled					: 1;

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	error_e				_lastError;
	bool				_lastTransOk					: 1;

	//     /////////////////     INTERNAL  CONTROLS     /////////////////     //
	uint8_t				_timeOut;
	twiState_e			_state;
	uint8_t				_bitRateRegister;
	twi0Prescaler_t		_bitRatePrescaler;

	//     ////////////////     INTERNAL DATA BUFFER     ////////////////     //
	uint8_t				* _bufferData;
	uint8_t				_bufferMaxSize					: 7;
	uint8_t				_bufferIndex					: 7;
	uint8_t				_bufferLength					: 7;
	uint8_t				_tempBytesToSend				: 7;

	// -------------------------------------------------------------------------
	// Protected properties
	// -------------------------------------------------------------------------
protected:

}; // class Twi0

// -----------------------------------------------------------------------------
// Class Twi0 - Class overloading operators
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class Twi0 - Class inline functions definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class Twi0 - End
// -----------------------------------------------------------------------------

#endif // TWI0_EXISTS

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

#if TWI0_EXISTS
extern Twi0 twi0;
#endif

#endif // __FUNSAPE_LIB_TWI_HPP
