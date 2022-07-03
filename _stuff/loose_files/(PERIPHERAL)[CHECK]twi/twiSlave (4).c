/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twiSlave.c
 * Module:			TWI Slave controller
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * Purpose:			Interfaces a TWI Slave data bus
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "twiSlave.h"
#if __TWI_SLAVE_H != 1
	#error Error 101 - Build mismatch on header and source code files (twiSlave).
#endif

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

static twiBuffer_t * twiBufferData;
static vuint8_t twiBufferIndex = 0;
static vuint8_t twiBufferSize = 0;
static vuint8_t twiCommIndex = 0;
static vuint8_t twiBusy = 0;

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

twiBuffer_t * twiSlaveInit(uint8_t twiSlaveAddr, uint8_t bufferSize, bool_t genCallAcceptance)
{
	TWAR = (twiSlaveAddr << 1) | (genCallAcceptance & 1);
	TWCR =	(1 << TWEN) |
			(1 << TWIE) | (1 << TWINT) |
			(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
			(0 << TWWC);
	twiBusy = 0;

	twiBufferData = (twiBuffer_t *)calloc(bufferSize, sizeof(uint8_t));

	if(twiBufferData == NULL) {
		return NULL;
	}

	twiBufferSize = bufferSize;

	return twiBufferData;
}

ISR(TWI_vect)
{
	switch (TWSR) {
	case TWI_STX_ADR_ACK:
	case TWI_STX_DATA_ACK:
		TWDR = twiBufferData[twiBufferIndex++];

		if(twiBufferIndex >= twiBufferSize) {
			twiBufferIndex = 0;
		}

		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 1;
		break;

	case TWI_STX_DATA_NACK:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;
		break;

	case TWI_SRX_GEN_ACK:
	case TWI_SRX_ADR_ACK:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiCommIndex = TRUE;
		twiBusy = 1;
		break;

	case TWI_SRX_ADR_DATA_ACK:
	case TWI_SRX_GEN_DATA_ACK:
		if(twiCommIndex == TRUE) {
			twiCommIndex = FALSE;
			twiBufferIndex = TWDR;
		}
		else {
			twiBufferIndex++;

			if(twiBufferIndex >= twiBufferSize) {
				twiBufferIndex = 0;
			}
		}

		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 1;
		break;

	case TWI_SRX_STOP_RESTART:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;
		break;

	case TWI_SRX_ADR_DATA_NACK:
	case TWI_SRX_GEN_DATA_NACK:
	case TWI_STX_DATA_ACK_LAST_BYTE:
	case TWI_BUS_ERROR:
		TWCR =	(0 << TWEN) |
				(0 << TWIE) | (1 << TWINT) |
				(0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;

		break;

	default:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;
		break;
	}
}
