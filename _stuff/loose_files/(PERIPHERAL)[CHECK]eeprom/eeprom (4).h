/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			eeprom.h
 * Module:			EEPROM interface
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			2
 * Last edition:	April 22, 2018
 * -------------------------------------------------------------------------- */

#ifndef __EEPROM_H
#define __EEPROM_H 2

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif
#include <avr/interrupt.h>
#include <util/atomic.h>

// -----------------------------------------------------------------------------
// Preliminary verifications ---------------------------------------------------

#ifndef EEPROM_SIZE
	#error Error 102 - EEPROM size is not defined for the selected device.
#elif EEPROM_SIZE == 0
	#error Error 103 - EEPROM is not available for the selected device.
#elif EEPROM_SIZE > 65535
	#error Error 104 - EEPROM is too large to handle.
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define EEPROM_MASK		(EEPROM_SIZE - 1)

#if EEPROM_SIZE < 256
	#define EEPROM_ADD_REG		EEARL
#else
	#define EEPROM_ADD_REG		EEAR
#endif

// -----------------------------------------------------------------------------
// Macro-functions definitions -------------------------------------------------

// Macro-function eepromIsReady()
#if defined(MCU_ATMEGAX8_ALL)
	#define eepromIsReady()			isBitClr(EECR, EEPE)
#elif defined(MCU_ATXMEGA_ALL)
	#define eepromIsReady()			isBitClr(NVM_STATUS, NVM_NVMBUSY_bp)
#elif defined(DEECR)
	#define eepromIsReady()			isBitClr(DEECR, BSY)
#else
	#define eepromIsReady()			isBitClr(EECR, EEWE)
#endif

// Macro-function waitUntilEepromIsReady()
#define waitUntilEepromIsReady()	do {} while (!eepromIsReady())

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

#if EEPROM_SIZE < 256
	typedef uint8_t		eepromAddress_t;
#else
	typedef uint16_t	eepromAddress_t;
#endif

typedef enum eepromMode_e {
	EEPROM_ERASE_AND_WRITE = 0,
	EEPROM_ERASE_ONLY,
	EEPROM_WITE_ONLY
} eepromMode_e;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

// Changes EEPROM programming mode
static void eepromSetOperationMode(eepromMode_e mode);
// Activates EEPROM Ready interrupt
static void eepromReadyActivateInterrupt(void);
// Deativates EEPROM Ready interrupt
static void eepromReadyDeactivateInterrupt(void);
// Reads a byte from EEPROM address
static uint8_t eepromReadByte(eepromAddress_t address);
// Reads one 16-bit word (little endian) from EEPROM address
static uint16_t eepromReadWord(eepromAddress_t address);
// Reads one 32-bit double word (little endian) from EEPROM address
static uint32_t eepromReadDoubleWord(eepromAddress_t address);
// Reads one float value (little endian) from EEPROM address
static float eepromReadFloat(eepromAddress_t address);
// Reads a data block from EEPROM address to SRAM
static void eepromReadDataBlock(eepromAddress_t address, void * destination, size_t size);
// Writes a byte to EEPROM address
static void eepromWriteByte(eepromAddress_t address, uint8_t data);
// Writes a 16-bit word to EEPROM address
static void eepromWriteWord(eepromAddress_t address, uint16_t data);
// Writes a 32-bit double word to EEPROM address
static void eepromWriteDoubleWord(eepromAddress_t address, uint32_t data);
// Writes a float to EEPROM address
static void eepromWriteFloat(eepromAddress_t address, float data);
// Writes a data block from SRAM to EEPROM address
static void eepromWriteDataBlock(eepromAddress_t address, void * data, size_t size);

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

static void inline __attribute__((always_inline)) eepromSetOperationMode(eepromMode_e mode)
{
	uint8_t aux8 = 0;
	aux8 = EECR & ~(0x03 << EEPM0);
	aux8 |= (mode << EEPM0);
	EECR = aux8;
	return;
}

