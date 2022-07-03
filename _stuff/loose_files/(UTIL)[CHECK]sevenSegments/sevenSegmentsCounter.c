/* -----------------------------------------------------------------------------
 * Project:			Counter using a 7-Segments Display
 * File:			sevenSegmentsCounter.c
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	March 18, 2018
 * Purpose:			Controls a 7-Segments Display Common Anode and increments
 *					its value cyclically using TIMER1 in CTC mode. The counter
 *					frequency is 2 Hz, the MCU Clock frequency is 16 MHz, and
 *					the device is an ATmega328P.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <globalDefines.h>
#include <timer1.h>
#include <util/hexToSevenSegments.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

#define DISPLAY_DDR		DDRD
#define DISPLAY_PORT	PORTD

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

vuint8_t counter = 0;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration

	// Variable initialization

	// DISPLAY configuration
	DISPLAY_PORT = 0xFF;
	DISPLAY_DDR = 0xFF;

	// TIMER1 configuration
	timer1Config(TIMER1_MODE_CTC_OCRA, TIMER1_PRESCALER_256);
	timer1SetCompareAValue(31250);
	timer1ActivateCompareAInterrupt();

	// Enable Global Interrupts
	sei();

	while(1){
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(TIMER1_COMPA_vect)
{
	DISPLAY_PORT = hexToSevenSegments(counter, DISPLAY_COMMON_ANODE, FALSE);
	counter = (counter == 9) ? 0 : (counter + 1);
}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
