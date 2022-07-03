/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	timer.hpp
 * Module:		Timer Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		July 07, 2018
 * ========================================================================== */

#ifndef __TIMER_HPP
#define __TIMER_HPP	1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[timer.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include <util/atomic.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

#if TIMER0_EXISTS
typedef enum timer0Mode_e {
	TIMER0_MODE_NO_CHANGE							= 0,
	TIMER0_MODE_NORMAL								= 3,
	TIMER0_MODE_CTC_OCRA							= 2,
	TIMER0_MODE_PWM_FAST_8_BITS						= 5,
	TIMER0_MODE_PWM_FAST_OCRA						= 8,
	TIMER0_MODE_PWM_PHASE_CORRECTED_8_BITS			= 10,
	TIMER0_MODE_PWM_PHASE_CORRECTED_OCRA			= 13,
} timer0Mode_e;

typedef enum timer0ClockPrescaler_e {
	TIMER0_CLOCK_PRESCALER_NO_CHANGE				= 0,
	TIMER0_CLOCK_DISABLE							= 1,
	TIMER0_CLOCK_PRESCALER_1						= 2,
	TIMER0_CLOCK_PRESCALER_8						= 3,
	TIMER0_CLOCK_PRESCALER_64						= 5,
	TIMER0_CLOCK_PRESCALER_256						= 7,
	TIMER0_CLOCK_PRESCALER_1024						= 8,
	TIMER0_CLOCK_PRESCALER_Tn_FALLING_EDGE			= 9,
	TIMER0_CLOCK_PRESCALER_Tn_RISING_EDGE			= 10,
} timer0ClockPrescaler_e;
#endif

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

#if TIMER1_EXISTS
typedef enum timer1Mode_e {
	TIMER1_MODE_NO_CHANGE							= 0,
	TIMER1_MODE_CTC_ICR								= 1,
	TIMER1_MODE_CTC_OCRA							= 2,
	TIMER1_MODE_NORMAL								= 3,
	TIMER1_MODE_PWM_FAST_10_BITS					= 4,
	TIMER1_MODE_PWM_FAST_8_BITS						= 5,
	TIMER1_MODE_PWM_FAST_9_BITS						= 6,
	TIMER1_MODE_PWM_FAST_ICR						= 7,
	TIMER1_MODE_PWM_FAST_OCRA						= 8,
	TIMER1_MODE_PWM_PHASE_CORRECTED_10_BITS			= 9,
	TIMER1_MODE_PWM_PHASE_CORRECTED_8_BITS			= 10,
	TIMER1_MODE_PWM_PHASE_CORRECTED_9_BITS			= 11,
	TIMER1_MODE_PWM_PHASE_CORRECTED_ICR				= 12,
	TIMER1_MODE_PWM_PHASE_CORRECTED_OCRA			= 13,
	TIMER1_MODE_PWM_PHASE_FREQUENCY_CORRECTED_ICR	= 14,
	TIMER1_MODE_PWM_PHASE_FREQUENCY_CORRECTED_OCRA	= 15,
} timer1Mode_e;

typedef enum timer1ClockPrescaler_e {
	TIMER1_CLOCK_PRESCALER_NO_CHANGE				= 0,
	TIMER1_CLOCK_DISABLE							= 1,
	TIMER1_CLOCK_PRESCALER_1						= 2,
	TIMER1_CLOCK_PRESCALER_8						= 3,
	TIMER1_CLOCK_PRESCALER_64						= 5,
	TIMER1_CLOCK_PRESCALER_256						= 7,
	TIMER1_CLOCK_PRESCALER_1024						= 8,
	TIMER1_CLOCK_PRESCALER_Tn_FALLING_EDGE			= 9,
	TIMER1_CLOCK_PRESCALER_Tn_RISING_EDGE			= 10,
} timer1ClockPrescaler_e;
#endif

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

#if TIMER2_EXISTS
typedef enum timer2Mode_e {
	TIMER2_MODE_NO_CHANGE							= 0,
	TIMER2_MODE_CTC_OCRA							= 2,
	TIMER2_MODE_NORMAL								= 3,
	TIMER2_MODE_PWM_FAST_8_BITS						= 5,
	TIMER2_MODE_PWM_FAST_OCRA						= 8,
	TIMER2_MODE_PWM_PHASE_CORRECTED_8_BITS			= 10,
	TIMER2_MODE_PWM_PHASE_CORRECTED_OCRA			= 13,
} timer2Mode_e;

