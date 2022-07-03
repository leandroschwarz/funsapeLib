/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twiMaster.h
 * Module:			Two Wire Interface module in master mode
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 *					Fábio Cabral Pacheco
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __TWI_MASTER_H
#define __TWI_MASTER_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#ifndef TWI_GENERAL_CALL_ADDRESS
	#define TWI_GENERAL_CALL_ADDRESS	0x00
#endif
#ifndef TWI_BUFFER_SIZE
	#define TWI_BUFFER_SIZE				10
#endif

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union twiStatus_t{
	struct{
		uint8_t lastTransOK		: 1;
		uint8_t rxDataInBuf		: 1;
		uint8_t genAddressCall	: 1;
		uint8_t unusedBits		: 5;
	};
	uint8_t all;
} twiStatus_t;

typedef enum twiGeneralCallEnable_t{
	TWI_DISABLE_GENERAL_CALL	= 0,
	TWI_ENABLE_GENERAL_CALL		= 1
} twiGeneralCallEnable_t;

typedef enum twiOperation_t {
	TWI_MASTER_WRITE = 0,
	TWI_MASTER_READ
} twiOperation_t;

typedef enum twiState_t{
	TWI_START					= 0x08,	// START has been transmitted  
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been tramsmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been tramsmitted and NACK received 
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been tramsmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been tramsmitted and NACK received 
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been tramsmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been tramsmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK tramsmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK tramsmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = "0"
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

extern twiStatus_t twiStatus;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void		twiMasterInit(uint32_t clockSpeed);
void		twiMasterSendData(uint8_t deviceAddress, uint8_t readWrite, uint8_t *message, uint8_t messageSize);
void		twiMasterResendData(void);
twiState_t	twiMasterErrorHandler(twiState_t twiErrorCode);

// -----------------------------------------------------------------------------
// Private functions declaration - do not use outside this module --------------

#endif