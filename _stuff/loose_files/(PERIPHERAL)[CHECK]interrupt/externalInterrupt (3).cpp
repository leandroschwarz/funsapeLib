/*!
	\file		externalInterrupt.c
	\brief		External Interrupts interface for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		May 15, 2018
*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "externalInterrupt.hpp"
#if __EXTERNAL_INTERRUPT_HPP != 1000
	#error [externaInterrupt.cpp] Error 101 - Build mismatch on header and source code files.
#endif

#define INT0_DIR		DDRD
#define INT0_OUT		PORTD
#define INT0_IN			PIND
#define INT0_BIT		PD0
#define INT1_DIR		DDRD
#define INT1_OUT		PORTD
#define INT1_IN			PIND
#define INT1_BIT		PD1
#define INT2_DIR		DDRD
#define INT2_OUT		PORTD
#define INT2_IN			PIND
#define INT2_BIT		PD2
#define INT3_DIR		DDRD
#define INT3_OUT		PORTD
#define INT3_IN			PIND
#define INT3_BIT		PD3
#define INT4_DIR		DDRE
#define INT4_OUT		PORTE
#define INT4_IN			PINE
#define INT4_BIT		PE4
#define INT5_DIR		DDRE
#define INT5_OUT		PORTE
#define INT5_IN			PINE
#define INT5_BIT		PE5
#define INT6_DIR		DDRE
#define INT6_OUT		PORTE
#define INT6_IN			PINE
#define INT6_BIT		PE6
#define INT7_DIR		DDRE
#define INT7_OUT		PORTE
#define INT7_IN			PINE
#define INT7_BIT		PE7


#define PCINT0_DIR		DDRB
#define PCINT0_OUT		PORTB
#define PCINT0_IN		PINB
#define PCINT0_BIT		PB0
#define PCINT1_DIR		DDRB
#define PCINT1_OUT		PORTB
#define PCINT1_IN		PINB
#define PCINT1_BIT		PB1
#define PCINT2_DIR		DDRB
#define PCINT2_OUT		PORTB
#define PCINT2_IN		PINB
#define PCINT2_BIT		PB2
#define PCINT3_DIR		DDRB
#define PCINT3_OUT		PORTB
#define PCINT3_IN		PINB
#define PCINT3_BIT		PB3
#define PCINT4_DIR		DDRB
#define PCINT4_OUT		PORTB
#define PCINT4_IN		PINB
#define PCINT4_BIT		PB4
#define PCINT5_DIR		DDRB
#define PCINT5_OUT		PORTB
#define PCINT5_IN		PINB
#define PCINT5_BIT		PB5
#define PCINT6_DIR		DDRB
#define PCINT6_OUT		PORTB
#define PCINT6_IN		PINB
#define PCINT6_BIT		PB6
#define PCINT7_DIR		DDRB
#define PCINT7_OUT		PORTB
#define PCINT7_IN		PINB
#define PCINT7_BIT		PB7
#define PCINT8_DIR		DDRC
#define PCINT8_OUT		PORTC
#define PCINT8_IN		PINC
#define PCINT8_BIT		PC0
#define PCINT9_DIR		DDRC
#define PCINT9_OUT		PORTC
#define PCINT9_IN		PINC
#define PCINT9_BIT		PC1
#define PCINT10_DIR		DDRC
#define PCINT10_OUT		PORTC
#define PCINT10_IN		PINC
#define PCINT10_BIT		PC2
#define PCINT11_DIR		DDRC
#define PCINT11_OUT		PORTC
#define PCINT11_IN		PINC
#define PCINT11_BIT		PC3
#define PCINT12_DIR		DDRC
#define PCINT12_OUT		PORTC
#define PCINT12_IN		PINC
#define PCINT12_BIT		PC4
#define PCINT13_DIR		DDRC
#define PCINT13_OUT		PORTC
#define PCINT13_IN		PINC
#define PCINT13_BIT		PC5
#define PCINT14_DIR		DDRC
#define PCINT14_OUT		PORTC
#define PCINT14_IN		PINC
#define PCINT14_BIT		PC6
#define PCINT16_DIR		DDRD
#define PCINT16_OUT		PORTD
#define PCINT16_IN		PIND
#define PCINT16_BIT		PD0
#define PCINT17_DIR		DDRD
#define PCINT17_OUT		PORTD
#define PCINT17_IN		PIND
#define PCINT17_BIT		PD1
#define PCINT18_DIR		DDRD
#define PCINT18_OUT		PORTD
#define PCINT18_IN		PIND
#define PCINT18_BIT		PD2
#define PCINT19_DIR		DDRD
#define PCINT19_OUT		PORTD
#define PCINT19_IN		PIND
#define PCINT19_BIT		PD3
#define PCINT20_DIR		DDRD
#define PCINT20_OUT		PORTD
#define PCINT20_IN		PIND
#define PCINT20_BIT		PD4
#define PCINT21_DIR		DDRD
#define PCINT21_OUT		PORTD
#define PCINT21_IN		PIND
#define PCINT21_BIT		PD5
#define PCINT22_DIR		DDRD
#define PCINT22_OUT		PORTD
#define PCINT22_IN		PIND
#define PCINT22_BIT		PD6
#define PCINT23_DIR		DDRD
#define PCINT23_OUT		PORTD
#define PCINT23_IN		PIND
#define PCINT23_BIT		PD7

/* -----------------------------------------------------------------------------
 * Configures the external interrupts INT
 * -------------------------------------------------------------------------- */

