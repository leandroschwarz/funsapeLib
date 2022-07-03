/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Integrated Library
 * File:			globalDefines.hc
 * Module:			Global definitions file for the GPDSE AVR8 Project
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 24, 2018
 * ---------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 101 - Build mismatch on header and source code files (globalDefines).
#endif

// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

systemConfiguration_t systemConfiguration = {.systemClock = 0, .clocksPerMs = 0};

// -----------------------------------------------------------------------------
// Public Functions Definitons -------------------------------------------------

void systemClock(uint32_t clock)
{
	systemConfiguration.systemClock = clock;
	systemConfiguration.clocksPerMs = (clock / 1000);

	return;
}
