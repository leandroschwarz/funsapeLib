/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			externalInterrupt.c
 * Module:			External Interrupt interface
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "externalInterrupt.h"
#if __EXTERNAL_INTERRUPT_H != 1
#error Error 101 - Build mismatch on header and source code files (externalInterrupt).
#endif

#define INT0_DDR		DDRD
#define INT0_PORT		PORTD
#define INT0_PIN		PIND
#define INT0_PORT_BIT		PD2
#define INT1_DDR		DDRD
#define INT1_PORT		PORTD
#define INT1_PIN		PIND
#define INT1_PORT_BIT		PD3
#define PCINT0_DDR		DDRB
#define PCINT0_PORT		PORTB
#define PCINT0_PIN		PINB
#define PCINT0_PORT_BIT		PB0
#define PCINT1_DDR		DDRB
#define PCINT1_PORT		PORTB
#define PCINT1_PIN		PINB
#define PCINT1_PORT_BIT		PB1
#define PCINT2_DDR		DDRB
#define PCINT2_PORT		PORTB
#define PCINT2_PIN		PINB
#define PCINT2_PORT_BIT		PB2
#define PCINT3_DDR		DDRB
#define PCINT3_PORT		PORTB
#define PCINT3_PIN		PINB
#define PCINT3_PORT_BIT		PB3
#define PCINT4_DDR		DDRB
#define PCINT4_PORT		PORTB
#define PCINT4_PIN		PINB
#define PCINT4_PORT_BIT		PB4
#define PCINT5_DDR		DDRB
#define PCINT5_PORT		PORTB
#define PCINT5_PIN		PINB
#define PCINT5_PORT_BIT		PB5
#define PCINT6_DDR		DDRB
#define PCINT6_PORT		PORTB
#define PCINT6_PIN		PINB
#define PCINT6_PORT_BIT		PB6
#define PCINT7_DDR		DDRB
#define PCINT7_PORT		PORTB
#define PCINT7_PIN		PINB
#define PCINT7_PORT_BIT		PB7
#define PCINT8_DDR		DDRC
#define PCINT8_PORT		PORTC
#define PCINT8_PIN		PINC
#define PCINT8_BIT		PC0
#define PCINT9_DDR		DDRC
#define PCINT9_PORT		PORTC
#define PCINT9_PIN		PINC
#define PCINT9_BIT		PC1
#define PCINT10_DDR		DDRC
#define PCINT10_PORT	PORTC
#define PCINT10_PIN		PINC
#define PCINT10_BIT		PC2
#define PCINT11_DDR		DDRC
#define PCINT11_PORT	PORTC
#define PCINT11_PIN		PINC
#define PCINT11_BIT		PC3
#define PCINT12_DDR		DDRC
#define PCINT12_PORT	PORTC
#define PCINT12_PIN		PINC
#define PCINT12_BIT		PC4
#define PCINT13_DDR		DDRC
#define PCINT13_PORT	PORTC
#define PCINT13_PIN		PINC
#define PCINT13_BIT		PC5
#define PCINT14_DDR		DDRC
#define PCINT14_PORT	PORTC
#define PCINT14_PIN		PINC
#define PCINT14_BIT		PC6
#define PCINT16_DDR		DDRD
#define PCINT16_PORT	PORTD
#define PCINT16_PIN		PIND
#define PCINT16_BIT		PD0
#define PCINT17_DDR		DDRD
#define PCINT17_PORT	PORTD
#define PCINT17_PIN		PIND
#define PCINT17_BIT		PD1
#define PCINT18_DDR		DDRD
#define PCINT18_PORT	PORTD
#define PCINT18_PIN		PIND
#define PCINT18_BIT		PD2
#define PCINT19_DDR		DDRD
#define PCINT19_PORT	PORTD
#define PCINT19_PIN		PIND
#define PCINT19_BIT		PD3
#define PCINT20_DDR		DDRD
#define PCINT20_PORT	PORTD
#define PCINT20_PIN		PIND
#define PCINT20_BIT		PD4
#define PCINT21_DDR		DDRD
#define PCINT21_PORT	PORTD
#define PCINT21_PIN		PIND
#define PCINT21_BIT		PD5
#define PCINT22_DDR		DDRD
#define PCINT22_PORT	PORTD
#define PCINT22_PIN		PIND
#define PCINT22_BIT		PD6
#define PCINT23_DDR		DDRD
#define PCINT23_PORT	PORTD
#define PCINT23_PIN		PIND
#define PCINT23_BIT		PD7