error_e int7Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicrb = EICRB;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT7_OUT, INT7_BIT);
		setBit(INT7_DIR, INT7_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT7_OUT, INT7_BIT);
		setBit(INT7_DIR, INT7_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT7_OUT, INT7_BIT);
		clrBit(INT7_DIR, INT7_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT7_OUT, INT7_BIT);
		clrBit(INT7_DIR, INT7_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT7_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicrb, ISC71);
		clrBit(eicrb, ISC70);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicrb, ISC71);
		setBit(eicrb, ISC70);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicrb, ISC71);
		clrBit(eicrb, ISC70);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicrb, ISC71);
		setBit(eicrb, ISC70);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT7_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRB = eicrb;

	return ERROR_NONE;
}

error_e int6Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicrb = EICRB;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT6_OUT, INT6_BIT);
		setBit(INT6_DIR, INT6_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT6_OUT, INT6_BIT);
		setBit(INT6_DIR, INT6_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT6_OUT, INT6_BIT);
		clrBit(INT6_DIR, INT6_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT6_OUT, INT6_BIT);
		clrBit(INT6_DIR, INT6_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT6_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicrb, ISC61);
		clrBit(eicrb, ISC60);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicrb, ISC61);
		setBit(eicrb, ISC60);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicrb, ISC61);
		clrBit(eicrb, ISC60);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicrb, ISC61);
		setBit(eicrb, ISC60);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT6_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRB = eicrb;

	return ERROR_NONE;
}

error_e int5Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicrb = EICRB;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT5_OUT, INT5_BIT);
		setBit(INT5_DIR, INT5_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT5_OUT, INT5_BIT);
		setBit(INT5_DIR, INT5_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT5_OUT, INT5_BIT);
		clrBit(INT5_DIR, INT5_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT5_OUT, INT5_BIT);
		clrBit(INT5_DIR, INT5_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT5_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicrb, ISC51);
		clrBit(eicrb, ISC50);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicrb, ISC51);
		setBit(eicrb, ISC50);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicrb, ISC51);
		clrBit(eicrb, ISC50);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicrb, ISC51);
		setBit(eicrb, ISC50);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT5_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRB = eicrb;

	return ERROR_NONE;
}

error_e int4Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicrb = EICRB;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT4_OUT, INT4_BIT);
		setBit(INT4_DIR, INT4_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT4_OUT, INT4_BIT);
		setBit(INT4_DIR, INT4_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT4_OUT, INT4_BIT);
		clrBit(INT4_DIR, INT4_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT4_OUT, INT4_BIT);
		clrBit(INT4_DIR, INT4_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT4_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicrb, ISC41);
		clrBit(eicrb, ISC40);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicrb, ISC41);
		setBit(eicrb, ISC40);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicrb, ISC41);
		clrBit(eicrb, ISC40);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicrb, ISC41);
		setBit(eicrb, ISC40);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT4_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRB = eicrb;

	return ERROR_NONE;
}

error_e int3Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicra = EICRA;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT3_OUT, INT3_BIT);
		setBit(INT3_DIR, INT3_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT3_OUT, INT3_BIT);
		setBit(INT3_DIR, INT3_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT3_OUT, INT3_BIT);
		clrBit(INT3_DIR, INT3_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT3_OUT, INT3_BIT);
		clrBit(INT3_DIR, INT3_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT3_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicra, ISC31);
		clrBit(eicra, ISC30);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicra, ISC31);
		setBit(eicra, ISC30);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicra, ISC31);
		clrBit(eicra, ISC30);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicra, ISC31);
		setBit(eicra, ISC30);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT3_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRA = eicra;

	return ERROR_NONE;
}

