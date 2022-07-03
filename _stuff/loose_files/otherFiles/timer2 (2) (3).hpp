/*!
	\file		timer2.h
	\brief		TIMER2 interface for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 27, 2018
*/

#ifndef __TIMER2_HPP
#ifndef __DOXYGEN__
#define __TIMER2_HPP 1002
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/*!
	\enum			timer2Mode_e
		\details	Modes associated with TIMER2.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum timer2Mode_e {
	TIMER2_MODE_NO_CHANGE					= 0,	//!< Keep last value configured
	TIMER2_MODE_NORMAL						= 1,	//!< Normal mode (direction = UP / TOP = 0xFF / UPDATE OCR2x = immediate / TOV Flag = 0xFF)
	TIMER2_MODE_CTC_OCRA					= 2,	//!< CTC mode (direction = UP / TOP = OCR2A / UPDATE OCR2x = immediate / TOV Flag = 0xFF)
	TIMER2_MODE_FAST_PWM_MAX				= 3,	//!< PWM mode fast (direction = UP / TOP = 0xFF / UPDATE OCR2x = 0x00 / TOV Flag = 0xFF)
	TIMER2_MODE_FAST_PWM_OCRA				= 4,	//!< PWM mode fast (direction = UP / TOP = OCR2A / UPDATE OCR2x = 0x00 / TOV Flag = 0xFF)
	TIMER2_MODE_PWM_PHASE_CORRECTED_MAX		= 5,	//!< PWM mode with phase correction (direction = DOWN then UP / TOP = 0xFF / UPDATE OCR2x = 0xFF / TOV Flag = 0x00)
	TIMER2_MODE_PWM_PHASE_CORRECTED_OCRA	= 6,	//!< PWM mode with phase correction (direction = DOWN then UP / TOP = OCR2A / UPDATE OCR2x = OCR2A / TOV Flag = 0x00)
} timer2Mode_e;

/*!
	\enum			timer2Prescaler_e
		\details	Clock prescaler options associated with TIMER2.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum timer2Prescaler_e {
	TIMER2_PRESCALER_NO_CHANGE			= 0,	//!< Keep last value configured
	TIMER2_CLOCK_DISABLE				= 1,	//!< Disables clock, timer is off
	TIMER2_PRESCALER_1					= 2,	//!< Disables prescaler, timer clock frequency will be equal to CPU clock
	TIMER2_PRESCALER_8					= 3,	//!< Timer clock frequency will be CPU clock divided by 8
	TIMER2_PRESCALER_32					= 4,	//!< Timer clock frequency will be CPU clock divided by 32
	TIMER2_PRESCALER_64					= 5,	//!< Timer clock frequency will be CPU clock divided by 64
	TIMER2_PRESCALER_128				= 6,	//!< Timer clock frequency will be CPU clock divided by 128
	TIMER2_PRESCALER_256				= 7,	//!< Timer clock frequency will be CPU clock divided by 256
	TIMER2_PRESCALER_1024				= 8		//!< Timer clock frequency will be CPU clock divided by 1024
} timer2Prescaler_e;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

/*!
	\fn			error_e timer2Config(timer2Mode_e mode, timer2Prescaler_e prescaler);
		\details	Configures the TIMER2 module
		\param		mode			desired mode for the TIMER2 module
		\param		prescaler		desired clock prescaler for the TIMER2 module
		\return						error_e enumeration
*/
error_e		timer2Config(timer2Mode_e mode, timer2Prescaler_e prescaler);

/*!
	\fn			error_e timer2OutputConfig(timerOutput_e compA, timerOutput_e compB);
		\details	Configures the desired behavior of OCR2x pins during compare match.
		\param		compA		desired behavior of OCR2A pin during a compare A match
		\param		compB		desired behavior of OCR2B pin during a compare B match
		\return					error_e enumeration
*/
error_e		timer2OutputConfig(timerOutput_e compA, timerOutput_e compB);

// -----------------------------------------------------------------------------
// Inline function definitions -------------------------------------------------

// Doxygen description
#ifdef __DOXYGEN__
/*!
\fn			void timer2ActivateOverflowInterrupt(void);
\details	Activates TIMER2 Overflow Interrupt.
*/
void timer2ActivateOverflowInterrupt(void);

/*!
\fn			void timer2DeactivateOverflowInterrupt(void);
\details	Deactivates TIMER2 Overflow Interrupt.
*/
void timer2DeactivateOverflowInterrupt(void);

/*!
\fn			void timer2ClearOverflowInterruptRequest(void);
\details	Clears TIMER2 Overflow Interrupt Request.
*/
void timer2ClearOverflowInterruptRequest(void);

/*!
\fn			void timer2ActivateCompareAInterrupt(void);
\details	Activates TIMER2 Compare A Match Interrupt.
*/
void timer2ActivateCompareAInterrupt(void);

/*!
\fn			void timer2DeactivateCompareAInterrupt(void);
\details	Deactivates TIMER2 Compare A Match Interrupt.
*/
void timer2DeactivateCompareAInterrupt(void);

