/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	timer.hpp
 * Module:		Timer Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		July 07, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "timer.hpp"
#if __TIMER_HPP != 1002
#	error [timer.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// Header files
#include <avr/interrupt.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// *INDENT-OFF*
// =============================================================================

#if TIMER0_EXISTS
	Timer0 timer0;
#endif
#if TIMER1_EXISTS
	Timer1 timer1;
#endif
#if TIMER2_EXISTS
	Timer2 timer2;
#endif
#if TIMER3_EXISTS
	Timer3 timer3;
#endif
#if TIMER4_EXISTS
	Timer4 timer4;
#endif
// *INDENT-ON*

// =============================================================================
// Class constructors
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

#if TIMER0_EXISTS
Timer0::Timer0(void)
{
	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return;
}
#endif

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

#if TIMER1_EXISTS
Timer1::Timer1(void)
{

	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return;
}
#endif

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

#if TIMER2_EXISTS
Timer2::Timer2(void)
{
	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return;
}
#endif

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

#if TIMER3_EXISTS
Timer3::Timer3(void)
{
	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return;
}
#endif

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

#if TIMER4_EXISTS
Timer4::Timer4(void)
{
	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return;
}
#endif

// =============================================================================
// Class public methods
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

#if TIMER0_EXISTS

bool Timer0::init(timer0Mode_e mode, timer0ClockPrescaler_e clockPrescaler)
{
	// Clock prescaler
	if (clockPrescaler != TIMER0_CLOCK_PRESCALER_NO_CHANGE) {
		clrMask(TIMER0_CLOCK_PRESCALER_REG, 0x07, TIMER0_CLOCK_PRESCALER_BIT);
		switch (clockPrescaler) {
		case TIMER0_CLOCK_DISABLE:
			break;
		case TIMER0_CLOCK_PRESCALER_1:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 1, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		case TIMER0_CLOCK_PRESCALER_8:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 2, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		case TIMER0_CLOCK_PRESCALER_64:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 3, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		case TIMER0_CLOCK_PRESCALER_256:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 4, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		case TIMER0_CLOCK_PRESCALER_1024:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 5, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		case TIMER0_CLOCK_PRESCALER_Tn_FALLING_EDGE:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 6, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		case TIMER0_CLOCK_PRESCALER_Tn_RISING_EDGE:
			setMask(TIMER0_CLOCK_PRESCALER_REG, 7, TIMER0_CLOCK_PRESCALER_BIT);
			break;
		default:
			this->lastError_ = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
			return false;
		}
	}

	// Mode selection
	if (mode != TIMER0_MODE_NO_CHANGE) {
		clrMask(TIMER0_OPERATION_MODE_A_REG, 0x03, TIMER0_OPERATION_MODE_A_BIT);
		clrBit(TIMER0_OPERATION_MODE_B_REG, TIMER0_OPERATION_MODE_B_BIT);
		switch (mode) {
		case TIMER0_MODE_NORMAL:
			break;
		case TIMER0_MODE_PWM_PHASE_CORRECTED_8_BITS:
			setMask(TIMER0_OPERATION_MODE_A_REG, 1, TIMER0_OPERATION_MODE_A_BIT);
			break;
		case TIMER0_MODE_CTC_OCRA:
			setMask(TIMER0_OPERATION_MODE_A_REG, 2, TIMER0_OPERATION_MODE_A_BIT);
			break;
		case TIMER0_MODE_PWM_FAST_8_BITS:
			setMask(TIMER0_OPERATION_MODE_A_REG, 3, TIMER0_OPERATION_MODE_A_BIT);
			break;
		case TIMER0_MODE_PWM_PHASE_CORRECTED_OCRA:
			setBit(TIMER0_OPERATION_MODE_B_REG, TIMER0_OPERATION_MODE_B_BIT);
			setMask(TIMER0_OPERATION_MODE_A_REG, 1, TIMER0_OPERATION_MODE_A_BIT);
			break;
		case TIMER0_MODE_PWM_FAST_OCRA:
			setBit(TIMER0_OPERATION_MODE_B_REG, TIMER0_OPERATION_MODE_B_BIT);
			setMask(TIMER0_OPERATION_MODE_A_REG, 3, TIMER0_OPERATION_MODE_A_BIT);
			break;
		default:
			this->lastError_ = ERROR_OPERATION_MODE_UNSUPPORTED;
			return false;
		}
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

#	if TIMER0_COMP_A_EXISTS || TIMER0_COMP_B_EXISTS

#		if TIMER0_COMP_A_EXISTS && TIMER0_COMP_B_EXISTS
bool Timer0::configOutputs(timerOutput_e compA, timerOutput_e compB)
#		elif TIMER0_COMP_A_EXISTS && !TIMER0_COMP_B_EXISTS
bool Timer0::configOutputs(timerOutput_e compA)
#		elif !TIMER0_COMP_A_EXISTS && TIMER0_COMP_B_EXISTS
bool Timer0::configOutputs(timerOutput_e compB)
#		endif
{
	// Output compare A
#		if TIMER0_COMP_A_EXISTS
	if (compA != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER0_COMP_A_OUTPUT_MODE_REG, 0x03, TIMER0_COMP_A_OUTPUT_MODE_BIT);
		switch (compA) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER0_COMP_A_OUTPUT_MODE_REG, 1, TIMER0_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER0_COMP_A_OUTPUT_MODE_REG, 2, TIMER0_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER0_COMP_A_OUTPUT_MODE_REG, 3, TIMER0_COMP_A_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Output compare B
#		if TIMER0_COMP_B_EXISTS
	if (compB != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER0_COMP_B_OUTPUT_MODE_REG, 0x03, TIMER0_COMP_B_OUTPUT_MODE_BIT);
		switch (compB) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER0_COMP_B_OUTPUT_MODE_REG, 1, TIMER0_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER0_COMP_B_OUTPUT_MODE_REG, 2, TIMER0_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER0_COMP_B_OUTPUT_MODE_REG, 3, TIMER0_COMP_B_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif // TIMER0_COMP_A_EXISTS || TIMER0_COMP_B_EXISTS

#endif // TIMER0_EXISTS

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

#if TIMER1_EXISTS

bool Timer1::init(timer1Mode_e mode, timer1ClockPrescaler_e clockPrescaler)
{
	// Clock prescaler
	if (clockPrescaler != TIMER1_CLOCK_PRESCALER_NO_CHANGE) {
		clrMask(TIMER1_CLOCK_PRESCALER_REG, 0x07, TIMER1_CLOCK_PRESCALER_BIT);
		switch (clockPrescaler) {
		case TIMER1_CLOCK_DISABLE:
			break;
		case TIMER1_CLOCK_PRESCALER_1:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 1, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		case TIMER1_CLOCK_PRESCALER_8:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 2, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		case TIMER1_CLOCK_PRESCALER_64:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 3, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		case TIMER1_CLOCK_PRESCALER_256:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 4, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		case TIMER1_CLOCK_PRESCALER_1024:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 5, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		case TIMER1_CLOCK_PRESCALER_Tn_FALLING_EDGE:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 6, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		case TIMER1_CLOCK_PRESCALER_Tn_RISING_EDGE:
			setMask(TIMER1_CLOCK_PRESCALER_REG, 7, TIMER1_CLOCK_PRESCALER_BIT);
			break;
		default:
			this->lastError_ = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
			return false;
		}
	}

	// Mode selection
	if (mode != TIMER1_MODE_NO_CHANGE) {
		clrMask(TIMER1_OPERATION_MODE_A_REG, 0x03, TIMER1_OPERATION_MODE_A_BIT);
		clrMask(TIMER1_OPERATION_MODE_B_REG, 0x03, TIMER1_OPERATION_MODE_B_BIT);
		switch (mode) {
		case TIMER1_MODE_NORMAL:
			break;
		case TIMER1_MODE_PWM_PHASE_CORRECTED_8_BITS:
			setMask(TIMER1_OPERATION_MODE_A_REG, 1, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_PHASE_CORRECTED_9_BITS:
			setMask(TIMER1_OPERATION_MODE_A_REG, 2, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_PHASE_CORRECTED_10_BITS:
			setMask(TIMER1_OPERATION_MODE_A_REG, 3, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_CTC_OCRA:
			setMask(TIMER1_OPERATION_MODE_B_REG, 1, TIMER1_OPERATION_MODE_B_BIT);
			break;
		case TIMER1_MODE_PWM_FAST_8_BITS:
			setMask(TIMER1_OPERATION_MODE_B_REG, 1, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 1, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_FAST_9_BITS:
			setMask(TIMER1_OPERATION_MODE_B_REG, 1, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 2, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_FAST_10_BITS:
			setMask(TIMER1_OPERATION_MODE_B_REG, 1, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 3, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_PHASE_FREQUENCY_CORRECTED_ICR:
			setMask(TIMER1_OPERATION_MODE_B_REG, 2, TIMER1_OPERATION_MODE_B_BIT);
			break;
		case TIMER1_MODE_PWM_PHASE_FREQUENCY_CORRECTED_OCRA:
			setMask(TIMER1_OPERATION_MODE_B_REG, 2, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 1, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_PHASE_CORRECTED_ICR:
			setMask(TIMER1_OPERATION_MODE_B_REG, 2, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 2, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_PHASE_CORRECTED_OCRA:
			setMask(TIMER1_OPERATION_MODE_B_REG, 2, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 3, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_CTC_ICR:
			setMask(TIMER1_OPERATION_MODE_B_REG, 3, TIMER1_OPERATION_MODE_B_BIT);
			break;
		case TIMER1_MODE_PWM_FAST_ICR:
			setMask(TIMER1_OPERATION_MODE_B_REG, 3, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 2, TIMER1_OPERATION_MODE_A_BIT);
			break;
		case TIMER1_MODE_PWM_FAST_OCRA:
			setMask(TIMER1_OPERATION_MODE_B_REG, 3, TIMER1_OPERATION_MODE_B_BIT);
			setMask(TIMER1_OPERATION_MODE_A_REG, 3, TIMER1_OPERATION_MODE_A_BIT);
			break;
		default:
			this->lastError_ = ERROR_OPERATION_MODE_UNSUPPORTED;
			return false;
		}
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

#	if TIMER1_COMP_A_EXISTS || TIMER1_COMP_B_EXISTS || TIMER1_COMP_C_EXISTS

#		if TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compA, timerOutput_e compB, timerOutput_e compC)
#		elif TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && !TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compA, timerOutput_e compB)
#		elif TIMER1_COMP_A_EXISTS && !TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compA, timerOutput_e compC)
#		elif TIMER1_COMP_A_EXISTS && !TIMER1_COMP_B_EXISTS && !TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compA)
#		elif !TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compB, timerOutput_e compC)
#		elif !TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && !TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compB)
#		elif !TIMER1_COMP_A_EXISTS && !TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
bool Timer1::configOutputs(timerOutput_e compC)
#		endif
{
	// Output compare A
#		if TIMER1_COMP_A_EXISTS
	if (compA != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER1_COMP_A_OUTPUT_MODE_REG, 0x03, TIMER1_COMP_A_OUTPUT_MODE_BIT);
		switch (compA) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER1_COMP_A_OUTPUT_MODE_REG, 1, TIMER1_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER1_COMP_A_OUTPUT_MODE_REG, 2, TIMER1_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER1_COMP_A_OUTPUT_MODE_REG, 3, TIMER1_COMP_A_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Output compare B
#		if TIMER1_COMP_B_EXISTS
	if (compB != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER1_COMP_B_OUTPUT_MODE_REG, 0x03, TIMER1_COMP_B_OUTPUT_MODE_BIT);
		switch (compB) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER1_COMP_B_OUTPUT_MODE_REG, 1, TIMER1_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER1_COMP_B_OUTPUT_MODE_REG, 2, TIMER1_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER1_COMP_B_OUTPUT_MODE_REG, 3, TIMER1_COMP_B_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Output compare C
#		if TIMER1_COMP_C_EXISTS
	if (compC != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER1_COMP_C_OUTPUT_MODE_REG, 0x03, TIMER1_COMP_C_OUTPUT_MODE_BIT);
		switch (compC) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER1_COMP_C_OUTPUT_MODE_REG, 1, TIMER1_COMP_C_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER1_COMP_C_OUTPUT_MODE_REG, 2, TIMER1_COMP_C_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER1_COMP_C_OUTPUT_MODE_REG, 3, TIMER1_COMP_C_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif // TIMER1_COMP_A_EXISTS || TIMER1_COMP_B_EXISTS || TIMER1_COMP_C_EXISTS

#	if TIMER1_CAPT_EXISTS

bool Timer1::configInputCapture(edge_t edgeSelection, bool noiseCanceler)
{
	// Edge selection
	switch (edgeSelection) {
	case FALLING_EDGE:
		clrBit(TIMER1_CAPT_EDGE_SEL_REG, TIMER1_CAPT_EDGE_SEL_BIT);
		break;
	case RISING_EDGE:
		setBit(TIMER1_CAPT_EDGE_SEL_REG, TIMER1_CAPT_EDGE_SEL_BIT);
		break;
	default:
		this->lastError_ = ERROR_WRONG_CONFIGURATION;
		return false;
	}

	// Noise canceller
	if (noiseCanceler) {
		clrBit(TIMER1_CAPT_NOISE_CANCEL_REG, TIMER1_CAPT_NOISE_CANCEL_BIT);
	} else {
		setBit(TIMER1_CAPT_NOISE_CANCEL_REG, TIMER1_CAPT_NOISE_CANCEL_BIT);
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif // TIMER1_CAPT_EXISTS

#endif // TIMER1_EXISTS

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

#if TIMER2_EXISTS

bool Timer2::init(timer2Mode_e mode, timer2ClockPrescaler_e clockPrescaler)
{
	// Clock prescaler
	if (clockPrescaler != TIMER2_CLOCK_PRESCALER_NO_CHANGE) {
		clrMask(TIMER2_CLOCK_PRESCALER_REG, 0x07, TIMER2_CLOCK_PRESCALER_BIT);
		switch (clockPrescaler) {
		case TIMER2_CLOCK_DISABLE:
			break;
		case TIMER2_CLOCK_PRESCALER_1:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 1, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		case TIMER2_CLOCK_PRESCALER_8:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 2, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		case TIMER2_CLOCK_PRESCALER_32:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 3, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		case TIMER2_CLOCK_PRESCALER_64:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 4, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		case TIMER2_CLOCK_PRESCALER_128:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 5, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		case TIMER2_CLOCK_PRESCALER_256:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 6, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		case TIMER2_CLOCK_PRESCALER_1024:
			setMask(TIMER2_CLOCK_PRESCALER_REG, 7, TIMER2_CLOCK_PRESCALER_BIT);
			break;
		default:
			this->lastError_ = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
			return false;
		}
	}

	// Mode selection
	if (mode != TIMER2_MODE_NO_CHANGE) {
		clrMask(TIMER2_OPERATION_MODE_A_REG, 0x03, TIMER2_OPERATION_MODE_A_BIT);
		clrBit(TIMER2_OPERATION_MODE_B_REG, TIMER2_OPERATION_MODE_B_BIT);
		switch (mode) {
		case TIMER2_MODE_NORMAL:
			break;
		case TIMER2_MODE_PWM_PHASE_CORRECTED_8_BITS:
			setMask(TIMER2_OPERATION_MODE_A_REG, 1, TIMER2_OPERATION_MODE_A_BIT);
			break;
		case TIMER2_MODE_CTC_OCRA:
			setMask(TIMER2_OPERATION_MODE_A_REG, 2, TIMER2_OPERATION_MODE_A_BIT);
			break;
		case TIMER2_MODE_PWM_FAST_8_BITS:
			setMask(TIMER2_OPERATION_MODE_A_REG, 3, TIMER2_OPERATION_MODE_A_BIT);
			break;
		case TIMER2_MODE_PWM_PHASE_CORRECTED_OCRA:
			setBit(TIMER2_OPERATION_MODE_B_REG, TIMER2_OPERATION_MODE_B_BIT);
			setMask(TIMER2_OPERATION_MODE_A_REG, 1, TIMER2_OPERATION_MODE_A_BIT);
			break;
		case TIMER2_MODE_PWM_FAST_OCRA:
			setBit(TIMER2_OPERATION_MODE_B_REG, TIMER2_OPERATION_MODE_B_BIT);
			setMask(TIMER2_OPERATION_MODE_A_REG, 3, TIMER2_OPERATION_MODE_A_BIT);
			break;
		default:
			this->lastError_ = ERROR_OPERATION_MODE_UNSUPPORTED;
			return true;
		}
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

#	if TIMER2_COMP_A_EXISTS || TIMER2_COMP_B_EXISTS

#		if TIMER2_COMP_A_EXISTS && TIMER2_COMP_B_EXISTS
bool Timer2::configOutputs(timerOutput_e compA, timerOutput_e compB)
#		elif TIMER2_COMP_A_EXISTS && !TIMER2_COMP_B_EXISTS
bool Timer2::configOutputs(timerOutput_e compA)
#		elif !TIMER2_COMP_A_EXISTS && TIMER2_COMP_B_EXISTS
bool Timer2::configOutputs(timerOutput_e compB)
#		endif
{
	// Output compare A
#		if TIMER2_COMP_A_EXISTS
	if (compA != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER2_COMP_A_OUTPUT_MODE_REG, 0x03, TIMER2_COMP_A_OUTPUT_MODE_BIT);
		switch (compA) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER2_COMP_A_OUTPUT_MODE_REG, 1, TIMER2_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER2_COMP_A_OUTPUT_MODE_REG, 2, TIMER2_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER2_COMP_A_OUTPUT_MODE_REG, 3, TIMER2_COMP_A_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Output compare B
#		if TIMER2_COMP_B_EXISTS
	if (compB != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER2_COMP_B_OUTPUT_MODE_REG, 0x03, TIMER2_COMP_B_OUTPUT_MODE_BIT);
		switch (compB) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER2_COMP_B_OUTPUT_MODE_REG, 1, TIMER2_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER2_COMP_B_OUTPUT_MODE_REG, 2, TIMER2_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER2_COMP_B_OUTPUT_MODE_REG, 3, TIMER2_COMP_B_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif // TIMER2_COMP_A_EXISTS || TIMER2_COMP_B_EXISTS

#endif // TIMER2_EXISTS

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

#if TIMER3_EXISTS

bool Timer3::init(timer3Mode_e mode, timer3ClockPrescaler_e clockPrescaler)
{
	// Clock prescaler
	if (clockPrescaler != TIMER3_CLOCK_PRESCALER_NO_CHANGE) {
		clrMask(TIMER3_CLOCK_PRESCALER_REG, 0x07, TIMER3_CLOCK_PRESCALER_BIT);
		switch (clockPrescaler) {
		case TIMER3_CLOCK_DISABLE:
			break;
		case TIMER3_CLOCK_PRESCALER_1:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 1, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		case TIMER3_CLOCK_PRESCALER_8:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 2, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		case TIMER3_CLOCK_PRESCALER_64:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 3, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		case TIMER3_CLOCK_PRESCALER_256:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 4, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		case TIMER3_CLOCK_PRESCALER_1024:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 5, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		case TIMER3_CLOCK_PRESCALER_Tn_FALLING_EDGE:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 6, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		case TIMER3_CLOCK_PRESCALER_Tn_RISING_EDGE:
			setMask(TIMER3_CLOCK_PRESCALER_REG, 7, TIMER3_CLOCK_PRESCALER_BIT);
			break;
		default:
			this->lastError_ = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
			return false;
		}
	}

	// Mode selection
	if (mode != TIMER3_MODE_NO_CHANGE) {
		clrMask(TIMER3_OPERATION_MODE_A_REG, 0x03, TIMER3_OPERATION_MODE_A_BIT);
		clrMask(TIMER3_OPERATION_MODE_B_REG, 0x03, TIMER3_OPERATION_MODE_B_BIT);
		switch (mode) {
		case TIMER3_MODE_NORMAL:
			break;
		case TIMER3_MODE_PWM_PHASE_CORRECTED_8_BITS:
			setMask(TIMER3_OPERATION_MODE_A_REG, 1, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_PHASE_CORRECTED_9_BITS:
			setMask(TIMER3_OPERATION_MODE_A_REG, 2, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_PHASE_CORRECTED_10_BITS:
			setMask(TIMER3_OPERATION_MODE_A_REG, 3, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_CTC_OCRA:
			setMask(TIMER3_OPERATION_MODE_B_REG, 1, TIMER3_OPERATION_MODE_B_BIT);
			break;
		case TIMER3_MODE_PWM_FAST_8_BITS:
			setMask(TIMER3_OPERATION_MODE_B_REG, 1, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 1, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_FAST_9_BITS:
			setMask(TIMER3_OPERATION_MODE_B_REG, 1, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 2, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_FAST_10_BITS:
			setMask(TIMER3_OPERATION_MODE_B_REG, 1, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 3, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_PHASE_FREQUENCY_CORRECTED_ICR:
			setMask(TIMER3_OPERATION_MODE_B_REG, 2, TIMER3_OPERATION_MODE_B_BIT);
			break;
		case TIMER3_MODE_PWM_PHASE_FREQUENCY_CORRECTED_OCRA:
			setMask(TIMER3_OPERATION_MODE_B_REG, 2, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 1, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_PHASE_CORRECTED_ICR:
			setMask(TIMER3_OPERATION_MODE_B_REG, 2, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 2, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_PHASE_CORRECTED_OCRA:
			setMask(TIMER3_OPERATION_MODE_B_REG, 2, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 3, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_CTC_ICR:
			setMask(TIMER3_OPERATION_MODE_B_REG, 3, TIMER3_OPERATION_MODE_B_BIT);
			break;
		case TIMER3_MODE_PWM_FAST_ICR:
			setMask(TIMER3_OPERATION_MODE_B_REG, 3, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 2, TIMER3_OPERATION_MODE_A_BIT);
			break;
		case TIMER3_MODE_PWM_FAST_OCRA:
			setMask(TIMER3_OPERATION_MODE_B_REG, 3, TIMER3_OPERATION_MODE_B_BIT);
			setMask(TIMER3_OPERATION_MODE_A_REG, 3, TIMER3_OPERATION_MODE_A_BIT);
			break;
		default:
			this->lastError_ = ERROR_OPERATION_MODE_UNSUPPORTED;
			return false;
		}
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

#	if TIMER3_COMP_A_EXISTS || TIMER3_COMP_B_EXISTS || TIMER3_COMP_C_EXISTS

#		if TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compA, timerOutput_e compB, timerOutput_e compC)
#		elif TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && !TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compA, timerOutput_e compB)
#		elif TIMER3_COMP_A_EXISTS && !TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compA, timerOutput_e compC)
#		elif TIMER3_COMP_A_EXISTS && !TIMER3_COMP_B_EXISTS && !TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compA)
#		elif !TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compB, timerOutput_e compC)
#		elif !TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && !TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compB)
#		elif !TIMER3_COMP_A_EXISTS && !TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
bool Timer3::configOutputs(timerOutput_e compC)
#		endif
{
	// Output compare A
#		if TIMER3_COMP_A_EXISTS
	if (compA != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER3_COMP_A_OUTPUT_MODE_REG, 0x03, TIMER3_COMP_A_OUTPUT_MODE_BIT);
		switch (compA) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER3_COMP_A_OUTPUT_MODE_REG, 1, TIMER3_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER3_COMP_A_OUTPUT_MODE_REG, 2, TIMER3_COMP_A_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER3_COMP_A_OUTPUT_MODE_REG, 3, TIMER3_COMP_A_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Output compare B
#		if TIMER3_COMP_B_EXISTS
	if (compB != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER3_COMP_B_OUTPUT_MODE_REG, 0x03, TIMER3_COMP_B_OUTPUT_MODE_BIT);
		switch (compB) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER3_COMP_B_OUTPUT_MODE_REG, 1, TIMER3_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER3_COMP_B_OUTPUT_MODE_REG, 2, TIMER3_COMP_B_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER3_COMP_B_OUTPUT_MODE_REG, 3, TIMER3_COMP_B_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Output compare C
#		if TIMER3_COMP_C_EXISTS
	if (compC != TIMER_PORT_NO_CHANGE) {
		clrMask(TIMER3_COMP_C_OUTPUT_MODE_REG, 0x03, TIMER3_COMP_C_OUTPUT_MODE_BIT);
		switch (compC) {
		case TIMER_PORT_NORMAL:
			break;
		case TIMER_PORT_TOGGLE_ON_COMPARE:
			setMask(TIMER3_COMP_C_OUTPUT_MODE_REG, 1, TIMER3_COMP_C_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_CLEAR_ON_COMPARE:
			setMask(TIMER3_COMP_C_OUTPUT_MODE_REG, 2, TIMER3_COMP_C_OUTPUT_MODE_BIT);
			break;
		case TIMER_PORT_SET_ON_COMPARE:
			setMask(TIMER3_COMP_C_OUTPUT_MODE_REG, 3, TIMER3_COMP_C_OUTPUT_MODE_BIT);
			break;
		default:
			this->lastError_ = ERROR_OUTPUT_COMPARE_UNSUPPORTED;
			return false;
		}
	}
#		endif

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif // TIMER3_COMP_A_EXISTS || TIMER3_COMP_B_EXISTS || TIMER3_COMP_C_EXISTS

#	if TIMER3_CAPT_EXISTS

bool Timer3::configInputCapture(edge_t edgeSelection, bool noiseCanceler)
{
	// Edge selection
	switch (edgeSelection) {
	case FALLING_EDGE:
		clrBit(TIMER3_CAPT_EDGE_SEL_REG, TIMER3_CAPT_EDGE_SEL_BIT);
		break;
	case RISING_EDGE:
		setBit(TIMER3_CAPT_EDGE_SEL_REG, TIMER3_CAPT_EDGE_SEL_BIT);
		break;
	default:
		this->lastError_ = ERROR_WRONG_CONFIGURATION;
		return false;
	}

	// Noise canceller
	if (noiseCanceler) {
		clrBit(TIMER3_CAPT_NOISE_CANCEL_REG, TIMER3_CAPT_NOISE_CANCEL_BIT);
	} else {
		setBit(TIMER3_CAPT_NOISE_CANCEL_REG, TIMER3_CAPT_NOISE_CANCEL_BIT);
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif // TIMER3_CAPT_EXISTS

#endif // TIMER3_EXISTS

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

#if TIMER4_EXISTS
#endif // TIMER4_EXISTS

// =============================================================================
// Class private methods
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

/* NONE */

// =============================================================================
// Class protected methods
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

/* NONE */

// =============================================================================
// General public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Interruption handlers
// =============================================================================

/* NONE */
