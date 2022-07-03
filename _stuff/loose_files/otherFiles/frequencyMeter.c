/* -----------------------------------------------------------------------------
 * Project:			Frequency Meter
 * File:			frequencyMeter.c
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	March 18, 2018
 * Purpose:			This project implements a frequency meter using the Input
 *					Capture feature of the TIMER1.The input MUST BE a square
 *					wave. The signal is measured and the frequency, the period,
 *					and the duty cycle are shown at a 20x4 character LCD. The
 *					MCU Clock frequency is 16 MHz, and the device is an
 *					ATmega328P.
 * Note:			There are some limitations that must be considered. The
 *					frequency of the signal MUST BE between 1 Hz and 50kHz.
 *					Additionally, the Duty Cycle MUST OBEY the inequation
 *									(100 - f/800) >= D >= f/800
 *					Otherwise, erroneous measurents will be shown. For example,
 *					if the signal has f = 10 kHz, then, 12.5% <= D < 87.5%.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <globalDefines.h>
#include <timer1.h>
#include <util/lcd.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union {
	struct {
		bool_t	risingEdge	: 1;
		uint8_t	unusedBits	: 7;
	};
	uint8_t allFlags;
} systemFlags_t;

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

systemFlags_t systemFlags;
vuint16_t eventStart = 0;
vuint8_t lowOverflow = 0;
vuint16_t lowStart = 0;
vuint16_t lowEnd = 0;
vuint8_t highOverflow = 0;
vuint16_t highStart = 0;
vuint16_t highEnd = 0;
vuint8_t overflowCounter = 0;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration
	lcd_t display = lcdCreate();
	uint8_t lowOverflowAux = 0;
	uint16_t lowStartAux = 0;
	uint16_t lowEndAux = 0;
	uint8_t highOverflowAux = 0;
	uint16_t highStartAux = 0;
	uint16_t highEndAux = 0;
	uint32_t lowPeriod = 0;
	uint32_t highPeriod = 0;
	uint16_t frequency = 0;
	uint64_t aux64 = 0;
	uint32_t aux32 = 0;
	uint16_t dutyCycle = 0;

	// Variable initialization
	systemFlags.allFlags = 0;

	// LCD configuration
	lcdSetControlPort(&display, &DDRC, &PORTC, PC0, PC1, PC2);
	lcdSetDataPort(&display, &DDRD, &PORTD, &PIND, PD4);
	lcdInit(&display, LCD_20X4, LCD_FONT_5X8, TRUE, FALSE);
	lcdStdio(&display);

	// TIMER1 configuration
	timer1Config(TIMER1_MODE_NORMAL, TIMER1_PRESCALER_1);
	timer1OutputConfig(TIMER_PORT_NORMAL, TIMER_PORT_NORMAL);
	timer1InputCaptureConfig(RISING_EDGE, TRUE);
	systemFlags.risingEdge = TRUE;
	timer1ClearOverflowInterruptRequest();
	timer1ActivateOverflowInterrupt();
	timer1ClearInputCaptureInterruptRequest();
	timer1ActivateInputCaptureInterrupt();

	// Enable Global Interrupts
	sei();

	while(1){
		if(systemFlags.risingEdge){
 			ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
				lowOverflowAux = lowOverflow;
				lowStartAux = lowStart;
				lowEndAux = lowEnd;
				highOverflowAux = highOverflow;
				highStartAux = highStart;
				highEndAux = highEnd;
			}
			// Low period
			aux64 = (uint64_t)(lowOverflowAux) * 65536;
			aux64 += (uint64_t)(lowEndAux);
			aux64 -= (uint64_t)(lowStartAux);
			aux64 *= 62500;
			aux64 /= 1000;
			lowPeriod = (uint32_t)aux64;
			// High period
			aux64 = (uint64_t)(highOverflowAux) * 65536;
			aux64 += (uint64_t)(highEndAux);
			aux64 -= (uint64_t)(highStartAux);
			aux64 *= 62500;
			aux64 /= 1000;
			highPeriod = (uint32_t)aux64;
			// Frequency
			aux32 = highPeriod + lowPeriod;
			if(aux32 == 0){
				aux32 = 1;
			}
			aux32 = 1000000000 / aux32;
			frequency = (uint16_t)aux32;
			// Duty cycle
			aux64 = (uint64_t)(highPeriod) * 10000;
			aux64 /= (uint64_t)(highPeriod + lowPeriod);
			dutyCycle = (uint16_t)aux64;
			lcdCursorMoveFirstLine(&display);
			printf("H(ns) = %lu\nL(ns) = %lu\nF(Hz) = %u\nD(%%)  = %u,%02u%%\n", highPeriod, lowPeriod, frequency, (dutyCycle / 100), (dutyCycle % 100));
			_delay_ms(200);
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(TIMER1_OVF_vect)
{
	overflowCounter++;
}

ISR(TIMER1_CAPT_vect)
{
	uint16_t eventEnd = 0;

	eventEnd = timer1GetInputCaptureValue();
	timer1InputCaptureConfig(!systemFlags.risingEdge, TRUE);

	if(systemFlags.risingEdge){		// Low
		lowOverflow = overflowCounter;
		lowStart = eventStart;
		lowEnd = eventEnd;
		systemFlags.risingEdge = FALSE;
	}else{							// High
		highOverflow = overflowCounter;
		highStart = eventStart;
		highEnd = eventEnd;
		systemFlags.risingEdge = TRUE;
	}
	overflowCounter = 0;
	eventStart = eventEnd;
}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
