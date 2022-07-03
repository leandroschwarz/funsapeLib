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
systemConfiguration_s systemConfiguration = {.systemClockKHz = 0};
#endif

// -----------------------------------------------------------------------------
// Public Functions Definitions ------------------------------------------------

void systemClock(uint32_t clock)
{
	systemConfiguration.systemClockKHz = (clock / 1000);

	return;
}

weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year)
{
	weekDay_t weekDay;
	year -= month < 3;
	weekDay = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + monthDay) % 7;
	weekDay++;

	return weekDay;
}
