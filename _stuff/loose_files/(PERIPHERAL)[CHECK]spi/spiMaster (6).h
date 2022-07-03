/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			spiMaster.h
 * Module:			Serial Peripheral Interface module in master mode
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Build:			1
 * Last edition:	September 6, 2017
 * -------------------------------------------------------------------------- */

#ifndef __SPIMASTER_H
#define __SPIMASTER_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBALDEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum spiModes_t {
	SPI_MODE_0 = 0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
} spiModes_t;

typedef enum spiDataOrder_t {
	SPI_MSB_FIRST = 0,
	SPI_LSB_FIRST
} spiDataOrder_t;

typedef enum spiClockPrescaler_t {
	SPI_PRESCALER_2 = 4,
	SPI_PRESCALER_4 = 0,
	SPI_PRESCALER_8 = 5,
	SPI_PRESCALER_16 = 1,
	SPI_PRESCALER_32 = 6,
	SPI_PRESCALER_64 = 2,
	SPI_PRESCALER_128 = 3
} spiClockPrescaler_t;

typedef volatile struct spiMasterDevice_t {
	volatile uint8 *	ssDdr;
	volatile uint8 *	ssPort;
	uint8				ssBit		: 3;
	spiClockPrescaler_t	prescaler	: 3;
	spiModes_t			mode		: 2;
	spiDataOrder_t		dataOrder	: 1;
	uint8				unusedBits	: 7;
} spiMasterDevice_t;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define createSpiMasterDevice() (spiMasterDevice_t){.ssDdr = NULL, .ssPort = NULL, .ssBit = 0, .prescaler = SPI_PRESCALER_128, .mode = SPI_MODE_0, .dataOrder = SPI_MSB_FIRST, .unusedBits = 0}

// -----------------------------------------------------------------------------
// Functions declarations ------------------------------------------------------

resultValue_t	spiMasterDeviceSetPort(spiMasterDevice_t * device, vuint8 * ssDdr, vuint8 * ssPort, vuint8 ssBit);
resultValue_t	spiMasterDeviceSetMode(spiMasterDevice_t * device, spiModes_t mode);
resultValue_t	spiMasterDeviceSetPrescaler(spiMasterDevice_t * device, spiClockPrescaler_t prescaler);
resultValue_t	spiMasterDeviceSetDataOrder(spiMasterDevice_t * device, spiDataOrder_t dataOrder);
resultValue_t	spiMasterInterruptEnable();
resultValue_t	spiMasterInterruptDisable();
resultValue_t	spiMasterInit();
resultValue_t	spiMasterActivateDevice(spiMasterDevice_t * device);
resultValue_t	spiMasterDeactivateDevice(spiMasterDevice_t * device);
uint8			spiMasterSendReceiveData(uint8 data);

#endif