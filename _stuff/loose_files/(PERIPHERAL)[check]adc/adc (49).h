/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			adc.h
 * Module:			ADC interface
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			2
 * Last edition:	Aprol 22, 2018
 * -------------------------------------------------------------------------- */

#ifndef __ADC_H
#define __ADC_H 2

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum adcPrescaler_e {
	ADC_PRESCALER_2				= 0,
	ADC_PRESCALER_4				= 1,
	ADC_PRESCALER_8				= 2,
	ADC_PRESCALER_16			= 3,
	ADC_PRESCALER_32			= 4,
	ADC_PRESCALER_64			= 5,
	ADC_PRESCALER_128			= 6,
	ADC_PRESCALER_NO_CHANGE		= 7
} adcPrescaler_e;

typedef enum adcReference_e {
	ADC_REFERENCE_EXTERNAL		= 0,
	ADC_REFERENCE_POWER_SUPPLY	= 1,
	ADC_REFERENCE_INTERNAL		= 2,
	ADC_REFERENCE_NO_CHANGE		= 3
} adcReference_e;

typedef enum adcChannel_e {
	ADC_CHANNEL_0				= 0,
	ADC_CHANNEL_1				= 1,
	ADC_CHANNEL_2				= 2,
	ADC_CHANNEL_3				= 3,
	ADC_CHANNEL_4				= 4,
	ADC_CHANNEL_5				= 5,
	ADC_CHANNEL_6				= 6,
	ADC_CHANNEL_7				= 7,
	ADC_CHANNEL_TEMPERATURE		= 8,
	ADC_CHANNEL_BAND_GAP		= 9,
	ADC_CHANNEL_GND				= 10,
	ADC_CHANNEL_NO_CHANGE		= 11
} adcChannel_e;

typedef enum adcDigitalInputs_f {
	ADC_INPUT_0		= 1,
	ADC_INPUT_1		= 2,
	ADC_INPUT_2		= 4,
	ADC_INPUT_3		= 8,
	ADC_INPUT_4		= 16,
	ADC_INPUT_5		= 32,
	ADC_INPUT_6		= 64,
	ADC_INPUT_7		= 128
} adcDigitalInputs_f;

typedef enum adcMode_e {
	ADC_MODE_SINGLE_CONVERSION		= 0,
	ADC_MODE_AUTO_CONTINUOUS		= 1,
	ADC_MODE_AUTO_ANALOG_COMP		= 2,
	ADC_MODE_AUTO_INT0				= 3,
	ADC_MODE_AUTO_TIMER0_COMPA		= 4,
	ADC_MODE_AUTO_TIMER0_OVERFLOW	= 5,
	ADC_MODE_AUTO_TIMER1_COMPB		= 6,
	ADC_MODE_AUTO_TIMER1_OVERFLOW	= 7,
	ADC_MODE_AUTO_TIMER1_CAPTURE	= 8,
	ADC_MODE_NO_CHANGE				= 9
} adcMode_e;

typedef enum adcDataPresentation_e {
	ADC_ADJUST_RESULT_LEFT			= 0,
	ADC_ADJUST_RESULT_RIGHT			= 1,
	ADC_ADJUST_RESULT_NO_CHANGE		= 2
} adcDataPresentation_e;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

error_e	adcConfig(adcMode_e mode, adcReference_e reference, adcPrescaler_e  prescaler);
error_e	adcSelectChannel(adcChannel_e channel);
error_e	adcDataPresentation(adcDataPresentation_e presentation);

// -----------------------------------------------------------------------------
// Static function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Enable adc digital inputs
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcEnableDigitalInput(adcDigitalInputs_f flagInputs)
{
	DIDR0 &= (~flagInputs);
	return;
}

/* -----------------------------------------------------------------------------
 * Enable adc digital inputs
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcDisableDigitalInput(adcDigitalInputs_f flagInputs)
{
	DIDR0 |= flagInputs;
	return;
}

/* -----------------------------------------------------------------------------
 * Enables the adc module
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcEnable(void)
{
	setBit(ADCSRA, ADEN);
	return;
}

/* -----------------------------------------------------------------------------
 * Disables the adc module
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcDisable(void)
{
	clrBit(ADCSRA, ADEN);
	return;
}

/* -----------------------------------------------------------------------------
 * Clears the adc interrupt request
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcClearInterruptRequest(void)
{
	setBit(ADCSRA, ADIF);
	return;
}

/* -----------------------------------------------------------------------------
 * Enables the adc interrupt
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcActivateInterrupt(void)
{
	setBit(ADCSRA, ADIE);
	return;
}

/* -----------------------------------------------------------------------------
 * Disables the adc interrupt
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcDeactivateInterrupt(void)
{
	clrBit(ADCSRA, ADIE);
	return;
}

/* -----------------------------------------------------------------------------
 * Starts an adc conversion
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcStartConversion(void)
{
	setBit(ADCSRA, ADSC);
	return;
}

/* -----------------------------------------------------------------------------
 * Returns if an adc conversion is running
 * -------------------------------------------------------------------------- */

bool_t inline __attribute__((always_inline)) adcIsBusy(void)
{
	return isBitSet(ADCSRA, ADSC);
}

/* -----------------------------------------------------------------------------
 * Waits until the current conversion is finished
 * -------------------------------------------------------------------------- */

void inline __attribute__((always_inline)) adcWaitUntilConversionFinish(void)
{
	waitUntilBitIsClear(ADCSRA, ADSC);
	return;
}

#endif