/* -----------------------------------------------------------------------------
 * Configures the external interrupts INT
 * -------------------------------------------------------------------------- */

resultValue_t int1Config(portMode_t port, intSenseMode_t sense)
{
	uint8_t eicra = INT0_SENSE_REG;

	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(INT1_PORT, INT1_PORT_BIT);
		setBit(INT1_DDR, INT1_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT1_PORT, INT1_PORT_BIT);
		setBit(INT1_DDR, INT1_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT1_PORT, INT1_PORT_BIT);
		clrBit(INT1_DDR, INT1_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT1_PORT, INT1_PORT_BIT);
		clrBit(INT1_DDR, INT1_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}

	switch (sense) {
	case SENSE_LOW_LEVEL:
		clrBit(eicra, INT1_SENSE_BIT_1);
		clrBit(eicra, INT1_SENSE_BIT_0);
		break;
	case SENSE_ANY_EDGE:
		clrBit(eicra, INT1_SENSE_BIT_1);
		setBit(eicra, INT1_SENSE_BIT_0);
		break;
	case SENSE_FALLING_EDGE:
		setBit(eicra, INT1_SENSE_BIT_1);
		clrBit(eicra, INT1_SENSE_BIT_0);
		break;
	case SENSE_RISING_EDGE:
		setBit(eicra, INT1_SENSE_BIT_1);
		setBit(eicra, INT1_SENSE_BIT_0);
		break;
	case SENSE_NO_CHANGE:
		break;
	default:
		return RESULT_INT_SENSE_VALUE_UNSUPPORTED;
	}
	INT0_SENSE_REG = eicra;

	return RESULT_OK;
}

resultValue_t int0Config(portMode_t port, intSenseMode_t sense)
{
	uint8_t eicra = INT0_SENSE_REG;

	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(INT0_PORT, INT0_PORT_BIT);
		setBit(INT0_DDR, INT0_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT0_PORT, INT0_PORT_BIT);
		setBit(INT0_DDR, INT0_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT0_PORT, INT0_PORT_BIT);
		clrBit(INT0_DDR, INT0_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT0_PORT, INT0_PORT_BIT);
		clrBit(INT0_DDR, INT0_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}

	switch (sense) {
	case SENSE_LOW_LEVEL:
		clrBit(eicra, INT0_SENSE_BIT_1);
		clrBit(eicra, INT0_SENSE_BIT_0);
		break;
	case SENSE_ANY_EDGE:
		clrBit(eicra, INT0_SENSE_BIT_1);
		setBit(eicra, INT0_SENSE_BIT_0);
		break;
	case SENSE_FALLING_EDGE:
		setBit(eicra, INT0_SENSE_BIT_1);
		clrBit(eicra, INT0_SENSE_BIT_0);
		break;
	case SENSE_RISING_EDGE:
		setBit(eicra, INT0_SENSE_BIT_1);
		setBit(eicra, INT0_SENSE_BIT_0);
		break;
	case SENSE_NO_CHANGE:
		break;
	default:
		return RESULT_INT_SENSE_VALUE_UNSUPPORTED;
	}
	INT0_SENSE_REG = eicra;

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Activates the external interrupts INT
 * -------------------------------------------------------------------------- */

void int1ActivateInterrupt(void)
{
	setBit(EIMSK, INT1);
	return;
}

void int0ActivateInterrupt(void)
{
	setBit(EIMSK, INT0);
	return;
}

/* -----------------------------------------------------------------------------
 * Deactivates the external interrupts INT
 * -------------------------------------------------------------------------- */

void int1DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT1);
	return;
}

void int0DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT0);
	return;
}

/* -----------------------------------------------------------------------------
 * Clears the requests of the external interrupts INT
 * -------------------------------------------------------------------------- */

void int1ClearInterruptRequest(void)
{
	setBit(EIFR, INTF1);
	return;
}

void int0ClearInterruptRequest(void)
{
	setBit(EIFR, INTF0);
	return;
}

/* -----------------------------------------------------------------------------
 * Enables the external interrupts PCINT
 * -------------------------------------------------------------------------- */

void pcint23_16Enable(void)
{
	setBit(PCICR, PCIE2);
	return;
}

void pcint14_8Enable(void)
{
	setBit(PCICR, PCIE1);
	return;
}

