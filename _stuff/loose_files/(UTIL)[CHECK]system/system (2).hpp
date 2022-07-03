/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	system.hpp
 * Module:		System definitions for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

#ifndef __SYSTEM_HPP
#define __SYSTEM_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[system.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include <stddef.h>
#include <stdint.h>

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

#if defined(_PLATFORM_AVR_)
typedef enum systemClockPrescaler_e {
	SYSTEM_CLOCK_PRESCALER_1		= 1,
	SYSTEM_CLOCK_PRESCALER_2		= 2,
	SYSTEM_CLOCK_PRESCALER_4		= 4,
	SYSTEM_CLOCK_PRESCALER_8		= 8,
	SYSTEM_CLOCK_PRESCALER_16		= 16,
	SYSTEM_CLOCK_PRESCALER_32		= 32,
	SYSTEM_CLOCK_PRESCALER_64		= 64,
	SYSTEM_CLOCK_PRESCALER_128		= 128,
	SYSTEM_CLOCK_PRESCALER_256		= 256
} systemClockPrescaler_e;
#endif

// =============================================================================
// SystemStatus Class
// =============================================================================

class SystemStatus {
public:
	// Methods
	SystemStatus(uint32_t mainClock = GPDSE_CPU_CLOCK);
#if defined(_PLATFORM_AVR_)
	bool changeClockPrescaler(systemClockPrescaler_e prescaler);
#endif
	uint32_t getCpuClock(void);
	error_e getLastError(void);
	uint32_t readStopwatch(void);
	void resumeStopwatch(void);
	void pauseStopwatch(void);
	void resetStopwatch(void);
	void setStopwatchMark(void);
	void incrementStopwatch(void);
	uint32_t calculateElapsedTime(bool setNewMark = true);

private:
	// Methods
	uint32_t calculateElapsedTime_(uint32_t customMark);

	// Members
private:
	// System clock
	uint32_t				cpuClock_			: 28;
#if defined(_PLATFORM_AVR_)
	systemClockPrescaler_e	clockPrescaler_		: 10;
	uint32_t				mainSourceClock_	: 28;
#endif
	// Stopwatch
	volatile uint32_t		stopwatchValue_;
	volatile uint32_t		stopwatchMark_;
	bool					stopwatchHalted_	: 1;
	// Error
	error_e					lastError_;
protected:
};

// Class external global objects
extern SystemStatus systemStatus;

#endif // __SYSTEM_HPP
