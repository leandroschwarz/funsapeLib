/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			spiMaster.c
 * Module:			Serial Peripheral Interface module in master mode
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "spiMaster.h"
#if __SPI_MASTER_H != 1
	#error Error 101 - Build mismatch on header and source code files (spiMaster).
#endif

// -----------------------------------------------------------------------------
// Private function declarations -----------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

static spiMaster_t spiMaster = {.prescaler = SPI_PRESCALER_128, .mode = SPI_MODE_0, .dataOrder = SPI_MSB_FIRST};

// -----------------------------------------------------------------------------
// Private functions -----------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Public functions ------------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	spiMasterInit
 * Purpose:		Initializes the SPI in Master Mode
 * Arguments:	prescaler	spiClockPrescaler_t enumeration
 *				mode		spiModes_t enumeration
 *				dataOrder	spiDataOrder_t enumeration
 * Returns:		none
 * -------------------------------------------------------------------------- */

void spiMasterInit(spiClockPrescaler_t prescaler, spiModes_t mode, spiDataOrder_t dataOrder)
{
	vuint8_t aux8 = 0;

	// Updates SPI structure
	spiMaster.mode = mode;
	spiMaster.prescaler = prescaler;
	spiMaster.dataOrder = dataOrder;

	// Port configuration
	setBit(SPI_DDR, SPI_MOSI);
	clrBit(SPI_DDR, SPI_MISO);
	setBit(SPI_DDR, SPI_SCLK);

	// Master mode
	setBit(aux8, MSTR);

	// Data order
	if(spiMaster.dataOrder == SPI_LSB_FIRST) {
		setBit(aux8, DORD);
	}

	// Mode selection
	switch(spiMaster.mode) {
	case SPI_MODE_0:
		break;
	case SPI_MODE_1:
		setBit(aux8, CPHA);
		break;
	case SPI_MODE_2:
		setBit(aux8, CPOL);
		break;
	case SPI_MODE_3:
		setBit(aux8, CPOL);
		setBit(aux8, CPHA);
		break;
	}

	// Clock prescaler
	switch(spiMaster.prescaler) {
	case SPI_PRESCALER_2:
		setBit(aux8, SPI2X);
		break;
	case SPI_PRESCALER_4:
		break;
	case SPI_PRESCALER_8:
		setBit(aux8, SPI2X);
		setBit(aux8, SPR0);
		break;
	case SPI_PRESCALER_16:
		setBit(aux8, SPR0);
		break;
	case SPI_PRESCALER_32:
		setBit(aux8, SPI2X);
		setBit(aux8, SPR1);
		break;
	case SPI_PRESCALER_64:
		setBit(aux8, SPR1);
		break;
	case SPI_PRESCALER_128:
		setBit(aux8, SPR1);
		setBit(aux8, SPR0);
		break;
	}

	// SPI enable
	setBit(aux8, SPE);

	// Configures SPI
	SPCR = aux8;

	// Clears Interrupt flags
	aux8 = SPSR;
	aux8 = SPDR;

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	spiMasterEnableInterrupt
 * Purpose:		Enables SPI interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline spiMasterEnableInterrupt(void)
{
	setBit(SPCR, SPIE);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	spiMasterDisableInterrupt
 * Purpose:		Disables SPI interrupt
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void inline spiMasterDisableInterrupt()
{
	clrBit(SPCR, SPIE);

	return;
}

/* -----------------------------------------------------------------------------
 * Function:	spiMasterSendReceiveData
 * Purpose:		Sends to and receives a byte from the SPI
 * Arguments:	byte	byte to be sent
 * Returns:		uint8_t	byte received
 * -------------------------------------------------------------------------- */

uint8_t spiMasterSendReceiveData(uint8_t data)
{
	SPDR = data;
	waitUntilBitIsSet(SPSR, SPIF);
	data = SPDR;

	return data;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

/* NONE */