void pcint7_0Enable(void)
{
	setBit(PCICR, PCIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Disables the external interrupts PCINT
 * -------------------------------------------------------------------------- */

void pcint23_16Disable(void)
{
	clrBit(PCICR, PCIE2);
	return;
}

void pcint14_8Disable(void)
{
	clrBit(PCICR, PCIE1);
	return;
}

void pcint7_0Disable(void)
{
	clrBit(PCICR, PCIE0);
	return;
}

/* -----------------------------------------------------------------------------
 * Clears the resquests of the external interrupts PCINT
 * -------------------------------------------------------------------------- */

void pcint23_16ClearInterruptRequest(void)
{
	setBit(PCIFR, PCIF2);
	return;
}

void pcint14_8ClearInterruptRequest(void)
{
	setBit(PCIFR, PCIF1);
	return;
}

void pcint7_0ClearInterruptRequest(void)
{
	setBit(PCIFR, PCIF0);
	return;
}



/* -----------------------------------------------------------------------------
 * Activates the external interrupts PCINT
 * -------------------------------------------------------------------------- */

resultValue_t pcint23ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT23_PORT, PCINT23_BIT);
		setBit(PCINT23_DDR, PCINT23_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT23_PORT, PCINT23_BIT);
		setBit(PCINT23_DDR, PCINT23_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT23_PORT, PCINT23_BIT);
		clrBit(PCINT23_DDR, PCINT23_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT23_PORT, PCINT23_BIT);
		clrBit(PCINT23_DDR, PCINT23_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT23);

	return RESULT_OK;
}

resultValue_t pcint22ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT22_PORT, PCINT22_BIT);
		setBit(PCINT22_DDR, PCINT22_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT22_PORT, PCINT22_BIT);
		setBit(PCINT22_DDR, PCINT22_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT22_PORT, PCINT22_BIT);
		clrBit(PCINT22_DDR, PCINT22_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT22_PORT, PCINT22_BIT);
		clrBit(PCINT22_DDR, PCINT22_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT22);

	return RESULT_OK;
}

resultValue_t pcint21ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT21_PORT, PCINT21_BIT);
		setBit(PCINT21_DDR, PCINT21_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT21_PORT, PCINT21_BIT);
		setBit(PCINT21_DDR, PCINT21_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT21_PORT, PCINT21_BIT);
		clrBit(PCINT21_DDR, PCINT21_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT21_PORT, PCINT21_BIT);
		clrBit(PCINT21_DDR, PCINT21_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT21);

	return RESULT_OK;
}

resultValue_t pcint20ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT20_PORT, PCINT20_BIT);
		setBit(PCINT20_DDR, PCINT20_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT20_PORT, PCINT20_BIT);
		setBit(PCINT20_DDR, PCINT20_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT20_PORT, PCINT20_BIT);
		clrBit(PCINT20_DDR, PCINT20_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT20_PORT, PCINT20_BIT);
		clrBit(PCINT20_DDR, PCINT20_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT20);

	return RESULT_OK;
}

resultValue_t pcint19ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT19_PORT, PCINT19_BIT);
		setBit(PCINT19_DDR, PCINT19_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT19_PORT, PCINT19_BIT);
		setBit(PCINT19_DDR, PCINT19_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT19_PORT, PCINT19_BIT);
		clrBit(PCINT19_DDR, PCINT19_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT19_PORT, PCINT19_BIT);
		clrBit(PCINT19_DDR, PCINT19_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT19);

	return RESULT_OK;
}

resultValue_t pcint18ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT18_PORT, PCINT18_BIT);
		setBit(PCINT18_DDR, PCINT18_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT18_PORT, PCINT18_BIT);
		setBit(PCINT18_DDR, PCINT18_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT18_PORT, PCINT18_BIT);
		clrBit(PCINT18_DDR, PCINT18_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT18_PORT, PCINT18_BIT);
		clrBit(PCINT18_DDR, PCINT18_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT18);

	return RESULT_OK;
}

resultValue_t pcint17ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT17_PORT, PCINT17_BIT);
		setBit(PCINT17_DDR, PCINT17_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT17_PORT, PCINT17_BIT);
		setBit(PCINT17_DDR, PCINT17_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT17_PORT, PCINT17_BIT);
		clrBit(PCINT17_DDR, PCINT17_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT17_PORT, PCINT17_BIT);
		clrBit(PCINT17_DDR, PCINT17_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT17);

	return RESULT_OK;
}

resultValue_t pcint16ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT16_PORT, PCINT16_BIT);
		setBit(PCINT16_DDR, PCINT16_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT16_PORT, PCINT16_BIT);
		setBit(PCINT16_DDR, PCINT16_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT16_PORT, PCINT16_BIT);
		clrBit(PCINT16_DDR, PCINT16_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT16_PORT, PCINT16_BIT);
		clrBit(PCINT16_DDR, PCINT16_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK2, PCINT16);

	return RESULT_OK;
}

