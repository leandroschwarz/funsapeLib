/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Integrated Library
 * File:			delay.h
 * Module:			Simple delay for the GPDSE AVR8 Project
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * ---------------------------------------------------------------------------*/

//// THIS CODE IS PENDING FOR FURTHER TESTS AND IS NOT 100% ACCURATE
//// DO NOT USE IN A REAL APPLICATION
//// TODO: Move f_cpu_ms location, add delayUs and delayS

#ifndef __DELAY_H
#define __DELAY_H 1

#include <globalDefines.h>

static inline __attribute__((always_inline)) void delayMs(volatile unsigned int ms)
{
	vuint32_t ticks;

	if(ms == 0) {
		return;
	}

	ticks = systemConfiguration.clocksPerMs * ms;
	ticks -= 576;
	ticks /= 100;

	while(ticks > 0) {
		__builtin_avr_delay_cycles(67);
		ticks--;
	}
	return;
}

#endif