typedef enum timer2ClockPrescaler_e {
	TIMER2_CLOCK_PRESCALER_NO_CHANGE				= 0,
	TIMER2_CLOCK_DISABLE							= 1,
	TIMER2_CLOCK_PRESCALER_1						= 2,
	TIMER2_CLOCK_PRESCALER_8						= 3,
	TIMER2_CLOCK_PRESCALER_32						= 4,
	TIMER2_CLOCK_PRESCALER_64						= 5,
	TIMER2_CLOCK_PRESCALER_128						= 6,
	TIMER2_CLOCK_PRESCALER_256						= 7,
	TIMER2_CLOCK_PRESCALER_1024						= 8,
} timer2ClockPrescaler_e;
#endif

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

#if TIMER3_EXISTS
typedef enum timer3Mode_e {
	TIMER3_MODE_NO_CHANGE							= 0,
	TIMER3_MODE_CTC_ICR								= 1,
	TIMER3_MODE_CTC_OCRA							= 2,
	TIMER3_MODE_NORMAL								= 3,
	TIMER3_MODE_PWM_FAST_10_BITS					= 4,
	TIMER3_MODE_PWM_FAST_8_BITS						= 5,
	TIMER3_MODE_PWM_FAST_9_BITS						= 6,
	TIMER3_MODE_PWM_FAST_ICR						= 7,
	TIMER3_MODE_PWM_FAST_OCRA						= 8,
	TIMER3_MODE_PWM_PHASE_CORRECTED_10_BITS			= 9,
	TIMER3_MODE_PWM_PHASE_CORRECTED_8_BITS			= 10,
	TIMER3_MODE_PWM_PHASE_CORRECTED_9_BITS			= 11,
	TIMER3_MODE_PWM_PHASE_CORRECTED_ICR				= 12,
	TIMER3_MODE_PWM_PHASE_CORRECTED_OCRA			= 13,
	TIMER3_MODE_PWM_PHASE_FREQUENCY_CORRECTED_ICR	= 14,
	TIMER3_MODE_PWM_PHASE_FREQUENCY_CORRECTED_OCRA	= 15,
} timer3Mode_e;

typedef enum timer3ClockPrescaler_e {
	TIMER3_CLOCK_PRESCALER_NO_CHANGE				= 0,
	TIMER3_CLOCK_DISABLE							= 1,
	TIMER3_CLOCK_PRESCALER_1						= 2,
	TIMER3_CLOCK_PRESCALER_8						= 3,
	TIMER3_CLOCK_PRESCALER_64						= 5,
	TIMER3_CLOCK_PRESCALER_256						= 7,
	TIMER3_CLOCK_PRESCALER_1024						= 8,
	TIMER3_CLOCK_PRESCALER_Tn_FALLING_EDGE			= 9,
	TIMER3_CLOCK_PRESCALER_Tn_RISING_EDGE			= 10,
} timer3ClockPrescaler_e;
#endif

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

#if TIMER4_EXISTS
typedef enum timer4Mode_e {
	TIMER4_MODE_NO_CHANGE							= 0,
} timer4Mode_e;

typedef enum timer4ClockPrescaler_e {
	TIMER4_CLOCK_PRESCALER_NO_CHANGE				= 0,
} timer4ClockPrescaler_e;
#endif

// =============================================================================
// Timer - Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

#if TIMER0_EXISTS
class Timer0
{
	// Constructors
public:
	Timer0(void);

	// Methods - Public
public:
	bool	init(timer0Mode_e mode, timer0ClockPrescaler_e clockPrescaler);
	bool	activateOverflowInterrupt(void);
	bool	clearOverflowInterruptRequest(void);
	bool	deactivateOverflowInterrupt(void);
	bool	getCounter(uint8_t * value);
	bool	setCounter(uint8_t value);
#	if TIMER0_COMP_A_EXISTS
	bool	activateCompareAInterrupt(void);
	bool	clearCompareAInterruptRequest(void);
	bool	deactivateCompareAInterrupt(void);
	bool	forceCompareA(void);
	bool	getCompareA(uint8_t * value);
	bool	setCompareA(uint8_t value);
#	endif
#	if TIMER0_COMP_B_EXISTS
	bool	activateCompareBInterrupt(void);
	bool	clearCompareBInterruptRequest(void);
	bool	deactivateCompareBInterrupt(void);
	bool	forceCompareB(void);
	bool	getCompareB(uint8_t * value);
	bool	setCompareB(uint8_t value);
#	endif
#	if TIMER0_COMP_A_EXISTS && TIMER0_COMP_B_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compB);
#	elif TIMER0_COMP_A_EXISTS && !TIMER0_COMP_B_EXISTS
	bool	configOutputs(timerOutput_e compA);
#	elif !TIMER0_COMP_A_EXISTS && TIMER0_COMP_B_EXISTS
	bool	configOutputs(timerOutput_e compB);
#	endif
	error_e	getLastError(void);