static void inline __attribute__((always_inline)) eepromReadyActivateInterrupt(void)
{
	setBit(EECR, EERIE);
	return;
}

static void inline __attribute__((always_inline)) eepromReadyDeactivateInterrupt(void)
{
	clrBit(EECR, EERIE);
	return;
}

static uint8_t inline __attribute__((always_inline)) eepromReadByte(eepromAddress_t address)
{
	waitUntilEepromIsReady();						// Wait for previous operation
	EEPROM_ADD_REG = (address & EEPROM_MASK);		// Setup new address
	setBit(EECR, EERE);								// Trigger read operation
	return EEDR;
}

static uint16_t inline __attribute__((always_inline)) eepromReadWord(eepromAddress_t address)
{
	uint16_t data = 0;
	uint8_t aux8 = 0;
	address += sizeof(uint16_t);
	for(uint8_t i = 0; i < sizeof(uint16_t); i++) {
		aux8 = eepromReadByte(--address);
		data <<= 8;
		data |= aux8;
	}
	return data;
}
static uint32_t inline __attribute__((always_inline)) eepromReadDoubleWord(eepromAddress_t address)
{
	uint32_t data = 0;
	uint8_t aux8 = 0;
	address += sizeof(uint32_t);
	for(uint8_t i = 0; i < sizeof(uint32_t); i++) {
		aux8 = eepromReadByte(--address);
		data <<= 8;
		data |= aux8;
	}
	return data;
}

static float inline __attribute__((always_inline)) eepromReadFloat(eepromAddress_t address)
{
	float auxFloat = 0;
	uint8_t aux8 = 0;
	uint8_t * ptr;
	ptr = (uint8_t *)&auxFloat;
	address += sizeof(float);
	for(uint8_t i = 0; i < sizeof(float); i++) {
		aux8 = eepromReadByte(--address);
		ptr[sizeof(float) - i - 1] = aux8;
	}
	return auxFloat;
}

static void inline __attribute__((always_inline)) eepromReadDataBlock(eepromAddress_t address, void * destination, size_t size)
{
	uint8_t aux8 = 0;
	address += size;
	for(uint8_t i = 0; i < size; i++) {
		aux8 = eepromReadByte(--address);
		((uint8_t *)destination)[size - i - 1] = aux8;
	}
	return;
}

static void inline __attribute__((always_inline)) eepromWriteByte(eepromAddress_t address, uint8_t data)
{
	waitUntilEepromIsReady();						// Wait for previous operation
	EEPROM_ADD_REG = (address & EEPROM_MASK);		// Setup new address
	EEDR = data;									// Setup new data
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		setBit(EECR, EEMPE);						// Prepare to write
		setBit(EECR, EEPE);							// Trigger write operation
	}
	return;
}

static void inline __attribute__((always_inline)) eepromWriteWord(eepromAddress_t address, uint16_t data)
{
	for(uint8_t i = 0 ; i < sizeof(uint16_t); i++) {
		eepromWriteByte(address++, (data & 0xFF));
		data >>= 8;
	}
	return;
}

static void inline __attribute__((always_inline)) eepromWriteDoubleWord(eepromAddress_t address, uint32_t data)
{
	for(uint8_t i = 0 ; i < sizeof(uint32_t); i++) {
		eepromWriteByte(address++, (data & 0xFF));
		data >>= 8;
	}
	return;
}

static void inline __attribute__((always_inline)) eepromWriteFloat(eepromAddress_t address, float data)
{
	uint8_t * ptr;
	ptr = (uint8_t *)&data;
	for(uint8_t i = 0 ; i < sizeof(float); i++) {
		eepromWriteByte(address++, (ptr[i] & 0xFF));
	}
	return;
}

static void inline __attribute__((always_inline)) eepromWriteDataBlock(eepromAddress_t address, void * data, size_t size)
{
	for(uint8_t i = 0 ; i < size; i++) {
		eepromWriteByte(address++, (((uint8_t *)data)[i] & 0xFF));
	}
	return;
}

#endif
