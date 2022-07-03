/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Integrated Library
 * File:			globalDefines.h
 * Module:			Global definitions file for the GPDSE AVR8 Project
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			2
 * Last edition:	April 22, 2018
 * ---------------------------------------------------------------------------*/

#include "globalDefines.h"

// #if defined(SIGNATURE_0) && defined(SIGNATURE_1) && defined(SIGNATURE_2)
// const unsigned char __signature[3] __attribute__((__used__, __section__(".signature"))) = { SIGNATURE_2, SIGNATURE_1, SIGNATURE_0 };
// #endif


// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

systemConfiguration_s systemConfiguration = {.systemClock = 0, .clocksPerMs = 0};

// -----------------------------------------------------------------------------
// Public Functions Definitons -------------------------------------------------

void systemClock(uint32_t clock)
{
	systemConfiguration.systemClock = clock;
	systemConfiguration.clocksPerMs = (clock / 1000);

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

