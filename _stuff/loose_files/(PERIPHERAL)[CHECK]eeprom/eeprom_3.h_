/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			eeprom.h
 * Module:			EEPROM interface
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __EEPROM_H
#define __EEPROM_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <util/atomic.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#if		defined(__AVR_ATtiny10__) || defined(__AVR_ATtiny20__) || defined(__AVR_ATtiny28__) || \
		defined(__AVR_ATtiny4__) || defined(__AVR_ATtiny40__) ||  defined(__AVR_ATtiny5__) || \
		defined(__AVR_ATtiny9__)
	#error Error 102 - EEPROM is not available in the selected device.
#elif	defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__) || defined(__AVR_ATtiny43U__) || \
		defined(__AVR_ATtiny48__) || defined(__AVR_ATtiny88__)
	#define EEPROM_SIZE 64
	#define EEPROM_ADDRESS_MASK	0x3F
#elif	defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny24__) || \
		defined(__AVR_ATtiny24A__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny26__) || \
		defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny261A__)
	#define EEPROM_SIZE 128
	#define EEPROM_ADDRESS_MASK	0x7F
#elif 	defined(__AVR_ATmega48PB__) || defined(__AVR_ATmega48__) || defined(__AVR_ATmega48PA__) || \
		defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega8HVA__) || \
		defined(__AVR_ATmega16HVA__) || defined(__AVR_ATtiny1634__) || defined(__AVR_ATtiny4313__) || \
		defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny44A__) || \
		defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny461A__) || \
		defined(__AVR_ATtiny828__)
	#define EEPROM_SIZE 256
	#define EEPROM_ADDRESS_MASK	0xFF
#elif	 defined(__AVR_ATA6285__) || defined(__AVR_ATA6286__)
	#define EEPROM_SIZE 320
	#define EEPROM_ADDRESS_MASK	0x013F
#elif	defined(__AVR_AT90PWM1__) || defined(__AVR_AT90PWM161__) || defined(__AVR_AT90PWM216__) || \
		defined(__AVR_AT90PWM2B__) || defined(__AVR_AT90PWM316__) || defined(__AVR_AT90PWM3B__) || \
		defined(__AVR_AT90PWM81__) || defined(__AVR_AT90USB162__) || defined(__AVR_AT90USB82__) || \
		defined(__AVR_ATA5272__) || defined(__AVR_ATA5505__) || defined(__AVR_ATA6612C__) || \
		defined(__AVR_ATA6613C__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega162__) || \
		defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164PA__) || \
		defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165P__) || defined(__AVR_ATmega165PA__) || \
		defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) || \
		defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega169A__) || \
		defined(__AVR_ATmega169P__) || defined(__AVR_ATmega169PA__) || defined(__AVR_ATmega16A__) || \
		defined(__AVR_ATmega16HVB__) || defined(__AVR_ATmega16HVBREVB__) || defined(__AVR_ATmega16M1__) || \
		defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega406__) || \
		defined(__AVR_ATmega8__) || defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__) || \
		defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || \
		defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__) || defined(__AVR_ATmega8A__) || \
		defined(__AVR_ATmega8U2__) || defined(__AVR_ATA6616C__) || defined(__AVR_ATA6617C__) || \
		defined(__AVR_ATA664251__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny84__) || \
		defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny85__) || \
		defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny87__) || \
		defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega8E5__)
	#define EEPROM_SIZE 512
	#define EEPROM_ADDRESS_MASK	0x01FF
#elif	defined(__AVR_AT90CAN32__) || defined(__AVR_ATA6614Q__) || defined(__AVR_ATmega32__) || \
		defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || \
		defined(__AVR_ATmega325__) || defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3250A__) || \
		defined(__AVR_ATmega3250P__) || defined(__AVR_ATmega3250PA__) || defined(__AVR_ATmega325A__) || \
		defined(__AVR_ATmega325P__) || defined(__AVR_ATmega325PA__) || defined(__AVR_ATmega328__) || \
		defined(__AVR_ATmega328P__) || defined(__AVR_ATmega329__) || defined(__AVR_ATmega3290__) || \
		defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290P__) || defined(__AVR_ATmega3290PA__) || \
		defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329P__)  || defined(__AVR_ATmega329PA__) || \
		defined(__AVR_ATmega32A__) || defined(__AVR_ATmega32C1__) || defined(__AVR_ATmega32HVB__) || \
		defined(__AVR_ATmega32HVBREVB__) || defined(__AVR_ATmega32M1__) || defined(__AVR_ATmega32U2__) || \
		defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega64HVE2__) || defined(__AVR_ATxmega16A4__) || \
		defined(__AVR_ATxmega16A4U__) || defined(__AVR_ATxmega16C4__) || defined(__AVR_ATxmega16D4__) || \
		defined(__AVR_ATxmega32A4__) || defined(__AVR_ATxmega32A4U__) || defined(__AVR_ATxmega32C3__) || \
		defined(__AVR_ATxmega32C4__) || defined(__AVR_ATxmega32D3__) || defined(__AVR_ATxmega32D4__) || \
		defined(__AVR_ATxmega32E5__)
	#define EEPROM_SIZE 1024
	#define EEPROM_ADDRESS_MASK	0x033F