	// Data members - Private
private:
	bool			initialized_	: 1;
	error_e			lastError_;

}; // class Timer0
#endif

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

#if TIMER1_EXISTS
class Timer1
{
	// Constructors
public:
	Timer1(void);

	// Methods - Public
public:
	bool	init(timer1Mode_e mode, timer1ClockPrescaler_e clockPrescaler);
	bool	activateOverflowInterrupt(void);
	bool	clearOverflowInterruptRequest(void);
	bool	deactivateOverflowInterrupt(void);
	bool	getCounter(uint16_t * value);
	bool	setCounter(uint16_t value);
#	if TIMER1_COMP_A_EXISTS
	bool	activateCompareAInterrupt(void);
	bool	clearCompareAInterruptRequest(void);
	bool	deactivateCompareAInterrupt(void);
	bool	forceCompareA(void);
	bool	getCompareA(uint16_t * value);
	bool	setCompareA(uint16_t value);
#	endif
#	if TIMER1_COMP_B_EXISTS
	bool	activateCompareBInterrupt(void);
	bool	clearCompareBInterruptRequest(void);
	bool	deactivateCompareBInterrupt(void);
	bool	forceCompareB(void);
	bool	getCompareB(uint16_t * value);
	bool	setCompareB(uint16_t value);
#	endif
#	if TIMER1_COMP_C_EXISTS
	bool	activateCompareCInterrupt(void);
	bool	clearCompareCInterruptRequest(void);
	bool	deactivateCompareCInterrupt(void);
	bool	forceCompareC(void);
	bool	getCompareC(uint16_t * value);
	bool	setCompareC(uint16_t value);
#	endif
#	if TIMER1_CAPT_EXISTS
	bool	activateInputCaptureInterrupt(void);
	bool	clearInputCaptureInterruptRequest(void);
	bool	configInputCapture(edge_t edgeSelection, bool noiseCanceler);
	bool	deactivateInputCaptureInterrupt(void);
	bool	getInputCapture(uint16_t * value);
	bool	setInputCapture(uint16_t value);
#	endif
#	if TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compB, timerOutput_e compC);
#	elif TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && !TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compB);
#	elif TIMER1_COMP_A_EXISTS && !TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compC);
#	elif TIMER1_COMP_A_EXISTS && !TIMER1_COMP_B_EXISTS && !TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA);
#	elif !TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compB, timerOutput_e compC);
#	elif !TIMER1_COMP_A_EXISTS && TIMER1_COMP_B_EXISTS && !TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compB);
#	elif !TIMER1_COMP_A_EXISTS && !TIMER1_COMP_B_EXISTS && TIMER1_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compC);
#	endif
	error_e	getLastError(void);

	// Data members - Private
private:
	bool			initialized_	: 1;
	error_e			lastError_;

}; // class Timer1
#endif

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

#if TIMER2_EXISTS
class Timer2
{
	// Constructors
public:
	Timer2(void);

	// Methods - Public
public:
	bool	init(timer2Mode_e mode, timer2ClockPrescaler_e clockPrescaler);
	bool	activateOverflowInterrupt(void);
	bool	clearOverflowInterruptRequest(void);
	bool	deactivateOverflowInterrupt(void);
	bool	getCounter(uint8_t * value);
	bool	setCounter(uint8_t value);
#	if TIMER2_COMP_A_EXISTS
	bool	activateCompareAInterrupt(void);
	bool	clearCompareAInterruptRequest(void);
	bool	deactivateCompareAInterrupt(void);
	bool	forceCompareA(void);
	bool	getCompareA(uint8_t * value);
	bool	setCompareA(uint8_t value);
#	endif
#	if TIMER2_COMP_B_EXISTS
	bool	activateCompareBInterrupt(void);
	bool	clearCompareBInterruptRequest(void);
	bool	deactivateCompareBInterrupt(void);
	bool	forceCompareB(void);
	bool	getCompareB(uint8_t * value);
	bool	setCompareB(uint8_t value);
#	endif
#	if TIMER2_COMP_A_EXISTS && TIMER2_COMP_B_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compB);
#	elif TIMER2_COMP_A_EXISTS && !TIMER2_COMP_B_EXISTS
	bool	configOutputs(timerOutput_e compA);
#	elif !TIMER2_COMP_A_EXISTS && TIMER2_COMP_B_EXISTS
	bool	configOutputs(timerOutput_e compB);
#	endif
	error_e	getLastError(void);