/*!
\fn			void timer2ClearCompareAInterruptRequest(void);
\details	Clears TIMER2 Compare A Match Interrupt Request.
*/
void timer2ClearCompareAInterruptRequest(void);

/*!
\fn			void timer2ActivateCompareBInterrupt(void);
\details	Activates TIMER2 Compare B Match Interrupt.
*/
void timer2ActivateCompareBInterrupt(void);

/*!
\fn			void timer2DeactivateCompareBInterrupt(void);
\details	Deactivates TIMER2 Compare B Match Interrupt.
*/
void timer2DeactivateCompareBInterrupt(void);

/*!
\fn			void timer2ClearCompareBInterruptRequest(void);
\details	Clears TIMER2 Compare B Match Interrupt Request.
*/
void timer2ClearCompareBInterruptRequest(void);

/*!
\fn			void timer2ForceCompareA(void);
\details	Forces a Compare A Match.
*/
void timer2ForceCompareA(void);

/*!
\fn			void timer2ForceCompareB(void);
\details	Forces a Compare B Match.
*/
void timer2ForceCompareB(void);

/*!
\fn			void timer2SetCounterValue(uint8_t value);
\details	Sets the TIMER2 counter value
\param		value		Value to be written in register
*/
void timer2SetCounterValue(uint8_t value);

/*!
\fn			uint8_t timer2GetCounterValue(void);
\details	Returns current value of the TIMER2 counter
\return		TIMER2 counter value
*/
uint8_t timer2GetCounterValue(void);

/*!
\fn			void timer2SetCompareAValue(uint8_t value);
\details	Sets the TIMER2 Compare A Match value
\param		value		Value to be written in register
*/
void timer2SetCompareAValue(uint8_t value);

/*!
\fn			uint8_t timer2GetCompareAValue(void);
\details	Returns current value of the TIMER2 Compare A Match
\return		TIMER2 compare A value
*/
uint8_t timer2GetCompareAValue(void);

/*!
\fn			void timer2SetCompareBValue(uint8_t value);
\details	Sets the TIMER2 Compare B Match value
\param		value		Value to be written in register
*/
void timer2SetCompareBValue(uint8_t value);

/*!
\fn			uint8_t timer2GetCompareBValue(void);
\details	Returns current value of the TIMER2 Compare B Match
\return		TIMER2 compare B value
*/
uint8_t timer2GetCompareBValue(void);
#endif

// Real implementation
#ifndef __DOXYGEN__

void inline __attribute__((always_inline)) timer2ActivateOverflowInterrupt(void)
{
	setBit(TIMSK2, TOIE2);
	return;
}

void inline __attribute__((always_inline)) timer2DeactivateOverflowInterrupt(void)
{
	clrBit(TIMSK2, TOIE2);
	return;
}

void inline __attribute__((always_inline)) timer2ClearOverflowInterruptRequest(void)
{
	setBit(TIFR2, TOV2);
	return;
}

void inline __attribute__((always_inline)) timer2ActivateCompareAInterrupt(void)
{
	setBit(TIMSK2, OCIE2A);
	return;
}

void inline __attribute__((always_inline)) timer2DeactivateCompareAInterrupt(void)
{
	clrBit(TIMSK2, OCIE2A);
	return;
}

void inline __attribute__((always_inline)) timer2ClearCompareAInterruptRequest(void)
{
	setBit(TIFR2, OCF2A);
	return;
}

void inline __attribute__((always_inline)) timer2ActivateCompareBInterrupt(void)
{
	setBit(TIMSK2, OCIE2B);
	return;
}

void inline __attribute__((always_inline)) timer2DeactivateCompareBInterrupt(void)
{
	clrBit(TIMSK2, OCIE2B);
	return;
}

void inline __attribute__((always_inline)) timer2ClearCompareBInterruptRequest(void)
{
	setBit(TIFR2, OCF2B);
	return;
}

void inline __attribute__((always_inline)) timer2ForceCompareA(void)
{
	setBit(TCCR2B, FOC2A);
	return;
}

void inline __attribute__((always_inline)) timer2ForceCompareB(void)
{
	setBit(TCCR2B, FOC2B);
	return;
}

void inline __attribute__((always_inline)) timer2SetCounterValue(uint8_t value)
{
	TCNT2 = value;
	return;
}

uint8_t inline __attribute__((always_inline)) timer2GetCounterValue(void)
{
	return TCNT2;
}

void inline __attribute__((always_inline)) timer2SetCompareAValue(uint8_t value)
{
	OCR2A = value;
	return;
}

uint8_t inline __attribute__((always_inline)) timer2GetCompareAValue(void)
{
	return OCR2A;
}

void inline __attribute__((always_inline)) timer2SetCompareBValue(uint8_t value)
{
	OCR2B = value;
	return;
}

uint8_t inline __attribute__((always_inline)) timer2GetCompareBValue(void)
{
	return OCR2B;
}

#endif
#endif
