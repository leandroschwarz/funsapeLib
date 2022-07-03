/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twiMaster.c
 * Module:			Two Wire Interface module in master mode
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 *					Fábio Cabral Pacheco
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "twiMaster.h"
#if __TWI_MASTER_H != 1
#error Error 101 - Build mismatch on header and source code files (twiMaster).
#endif
#include "system.hpp"
#include <avr/interrupt.h>

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
// Private function declarations -----------------------------------------------

static uint8_t	twiMasterIsBusy(void);
static void		twiMasterReadFromBuffer(uint8_t *message, uint8_t messageSize);
static void		twiMasterResendData(void);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

static uint8_t		twiMasterBufferData[20];	// Transceiver buffer
static uint8_t		twiMasterBufferSize;					// Number of bytes to be transmitted
static twiState_e	twiMasterState = TWI_NO_STATE;			// twiSatet_t is defined in twiMaster.h
volatile twiMasterStatus_t	twiMasterStatus = {.allFlags = 0};		// twiStatus_t is defined in twiMaster.h

// -----------------------------------------------------------------------------
// Public functions ------------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	twiMasterInit
 * Purpose:		Initializes the TWI module in master mode
 * Arguments:	clockSpeed		Approximate value of the SCL clock speed
 * Returns:		none
 * Notes:		Since the controller is interrupt-driven, the interruptions must
 * 				be enabled in the main code just after this function is called
 * -------------------------------------------------------------------------- */

