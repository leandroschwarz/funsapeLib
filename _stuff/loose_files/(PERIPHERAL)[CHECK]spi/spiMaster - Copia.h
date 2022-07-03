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
	SPI_MODE_0 = 0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3,
	SPI_MODE_NO_CHANGE
} spiModes_t;

typedef enum spiDataOrder_t {
	SPI_MSB_FIRST = 0,
	SPI_LSB_FIRST,
	SPI_ORDER_NO_CHANGE
} spiDataOrder_t;

typedef enum spiClockPrescaler_t {
	SPI_PRESCALER_2 = 0,
	SPI_PRESCALER_4,
	SPI_PRESCALER_8,
	SPI_PRESCALER_16,
	SPI_PRESCALER_32,
	SPI_PRESCALER_64,
	SPI_PRESCALER_128,
	SPI_PRESCALER_NO_CHANGE
} spiClockPrescaler_t;

typedef volatile struct spiMaster_t {
	spiClockPrescaler_t	prescaler	: 3;
	spiModes_t			mode		: 3;
	spiDataOrder_t		dataOrder	: 2;
} spiMaster_t;

// -----------------------------------------------------------------------------
// Global variable declarations ------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void	spiMasterInit(spiClockPrescaler_t prescaler, spiModes_t mode, spiDataOrder_t dataOrder);
void 	spiMasterEnableInterrupt();
void 	spiMasterDisableInterrupt();
uint8_t	spiMasterSendReceiveData(uint8_t data);

#endif