resultValue_t pcint14ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT14_PORT, PCINT14_BIT);
		setBit(PCINT14_DDR, PCINT14_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT14_PORT, PCINT14_BIT);
		setBit(PCINT14_DDR, PCINT14_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT14_PORT, PCINT14_BIT);
		clrBit(PCINT14_DDR, PCINT14_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT14_PORT, PCINT14_BIT);
		clrBit(PCINT14_DDR, PCINT14_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT14);

	return RESULT_OK;
}

resultValue_t pcint13ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT13_PORT, PCINT13_BIT);
		setBit(PCINT13_DDR, PCINT13_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT13_PORT, PCINT13_BIT);
		setBit(PCINT13_DDR, PCINT13_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT13_PORT, PCINT13_BIT);
		clrBit(PCINT13_DDR, PCINT13_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT13_PORT, PCINT13_BIT);
		clrBit(PCINT13_DDR, PCINT13_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT13);

	return RESULT_OK;
}

resultValue_t pcint12ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT12_PORT, PCINT12_BIT);
		setBit(PCINT12_DDR, PCINT12_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT12_PORT, PCINT12_BIT);
		setBit(PCINT12_DDR, PCINT12_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT12_PORT, PCINT12_BIT);
		clrBit(PCINT12_DDR, PCINT12_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT12_PORT, PCINT12_BIT);
		clrBit(PCINT12_DDR, PCINT12_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT12);

	return RESULT_OK;
}

resultValue_t pcint11ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT11_PORT, PCINT11_BIT);
		setBit(PCINT11_DDR, PCINT11_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT11_PORT, PCINT11_BIT);
		setBit(PCINT11_DDR, PCINT11_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT11_PORT, PCINT11_BIT);
		clrBit(PCINT11_DDR, PCINT11_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT11_PORT, PCINT11_BIT);
		clrBit(PCINT11_DDR, PCINT11_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT11);

	return RESULT_OK;
}

resultValue_t pcint10ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT10_PORT, PCINT10_BIT);
		setBit(PCINT10_DDR, PCINT10_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT10_PORT, PCINT10_BIT);
		setBit(PCINT10_DDR, PCINT10_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT10_PORT, PCINT10_BIT);
		clrBit(PCINT10_DDR, PCINT10_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT10_PORT, PCINT10_BIT);
		clrBit(PCINT10_DDR, PCINT10_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT10);

	return RESULT_OK;
}

resultValue_t pcint9ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT9_PORT, PCINT9_BIT);
		setBit(PCINT9_DDR, PCINT9_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT9_PORT, PCINT9_BIT);
		setBit(PCINT9_DDR, PCINT9_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT9_PORT, PCINT9_BIT);
		clrBit(PCINT9_DDR, PCINT9_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT9_PORT, PCINT9_BIT);
		clrBit(PCINT9_DDR, PCINT9_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT9);

	return RESULT_OK;
}

resultValue_t pcint8ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT8_PORT, PCINT8_BIT);
		setBit(PCINT8_DDR, PCINT8_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT8_PORT, PCINT8_BIT);
		setBit(PCINT8_DDR, PCINT8_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT8_PORT, PCINT8_BIT);
		clrBit(PCINT8_DDR, PCINT8_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT8_PORT, PCINT8_BIT);
		clrBit(PCINT8_DDR, PCINT8_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK1, PCINT8);

	return RESULT_OK;
}

resultValue_t pcint7ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT7_PORT, PCINT7_PORT_BIT);
		setBit(PCINT7_DDR, PCINT7_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT7_PORT, PCINT7_PORT_BIT);
		setBit(PCINT7_DDR, PCINT7_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT7_PORT, PCINT7_PORT_BIT);
		clrBit(PCINT7_DDR, PCINT7_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT7_PORT, PCINT7_PORT_BIT);
		clrBit(PCINT7_DDR, PCINT7_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT7);

	return RESULT_OK;
}

resultValue_t pcint6ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT6_PORT, PCINT6_PORT_BIT);
		setBit(PCINT6_DDR, PCINT6_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT6_PORT, PCINT6_PORT_BIT);
		setBit(PCINT6_DDR, PCINT6_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT6_PORT, PCINT6_PORT_BIT);
		clrBit(PCINT6_DDR, PCINT6_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT6_PORT, PCINT6_PORT_BIT);
		clrBit(PCINT6_DDR, PCINT6_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT6);

	return RESULT_OK;
}