void twiMasterInit(uint32_t clockSpeed)
{
	uint8_t aux8 = 0;
	uint32_t aux32 = systemStatus.getCpuClock() / (uint32_t)clockSpeed;

	if (aux32 <= 526) {					// Prescaler 1
		aux8 = (uint8_t)(((aux32) - 16) / 2);
		clrBit(TWSR, TWPS1);
		clrBit(TWSR, TWPS0);
	} else if (aux32 <= 2056) {			// Prescaler 4
		aux8 = (uint8_t)(((aux32) - 16) / 8);
		clrBit(TWSR, TWPS1);
		setBit(TWSR, TWPS0);
	} else if (aux32 <= 8176) {			// Prescaler 16
		aux8 = (uint8_t)(((aux32) - 16) / 32);
		setBit(TWSR, TWPS1);
		clrBit(TWSR, TWPS0);
	} else if (aux32 <= 32656) {			// Prescaler 64
		aux8 = (uint8_t)(((aux32) - 16) / 128);
		setBit(TWSR, TWPS1);
		setBit(TWSR, TWPS0);
	} else {
		return;
	}
	TWBR = aux8;
	TWDR = 0xFF;						// Release SDA
	TWCR = 1 << TWEN;					// Activate TWI interface

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	twiMasterSendData
 * Purpose:		Sends data from master to slave device
 * Arguments:	deviceAddress		Bus address of the slave device or
 * 									TWI_GENERAL_CALL_ADDRESS for a general call
 * 				readWrite			twiOperation_t enumeration
 * 				message				message to be transmitted
 * 				messageSize			number of bytes to be sent or read
 * Returns:		none
 * Notes:		When reading the device, the bytes received will be available
 * 				in message argument
 * -------------------------------------------------------------------------- */

void twiMasterSendData(uint8_t deviceAddress, twiOperation_t readWrite, uint8_t *message, uint8_t messageSize)
{
	uint8_t i;

	while (twiMasterIsBusy())
		;	// Wait until TWI is ready for next transmission.

	twiMasterBufferData[0] = (deviceAddress << 1) |  readWrite;
	twiMasterBufferSize = messageSize + 1;
	if (readWrite == TWI_WRITE) {
		for (i = 0; i < messageSize; i++) {
			twiMasterBufferData[i + 1] = message[i];
		}
	}
	twiMasterStatus.allFlags = 0;
	twiMasterState = TWI_NO_STATE;
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWSTA);
	if (readWrite == TWI_READ) {
		twiMasterReadFromBuffer(message, messageSize);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	twiMasterResendData
 * Purpose:		Resends the data of the buffer to the slave device
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void twiMasterResendData(void)
{
	while (twiMasterIsBusy())
		;	// Wait until TWI is ready for next transmission
	twiMasterStatus.allFlags = 0;
	twiMasterState = TWI_NO_STATE;
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWSTA);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	twiMasterErrorHandler
 * Purpose:		Returns the state of the last transmission and resends the
 * 				message if necessary
 * Arguments:	none
 * Returns:		twiState_e		error flags
 * -------------------------------------------------------------------------- */

twiState_e twiMasterErrorHandler(twiState_e twiErrorCode)
{
	if ((twiErrorCode == TWI_MTX_ADR_NACK) || (twiErrorCode == TWI_MRX_ADR_NACK)) {
		twiMasterResendData();
	}
	return twiErrorCode;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Handler:		TWI_vect
 * Purpose:		Manages the TWI interruption
 * -------------------------------------------------------------------------- */

ISR(TWI_vect)
{
	static uint8_t twiBufferPointer;

	switch (TWSR & 0xFC) {
	case TWI_START:			// START has been transmitted
	case TWI_REP_START:		// Repeated START has been transmitted
		twiBufferPointer = 0;	// Set buffer pointer to the TWI Address location
	case TWI_MTX_ADR_ACK:		// SLA+W has been transmitted and ACK received
	case TWI_MTX_DATA_ACK:		// Data byte has been transmitted and ACK received
		if (twiBufferPointer < twiMasterBufferSize) {
			TWDR = twiMasterBufferData[twiBufferPointer++];
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
		} else {			// Send STOP after last byte
			twiMasterStatus.lastTransOK = 1;	// Set status bits to completed successfully
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
		}
		break;
	case TWI_MRX_DATA_ACK:		// Data byte has been received and ACK transmitted
		twiMasterBufferData[twiBufferPointer++] = TWDR;
	case TWI_MRX_ADR_ACK:		// SLA+R has been transmitted and ACK received
		if (twiBufferPointer < (twiMasterBufferSize - 1)) {	// Detect the last byte to NACK it
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
		} else {				// Send NACK after next reception
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
		}
		break;
	case TWI_MRX_DATA_NACK:		// Data byte has been received and NACK transmitted
		twiMasterBufferData[twiBufferPointer] = TWDR;
		twiMasterStatus.lastTransOK = 1;	// Set status bits to completed successfully
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
		break;
	case TWI_ARB_LOST:			// Arbitration lost
		TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWSTA);
		break;
	case TWI_MTX_ADR_NACK:		// SLA+W has been transmitted and NACK received
	case TWI_MRX_ADR_NACK:		// SLA+R has been transmitted and NACK received
	case TWI_MTX_DATA_NACK:		// Data byte has been transmitted and NACK received
	case TWI_BUS_ERROR:			// Bus error due to an illegal START or STOP condition
	default:
		twiMasterState = (twiState_e)TWSR;		// Store TWSR and automatically sets clears noErrors bit
		TWCR = (1 << TWEN);		// Reset TWI Interface
		break;
	}
}

// -----------------------------------------------------------------------------
// Private functions -----------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	twiMasterIsBusy
 * Purpose:		Checks if transceiver is busy
 * Arguments:	none
 * Returns:		uint8_t		1 / 0
 * -------------------------------------------------------------------------- */

uint8_t twiMasterIsBusy(void)
{
	return ((TWCR >> TWIE) & 1);
}
/* -----------------------------------------------------------------------------
 * Function:	twiMasterReadFromBuffer
 * Purpose:		Reads data from buffer after a reception
 * Arguments:	message				pointer to where data must be copied into
 * 				messageSize			number of bytes to be read from buffer
 * Returns:		none
 * -------------------------------------------------------------------------- */

void twiMasterReadFromBuffer(uint8_t *message, uint8_t messageSize)
{
	uint8_t i;

	while (twiMasterIsBusy())
		;	// Wait until TWI is ready for next transmission
	if (twiMasterStatus.lastTransOK) {			// Last transmission competed successfully
		for (i = 0; i < messageSize; i++) {	// Copy data from Transceiver buffer
			message[i] = twiMasterBufferData[i + 1];
		}
	}

	return;
}
