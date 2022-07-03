/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			oneWire.c
 * Module:			Dallas 1-Wire master interface
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * Purpose:			Controls and interfaces a Dallas 1-Wire bus
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "oneWire.h"
#if __ONE_WIRE_H != 1
	#error Error 101 - Build mismatch on header and source code files (onewire).
#endif

// -----------------------------------------------------------------------------
// Private function definitions ------------------------------------------------

static void owiWriteBit(owi_t* ow, unsigned char bit)
{
	reg_t sreg;
	sreg = SREG;
	cli();

	*(ow->ddrReg) |= ow->mask;
	*(ow->portReg) &= ~ow->mask;

	if(bit)	{
		_delay_us(OWI_DELAY_A);
	} else	{
		_delay_us(OWI_DELAY_C);
	}

	*(ow->ddrReg) &= ~ow->mask;

	if(bit)	{
		_delay_us(OWI_DELAY_B);
	} else	{
		_delay_us(OWI_DELAY_D);
	}

	SREG = sreg;

	return;
}

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

void owiPinConfigure(owi_t *ow, reg_t* ddrReg, reg_t* portReg, reg_t* pinReg, unsigned char pin)
{
	ow->ddrReg	      = ddrReg;
	ow->portReg	      = portReg;
	ow->pinReg	      = pinReg;
	ow->pin		      = pin;
	ow->mask		  = (1 << pin);
	ow->lastDeviation = 0;

	*(ow->ddrReg) &= ~ow->mask;

	_delay_ms(OWI_DELAY_H);

	return;
}

unsigned char owiReadBit(owi_t* ow)
{
	unsigned char bitRead = 0;

	reg_t sreg;
	sreg = SREG;
	cli();

	*(ow->ddrReg) |= ow->mask;
	*(ow->portReg) &= ~ow->mask;

	_delay_us(OWI_DELAY_A);

	*(ow->ddrReg) &= ~ow->mask;

	_delay_us(OWI_DELAY_E);

	bitRead = (*(ow->pinReg) & ow->mask) ? 1 : 0;

	_delay_us(OWI_DELAY_F);

	SREG = sreg;

	return bitRead;
}

uint8_t owiDetectPresence(owi_t* ow)
{
	unsigned char bitRead = 0;

	reg_t sreg;
	sreg = SREG;
	cli();

	*(ow->ddrReg) |= ow->mask;
	*(ow->portReg) &= ~ow->mask;

	_delay_us(OWI_DELAY_H);

	*(ow->ddrReg) &= ~ow->mask;

	_delay_us(OWI_DELAY_I);

	bitRead = (*(ow->pinReg) & ow->mask);

	_delay_us(OWI_DELAY_J);

	SREG = sreg;

	return bitRead;
}

void owiSendByte(owi_t* ow, unsigned char data)
{
	unsigned char bitsSent;

	for(bitsSent = 0; bitsSent < 8; bitsSent++) {
		owiWriteBit(ow, (data & 1));
		data >>= 1;
	}

	return;
}

unsigned char owiReceiveByte(owi_t* ow)
{
	unsigned char data = 0;
	unsigned char bitsReceived;

	for(bitsReceived = 0; bitsReceived < 8; bitsReceived++) {
		data >>= 1;

		if(owiReadBit(ow)) {
			data |= 0x80;
		}
	}

	return data;
}

void owiReadRom(owi_t* ow, owiDevice_t* device)
{
	unsigned char index;

	owiSendByte(ow, OWI_READ_ROM);

	for(index = 0; index < 8; index++) {
		device->address[index] = owiReceiveByte(ow);
	}

	return;
}

void owiMatchRom(owi_t* ow, owiDevice_t* device)
{
	unsigned char index;

	owiSendByte(ow, OWI_MATCH_ROM);

	for(index = 0; index < 8; index++) {
		owiSendByte(ow, device->address[index]);
	}

	return;
}

unsigned char owiSearchRom(owi_t *ow, owiDevice_t* device)
{
	unsigned char bitIndex = 1;
	unsigned char * bitAddr = device->address;
	unsigned char newDeviation = 0;
	unsigned char bitMask = 0x01;
	unsigned char bitA;
	unsigned char bitB;

	owiSendByte(ow, OWI_SEARCH_ROM);

	while(bitIndex <= 64) {
		bitA = owiReadBit(ow);
		bitB = owiReadBit(ow);

		if(bitA && bitB) {
			newDeviation = -1;
			return newDeviation;
		} else
			if(bitA ^ bitB) {
				if(bitA) {
					(*bitAddr) |= bitMask;
				} else {
					(*bitAddr) &= ~bitMask;
				}
			} else {
				if(bitIndex == ow->lastDeviation) {
					(*bitAddr) |= bitMask;
				} else
					if(bitIndex > ow->lastDeviation) {
						(*bitAddr) &= ~bitMask;
						newDeviation = bitIndex;
					} else
						if(!((*bitAddr) & bitMask)) {
							newDeviation = bitIndex;
						}
			}

		owiWriteBit(ow, (*bitAddr) & bitMask);

		bitIndex++;
		bitMask <<= 1;

		if(!bitMask) {
			bitMask = 0x01;
			bitAddr++;
		}
	}

	ow->lastDeviation = newDeviation;

	return newDeviation;
}