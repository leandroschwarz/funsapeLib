/*!
	\file		timer2.c
	\brief		TIMER2 interface for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 27, 2018
*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "timer2.hpp"
#if __TIMER2_HPP != 1002
#error Error 101 - Build mismatch on header and source code files (timer2).
#endif

/* -----------------------------------------------------------------------------
 * Configures the timer2 mode and prescaler
 * -------------------------------------------------------------------------- */

error_e timer2Config(timer2Mode_e mode, timer2Prescaler_e prescaler)
{
	uint8_t regA = TCCR2A;
	uint8_t regB = TCCR2B;

	if (prescaler != TIMER2_PRESCALER_NO_CHANGE) {
		clrMask(regB, 0x07, CS20);
		switch (prescaler) {
		case TIMER2_CLOCK_DISABLE:
			break;
		case TIMER2_PRESCALER_1:
			setMask(regB, 1, CS20);
			break;
		case TIMER2_PRESCALER_8:
			setMask(regB, 2, CS20);
			break;
		case TIMER2_PRESCALER_32:
			setMask(regB, 3, CS20);
			break;
		case TIMER2_PRESCALER_64:
			setMask(regB, 4, CS20);
			break;
		case TIMER2_PRESCALER_128:
			setMask(regB, 5, CS20);
			break;
		case TIMER2_PRESCALER_256:
			setMask(regB, 6, CS20);
			break;
		case TIMER2_PRESCALER_1024:
			setMask(regB, 7, CS00);
			break;
		default:
			return ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER;
		}
	}

	if (mode != TIMER2_MODE_NO_CHANGE) {
		clrMask(regA, 0x03, WGM20);
		clrBit(regB, WGM22);
		switch (mode) {
		case TIMER2_MODE_NORMAL:
			break;
		case TIMER2_MODE_PWM_PHASE_CORRECTED_MAX:
			setMask(regA, 1, WGM20);
			break;
		case TIMER2_MODE_CTC_OCRA:
			setMask(regA, 2, WGM20);
			break;
		case TIMER2_MODE_FAST_PWM_MAX:
			setMask(regA, 3, WGM20);
			break;
		case TIMER2_MODE_PWM_PHASE_CORRECTED_OCRA:
			setBit(regB, WGM22);
			setMask(regA, 1, WGM20);
			break;
		case TIMER2_MODE_FAST_PWM_OCRA:
			setBit(regB, WGM22);
			setMask(regA, 3, WGM20);
			break;
		default:
			return ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_MODE;
		}
	}

	TCCR2A = regA;
	TCCR2B = regB;

	return ERROR_NONE;
}

/* -----------------------------------------------------------------------------
 * Configures the timer2 compare outputs
 * -------------------------------------------------------------------------- */

error_e timer2OutputConfig(timerOutput_e compA, timerOutput_e compB)
{
	uint8_t reg = TCCR2A;

	if (compA != TIMER_PORT_NO_CHANGE) {
		clrMask(reg, 0x03, COM2A0);
		switch (compA) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(reg, 1, COM2A0);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(reg, 2, COM2A0);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(reg, 3, COM2A0);
			break;
		default:
			return ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE;
		}
	}

	if (compB != TIMER_PORT_NO_CHANGE) {
		clrMask(reg, 0x03, COM2B0);
		switch (compB) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(reg, 1, COM2B0);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(reg, 2, COM2B0);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(reg, 3, COM2B0);
			break;
		default:
			return ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE;
		}
	}

	TCCR2A = reg;

	return ERROR_NONE;
}
