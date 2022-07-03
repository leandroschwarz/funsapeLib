/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			adc.c
 * Module:			ADC interface
 * Author:			Leandro Schwarz
 * Version:			13.0
 * Last edition:	2017-06-17
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "adc.h"
#if __ADC_H != 130
	#error Error 101 - Version mismatch on header and source code files (adc).
#endif

/* -----------------------------------------------------------------------------
 * Configures the adc module
 * -------------------------------------------------------------------------- */

resultValue_t adcConfig(adcMode_t mode, adcReference_t reference, adcPrescaler_t  prescaler)
{
	uint8 reg = 0;

	if(reference != ADC_REFERENCE_NO_CHANGE){
		reg = ADMUX;
		reg &= ~(0x03 << REFS0);
		switch(reference){
			case ADC_REFERENCE_EXTERNAL:		break;
			case ADC_REFERENCE_POWER_SUPPLY:	reg |= 1 << REFS0;	break;
			case ADC_REFERENCE_INTERNAL:		reg |= 3 << REFS0;	break;
			default:							return RESULT_UNSUPPORTED_ADC_REFERENCE;
		}
		ADMUX = reg;
	}

	if(prescaler != ADC_PRESCALER_NO_CHANGE){
		reg = ADCSRA;
		reg &= ~(0x07 << ADPS0);
		switch(prescaler){
			case ADC_PRESCALER_OFF:		break;
			case ADC_PRESCALER_2:		reg |= 1 << ADPS0;	break;
			case ADC_PRESCALER_4:		reg |= 2 << ADPS0;	break;
			case ADC_PRESCALER_8:		reg |= 3 << ADPS0;	break;
			case ADC_PRESCALER_16:		reg |= 4 << ADPS0;	break;
			case ADC_PRESCALER_32:		reg |= 5 << ADPS0;	break;
			case ADC_PRESCALER_64:		reg |= 6 << ADPS0;	break;
			case ADC_PRESCALER_128:		reg |= 7 << ADPS0;	break;
			default:					return RESULT_UNSUPPORTED_ADC_PRESCALER_VALUE;
		}
		ADCSRA = reg;
	}

	if(mode != ADC_MODE_NO_CHANGE){
		if(mode == ADC_MODE_SINGLE_CONVERSION)
			clrBit(ADCSRA, ADATE);
		else{
			reg = ADCSRB;
			reg &= ~(0x07 << ADTS0);
			switch(mode){
				case ADC_MODE_AUTO_ANALOG_COMP:		reg |= (1 << ADTS0);	break;
				case ADC_MODE_AUTO_INT0:			reg |= (2 << ADTS0);	break;
				case ADC_MODE_AUTO_TIMER0_COMPA:	reg |= (3 << ADTS0);	break;
				case ADC_MODE_AUTO_TIMER0_OVERFLOW:	reg |= (4 << ADTS0);	break;
				case ADC_MODE_AUTO_TIMER1_COMPB:	reg |= (5 << ADTS0);	break;
				case ADC_MODE_AUTO_TIMER1_OVERFLOW:	reg |= (6 << ADTS0);	break;
				case ADC_MODE_AUTO_TIMER1_CAPTURE:	reg |= (7 << ADTS0);	break;
				default:							return RESULT_UNSUPPORTED_ADC_MODE;	break;
			}
			ADCSRB = reg;
			setBit(ADCSRA, ADATE);
		}
	}

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Selects the adc channel
 * -------------------------------------------------------------------------- */

resultValue_t adcSelectChannel(adcChannel_t channel)
{
	uint8 reg = ADMUX;

	if(channel != ADC_CHANNEL_NO_CHANGE){
		reg &= ~(0x0F << MUX0);
		switch(channel){
			case ADC_CHANNEL_0:				reg |= (0 << MUX0);		break;
			case ADC_CHANNEL_1:				reg |= (1 << MUX0);		break;
			case ADC_CHANNEL_2:				reg |= (2 << MUX0);		break;
			case ADC_CHANNEL_3:				reg |= (3 << MUX0);		break;
			case ADC_CHANNEL_4:				reg |= (4 << MUX0);		break;
			case ADC_CHANNEL_5:				reg |= (5 << MUX0);		break;
			case ADC_CHANNEL_6:				reg |= (6 << MUX0);		break;
			case ADC_CHANNEL_7:				reg |= (7 << MUX0);		break;
			case ADC_CHANNEL_TEMPERATURE:	reg |= (8 << MUX0);		break;
			case ADC_CHANNEL_BAND_GAP:		reg |= (14 << MUX0);	break;
			case ADC_CHANNEL_GND:			reg |= (15 << MUX0);	break;
			default:						return RESULT_UNSUPPORTED_ADC_CHANNEL;	break;
		}
		ADMUX = reg;
	}

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Enable adc digital inputs
 * -------------------------------------------------------------------------- */

resultValue_t adcEnableDigitalInput(adcDigitalInputs_t flagInputs)
{
	DIDR0 &= (~flagInputs);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Enable adc digital inputs
 * -------------------------------------------------------------------------- */

resultValue_t adcDisableDigitalInput(adcDigitalInputs_t flagInputs)
{
	DIDR0 |= flagInputs;

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Sets the adc data presentation
 * -------------------------------------------------------------------------- */

resultValue_t adcDataPresentation(adcDataPresentation_t presentation)
{
	switch(presentation){
		case ADC_ADJUST_RESULT_LEFT:			setBit(ADMUX, ADLAR);	break;
		case ADC_ADJUST_RESULT_RIGHT:			clrBit(ADMUX, ADLAR);	break;
		case ADC_ADJUST_RESULT_NO_CHANGE:		return RESULT_OK;		break;
	}

	return RESULT_UNSUPPORTED_ADC_ADJUST_RESULT;
}

/* -----------------------------------------------------------------------------
 * Enables the adc module
 * -------------------------------------------------------------------------- */

resultValue_t adcEnable(void)
{
	setBit(ADCSRA, ADEN);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Disables the adc module
 * -------------------------------------------------------------------------- */

resultValue_t adcDisable(void)
{
	clrBit(ADCSRA, ADEN);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Clears the adc interrupt request
 * -------------------------------------------------------------------------- */

resultValue_t adcClearInterruptRequest(void)
{
	setBit(ADCSRA, ADIF);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Enables the adc interrupt
 * -------------------------------------------------------------------------- */

resultValue_t adcActivateInterrupt(void)
{
	setBit(ADCSRA, ADIE);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Disables the adc interrupt
 * -------------------------------------------------------------------------- */

resultValue_t adcDeactivateInterrupt(void)
{
	clrBit(ADCSRA, ADIE);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Starts an adc conversion
 * -------------------------------------------------------------------------- */

resultValue_t adcStartConversion(void)
{
	setBit(ADCSRA, ADSC);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Returns if an adc conversion is running
 * -------------------------------------------------------------------------- */

bool_t adcIsBusy(void)
{
	return isBitSet(ADCSRA, ADSC);
}

/* -----------------------------------------------------------------------------
 * Waits until the current conversion is finished
 * -------------------------------------------------------------------------- */

resultValue_t adcWaitUntilConversionFinish(void)
{
	waitUntilBitIsClear(ADCSRA, ADSC);
	return RESULT_OK;
}
