/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			spiMaster.h
 * Module:			Serial Peripheral Interface module in master mode
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __SPI_MASTER_H
#define __SPI_MASTER_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum spiModes_t {
	SPI_MODE_0			= 0,
	SPI_MODE_1			,
	SPI_MODE_2			,
	SPI_MODE_3
} spiModes_t;

typedef enum spiDataOrder_t {
	SPI_MSB_FIRST		= 0,
	SPI_LSB_FIRST
} spiDataOrder_t;

typedef enum spiClockPrescaler_t {
	SPI_PRESCALER_4			= 0,
	SPI_PRESCALER_16		= 1,
	SPI_PRESCALER_64		= 2,
	SPI_PRESCALER_128		= 3,
	SPI_PRESCALER_2			= 4,
	SPI_PRESCALER_8			= 5,
	SPI_PRESCALER_32		= 6
} spiClockPrescaler_t;

typedef enum spiProtocol_t {
	SPI_BIT_BANG			= 0,
	SPI_INTERRUPT_DRIVEN	= 1
} spiProtocol_t;

typedef struct spiMaster_t {
	spiClockPrescaler_t	prescaler	: 3;
	spiModes_t			mode		: 2;
	spiDataOrder_t		dataOrder	: 1;
	spiProtocol_t		protocol	: 1;
	bool_t				initialized	: 1;
} spiMaster_t;

// -----------------------------------------------------------------------------
// Global variable declarations ------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define spiMasterOff()	clrBit(SPI_PORT, SPI_MOSI);clrBit(SPI_PORT, SPI_MISO);clrBit(SPI_PORT, SPI_SCLK);clrBit(SPI_DDR, SPI_MOSI);clrBit(SPI_DDR, SPI_MISO);clrBit(SPI_DDR, SPI_SCLK)
#define spiMasterOn()	clrBit(SPI_PORT, SPI_MOSI);clrBit(SPI_PORT, SPI_MISO);clrBit(SPI_PORT, SPI_SCLK);setBit(SPI_DDR, SPI_MOSI);clrBit(SPI_DDR, SPI_MISO);setBit(SPI_DDR, SPI_SCLK)

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void	spiMasterInit(spiClockPrescaler_t prescaler, spiModes_t mode, spiDataOrder_t dataOrder, spiProtocol_t protocol);
void 	spiMasterEnableInterrupt(void);
void 	spiMasterDisableInterrupt(void);
void	spiMasterSendReceiveData(uint8_t * txBuffer, uint8_t * rxBuffer, uint8_t sizeBuffer);
uint8_t spiMasterSendReceiveByte(uint8_t data);
#endif