error_e int2Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicra = EICRA;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT2_OUT, INT2_BIT);
		setBit(INT2_DIR, INT2_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT2_OUT, INT2_BIT);
		setBit(INT2_DIR, INT2_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT2_OUT, INT2_BIT);
		clrBit(INT2_DIR, INT2_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT2_OUT, INT2_BIT);
		clrBit(INT2_DIR, INT2_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT2_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicra, ISC21);
		clrBit(eicra, ISC20);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicra, ISC21);
		setBit(eicra, ISC20);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicra, ISC21);
		clrBit(eicra, ISC20);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicra, ISC21);
		setBit(eicra, ISC20);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT2_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRA = eicra;

	return ERROR_NONE;
}





error_e int1Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicra = EICRA;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT1_OUT, INT1_BIT);
		setBit(INT1_DIR, INT1_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT1_OUT, INT1_BIT);
		setBit(INT1_DIR, INT1_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT1_OUT, INT1_BIT);
		clrBit(INT1_DIR, INT1_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT1_OUT, INT1_BIT);
		clrBit(INT1_DIR, INT1_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT1_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicra, ISC11);
		clrBit(eicra, ISC10);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicra, ISC11);
		setBit(eicra, ISC10);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicra, ISC11);
		clrBit(eicra, ISC10);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicra, ISC11);
		setBit(eicra, ISC10);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT1_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRA = eicra;

	return ERROR_NONE;
}

error_e int0Config(portMode_e outputMode, intSenseMode_e sense)
{
	uint8_t eicra = EICRA;

	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT0_OUT, INT0_BIT);
		setBit(INT0_DIR, INT0_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT0_OUT, INT0_BIT);
		setBit(INT0_DIR, INT0_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT0_OUT, INT0_BIT);
		clrBit(INT0_DIR, INT0_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT0_OUT, INT0_BIT);
		clrBit(INT0_DIR, INT0_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return ERROR_INT0_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	switch(sense) {
	case INT_SENSE_LOW_LEVEL:
		clrBit(eicra, ISC01);
		clrBit(eicra, ISC00);
		break;
	case INT_SENSE_ANY_EDGE:
		clrBit(eicra, ISC01);
		setBit(eicra, ISC00);
		break;
	case INT_SENSE_FALLING_EDGE:
		setBit(eicra, ISC01);
		clrBit(eicra, ISC00);
		break;
	case INT_SENSE_RISING_EDGE:
		setBit(eicra, ISC01);
		setBit(eicra, ISC00);
		break;
	case INT_SENSE_NO_CHANGE:
		break;
	default:
		return ERROR_INT0_UNSUPPORTED_CONFIGURATION_SENSE_MODE;
	}
	EICRA = eicra;

	return ERROR_NONE;
}

/* -----------------------------------------------------------------------------
 * Activates the external interrupts PCINT
 * -------------------------------------------------------------------------- */

