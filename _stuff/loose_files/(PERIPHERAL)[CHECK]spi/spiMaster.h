/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			spiMaster.h
 * Module:			Serial Peripheral Interface master controller
 * Author:			Leandro Schwarz
 *					Fabio Cabral Pacheco
 * Version:			2.0
 * Last edition:	2016-02-16
 * Purpose:			SPI bus controller using the dedicated peripheral (interrupt-
 *					driven)
 * -------------------------------------------------------------------------- */

#ifndef __SPIMASTER_H
#define __SPIMASTER_H 20

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBALDEFINES_H < 123
	#error Error 100 - The defintion file is outdated (globalDefines must be version 12.3 or greater).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

//#ifdef MCU
//	#if MCU == _ATMEGA328_ || MCU == _ATMEGA168_ || MCU == _ATMEGA88_ || MCU == _ATMEGA48_
		#define SPI_MISO_DDR	DDRB
		#define SPI_MISO_PORT	PORTB
		#define SPI_MISO_PIN	PINB
		#define SPI_MISO_BIT	PB4
		#define SPI_MOSI_DDR	DDRB
		#define SPI_MOSI_PORT	PORTB
		#define SPI_MOSI_PIN	PINB
		#define SPI_MOSI_BIT	PB3
		#define SPI_SCLK_DDR	DDRB
		#define SPI_SCLK_PORT	PORTB
		#define SPI_SCLK_PIN	PINB
		#define SPI_SCLK_BIT	PB5
		#define SPI_SS_DDR		DDRB
		#define SPI_SS_PORT		PORTB
		#define SPI_SS_PIN		PINB
		#define SPI_SS_BIT		PB2
//	#else
//		#error Error 200 - The microcontroller is not supported (spiMaster).
//	#endif
//#else
//	#error Error 201 - The microcontroller is not defined (spiMaster).
//#endif

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef union spiStatus_t{
	uint8 all;
	struct{
		uint8 clockPolarity		: 1;
		uint8 clockPhase		: 1;
		uint8 dataEndianness	: 1;
		uint8 prescaler			: 3;

		uint8 unusedBits		: 2;
	};
} spiStatus_t;

typedef enum spiResult_t{
	SPI_OK						= 0,
	SPI_CLOCK_PRESCALER_UNSUPPORTED,
	SPI_CLOCK_POLARITY_UNSUPPORTED,
	SPI_CLOCK_PHASE_UNSUPPORTED,
	SPI_DATA_ENDIANNESS_UNSUPPORTED
} spiResult_t;

typedef enum spiClockPolarity_t{
	SPI_POLARITY_IDLE_LOW	= 0,
	SPI_POLARITY_IDLE_HIGH	= 1
} spiClockPolarity_t;

typedef enum spiClockPhase_t{
	SPI_SAMPLE_THEN_SETUP	= 0,
	SPI_SETUP_THEN_SAMPLE	= 1
} spiClockPhase_t;

typedef enum spiDataEndianness_t{
	SPI_DATA_MSB_FIRST	= 0,
	SPI_DATA_LSB_FIRST	= 1
} spiDataEndianness_t;

typedef enum spiPrescalerValue_t{
	SPI_PRESCALER_2 = 0,
	SPI_PRESCALER_4 = 1,
	SPI_PRESCALER_8 = 2,
	SPI_PRESCALER_16 = 3,
	SPI_PRESCALER_32 = 4,
	SPI_PRESCALER_64 = 5,
	SPI_PRESCALER_128 = 6,
	SPI_PRESCALER_NO_CHANGE = 255
} spiPrescalerValue_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

extern spiStatus_t spiStatus;

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

spiResult_t spiMasterInit(void);
spiResult_t spiSetClockPrescaler(spiPrescalerValue_t prescaler);
spiResult_t spiSetClockPolarity(spiClockPolarity_t polarity);
spiResult_t spiSetClockPhase(spiClockPhase_t phase);
spiResult_t	spiSetDataEndianness(spiDataEndianness_t endianness);
uint8		spiSendReceiveByte(uint8 data);
spiResult_t	spiSendByte(uint8 data);
uint8		spiReceiveByte(void);

// -----------------------------------------------------------------------------
// Private functions declaration - do not use outside this module --------------

#endif