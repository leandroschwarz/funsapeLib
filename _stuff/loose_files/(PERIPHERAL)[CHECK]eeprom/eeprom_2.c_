/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			eeprom.c
 * Module:			EEPROM interface
 * Author:			Leandro Schwarz
 * Version:			13.0
 * Last edition:	2017-06-17
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "eeprom.h"
#if __EEPROM_H != 130
	#error Error 101 - Version mismatch on header and source code files (eeprom).
#endif

/* -----------------------------------------------------------------------------
 * Changes the EEPROM operation mode
 * -------------------------------------------------------------------------- */

void eepromSetOperationMode(eepromMode_t mode)
{
	uint8 aux8 = 0;

	aux8 = EECR & ~(0x03 << EEPM0);
	aux8 |= (mode << EEPM0);
	EECR = aux8;

	return;
}

/* -----------------------------------------------------------------------------
 * Activates the EEPROM ready interrupt
 * -------------------------------------------------------------------------- */

void eepromReadyActivateInterrupt(void)
{
	setBit(EECR, EERIE);
	return;
}

/* -----------------------------------------------------------------------------
 * Deactivates the EEPROM ready interrupt
 * -------------------------------------------------------------------------- */

void eepromReadyDeactivateInterrupt(void)
{
	clrBit(EECR, EERIE);

	return;
}

/* -----------------------------------------------------------------------------
 * Writes a byte in the specified address in the EEPROM
 * -------------------------------------------------------------------------- */
 
#if EEPROM_SIZE <= 256
void eepromWrite(uint8 address, uint8 data)
#else
void eepromWrite(uint16 address, uint8 data)
#endif
{
	waitUntilBitIsClear(EECR, EEPE);
	waitUntilBitIsClear(SPMEN, SPMCSR);
	EEAR = (address & EEPROM_ADDRESS_MASK);
	EEDR = data;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		setBit(EECR, EEMPE);
		setBit(EECR, EEPE);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * Reads a byte from the specified address in the EEPROM
 * -------------------------------------------------------------------------- */

 #if EEPROM_SIZE <= 256
uint8 eepromRead(uint8 address)
#else
uint8 eepromRead(uint16 address)
#endif
{
	waitUntilBitIsClear(EECR, EEPE);
	EEAR = (address & EEPROM_ADDRESS_MASK);
	setBit(EECR, EERE);

	return EEDR;
}
