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
// Static function declarations ------------------------------------------------

//static uint8_t spiMasterSendReceiveByte(uint8_t data);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

static spiMaster_t spiMaster = {.prescaler = SPI_PRESCALER_128, .mode = SPI_MODE_0, .dataOrder = SPI_MSB_FIRST, .initialized = FALSE};

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	spiMasterInit
 * Purpose:		Initializes the SPI in Master Mode
 * Arguments:	prescaler	spiClockPrescaler_t enumeration
 *				mode		spiModes_t enumeration
 *				dataOrder	spiDataOrder_t enumeration
 * Returns:		none
 * -------------------------------------------------------------------------- */

void spiMasterInit(spiClockPrescaler_t prescaler, spiModes_t mode, spiDataOrder_t dataOrder, spiProtocol_t protocol)
{
	vuint8_t spcr = 0;

	// Updates SPI structure
	spiMaster.mode = mode;
	spiMaster.prescaler = prescaler;
	spiMaster.dataOrder = dataOrder;
	spiMaster.protocol = protocol;

	// Port configuration
	setBit(SPI_DDR, SPI_MOSI);
	clrBit(SPI_DDR, SPI_MISO);
	setBit(SPI_DDR, SPI_SCLK);

	spcr |= (1 << MSTR);												// Master mode
	spcr |= (dataOrder << DORD);										// Data order
	spcr |= ((mode >> 1) << CPOL) | ((mode & 1) << CPHA);				// Mode selection
	spcr |= ((prescaler >> 2) << SPI2X) | ((prescaler & 3) << SPR0);	// Clock prescaler
	spcr |= (1 << SPE);													// Enable SPI
	SPCR = spcr;

	// Clears Interrupt flags
	spcr = SPSR;
	spcr = SPDR;

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

void spiMasterSendReceiveData(uint8_t * txBuffer, uint8_t * rxBuffer, uint8_t sizeBuffer)
{
	uint8_t i = 0;

	for(i = 0; i < sizeBuffer; i++) {
		rxBuffer[i] = spiMasterSendReceiveByte(txBuffer[i]);
	}

	return;
}

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	spiMasterSendReceiveData
 * Purpose:		Sends to and receives a byte from the SPI
 * Arguments:	byte	byte to be sent
 * Returns:		uint8_t	byte received
 * -------------------------------------------------------------------------- */

uint8_t spiMasterSendReceiveByte(uint8_t data)
{
	SPCR |= (1 << MSTR);
	SPDR = data;
	waitUntilBitIsSet(SPSR, SPIF);
	data = SPDR;

	return data;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

/* NONE */
