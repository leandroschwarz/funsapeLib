/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	system.cpp
 * Module:		System definitions for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "system.hpp"
#if __SYSTEM_HPP != 1002
#	error [system.cpp] Error 102 - Build mismatch on header and source code files.
#endif

#if defined(_PLATFORM_AVR_)
#	include <avr/io.h>
#	include <util/atomic.h>
#endif

// =============================================================================
// Constants - File exclusive
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

SystemStatus systemStatus;

// =============================================================================
// Public Functions Definitions
// =============================================================================

/* NONE */

// =============================================================================
// Class constructors
// =============================================================================

SystemStatus::SystemStatus(uint32_t mainClock)
{
	this->cpuClock_ = mainClock;
#if defined(_PLATFORM_AVR_)
	this->mainSourceClock_ = mainClock;
	this->clockPrescaler_ = SYSTEM_CLOCK_PRESCALER_1;
#endif
	this->lastError_ = ERROR_NONE;
	this->stopWatchMark_ = 0;
	this->stopWatchHalted_ = false;
	this->lastError_ = ERROR_NONE;
}

// =============================================================================
// Class public methods
// =============================================================================

#if defined(_PLATFORM_AVR_)
bool SystemStatus::changeClockPrescaler(systemClockPrescaler_e prescaler)
{
	uint8_t auxPrescaler = 0;

	// Decode prescaler
	switch (prescaler) {
	case SYSTEM_CLOCK_PRESCALER_1:
		auxPrescaler = 0;
		break;
	case SYSTEM_CLOCK_PRESCALER_2:
		auxPrescaler = 1;
		break;
	case SYSTEM_CLOCK_PRESCALER_4:
		auxPrescaler = 2;
		break;
	case SYSTEM_CLOCK_PRESCALER_8:
		auxPrescaler = 3;
		break;
	case SYSTEM_CLOCK_PRESCALER_32:
		auxPrescaler = 4;
		break;
	case SYSTEM_CLOCK_PRESCALER_64:
		auxPrescaler = 5;
		break;
	case SYSTEM_CLOCK_PRESCALER_128:
		auxPrescaler = 6;
		break;
	case SYSTEM_CLOCK_PRESCALER_256:
		auxPrescaler = 7;
		break;
	default:
		this->lastError_ = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
		return false;
	}

	// Changes system clock prescaler
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		CLKPR = 0b10000000;
		CLKPR = auxPrescaler;
	}

	// Verifies if the change was successful
	asm volatile("nop");
	if ((CLKPR & 0x7F) != auxPrescaler) {
		this->lastError_ = ERROR_CLOCK_PRESCALER_CHANGE_FAILED;
		return false;
	}

	// Update member data
	this->clockPrescaler_ = prescaler;
	this->cpuClock_ = this->mainSourceClock_ / prescaler;
	this->lastError_ = ERROR_NONE;

	return true;
}
#endif

uint32_t SystemStatus::getCpuClock(void)
{
	return this->cpuClock_;
}

error_e SystemStatus::getLastError(void)
{
	return this->lastError_;
}

uint32_t SystemStatus::readStopWatch(void)
{
	return this->stopWatchValue_;
}

void SystemStatus::resumeStopWatch(void)
{
	this->stopWatchHalted_ = false;
}

void SystemStatus::pauseStopWatch(void)
{
	this->stopWatchHalted_ = true;
}

void SystemStatus::resetStopWatch(void)
{
	this->stopWatchValue_ = 0;
	this->stopWatchMark_ = 0;
}

void SystemStatus::setStopWatchMark(void)
{
	this->stopWatchMark_ = this->stopWatchValue_;

}

void SystemStatus::incrementStopWatch(void)
{
	this->stopWatchValue_++;
}

uint32_t SystemStatus::calculateElapsedTime(bool setNewMark)
{
	uint32_t start = this->stopWatchMark_;
	uint32_t current = this->stopWatchValue_;
	uint32_t elapsed = 0;

	elapsed = (current > start) ? (current - start) : (((0xFFFFFFFF - start)  + current) + 1);
	if (setNewMark) {
		this->stopWatchMark_ = current;
	}

	return elapsed;
}

// =============================================================================
// Class private methods
// =============================================================================

/* NONE */

// =============================================================================
// Class public methods
// =============================================================================

/* NONE */
