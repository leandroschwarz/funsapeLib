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

systemConfiguration_s systemConfiguration = {.systemClockKHz = 0};

// -----------------------------------------------------------------------------
// Public Functions Definitons -------------------------------------------------

/*!	\fn		void systemClock(uint32_t clock)
	\brief	Sets the \a clock as the main system clock.
	\param clock	Clock frequency in Hz.

		The function will divide the given clock frequency value by 1000 to obtain the clock frequency in kilohertz. The valus is store in the systemConfiguration_s structure.
*/
void systemClock(uint32_t clock)
{
	systemConfiguration.systemClockKHz = (uint16_t)(clock / 1000);

	return;
}