	// Data members - Private
private:
	bool			initialized_	: 1;
	error_e			lastError_;

}; // class Timer2
#endif

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

#if TIMER3_EXISTS
class Timer3
{
	// Constructors
public:
	Timer3(void);

	// Methods - Public
public:
	bool	init(timer3Mode_e mode, timer3ClockPrescaler_e clockPrescaler);
	bool	activateOverflowInterrupt(void);
	bool	clearOverflowInterruptRequest(void);
	bool	deactivateOverflowInterrupt(void);
	bool	getCounter(uint16_t * value);
	bool	setCounter(uint16_t value);
#	if TIMER3_COMP_A_EXISTS
	bool	activateCompareAInterrupt(void);
	bool	clearCompareAInterruptRequest(void);
	bool	deactivateCompareAInterrupt(void);
	bool	forceCompareA(void);
	bool	getCompareA(uint16_t * value);
	bool	setCompareA(uint16_t value);
#	endif
#	if TIMER3_COMP_B_EXISTS
	bool	activateCompareBInterrupt(void);
	bool	clearCompareBInterruptRequest(void);
	bool	deactivateCompareBInterrupt(void);
	bool	forceCompareB(void);
	bool	getCompareB(uint16_t * value);
	bool	setCompareB(uint16_t value);
#	endif
#	if TIMER3_COMP_C_EXISTS
	bool	activateCompareCInterrupt(void);
	bool	clearCompareCInterruptRequest(void);
	bool	deactivateCompareCInterrupt(void);
	bool	forceCompareC(void);
	bool	getCompareC(uint16_t * value);
	bool	setCompareC(uint16_t value);
#	endif
#	if TIMER3_CAPT_EXISTS
	bool	activateInputCaptureInterrupt(void);
	bool	clearInputCaptureInterruptRequest(void);
	bool	configInputCapture(edge_t edgeSelection, bool noiseCanceler);
	bool	deactivateInputCaptureInterrupt(void);
	bool	getInputCapture(uint16_t * value);
	bool	setInputCapture(uint16_t value);
#	endif
#if TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compB, timerOutput_e compC);
#	elif TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && !TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compB);
#	elif TIMER3_COMP_A_EXISTS && !TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA, timerOutput_e compC);
#	elif TIMER3_COMP_A_EXISTS && !TIMER3_COMP_B_EXISTS && !TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compA);
#	elif !TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compB, timerOutput_e compC);
#	elif !TIMER3_COMP_A_EXISTS && TIMER3_COMP_B_EXISTS && !TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compB);
#	elif !TIMER3_COMP_A_EXISTS && !TIMER3_COMP_B_EXISTS && TIMER3_COMP_C_EXISTS
	bool	configOutputs(timerOutput_e compC);
#	endif
	error_e	getLastError(void);

	// Data members - Private
private:
	bool			initialized_	: 1;
	error_e			lastError_;

}; // class Timer3
#endif

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

#if TIMER4_EXISTS
class Timer4
{
	// Constructors
public:
	Timer4(void);

	// Methods - Public
public:
	bool	init(timer4Mode_e mode, timer4ClockPrescaler_e clockPrescaler);
	error_e	getLastError(void);

	// Data members - Private
private:
	bool			initialized_	: 1;
	error_e			lastError_;

}; // class Timer4
#endif

// =============================================================================
// Timer - Class inline function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Timer0 ----------------------------------------------------------------------

