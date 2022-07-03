/*!
	\file		timer0.h
	\brief		TIMER0 interface for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 27, 2018
*/

#ifndef __TIMER0_HPP
#ifndef __DOXYGEN__
#define __TIMER0_HPP 1002
#endif

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#error Error 100 - globalDefines.hpp - wrong build (globalDefines must be build 2).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/*!
	\enum			timer0Mode_e
		\details	Modes associated with TIMER0.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum timer0Mode_e {
	TIMER0_MODE_NO_CHANGE					= 0,	//!< Keep last value configured
	TIMER0_MODE_NORMAL						= 1,	//!< Normal mode (direction = UP / TOP = 0xFF / UPDATE OCR0x = immediate / TOV Flag = 0xFF)
	TIMER0_MODE_CTC_OCRA					= 2,	//!< CTC mode (direction = UP / TOP = OCR0A / UPDATE OCR0x = immediate / TOV Flag = 0xFF)
	TIMER0_MODE_FAST_PWM_MAX				= 3,	//!< PWM mode fast (direction = UP / TOP = 0xFF / UPDATE OCR0x = 0x00 / TOV Flag = 0xFF)
	TIMER0_MODE_FAST_PWM_OCRA				= 4,	//!< PWM mode fast (direction = UP / TOP = OCR0A / UPDATE OCR0x = 0x00 / TOV Flag = 0xFF)
	TIMER0_MODE_PWM_PHASE_CORRECTED_MAX		= 5,	//!< PWM mode with phase correction (direction = DOWN then UP / TOP = 0xFF / UPDATE OCR0x = 0xFF / TOV Flag = 0x00)
	TIMER0_MODE_PWM_PHASE_CORRECTED_OCRA	= 6,	//!< PWM mode with phase correction (direction = DOWN then UP / TOP = OCR0A / UPDATE OCR0x = OCR0A / TOV Flag = 0x00)
} timer0Mode_e;

/*!
	\enum			timer0Prescaler_e
		\details	Clock prescaler options associated with TIMER0.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum timer0Prescaler_e {
	TIMER0_PRESCALER_NO_CHANGE			= 0,	//!< Keep last value configured
	TIMER0_CLOCK_DISABLE				= 1,	//!< Disables clock, timer is off
	TIMER0_PRESCALER_1					= 2,	//!< Disables prescaler, timer clock frequency will be equal to CPU clock
	TIMER0_PRESCALER_8					= 3,	//!< Timer clock frequency will be CPU clock divided by 8
	TIMER0_PRESCALER_64					= 4,	//!< Timer clock frequency will be CPU clock divided by 64
	TIMER0_PRESCALER_256				= 5,	//!< Timer clock frequency will be CPU clock divided by 256
	TIMER0_PRESCALER_1024				= 6,	//!< Timer clock frequency will be CPU clock divided by 1024
	TIMER0_PRESCALER_T0_FALLING_EDGE	= 7,	//!< Timer clock will be based on the falling edge of a signal placed on T0 pin
	TIMER0_PRESCALER_T0_RISING_EDGE		= 8		//!< Timer clock will be based on the rising edge of a signal placed on T0 pin
} timer0Prescaler_e;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

/*!
	\fn			error_e timer0Config(timer0Mode_e mode, timer0Prescaler_e prescaler);
		\details	Configures the TIMER0 module
		\param		mode			desired mode for the TIMER0 module
		\param		prescaler		desired clock prescaler for the TIMER0 module
		\return						error_e enumeration
*/
error_e		timer0Config(timer0Mode_e mode, timer0Prescaler_e prescaler);

/*!
	\fn			error_e timer0OutputConfig(timerOutput_e compA, timerOutput_e compB);
		\details	Configures the desired behavior of OCR0x pins during compare match.
		\param		compA		desired behavior of OCR0A pin during a compare A match
		\param		compB		desired behavior of OCR0B pin during a compare B match
		\return					error_e enumeration
*/
error_e		timer0OutputConfig(timerOutput_e compA, timerOutput_e compB);

// -----------------------------------------------------------------------------
// Inline function definitions -------------------------------------------------

// Doxygen description
#ifdef __DOXYGEN__
/*!
\fn			void timer0ActivateOverflowInterrupt(void);
\details	Activates TIMER0 Overflow Interrupt.
*/
void timer0ActivateOverflowInterrupt(void);

/*!
\fn			void timer0DeactivateOverflowInterrupt(void);
\details	Deactivates TIMER0 Overflow Interrupt.
*/
void timer0DeactivateOverflowInterrupt(void);

/*!
\fn			void timer0ClearOverflowInterruptRequest(void);
\details	Clears TIMER0 Overflow Interrupt Request.
*/
void timer0ClearOverflowInterruptRequest(void);

/*!
\fn			void timer0ActivateCompareAInterrupt(void);
\details	Activates TIMER0 Compare A Match Interrupt.
*/
void timer0ActivateCompareAInterrupt(void);