error_e pcint2ActivatePins(pcint2Pins_f pinsList, portMode_e outputMode)
{
	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		if(pinsList & PCINT23_PIN) {
			clrBit(PCINT23_OUT, PCINT23_BIT);
			setBit(PCINT23_DIR, PCINT23_BIT);
		}
		if(pinsList & PCINT22_PIN) {
			clrBit(PCINT22_OUT, PCINT22_BIT);
			setBit(PCINT22_DIR, PCINT22_BIT);
		}
		if(pinsList & PCINT21_PIN) {
			clrBit(PCINT21_OUT, PCINT21_BIT);
			setBit(PCINT21_DIR, PCINT21_BIT);
		}
		if(pinsList & PCINT20_PIN) {
			clrBit(PCINT20_OUT, PCINT20_BIT);
			setBit(PCINT20_DIR, PCINT20_BIT);
		}
		if(pinsList & PCINT19_PIN) {
			clrBit(PCINT19_OUT, PCINT19_BIT);
			setBit(PCINT19_DIR, PCINT19_BIT);
		}
		if(pinsList & PCINT18_PIN) {
			clrBit(PCINT18_OUT, PCINT18_BIT);
			setBit(PCINT18_DIR, PCINT18_BIT);
		}
		if(pinsList & PCINT17_PIN) {
			clrBit(PCINT17_OUT, PCINT17_BIT);
			setBit(PCINT17_DIR, PCINT17_BIT);
		}
		if(pinsList & PCINT16_PIN) {
			clrBit(PCINT16_OUT, PCINT16_BIT);
			setBit(PCINT16_DIR, PCINT16_BIT);
		}
		break;
	case PORT_OUTPUT_HIGH:
		if(pinsList & PCINT23_PIN) {
			setBit(PCINT23_OUT, PCINT23_BIT);
			setBit(PCINT23_DIR, PCINT23_BIT);
		}
		if(pinsList & PCINT22_PIN) {
			setBit(PCINT22_OUT, PCINT22_BIT);
			setBit(PCINT22_DIR, PCINT22_BIT);
		}
		if(pinsList & PCINT21_PIN) {
			setBit(PCINT21_OUT, PCINT21_BIT);
			setBit(PCINT21_DIR, PCINT21_BIT);
		}
		if(pinsList & PCINT20_PIN) {
			setBit(PCINT20_OUT, PCINT20_BIT);
			setBit(PCINT20_DIR, PCINT20_BIT);
		}
		if(pinsList & PCINT19_PIN) {
			setBit(PCINT19_OUT, PCINT19_BIT);
			setBit(PCINT19_DIR, PCINT19_BIT);
		}
		if(pinsList & PCINT18_PIN) {
			setBit(PCINT18_OUT, PCINT18_BIT);
			setBit(PCINT18_DIR, PCINT18_BIT);
		}
		if(pinsList & PCINT17_PIN) {
			setBit(PCINT17_OUT, PCINT17_BIT);
			setBit(PCINT17_DIR, PCINT17_BIT);
		}
		if(pinsList & PCINT16_PIN) {
			setBit(PCINT16_OUT, PCINT16_BIT);
			setBit(PCINT16_DIR, PCINT16_BIT);
		}
		break;
	case PORT_INPUT_TRISTATE:
		if(pinsList & PCINT23_PIN) {
			clrBit(PCINT23_OUT, PCINT23_BIT);
			clrBit(PCINT23_DIR, PCINT23_BIT);
		}
		if(pinsList & PCINT22_PIN) {
			clrBit(PCINT22_OUT, PCINT22_BIT);
			clrBit(PCINT22_DIR, PCINT22_BIT);
		}
		if(pinsList & PCINT21_PIN) {
			clrBit(PCINT21_OUT, PCINT21_BIT);
			clrBit(PCINT21_DIR, PCINT21_BIT);
		}
		if(pinsList & PCINT20_PIN) {
			clrBit(PCINT20_OUT, PCINT20_BIT);
			clrBit(PCINT20_DIR, PCINT20_BIT);
		}
		if(pinsList & PCINT19_PIN) {
			clrBit(PCINT19_OUT, PCINT19_BIT);
			clrBit(PCINT19_DIR, PCINT19_BIT);
		}
		if(pinsList & PCINT18_PIN) {
			clrBit(PCINT18_OUT, PCINT18_BIT);
			clrBit(PCINT18_DIR, PCINT18_BIT);
		}
		if(pinsList & PCINT17_PIN) {
			clrBit(PCINT17_OUT, PCINT17_BIT);
			clrBit(PCINT17_DIR, PCINT17_BIT);
		}
		if(pinsList & PCINT16_PIN) {
			clrBit(PCINT16_OUT, PCINT16_BIT);
			clrBit(PCINT16_DIR, PCINT16_BIT);
		}
		break;
	case PORT_INPUT_PULL_UP:
		if(pinsList & PCINT23_PIN) {
			setBit(PCINT23_OUT, PCINT23_BIT);
			clrBit(PCINT23_DIR, PCINT23_BIT);
		}
		if(pinsList & PCINT22_PIN) {
			setBit(PCINT22_OUT, PCINT22_BIT);
			clrBit(PCINT22_DIR, PCINT22_BIT);
		}
		if(pinsList & PCINT21_PIN) {
			setBit(PCINT21_OUT, PCINT21_BIT);
			clrBit(PCINT21_DIR, PCINT21_BIT);
		}
		if(pinsList & PCINT20_PIN) {
			setBit(PCINT20_OUT, PCINT20_BIT);
			clrBit(PCINT20_DIR, PCINT20_BIT);
		}
		if(pinsList & PCINT19_PIN) {
			setBit(PCINT19_OUT, PCINT19_BIT);
			clrBit(PCINT19_DIR, PCINT19_BIT);
		}
		if(pinsList & PCINT18_PIN) {
			setBit(PCINT18_OUT, PCINT18_BIT);
			clrBit(PCINT18_DIR, PCINT18_BIT);
		}
		if(pinsList & PCINT17_PIN) {
			setBit(PCINT17_OUT, PCINT17_BIT);
			clrBit(PCINT17_DIR, PCINT17_BIT);
		}
		if(pinsList & PCINT16_PIN) {
			setBit(PCINT16_OUT, PCINT16_BIT);
			clrBit(PCINT16_DIR, PCINT16_BIT);
		}
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return 	ERROR_PCINT2_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	if(pinsList & PCINT23_PIN) {
		setBit(PCMSK2, PCINT23);
	}
	if(pinsList & PCINT22_PIN) {
		setBit(PCMSK2, PCINT22);
	}
	if(pinsList & PCINT21_PIN) {
		setBit(PCMSK2, PCINT21);
	}
	if(pinsList & PCINT20_PIN) {
		setBit(PCMSK2, PCINT20);
	}
	if(pinsList & PCINT19_PIN) {
		setBit(PCMSK2, PCINT19);
	}
	if(pinsList & PCINT18_PIN) {
		setBit(PCMSK2, PCINT18);
	}
	if(pinsList & PCINT17_PIN) {
		setBit(PCMSK2, PCINT17);
	}
	if(pinsList & PCINT16_PIN) {
		setBit(PCMSK2, PCINT16);
	}

	return ERROR_NONE;
}