#elif	defined(__AVR_ATA5781__) || defined(__AVR_ATA5782__) || defined(__AVR_ATA5783__) || \
		defined(__AVR_ATA5831__) || defined(__AVR_ATA5832__) || defined(__AVR_ATA5833__) || \
		defined(__AVR_ATA8210__) || defined(__AVR_ATA8215__) || defined(__AVR_ATA8510__) || \
		defined(__AVR_ATA8515__)
	#define EEPROM_SIZE 1152
	#define EEPROM_ADDRESS_MASK	0x047F
#elif	defined(__AVR_AT90CAN64__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || \
		defined(__AVR_ATA5790__) || defined(__AVR_ATA5790N__) || defined(__AVR_ATA5791__) || \
		defined(__AVR_ATA5795__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega644__) || \
		defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || \
		defined(__AVR_ATmega644RFR2__) || defined(__AVR_ATmega645__) || defined(__AVR_ATmega6450__) || \
		defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450P__) || defined(__AVR_ATmega645A__) || \
		defined(__AVR_ATmega645P__) || defined(__AVR_ATmega649__) || defined(__AVR_ATmega6490__) || \
		defined(__AVR_ATmega6490A__) || defined(__AVR_ATmega6490P__) || defined(__AVR_ATmega649A__) || \
		defined(__AVR_ATmega649P__) || defined(__AVR_ATmega64A__) || defined(__AVR_ATmega64C1__) || \
		defined(__AVR_ATmega64M1__) || defined(__AVR_ATmega64RFR2__) || defined(__AVR_ATxmega128A1__) || \
		defined(__AVR_ATxmega128A1U__) || defined(__AVR_ATxmega128A3__) || defined(__AVR_ATxmega128A3U__) || \
		defined(__AVR_ATxmega128A4U__) || defined(__AVR_ATxmega128B1__) || defined(__AVR_ATxmega128B3__) || \
		defined(__AVR_ATxmega128C3__) || defined(__AVR_ATxmega128D3__) || defined(__AVR_ATxmega128D4__) || \
		defined(__AVR_ATxmega192A3__) || defined(__AVR_ATxmega192A3U__) || defined(__AVR_ATxmega192C3__) || \
		defined(__AVR_ATxmega192D3__) || defined(__AVR_ATxmega64A1__) || defined(__AVR_ATxmega64A1U__) || \
		defined(__AVR_ATxmega64A3__) || defined(__AVR_ATxmega64A3U__) || defined(__AVR_ATxmega64A4U__) || \
		defined(__AVR_ATxmega64B1__) || defined(__AVR_ATxmega64B3__) || defined(__AVR_ATxmega64C3__) || \
		defined(__AVR_ATxmega64D3__) || defined(__AVR_ATxmega64D4__)
	#define EEPROM_SIZE 2048
	#define EEPROM_ADDRESS_MASK	0x07FF
#elif defined(__AVR_ATA5702M322__)
	#define EEPROM_SIZE 2304
	#define EEPROM_ADDRESS_MASK	0x08FF
#elif	defined(__AVR_AT90CAN128__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) || \
		defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
		defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284RFR2__) || \
		defined(__AVR_ATmega128A__) || defined(__AVR_ATmega128RFA1__) || defined(__AVR_ATmega128RFR2__) || \
		defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega640__) || \
		defined(__AVR_ATxmega256A3__) || defined(__AVR_ATxmega256A3B__) || defined(__AVR_ATxmega256A3BU__) || \
		defined(__AVR_ATxmega256A3U__) || defined(__AVR_ATxmega256C3__) || defined(__AVR_ATxmega256D3__) || \
		defined(__AVR_ATxmega384C3__) || defined(__AVR_ATxmega384D3__)
	#define EEPROM_SIZE 4096
	#define EEPROM_ADDRESS_MASK	0x0FFF
#elif	defined(__AVR_ATmega2564RFR2__) || defined(__AVR_ATmega256RFR2__)
	#define EEPROM_SIZE 8192
	#define EEPROM_ADDRESS_MASK	0x1FFF
#endif

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum eepromMode_t{
	EEPROM_ERASE_WRITE = 0,
	EEPROM_ERASE_ONLY,
	EEPROM_WITE_ONLY
} eepromMode_t;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void	eepromSetOperationMode(eepromMode_t mode);
void	eepromReadyActivateInterrupt(void);
void	eepromReadyDeactivateInterrupt(void);
#if EEPROM_SIZE <= 256
uint8_t	eepromRead(uint8_t address);
void	eepromWrite(uint8_t address, uint8_t data);
#else
uint8_t	eepromRead(uint16_t address);
void	eepromWrite(uint16_t address, uint8_t data);
#endif
 
#endif
