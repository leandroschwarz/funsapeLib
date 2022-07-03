/*
 * ledMatrix.cpp
 *
 * Created: 06/08/2018 19:04:32
 * Author : Schwarz
 */

#include "gpdse/globalDefines.hpp"
#include "gpdse/timer.hpp"

#define SN74595_DIR		DDRC
#define SN74595_OUT		PORTC
#define SN74595_DATA	PC0
#define SN74595_SHIFT	PC1
#define SN74595_STORE	PC2

#define NUM_DISPLAYS	1
#define NUM_LINES		1

// *INDENT-OFF*
const uint8_t charset[][8] = {
	{0b00111100, 0b01100110, 0b01101110, 0b01110110, 0b01100110, 0b01100110, 0b00111100, 0b00000000},	// 0
	{0b00011000, 0b00011000, 0b00111000, 0b00011000, 0b00011000, 0b00011000, 0b01111110, 0b00000000},	// 1
	{0b00111100, 0b01100110, 0b00000110, 0b00001100, 0b00110000, 0b01100000, 0b01111110, 0b00000000},	// 2
	{0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00000110, 0b01100110, 0b00111100, 0b00000000},	// 3
	{0b00001100, 0b00011100, 0b00101100, 0b01001100, 0b01111110, 0b00001100, 0b00001100, 0b00000000},	// 4
	{0b01111110, 0b01100000, 0b01111100, 0b00000110, 0b00000110, 0b01100110, 0b00111100, 0b00000000},	// 5
	{0b00111100, 0b01100110, 0b01100000, 0b01111100, 0b01100110, 0b01100110, 0b00111100, 0b00000000},	// 6
	{0b01111110, 0b01100110, 0b00001100, 0b00001100, 0b00011000, 0b00011000, 0b00011000, 0b00000000},	// 7
	{0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b00000000},	// 8
	{0b00111100, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b01100110, 0b00111100, 0b00000000}	// 9
};
// *INDENT-ON*

vuint8_t charIndex = 0;
vuint8_t line = 0;

void sn74595TriggerRefresh(void);


int main(void)
{
	// SN74595 configuration
	clrBit(SN74595_OUT, SN74595_DATA);
	setBit(SN74595_DIR, SN74595_DATA);
	clrBit(SN74595_OUT, SN74595_SHIFT);
	setBit(SN74595_DIR, SN74595_SHIFT);
	clrBit(SN74595_OUT, SN74595_STORE);
	setBit(SN74595_DIR, SN74595_STORE);

	clrBit(SN74595_OUT, SN74595_DATA);
	for(uint8_t i = 0; i < 20; i++) {
		setBit(SN74595_OUT, SN74595_SHIFT);
		clrBit(SN74595_OUT, SN74595_SHIFT);
	}
	setBit(SN74595_OUT, SN74595_STORE);
	clrBit(SN74595_OUT, SN74595_STORE);

	// TIMER1 configuration
	timer1.init(TIMER1_MODE_CTC_OCRA, TIMER1_CLOCK_PRESCALER_8);
	timer1.setCompareA(1562);
	timer1.activateCompareAInterrupt();

	sei();

	while(1) {
		_delay_ms(1000);
		charIndex = (charIndex == 9) ? 0 : (charIndex + 1);
	}
}

void sn74595TriggerRefresh(void)
{
	uint8_t aux8 = 0;

	// Select line
	setBit(aux8, line);
	for(uint8_t i = 0; i < 8; i++) {
		if(isBitSet(aux8, i)) {
			setBit(SN74595_OUT, SN74595_DATA);
		} else {
			clrBit(SN74595_OUT, SN74595_DATA);
		}
		setBit(SN74595_OUT, SN74595_SHIFT);
		clrBit(SN74595_OUT, SN74595_SHIFT);
	}
	// Line value
	aux8 = charset[charIndex][line];
	for(uint8_t i = 0; i < 8; i++) {
		if(isBitSet(aux8, 7 - i)) {
			clrBit(SN74595_OUT, SN74595_DATA);
		} else {
			setBit(SN74595_OUT, SN74595_DATA);
		}
		setBit(SN74595_OUT, SN74595_SHIFT);
		clrBit(SN74595_OUT, SN74595_SHIFT);
	}
	setBit(SN74595_OUT, SN74595_DATA);
	setBit(SN74595_OUT, SN74595_STORE);
	clrBit(SN74595_OUT, SN74595_STORE);
	// Update line
	line = (line == 7) ? 0 : (line + 1);
}

ISR(TIMER1_COMPA_vect)
{
	sn74595TriggerRefresh();
}