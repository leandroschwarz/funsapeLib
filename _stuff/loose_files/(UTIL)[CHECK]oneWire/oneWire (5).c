/*
 *	onewire.c
 *
 *	Created: 18/11/2014 12:09:50
 *	Author: Hazael
 */ 

#include "onewire.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void owiPinConfigure(onewire *ow, reg_t* ddrReg, reg_t* portReg, reg_t* pinReg, unsigned char pin)
{
	ow->ddrReg	= ddrReg;
	ow->portReg	= portReg;
	ow->pinReg	= pinReg;
	ow->pin		= pin;
	ow->mask	= (1 << pin);
	
	return;
}

static void owiWriteBit(onewire* ow, unsigned char bit)
{
	reg_t sreg;
	sreg = SREG;
	cli();
	
	*(ow->ddrReg) |= ow->mask;
	*(ow->portReg) &= ~ow->mask;
	
	if(bit)	_delay_us(__DELAY_A);
	else	_delay_us(__DELAY_C);
	
	*(ow->ddrReg) &= ~ow->mask;
	
	if(bit)	_delay_us(__DELAY_B);
	else	_delay_us(__DELAY_D);
	
	SREG = sreg;
	
	return;
}

static unsigned char owiReadBit(onewire* ow)
{
	unsigned char bitRead = 0;
	
	reg_t sreg;
	sreg = SREG;
	cli();
	
	*(ow->ddrReg) |= ow->mask;
	*(ow->portReg) &= ~ow->mask;
	
	_delay_us(__DELAY_A);
	
	*(ow->ddrReg) &= ~ow->mask;
	
	_delay_us(__DELAY_E);
	
	bitRead = (*(ow->pinReg) & ow->mask);
	
	_delay_us(__DELAY_F);
	
	SREG = sreg;
	
	return bitRead;
}

uint8_t owiDetectPresence(onewire* ow)
{
	unsigned char bitRead = 0;
	
	reg_t sreg;
	sreg = SREG;
	cli();
	
	*(ow->ddrReg) |= ow->mask;
	*(ow->portReg) &= ~ow->mask;
	
	_delay_us(__DELAY_H);
	
	*(ow->ddrReg) &= ~ow->mask;
	
	_delay_us(__DELAY_I);
	
	bitRead = (*(ow->pinReg) & ow->mask);
	
	_delay_us(__DELAY_J);
	
	SREG = sreg;
	
	return bitRead;
}

void owiSendByte(onewire* ow, unsigned char data)
{
	unsigned char bitsSent;
	
	for(bitsSent = 0; bitsSent < 8; bitsSent++) {
		owiWriteBit(ow, (data & 1));
		data >>= 1;
	}
	
	return;
}

unsigned char owiReceiveByte(onewire* ow)
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

void owiReadRom(onewire* ow)
{
	unsigned char index;
	
	owiSendByte(ow, 0x33);
	for(index = 0; index < 8; index++)
		ow->address[index] = owiReceiveByte(ow);
	
	return;
}

void owiMatchRom(onewire* ow)
{
	unsigned char index;
	
	owiSendByte(ow, OWI_MATCH_ROM);
	for(index = 0; index < 8; index++)
		owiSendByte(ow, ow->address[index]);
	
	return;
}

unsigned char owiSearchRom(onewire *ow)
{
	unsigned char bitIndex = 0;
	unsigned char byteIndex = 0;
	unsigned char newDeviation = 0;
	unsigned char bitMask = 0x01;
	unsigned char bitA;
	unsigned char bitB;
	
	owiSendByte(ow, OWI_SEARCH_ROM);
	
	while (bitIndex < 64) {
		bitA = owiReadBit(ow);
		bitB = owiReadBit(ow);
		
		if(bitA && bitB) {
			newDeviation = -1;
			return newDeviation;
		} else if(bitA ^ bitB) {
			if(bitA)
				ow->address[byteIndex] |= bitMask;
			else
				ow->address[byteIndex] &= ~bitMask;
		} else {
			if(bitIndex == ow->lastDeviation) {
				ow->address[byteIndex] |= bitMask;
			} else if(bitIndex > ow->lastDeviation) {
				ow->address[byteIndex] &= ~bitMask;
				newDeviation = bitIndex;
			} else if(!(ow->address[byteIndex] & bitMask)) {
				newDeviation = bitIndex;
			}
		}
		
		owiWriteBit(ow, ow->address[byteIndex] & bitMask);
		
		bitIndex++;
		bitMask <<= 1;
		if(!bitMask) {
			bitMask = 0x01;
			byteIndex++;
		}
	}
	
	return newDeviation;
}