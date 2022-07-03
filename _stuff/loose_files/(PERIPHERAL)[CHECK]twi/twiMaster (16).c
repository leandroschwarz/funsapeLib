/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twi.c
 * Module:			Two Wire Interface module in master mode
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 *					Fábio Cabral Pacheco
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "twi.h"
#if __TWI_MASTER_H != 1
#error Error 101 - Build mismatch on header and source code files (twi).
#endif

// -----------------------------------------------------------------------------
// Private function declarations -----------------------------------------------

static bool_t	twiIsBusy(void);
static void		twiReadFromBuffer(uint8_t * message, uint8_t messageSize);
static void		twiResendData(void);


// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

static uint8_t		twiBufferData[TWI_BUFFER_SIZE];	// Transceiver buffer
static uint8_t		twiBufferSize;					// Number of bytes to be transmitted
static twiState_t	twiState = TWI_NO_STATE;			// twiSatet_t is defined in twi.h
twiStatus_t	twiStatus = {.allFlags = 0};		// twiStatus_t is defined in twi.h

// -----------------------------------------------------------------------------
// Public functions ------------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	twiInit
 * Purpose:		Initializes the TWI module in master mode
 * Arguments:	clockSpeed		Approximate value of the SCL clock speed
 * Returns:		none
 * Notes:		Since the controller is interrupt-driven, the interruptions must
 * 				be enabled in the main code just after this function is called
 * -------------------------------------------------------------------------- */

void twiInit(uint32_t clockSpeed)
{
	uint32_t aux32 = 0;
	uint8_t aux8 = 0;

	aux32 = (uint32_t)systemConfiguration.systemClock / (uint32_t)clockSpeed;

	if (aux32 <= 526) {					// Prescaler 1
		aux8 = (uint8_t)((((uint32_t)systemConfiguration.systemClock / (uint32_t)clockSpeed) - 16) / 2);
		clrBit(TWSR, TWPS1);
		clrBit(TWSR, TWPS0);
	} else if (aux32 <= 2056) {			// Prescaler 4
		aux8 = (uint8_t)((((uint32_t)systemConfiguration.systemClock / (uint32_t)clockSpeed) - 16) / 8);
		clrBit(TWSR, TWPS1);
		setBit(TWSR, TWPS0);
	} else if (aux32 <= 8176) {			// Prescaler 16
		aux8 = (uint8_t)((((uint32_t)systemConfiguration.systemClock / (uint32_t)clockSpeed) - 16) / 32);
		setBit(TWSR, TWPS1);
		clrBit(TWSR, TWPS0);
	} else if (aux32 <= 32656) {			// Prescaler 64
		aux8 = (uint8_t)((((uint32_t)systemConfiguration.systemClock / (uint32_t)clockSpeed) - 16) / 128);
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
 * Function:	twiSendData
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

void twiSendData(uint8_t deviceAddress, twiOperation_t readWrite, uint8_t *message, uint8_t messageSize)
{
	uint8_t i;

	while (twiIsBusy())
		;	// Wait until TWI is ready for next transmission.

	twiBufferData[0] = (deviceAddress << 1) |  readWrite;
	twiBufferSize = messageSize + 1;
	if (readWrite == TWI_WRITE) {
		for (i = 0; i < messageSize; i++) {
			twiBufferData[i + 1] = message[i];
		}
	}
	twiStatus.allFlags = 0;
	twiState = TWI_NO_STATE;
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWSTA);
	if (readWrite == TWI_READ) {
		twiReadFromBuffer(message, messageSize);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	twiResendData
 * Purpose:		Resends the data of the buffer to the slave device
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void twiResendData(void)
{
	while (twiIsBusy())
		;	// Wait until TWI is ready for next transmission
	twiStatus.allFlags = 0;
	twiState = TWI_NO_STATE;
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWSTA);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	twiErrorHandler
 * Purpose:		Returns the state of the last transmission and resends the
 * 				message if necessary
 * Arguments:	none
 * Returns:		twiState_t		error flags
 * -------------------------------------------------------------------------- */

twiState_t twiErrorHandler(twiState_t twiErrorCode)
{
	if ((twiErrorCode == TWI_MTX_ADR_NACK) || (twiErrorCode == TWI_MRX_ADR_NACK)) {
		twiResendData();
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
		if (twiBufferPointer < twiBufferSize) {
			TWDR = twiBufferData[twiBufferPointer++];
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
		} else {			// Send STOP after last byte
			twiStatus.lastTransOK = TRUE;	// Set status bits to completed successfully
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
		}
		break;
	case TWI_MRX_DATA_ACK:		// Data byte has been received and ACK transmitted
		twiBufferData[twiBufferPointer++] = TWDR;
	case TWI_MRX_ADR_ACK:		// SLA+R has been transmitted and ACK received
		if (twiBufferPointer < (twiBufferSize - 1)) {	// Detect the last byte to NACK it
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
		} else {				// Send NACK after next reception
			TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
		}
		break;
	case TWI_MRX_DATA_NACK:		// Data byte has been received and NACK transmitted
		twiBufferData[twiBufferPointer] = TWDR;
		twiStatus.lastTransOK = TRUE;	// Set status bits to completed successfully
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
		twiState = TWSR;		// Store TWSR and automatically sets clears noErrors bit
		TWCR = (1 << TWEN);		// Reset TWI Interface
		break;
	}
}

// -----------------------------------------------------------------------------
// Private functions -----------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	twiIsBusy
 * Purpose:		Checks if transceiver is busy
 * Arguments:	none
 * Returns:		bool_t		TRUE / FALSE
 * -------------------------------------------------------------------------- */

bool_t twiIsBusy(void)
{
	if (isBitSet(TWCR, TWIE)) {
		return TRUE;
	}
	return FALSE;
}
/* -----------------------------------------------------------------------------
 * Function:	twiReadFromBuffer
 * Purpose:		Reads data from buffer after a reception
 * Arguments:	message				pointer to where data must be copied into
 * 				messageSize			number of bytes to be read from buffer
 * Returns:		none
 * -------------------------------------------------------------------------- */

void twiReadFromBuffer(uint8_t * message, uint8_t messageSize)
{
	uint8_t i;

	while (twiIsBusy())
		;	// Wait until TWI is ready for next transmission
	if (twiStatus.lastTransOK) {			// Last transmission competed successfully
		for (i = 0; i < messageSize; i++) {	// Copy data from Transceiver buffer
			message[i] = twiBufferData[i + 1];
		}
	}

	return;
}