/*!
\fn			void timer0DeactivateCompareAInterrupt(void);
\details	Deactivates TIMER0 Compare A Match Interrupt.
*/
void timer0DeactivateCompareAInterrupt(void);

/*!
\fn			void timer0ClearCompareAInterruptRequest(void);
\details	Clears TIMER0 Compare A Match Interrupt Request.
*/
void timer0ClearCompareAInterruptRequest(void);

/*!
\fn			void timer0ActivateCompareBInterrupt(void);
\details	Activates TIMER0 Compare B Match Interrupt.
*/
void timer0ActivateCompareBInterrupt(void);

/*!
\fn			void timer0DeactivateCompareBInterrupt(void);
\details	Deactivates TIMER0 Compare B Match Interrupt.
*/
void timer0DeactivateCompareBInterrupt(void);

/*!
\fn			void timer0ClearCompareBInterruptRequest(void);
\details	Clears TIMER0 Compare B Match Interrupt Request.
*/
void timer0ClearCompareBInterruptRequest(void);

/*!
\fn			void timer0ForceCompareA(void);
\details	Forces a Compare A Match.
*/
void timer0ForceCompareA(void);

/*!
\fn			void timer0ForceCompareB(void);
\details	Forces a Compare B Match.
*/
void timer0ForceCompareB(void);

/*!
\fn			void timer0SetCounterValue(uint8_t value);
\details	Sets the TIMER0 counter value
\param		value		Value to be written in register
*/
void timer0SetCounterValue(uint8_t value);

/*!
\fn			uint8_t timer0GetCounterValue(void);
\details	Returns current value of the TIMER0 counter
\return		TIMER0 counter value
*/
uint8_t timer0GetCounterValue(void);

/*!
\fn			void timer0SetCompareAValue(uint8_t value);
\details	Sets the TIMER0 Compare A Match value
\param		value		Value to be written in register
*/
void timer0SetCompareAValue(uint8_t value);

/*!
\fn			uint8_t timer0GetCompareAValue(void);
\details	Returns current value of the TIMER0 Compare A Match
\return		TIMER0 compare A value
*/
uint8_t timer0GetCompareAValue(void);

/*!
\fn			void timer0SetCompareBValue(uint8_t value);
\details	Sets the TIMER0 Compare B Match value
\param		value		Value to be written in register
*/
void timer0SetCompareBValue(uint8_t value);

/*!
\fn			uint8_t timer0GetCompareBValue(void);
\details	Returns current value of the TIMER0 Compare B Match
\return		TIMER0 compare B value
*/
uint8_t timer0GetCompareBValue(void);
#endif

// Real implementation
#ifndef __DOXYGEN__

void inline __attribute__((always_inline)) timer0ActivateOverflowInterrupt(void)
{
	setBit(TIMSK0, TOIE0);
	return;
}

void inline __attribute__((always_inline)) timer0DeactivateOverflowInterrupt(void)
{
	clrBit(TIMSK0, TOIE0);
	return;
}

void inline __attribute__((always_inline)) timer0ClearOverflowInterruptRequest(void)
{
	setBit(TIFR0, TOV0);
	return;
}

void inline __attribute__((always_inline)) timer0ActivateCompareAInterrupt(void)
{
	setBit(TIMSK0, OCIE0A);
	return;
}

void inline __attribute__((always_inline)) timer0DeactivateCompareAInterrupt(void)
{
	clrBit(TIMSK0, OCIE0A);
	return;
}

void inline __attribute__((always_inline)) timer0ClearCompareAInterruptRequest(void)
{
	setBit(TIFR0, OCF0A);
	return;
}

void inline __attribute__((always_inline)) timer0ActivateCompareBInterrupt(void)
{
	setBit(TIMSK0, OCIE0B);
	return;
}

void inline __attribute__((always_inline)) timer0DeactivateCompareBInterrupt(void)
{
	clrBit(TIMSK0, OCIE0B);
	return;
}

void inline __attribute__((always_inline)) timer0ClearCompareBInterruptRequest(void)
{
	setBit(TIFR0, OCF0B);
	return;
}

void inline __attribute__((always_inline)) timer0ForceCompareA(void)
{
	setBit(TCCR0B, FOC0A);
	return;
}

void inline __attribute__((always_inline)) timer0ForceCompareB(void)
{
	setBit(TCCR0B, FOC0B);
	return;
}

void inline __attribute__((always_inline)) timer0SetCounterValue(uint8_t value)
{
	TCNT0 = value;
	return;
}

uint8_t inline __attribute__((always_inline)) timer0GetCounterValue(void)
{
	return TCNT0;
}

void inline __attribute__((always_inline)) timer0SetCompareAValue(uint8_t value)
{
	OCR0A = value;
	return;
}

uint8_t inline __attribute__((always_inline)) timer0GetCompareAValue(void)
{
	return OCR0A;
}

void inline __attribute__((always_inline)) timer0SetCompareBValue(uint8_t value)
{
	OCR0B = value;
	return;
}

uint8_t inline __attribute__((always_inline)) timer0GetCompareBValue(void)
{
	return OCR0B;
}

#endif
#endif