resultValue_t pcint5ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT5_PORT, PCINT5_PORT_BIT);
		setBit(PCINT5_DDR, PCINT5_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT5_PORT, PCINT5_PORT_BIT);
		setBit(PCINT5_DDR, PCINT5_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT5_PORT, PCINT5_PORT_BIT);
		clrBit(PCINT5_DDR, PCINT5_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT5_PORT, PCINT5_PORT_BIT);
		clrBit(PCINT5_DDR, PCINT5_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT5);

	return RESULT_OK;
}

resultValue_t pcint4ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT4_PORT, PCINT4_PORT_BIT);
		setBit(PCINT4_DDR, PCINT4_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT4_PORT, PCINT4_PORT_BIT);
		setBit(PCINT4_DDR, PCINT4_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT4_PORT, PCINT4_PORT_BIT);
		clrBit(PCINT4_DDR, PCINT4_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT4_PORT, PCINT4_PORT_BIT);
		clrBit(PCINT4_DDR, PCINT4_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT4);

	return RESULT_OK;
}

resultValue_t pcint3ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT3_PORT, PCINT3_PORT_BIT);
		setBit(PCINT3_DDR, PCINT3_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT3_PORT, PCINT3_PORT_BIT);
		setBit(PCINT3_DDR, PCINT3_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT3_PORT, PCINT3_PORT_BIT);
		clrBit(PCINT3_DDR, PCINT3_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT3_PORT, PCINT3_PORT_BIT);
		clrBit(PCINT3_DDR, PCINT3_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT3);

	return RESULT_OK;
}

resultValue_t pcint2ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT2_PORT, PCINT2_PORT_BIT);
		setBit(PCINT2_DDR, PCINT2_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT2_PORT, PCINT2_PORT_BIT);
		setBit(PCINT2_DDR, PCINT2_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT2_PORT, PCINT2_PORT_BIT);
		clrBit(PCINT2_DDR, PCINT2_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT2_PORT, PCINT2_PORT_BIT);
		clrBit(PCINT2_DDR, PCINT2_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT2);

	return RESULT_OK;
}

resultValue_t pcint1ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT1_PORT, PCINT1_PORT_BIT);
		setBit(PCINT1_DDR, PCINT1_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT1_PORT, PCINT1_PORT_BIT);
		setBit(PCINT1_DDR, PCINT1_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT1_PORT, PCINT1_PORT_BIT);
		clrBit(PCINT1_DDR, PCINT1_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT1_PORT, PCINT1_PORT_BIT);
		clrBit(PCINT1_DDR, PCINT1_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT1);

	return RESULT_OK;
}

resultValue_t pcint0ActivateInterrupt(portMode_t port)
{
	switch (port) {
	case PORT_OUTPUT_LOW:
		clrBit(PCINT0_PORT, PCINT0_PORT_BIT);
		setBit(PCINT0_DDR, PCINT0_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(PCINT0_PORT, PCINT0_PORT_BIT);
		setBit(PCINT0_DDR, PCINT0_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(PCINT0_PORT, PCINT0_PORT_BIT);
		clrBit(PCINT0_DDR, PCINT0_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(PCINT0_PORT, PCINT0_PORT_BIT);
		clrBit(PCINT0_DDR, PCINT0_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		return RESULT_PORT_VALUE_UNSUPPORTED;
	}
	setBit(PCMSK0, PCINT0);

	return RESULT_OK;
}

/* -----------------------------------------------------------------------------
 * Deactivates the external interrupts PCINT
 * -------------------------------------------------------------------------- */

void pcint23DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT23);
	return;
}

void pcint22DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT22);
	return;
}

void pcint21DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT21);
	return;
}

void pcint20DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT20);
	return;
}

void pcint19DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT19);
	return;
}

void pcint18DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT18);
	return;
}

void pcint17DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT17);
	return;
}

void pcint16DeactivateInterrupt(void)
{
	clrBit(PCMSK2, PCINT16);
	return;
}

void pcint14DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT14);
	return;
}

void pcint13DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT13);
	return;
}

void pcint12DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT12);
	return;
}

void pcint11DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT11);
	return;
}

void pcint10DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT10);
	return;
}

void pcint9DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT9);
	return;
}

void pcint8DeactivateInterrupt(void)
{
	clrBit(PCMSK1, PCINT8);
	return;
}

void pcint7DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT7);
	return;
}

void pcint6DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT6);
	return;
}

void pcint5DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT5);
	return;
}

void pcint4DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT4);
	return;
}

void pcint3DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT3);
	return;
}

void pcint2DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT2);
	return;
}

void pcint1DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT1);
	return;
}

void pcint0DeactivateInterrupt(void)
{
	clrBit(PCMSK0, PCINT0);
	return;
}