error_e pcint1ActivatePins(pcint1Pins_f pinsList, portMode_e outputMode)
{
	switch(outputMode) {

	case PORT_OUTPUT_LOW:
		if(pinsList & PCINT14_PIN) {
			clrBit(PCINT14_OUT, PCINT14_BIT);
			setBit(PCINT14_DIR, PCINT14_BIT);
		}
		if(pinsList & PCINT13_PIN) {
			clrBit(PCINT13_OUT, PCINT13_BIT);
			setBit(PCINT13_DIR, PCINT13_BIT);
		}
		if(pinsList & PCINT12_PIN) {
			clrBit(PCINT12_OUT, PCINT12_BIT);
			setBit(PCINT12_DIR, PCINT12_BIT);
		}
		if(pinsList & PCINT11_PIN) {
			clrBit(PCINT11_OUT, PCINT11_BIT);
			setBit(PCINT11_DIR, PCINT11_BIT);
		}
		if(pinsList & PCINT10_PIN) {
			clrBit(PCINT10_OUT, PCINT10_BIT);
			setBit(PCINT10_DIR, PCINT10_BIT);
		}
		if(pinsList & PCINT9_PIN) {
			clrBit(PCINT9_OUT, PCINT9_BIT);
			setBit(PCINT9_DIR, PCINT9_BIT);
		}
		if(pinsList & PCINT8_PIN) {
			clrBit(PCINT8_OUT, PCINT8_BIT);
			setBit(PCINT8_DIR, PCINT8_BIT);
		}
		break;
	case PORT_OUTPUT_HIGH:
		if(pinsList & PCINT14_PIN) {
			setBit(PCINT14_OUT, PCINT14_BIT);
			setBit(PCINT14_DIR, PCINT14_BIT);
		}
		if(pinsList & PCINT13_PIN) {
			setBit(PCINT13_OUT, PCINT13_BIT);
			setBit(PCINT13_DIR, PCINT13_BIT);
		}
		if(pinsList & PCINT12_PIN) {
			setBit(PCINT12_OUT, PCINT12_BIT);
			setBit(PCINT12_DIR, PCINT12_BIT);
		}
		if(pinsList & PCINT11_PIN) {
			setBit(PCINT11_OUT, PCINT11_BIT);
			setBit(PCINT11_DIR, PCINT11_BIT);
		}
		if(pinsList & PCINT10_PIN) {
			setBit(PCINT10_OUT, PCINT10_BIT);
			setBit(PCINT10_DIR, PCINT10_BIT);
		}
		if(pinsList & PCINT9_PIN) {
			setBit(PCINT9_OUT, PCINT9_BIT);
			setBit(PCINT9_DIR, PCINT9_BIT);
		}
		if(pinsList & PCINT8_PIN) {
			setBit(PCINT8_OUT, PCINT8_BIT);
			setBit(PCINT8_DIR, PCINT8_BIT);
		}
		break;
	case PORT_INPUT_TRISTATE:
		if(pinsList & PCINT14_PIN) {
			clrBit(PCINT14_OUT, PCINT14_BIT);
			clrBit(PCINT14_DIR, PCINT14_BIT);
		}
		if(pinsList & PCINT13_PIN) {
			clrBit(PCINT13_OUT, PCINT13_BIT);
			clrBit(PCINT13_DIR, PCINT13_BIT);
		}
		if(pinsList & PCINT12_PIN) {
			clrBit(PCINT12_OUT, PCINT12_BIT);
			clrBit(PCINT12_DIR, PCINT12_BIT);
		}
		if(pinsList & PCINT11_PIN) {
			clrBit(PCINT11_OUT, PCINT11_BIT);
			clrBit(PCINT11_DIR, PCINT11_BIT);
		}
		if(pinsList & PCINT10_PIN) {
			clrBit(PCINT10_OUT, PCINT10_BIT);
			clrBit(PCINT10_DIR, PCINT10_BIT);
		}
		if(pinsList & PCINT9_PIN) {
			clrBit(PCINT9_OUT, PCINT9_BIT);
			clrBit(PCINT9_DIR, PCINT9_BIT);
		}
		if(pinsList & PCINT8_PIN) {
			clrBit(PCINT8_OUT, PCINT8_BIT);
			clrBit(PCINT8_DIR, PCINT8_BIT);
		}
		break;
	case PORT_INPUT_PULL_UP:
		if(pinsList & PCINT14_PIN) {
			setBit(PCINT14_OUT, PCINT14_BIT);
			clrBit(PCINT14_DIR, PCINT14_BIT);
		}
		if(pinsList & PCINT13_PIN) {
			setBit(PCINT13_OUT, PCINT13_BIT);
			clrBit(PCINT13_DIR, PCINT13_BIT);
		}
		if(pinsList & PCINT12_PIN) {
			setBit(PCINT12_OUT, PCINT12_BIT);
			clrBit(PCINT12_DIR, PCINT12_BIT);
		}
		if(pinsList & PCINT11_PIN) {
			setBit(PCINT11_OUT, PCINT11_BIT);
			clrBit(PCINT11_DIR, PCINT11_BIT);
		}
		if(pinsList & PCINT10_PIN) {
			setBit(PCINT10_OUT, PCINT10_BIT);
			clrBit(PCINT10_DIR, PCINT10_BIT);
		}
		if(pinsList & PCINT9_PIN) {
			setBit(PCINT9_OUT, PCINT9_BIT);
			clrBit(PCINT9_DIR, PCINT9_BIT);
		}
		if(pinsList & PCINT8_PIN) {
			setBit(PCINT8_OUT, PCINT8_BIT);
			clrBit(PCINT8_DIR, PCINT8_BIT);
		}
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return 	ERROR_PCINT1_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	if(pinsList & PCINT14_PIN) {
		setBit(PCMSK1, PCINT14);
	}
	if(pinsList & PCINT13_PIN) {
		setBit(PCMSK1, PCINT13);
	}
	if(pinsList & PCINT12_PIN) {
		setBit(PCMSK1, PCINT12);
	}
	if(pinsList & PCINT11_PIN) {
		setBit(PCMSK1, PCINT11);
	}
	if(pinsList & PCINT10_PIN) {
		setBit(PCMSK1, PCINT10);
	}
	if(pinsList & PCINT9_PIN) {
		setBit(PCMSK1, PCINT9);
	}
	if(pinsList & PCINT8_PIN) {
		setBit(PCMSK1, PCINT8);
	}

	return ERROR_NONE;
}

error_e pcint0ActivatePins(pcint0Pins_f pinsList, portMode_e outputMode)
{
	switch(outputMode) {
	case PORT_OUTPUT_LOW:
		if(pinsList & PCINT7_PIN) {
			clrBit(PCINT7_OUT, PCINT7_BIT);
			setBit(PCINT7_DIR, PCINT7_BIT);
		}
		if(pinsList & PCINT6_PIN) {
			clrBit(PCINT6_OUT, PCINT6_BIT);
			setBit(PCINT6_DIR, PCINT6_BIT);
		}
		if(pinsList & PCINT5_PIN) {
			clrBit(PCINT5_OUT, PCINT5_BIT);
			setBit(PCINT5_DIR, PCINT5_BIT);
		}
		if(pinsList & PCINT4_PIN) {
			clrBit(PCINT4_OUT, PCINT4_BIT);
			setBit(PCINT4_DIR, PCINT4_BIT);
		}
		if(pinsList & PCINT3_PIN) {
			clrBit(PCINT3_OUT, PCINT3_BIT);
			setBit(PCINT3_DIR, PCINT3_BIT);
		}
		if(pinsList & PCINT2_PIN) {
			clrBit(PCINT2_OUT, PCINT2_BIT);
			setBit(PCINT2_DIR, PCINT2_BIT);
		}
		if(pinsList & PCINT1_PIN) {
			clrBit(PCINT1_OUT, PCINT1_BIT);
			setBit(PCINT1_DIR, PCINT1_BIT);
		}
		if(pinsList & PCINT0_PIN) {
			clrBit(PCINT0_OUT, PCINT0_BIT);
			setBit(PCINT0_DIR, PCINT0_BIT);
		}
		break;
	case PORT_OUTPUT_HIGH:
		if(pinsList & PCINT7_PIN) {
			setBit(PCINT7_OUT, PCINT7_BIT);
			setBit(PCINT7_DIR, PCINT7_BIT);
		}
		if(pinsList & PCINT6_PIN) {
			setBit(PCINT6_OUT, PCINT6_BIT);
			setBit(PCINT6_DIR, PCINT6_BIT);
		}
		if(pinsList & PCINT5_PIN) {
			setBit(PCINT5_OUT, PCINT5_BIT);
			setBit(PCINT5_DIR, PCINT5_BIT);
		}
		if(pinsList & PCINT4_PIN) {
			setBit(PCINT4_OUT, PCINT4_BIT);
			setBit(PCINT4_DIR, PCINT4_BIT);
		}
		if(pinsList & PCINT3_PIN) {
			setBit(PCINT3_OUT, PCINT3_BIT);
			setBit(PCINT3_DIR, PCINT3_BIT);
		}
		if(pinsList & PCINT2_PIN) {
			setBit(PCINT2_OUT, PCINT2_BIT);
			setBit(PCINT2_DIR, PCINT2_BIT);
		}
		if(pinsList & PCINT1_PIN) {
			setBit(PCINT1_OUT, PCINT1_BIT);
			setBit(PCINT1_DIR, PCINT1_BIT);
		}
		if(pinsList & PCINT0_PIN) {
			setBit(PCINT0_OUT, PCINT0_BIT);
			setBit(PCINT0_DIR, PCINT0_BIT);
		}
		break;
	case PORT_INPUT_TRISTATE:
		if(pinsList & PCINT7_PIN) {
			clrBit(PCINT7_OUT, PCINT7_BIT);
			clrBit(PCINT7_DIR, PCINT7_BIT);
		}
		if(pinsList & PCINT6_PIN) {
			clrBit(PCINT6_OUT, PCINT6_BIT);
			clrBit(PCINT6_DIR, PCINT6_BIT);
		}
		if(pinsList & PCINT5_PIN) {
			clrBit(PCINT5_OUT, PCINT5_BIT);
			clrBit(PCINT5_DIR, PCINT5_BIT);
		}
		if(pinsList & PCINT4_PIN) {
			clrBit(PCINT4_OUT, PCINT4_BIT);
			clrBit(PCINT4_DIR, PCINT4_BIT);
		}
		if(pinsList & PCINT3_PIN) {
			clrBit(PCINT3_OUT, PCINT3_BIT);
			clrBit(PCINT3_DIR, PCINT3_BIT);
		}
		if(pinsList & PCINT2_PIN) {
			clrBit(PCINT2_OUT, PCINT2_BIT);
			clrBit(PCINT2_DIR, PCINT2_BIT);
		}
		if(pinsList & PCINT1_PIN) {
			clrBit(PCINT1_OUT, PCINT1_BIT);
			clrBit(PCINT1_DIR, PCINT1_BIT);
		}
		if(pinsList & PCINT0_PIN) {
			clrBit(PCINT0_OUT, PCINT0_BIT);
			clrBit(PCINT0_DIR, PCINT0_BIT);
		}
		break;
	case PORT_INPUT_PULL_UP:
		if(pinsList & PCINT7_PIN) {
			setBit(PCINT7_OUT, PCINT7_BIT);
			clrBit(PCINT7_DIR, PCINT7_BIT);
		}
		if(pinsList & PCINT6_PIN) {
			setBit(PCINT6_OUT, PCINT6_BIT);
			clrBit(PCINT6_DIR, PCINT6_BIT);
		}
		if(pinsList & PCINT5_PIN) {
			setBit(PCINT5_OUT, PCINT5_BIT);
			clrBit(PCINT5_DIR, PCINT5_BIT);
		}
		if(pinsList & PCINT4_PIN) {
			setBit(PCINT4_OUT, PCINT4_BIT);
			clrBit(PCINT4_DIR, PCINT4_BIT);
		}
		if(pinsList & PCINT3_PIN) {
			setBit(PCINT3_OUT, PCINT3_BIT);
			clrBit(PCINT3_DIR, PCINT3_BIT);
		}
		if(pinsList & PCINT2_PIN) {
			setBit(PCINT2_OUT, PCINT2_BIT);
			clrBit(PCINT2_DIR, PCINT2_BIT);
		}
		if(pinsList & PCINT1_PIN) {
			setBit(PCINT1_OUT, PCINT1_BIT);
			clrBit(PCINT1_DIR, PCINT1_BIT);
		}
		if(pinsList & PCINT0_PIN) {
			setBit(PCINT0_OUT, PCINT0_BIT);
			clrBit(PCINT0_DIR, PCINT0_BIT);
		}
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return 	ERROR_PCINT0_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE;
	}

	if(pinsList & PCINT7_PIN) {
		setBit(PCMSK0, PCINT7);
	}
	if(pinsList & PCINT6_PIN) {
		setBit(PCMSK0, PCINT6);
	}
	if(pinsList & PCINT5_PIN) {
		setBit(PCMSK0, PCINT5);
	}
	if(pinsList & PCINT4_PIN) {
		setBit(PCMSK0, PCINT4);
	}
	if(pinsList & PCINT3_PIN) {
		setBit(PCMSK0, PCINT3);
	}
	if(pinsList & PCINT2_PIN) {
		setBit(PCMSK0, PCINT2);
	}
	if(pinsList & PCINT1_PIN) {
		setBit(PCMSK0, PCINT1);
	}
	if(pinsList & PCINT0_PIN) {
		setBit(PCMSK0, PCINT0);
	}

	return ERROR_NONE;
}

/* -----------------------------------------------------------------------------
 * Deactivates the external interrupts PCINT
 * -------------------------------------------------------------------------- */

error_e pcint2DeactivatePins(pcint2Pins_f pinsList)
{
	if(pinsList & PCINT23_PIN) {
		clrBit(PCMSK2, PCINT23);
	}
	if(pinsList & PCINT22_PIN) {
		clrBit(PCMSK2, PCINT22);
	}
	if(pinsList & PCINT21_PIN) {
		clrBit(PCMSK2, PCINT21);
	}
	if(pinsList & PCINT20_PIN) {
		clrBit(PCMSK2, PCINT20);
	}
	if(pinsList & PCINT19_PIN) {
		clrBit(PCMSK2, PCINT19);
	}
	if(pinsList & PCINT18_PIN) {
		clrBit(PCMSK2, PCINT18);
	}
	if(pinsList & PCINT17_PIN) {
		clrBit(PCMSK2, PCINT17);
	}
	if(pinsList & PCINT16_PIN) {
		clrBit(PCMSK2, PCINT16);
	}

	return ERROR_NONE;
}

error_e pcint1DeactivatePins(pcint1Pins_f pinsList)
{
	if(pinsList & PCINT14_PIN) {
		clrBit(PCMSK1, PCINT14);
	}
	if(pinsList & PCINT13_PIN) {
		clrBit(PCMSK1, PCINT13);
	}
	if(pinsList & PCINT12_PIN) {
		clrBit(PCMSK1, PCINT12);
	}
	if(pinsList & PCINT11_PIN) {
		clrBit(PCMSK1, PCINT11);
	}
	if(pinsList & PCINT10_PIN) {
		clrBit(PCMSK1, PCINT10);
	}
	if(pinsList & PCINT9_PIN) {
		clrBit(PCMSK1, PCINT9);
	}
	if(pinsList & PCINT8_PIN) {
		clrBit(PCMSK1, PCINT8);
	}

	return ERROR_NONE;
}

error_e pcint0DeactivatePins(pcint0Pins_f pinsList)
{
	if(pinsList & PCINT7_PIN) {
		clrBit(PCMSK0, PCINT7);
	}
	if(pinsList & PCINT6_PIN) {
		clrBit(PCMSK0, PCINT6);
	}
	if(pinsList & PCINT5_PIN) {
		clrBit(PCMSK0, PCINT5);
	}
	if(pinsList & PCINT4_PIN) {
		clrBit(PCMSK0, PCINT4);
	}
	if(pinsList & PCINT3_PIN) {
		clrBit(PCMSK0, PCINT3);
	}
	if(pinsList & PCINT2_PIN) {
		clrBit(PCMSK0, PCINT2);
	}
	if(pinsList & PCINT1_PIN) {
		clrBit(PCMSK0, PCINT1);
	}
	if(pinsList & PCINT0_PIN) {
		clrBit(PCMSK0, PCINT0);
	}

	return ERROR_NONE;
}