#if TIMER0_EXISTS
bool inline __attribute__((always_inline)) Timer0::activateOverflowInterrupt(void)
{
	setBit(TIMER0_OVF_INT_ENABLE_REG, TIMER0_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::clearOverflowInterruptRequest(void)
{
	setBit(TIMER0_OVF_INT_FLAG_REG, TIMER0_OVF_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::deactivateOverflowInterrupt(void)
{
	clrBit(TIMER0_OVF_INT_ENABLE_REG, TIMER0_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::getCounter(uint8_t *value)
{
	*value = TIMER0_COUNTER_VALUE_REG;
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::setCounter(uint8_t value)
{
	TIMER0_COUNTER_VALUE_REG = value;
	this->lastError_ = ERROR_NONE;
	return true;
}
#	if TIMER0_COMP_A_EXISTS
bool inline __attribute__((always_inline)) Timer0::activateCompareAInterrupt(void)
{
	setBit(TIMER0_COMP_A_INT_ENABLE_REG, TIMER0_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::clearCompareAInterruptRequest(void)
{
	setBit(TIMER0_COMP_A_INT_FLAG_REG, TIMER0_COMP_A_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::deactivateCompareAInterrupt(void)
{
	clrBit(TIMER0_COMP_A_INT_ENABLE_REG, TIMER0_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::forceCompareA(void)
{
	setBit(TIMER0_COMP_A_FORCE_MATCH_REG, TIMER0_COMP_A_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::getCompareA(uint8_t *value)
{
	*value = TIMER0_COMP_A_VALUE_REG;
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::setCompareA(uint8_t value)
{
	TIMER0_COMP_A_VALUE_REG = value;
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER0_COMP_B_EXISTS
bool inline __attribute__((always_inline)) Timer0::activateCompareBInterrupt(void)
{
	setBit(TIMER0_COMP_B_INT_ENABLE_REG, TIMER0_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::clearCompareBInterruptRequest(void)
{
	setBit(TIMER0_COMP_B_INT_FLAG_REG, TIMER0_COMP_B_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::deactivateCompareBInterrupt(void)
{
	clrBit(TIMER0_COMP_B_INT_ENABLE_REG, TIMER0_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::forceCompareB(void)
{
	setBit(TIMER0_COMP_B_FORCE_MATCH_REG, TIMER0_COMP_B_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::getCompareB(uint8_t *value)
{
	*value = TIMER0_COMP_B_VALUE_REG;
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer0::setCompareB(uint8_t value)
{
	TIMER0_COMP_B_VALUE_REG = value;
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
error_e	inline __attribute__((always_inline)) Timer0::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// Timer1 ----------------------------------------------------------------------

#if TIMER1_EXISTS
bool inline __attribute__((always_inline)) Timer1::activateOverflowInterrupt(void)
{
	setBit(TIMER1_OVF_INT_ENABLE_REG, TIMER1_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::clearOverflowInterruptRequest(void)
{
	setBit(TIMER1_OVF_INT_FLAG_REG, TIMER1_OVF_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::deactivateOverflowInterrupt(void)
{
	clrBit(TIMER1_OVF_INT_ENABLE_REG, TIMER1_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::getCounter(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER1_COUNTER_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::setCounter(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER1_COUNTER_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	if TIMER1_COMP_A_EXISTS
bool inline __attribute__((always_inline)) Timer1::activateCompareAInterrupt(void)
{
	setBit(TIMER1_COMP_A_INT_ENABLE_REG, TIMER1_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::clearCompareAInterruptRequest(void)
{
	setBit(TIMER1_COMP_A_INT_FLAG_REG, TIMER1_COMP_A_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::deactivateCompareAInterrupt(void)
{
	clrBit(TIMER1_COMP_A_INT_ENABLE_REG, TIMER1_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::forceCompareA(void)
{
	setBit(TIMER1_COMP_A_FORCE_MATCH_REG, TIMER1_COMP_A_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::getCompareA(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER1_COMP_A_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::setCompareA(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER1_COMP_A_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER1_COMP_B_EXISTS
bool inline __attribute__((always_inline)) Timer1::activateCompareBInterrupt(void)
{
	setBit(TIMER1_COMP_B_INT_ENABLE_REG, TIMER1_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::clearCompareBInterruptRequest(void)
{
	setBit(TIMER1_COMP_B_INT_FLAG_REG, TIMER1_COMP_B_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::deactivateCompareBInterrupt(void)
{
	clrBit(TIMER1_COMP_B_INT_ENABLE_REG, TIMER1_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::forceCompareB(void)
{
	setBit(TIMER1_COMP_B_FORCE_MATCH_REG, TIMER1_COMP_B_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::getCompareB(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER1_COMP_B_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::setCompareB(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER1_COMP_B_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER1_COMP_C_EXISTS
bool inline __attribute__((always_inline)) Timer1::activateCompareCInterrupt(void)
{
	setBit(TIMER1_COMP_C_INT_ENABLE_REG, TIMER1_COMP_C_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::clearCompareCInterruptRequest(void)
{
	setBit(TIMER1_COMP_C_INT_FLAG_REG, TIMER1_COMP_C_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::deactivateCompareCInterrupt(void)
{
	clrBit(TIMER1_COMP_C_INT_ENABLE_REG, TIMER1_COMP_C_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::forceCompareC(void)
{
	setBit(TIMER1_COMP_C_FORCE_MATCH_REG, TIMER1_COMP_C_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::getCompareC(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER1_COMP_C_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::setCompareC(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER1_COMP_C_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER1_CAPT_EXISTS
bool inline __attribute__((always_inline)) Timer1::activateInputCaptureInterrupt(void)
{
	setBit(TIMER1_CAPT_INT_ENABLE_REG, TIMER1_CAPT_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::clearInputCaptureInterruptRequest(void)
{
	setBit(TIMER1_CAPT_INT_FLAG_REG, TIMER1_CAPT_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::deactivateInputCaptureInterrupt(void)
{
	clrBit(TIMER1_CAPT_INT_ENABLE_REG, TIMER1_CAPT_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::getInputCapture(uint16_t * value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER1_CAPT_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer1::setInputCapture(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER1_CAPT_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
error_e	inline __attribute__((always_inline)) Timer1::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// Timer2 ----------------------------------------------------------------------

#if TIMER2_EXISTS
bool inline __attribute__((always_inline)) Timer2::activateOverflowInterrupt(void)
{
	setBit(TIMER2_OVF_INT_ENABLE_REG, TIMER2_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::clearOverflowInterruptRequest(void)
{
	setBit(TIMER2_OVF_INT_FLAG_REG, TIMER2_OVF_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::deactivateOverflowInterrupt(void)
{
	clrBit(TIMER2_OVF_INT_ENABLE_REG, TIMER2_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::getCounter(uint8_t *value)
{
	*value = TIMER2_COUNTER_VALUE_REG;
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::setCounter(uint8_t value)
{
	TIMER2_COUNTER_VALUE_REG = value;
	this->lastError_ = ERROR_NONE;
	return true;
}
#	if TIMER2_COMP_A_EXISTS
bool inline __attribute__((always_inline)) Timer2::activateCompareAInterrupt(void)
{
	setBit(TIMER2_COMP_A_INT_ENABLE_REG, TIMER2_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::clearCompareAInterruptRequest(void)
{
	setBit(TIMER2_COMP_A_INT_FLAG_REG, TIMER2_COMP_A_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::deactivateCompareAInterrupt(void)
{
	clrBit(TIMER2_COMP_A_INT_ENABLE_REG, TIMER2_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::forceCompareA(void)
{
	setBit(TIMER2_COMP_A_FORCE_MATCH_REG, TIMER2_COMP_A_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::getCompareA(uint8_t *value)
{
	*value = TIMER2_COMP_A_VALUE_REG;
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::setCompareA(uint8_t value)
{
	TIMER2_COMP_A_VALUE_REG = value;
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER2_COMP_B_EXISTS
bool inline __attribute__((always_inline)) Timer2::activateCompareBInterrupt(void)
{
	setBit(TIMER2_COMP_B_INT_ENABLE_REG, TIMER2_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::clearCompareBInterruptRequest(void)
{
	setBit(TIMER2_COMP_B_INT_FLAG_REG, TIMER2_COMP_B_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::deactivateCompareBInterrupt(void)
{
	clrBit(TIMER2_COMP_B_INT_ENABLE_REG, TIMER2_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::forceCompareB(void)
{
	setBit(TIMER2_COMP_B_FORCE_MATCH_REG, TIMER2_COMP_B_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::getCompareB(uint8_t *value)
{
	*value = TIMER2_COMP_B_VALUE_REG;
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer2::setCompareB(uint8_t value)
{
	TIMER2_COMP_B_VALUE_REG = value;
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
error_e	inline __attribute__((always_inline)) Timer2::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// Timer3 ----------------------------------------------------------------------

#if TIMER3_EXISTS
bool inline __attribute__((always_inline)) Timer3::activateOverflowInterrupt(void)
{
	setBit(TIMER3_OVF_INT_ENABLE_REG, TIMER3_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::clearOverflowInterruptRequest(void)
{
	setBit(TIMER3_OVF_INT_FLAG_REG, TIMER3_OVF_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::deactivateOverflowInterrupt(void)
{
	clrBit(TIMER3_OVF_INT_ENABLE_REG, TIMER3_OVF_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::getCounter(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER3_COUNTER_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::setCounter(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER3_COUNTER_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	if TIMER3_COMP_A_EXISTS
bool inline __attribute__((always_inline)) Timer3::activateCompareAInterrupt(void)
{
	setBit(TIMER3_COMP_A_INT_ENABLE_REG, TIMER3_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::clearCompareAInterruptRequest(void)
{
	setBit(TIMER3_COMP_A_INT_FLAG_REG, TIMER3_COMP_A_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::deactivateCompareAInterrupt(void)
{
	clrBit(TIMER3_COMP_A_INT_ENABLE_REG, TIMER3_COMP_A_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::forceCompareA(void)
{
	setBit(TIMER3_COMP_A_FORCE_MATCH_REG, TIMER3_COMP_A_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::getCompareA(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER3_COMP_A_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::setCompareA(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER3_COMP_A_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER3_COMP_B_EXISTS
bool inline __attribute__((always_inline)) Timer3::activateCompareBInterrupt(void)
{
	setBit(TIMER3_COMP_B_INT_ENABLE_REG, TIMER3_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::clearCompareBInterruptRequest(void)
{
	setBit(TIMER3_COMP_B_INT_FLAG_REG, TIMER3_COMP_B_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::deactivateCompareBInterrupt(void)
{
	clrBit(TIMER3_COMP_B_INT_ENABLE_REG, TIMER3_COMP_B_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::forceCompareB(void)
{
	setBit(TIMER3_COMP_B_FORCE_MATCH_REG, TIMER3_COMP_B_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::getCompareB(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER3_COMP_B_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::setCompareB(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER3_COMP_B_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER3_COMP_C_EXISTS
bool inline __attribute__((always_inline)) Timer3::activateCompareCInterrupt(void)
{
	setBit(TIMER3_COMP_C_INT_ENABLE_REG, TIMER3_COMP_C_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::clearCompareCInterruptRequest(void)
{
	setBit(TIMER3_COMP_C_INT_FLAG_REG, TIMER3_COMP_C_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::deactivateCompareCInterrupt(void)
{
	clrBit(TIMER3_COMP_C_INT_ENABLE_REG, TIMER3_COMP_C_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::forceCompareC(void)
{
	setBit(TIMER3_COMP_C_FORCE_MATCH_REG, TIMER3_COMP_C_FORCE_MATCH_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::getCompareC(uint16_t *value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER3_COMP_C_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::setCompareC(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER3_COMP_C_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
#	if TIMER3_CAPT_EXISTS
bool inline __attribute__((always_inline)) Timer3::activateInputCaptureInterrupt(void)
{
	setBit(TIMER3_CAPT_INT_ENABLE_REG, TIMER3_CAPT_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::clearInputCaptureInterruptRequest(void)
{
	setBit(TIMER3_CAPT_INT_FLAG_REG, TIMER3_CAPT_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::deactivateInputCaptureInterrupt(void)
{
	clrBit(TIMER3_CAPT_INT_ENABLE_REG, TIMER3_CAPT_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::getInputCapture(uint16_t * value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*value = TIMER3_CAPT_VALUE_REG;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
bool inline __attribute__((always_inline)) Timer3::setInputCapture(uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TIMER3_CAPT_VALUE_REG = value;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}
#	endif
error_e	inline __attribute__((always_inline)) Timer3::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// Timer4 ----------------------------------------------------------------------

#if TIMER4_EXISTS
error_e	inline __attribute__((always_inline)) Timer4::getLastError(void)
{
	return this->lastError_;
}
#endif

// =============================================================================
// Extern global variables
// *INDENT-OFF*
// =============================================================================

#if TIMER0_EXISTS
	extern Timer0 timer0;
#endif
#if TIMER1_EXISTS
	extern Timer1 timer1;
#endif
#if TIMER2_EXISTS
	extern Timer2 timer2;
#endif
#if TIMER3_EXISTS
	extern Timer3 timer3;
#endif
#if TIMER4_EXISTS
	extern Timer4 timer4;
#endif
// *INDENT-ON*

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __TIMER_HPP
