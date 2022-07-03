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

volatile static unsigned long f_cpu_ms;

__attribute__((always_inline)) static inline void setDelayClock(volatile unsigned long clk)
{
    f_cpu_ms = clk / 100000;
}

__attribute__((always_inline)) static inline void delayMs(volatile unsigned int ms)
{
    extern void __builtin_avr_delay_cycles(unsigned long);
    volatile unsigned long ticks = f_cpu_ms * ms - 1;

    __builtin_avr_delay_cycles(31);

    do {
        __builtin_avr_delay_cycles(75);
    }
    while(--ticks);

    __builtin_avr_delay_cycles(1);
}

#endif