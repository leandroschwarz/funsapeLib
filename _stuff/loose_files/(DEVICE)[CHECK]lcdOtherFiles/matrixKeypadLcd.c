/* -----------------------------------------------------------------------------
 * Project:			Frequency Meter
 * File:			matrixKeypadLcd.c
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	March 18, 2018
 * Purpose:			This project shows how to use a character LCD and a 4x4
 *					matrix keypad. The keys are shown at the display. The MCU
 *					Clock frequency is 16 MHz, and the device is an ATmega328P.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <globalDefines.h>
#include <util/matrixKeypad.h>
#include <util/lcd.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration
	lcd_t display = lcdCreate();
	matrixKeypad_t keypad = matrixKeypadCreate();
	uint8_t key = 0xFF;

	// Variable initialization

	// LCD configuration
	lcdSetControlPort(&display, &DDRD, &PORTD, PD0, PD1, PD2);
	lcdSetDataPort(&display, &DDRD, &PORTD, &PIND, PD3);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8, TRUE, TRUE);
	lcdStdio(&display);
	printf("  Keypad + LCD  \n");

	// Matrix Keypad configuration
	matrixKeypadSetPort(&keypad, &DDRB, &PORTB, &PINB, PB0, &DDRC, &PORTC, PC1);
	matrixKeypadSetKeys(&keypad, KEYPAD_4X4,	0x07, 0x08, 0x09, 0x0A,
												0x04, 0x05, 0x06, 0x0B,
												0x01, 0x02, 0x03, 0x0C,
												0x0E, 0x00, 0x0F, 0x0D);
	matrixKeypadInit(&keypad, 20);

	// Enable Global Interrupts
	sei();

	while(1){
		key = matrixKeypadRead(&keypad);
		if(key != 0xFF){
			printf("%x", key);
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
