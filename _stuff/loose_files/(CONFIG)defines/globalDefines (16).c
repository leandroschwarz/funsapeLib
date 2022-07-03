/*!
	\file		globalDefines.c
	\brief		Global definitions for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 27, 2018
*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 101 - Build mismatch on header and source code files (globalDefines).
#endif

// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

#ifndef __DOXYGEN__
systemConfiguration_s systemConfiguration = {.mainSourceClock = SYSTEM_MAIN_SOURCE_CLOCK_DEFAULT, .clockPrescaler = SYSTEM_CLOCK_PRESCALER_DEFAULT, .cpuClock = SYSTEM_CPU_CLOCK_DEFAULT};
#endif

// -----------------------------------------------------------------------------
// Public Functions Definitions ------------------------------------------------


// NOTE: the function WILL NOT evaluate other peripherals clocks. clock changes
//		must take that into account.
error_e systemClockPrescalerChange(systemClockPrescaler_e prescaler)
{
	uint8_t shifter = 0;

	// Makes use of the fact that every prescaler is a division by 2, therefore,
	//		implements prescaler as a bit-shifter as result - switch has NO-BREAKs
	shifter = 0;
	switch(prescaler) {
	case SYSTEM_CLOCK_PRESCALER_256:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_128:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_64:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_32:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_8:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_4:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_2:
		shifter++;
	case SYSTEM_CLOCK_PRESCALER_1:
		break;
	case SYSTEM_CLOCK_PRESCALER_NO_CHANGE:
		return ERROR_NONE;
	default:
		return ERROR_UNSUPPORTED_CONFIGURATION_SYSTEM_CLOCK_PRESCALER;
	}

	// Changes system clock prescaler
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		CLKPR = 0b10000000;
		CLKPR = shifter;
	}

	// Verifies if the change was successful
	asm volatile("nop");
	asm volatile("nop");
	if((CLKPR & 0x80) != shifter) {
		return ERROR_SYSTEM_CLOCK_PRESCALER_COULD_NOT_BE_CHANGED;
	}

	// Recalculate system values
	systemConfiguration.clockPrescaler = prescaler;
	systemConfiguration.cpuClock = systemConfiguration.mainSourceClock >> shifter;

	return ERROR_NONE;
}

weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year)
{
	weekDay_t weekDay;
	year -= month < 3;
	weekDay = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + monthDay) % 7;
	weekDay++;

	return weekDay;
}
