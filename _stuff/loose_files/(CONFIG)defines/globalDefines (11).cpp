/* -----------------------------------------------------------------------------
 * Project:		GPSDE++ AVR Library
 * File name:	globalDefines.cpp
 * Module:		Global definitions for GPDSE++ AVR Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1001
 * Date:		May 26, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalConfigs.hpp"
#ifndef __GLOBAL_CONFIGS_HPP
#	error	[globalDefines.hpp] Error 100 - System configuration file (globalConfigs.hpp) is missing
#else
#	if __GLOBAL_CONFIGS_HPP != 1001
#		error 	[globalDefines.hpp] Error 101 - Build mismatch (globalConfigs.hpp and twiMaster.hpp)
#	endif		// __GLOBAL_CONFIGS_HPP != 1001
#endif		// __GLOBAL_CONFIGS_HPP

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1001
#	error [globalDefines.cpp] Error 103 - Build mismatch on header and source code files.
#endif

// -----------------------------------------------------------------------------
// File exclusive - Constant definitions ---------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - New data types ---------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - Macro-functions definitions --------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

SystemConfiguration systemConfiguration;

// -----------------------------------------------------------------------------
// SystemConfiguration class - Constructors definitions ------------------------

SystemConfiguration::SystemConfiguration(uint32_t mainClock)
{
	this->mainSourceClock_	= mainClock;
	this->clockPrescaler_	= SYSTEM_CLOCK_PRESCALER_1;
	this->cpuClock_			= mainClock;
	this->stopWatchValue_	= 0;
	this->stopWatchMark_	= 0;
	this->stopWatchHalted_	= TRUE;
	this->lastError_		= ERROR_NONE;
}

// -----------------------------------------------------------------------------
// SystemConfiguration class - Public methods definitions ----------------------

error_e SystemConfiguration::changeClockPrescaler(systemClockPrescaler_e prescaler)
{
	uint8_t auxPrescaler = 0;

	// Decode prescaler
	switch(prescaler) {
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
		this->lastError_ = ERROR_UNSUPPORTED_CONFIGURATION_SYSTEM_CLOCK_PRESCALER;
		return ERROR_UNSUPPORTED_CONFIGURATION_SYSTEM_CLOCK_PRESCALER;
	}

	// Changes system clock prescaler
	GPDSE_ATOMIC_BLOCK {
		CLKPR = 0b10000000;
		CLKPR = auxPrescaler;
	}

	// Verifies if the change was successful
	asm volatile("nop");
	asm volatile("nop");
	if((CLKPR & 0x7F) != auxPrescaler) {
		this->lastError_ = ERROR_SYSTEM_CLOCK_PRESCALER_COULD_NOT_BE_CHANGED;
		return ERROR_SYSTEM_CLOCK_PRESCALER_COULD_NOT_BE_CHANGED;
	}

	// Recalculate system values
	this->clockPrescaler_ = prescaler;
	this->cpuClock_ = this->mainSourceClock_ / prescaler;

	this->lastError_ = ERROR_NONE;
	return ERROR_NONE;
}

uint32_t SystemConfiguration::getCpuClock(void)
{
	return this->cpuClock_;
}

error_e SystemConfiguration::getLastError(void)
{
	return this->lastError_;
}

uint32_t SystemConfiguration::readStopWatch(void)
{
	return this->stopWatchValue_;
}

void SystemConfiguration::resumeStopWatch(void)
{
	this->stopWatchHalted_ = FALSE;
}

void SystemConfiguration::pauseStopWatch(void)
{
	this->stopWatchHalted_ = TRUE;
}

void SystemConfiguration::resetStopWatch(void)
{
	this->stopWatchValue_ = 0;
	this->stopWatchMark_ = 0;
}

void SystemConfiguration::setStopWatchMark(void)
{
	this->stopWatchMark_ = this->stopWatchValue_;
}

void SystemConfiguration::incrementStopWatch(void)
{
	this->stopWatchValue_++;
}

uint32_t SystemConfiguration::calculateElapsedTime(bool_t setNewMark)
{
	uint32_t start = this->stopWatchMark_;
	uint32_t current = this->stopWatchValue_;
	uint32_t elapsed = 0;

	elapsed = (current > start) ? (current - start) : (((0xFFFFFFFF - start)  + current) + 1);
	if(setNewMark) {
		this->stopWatchMark_ = current;
	}

	return elapsed;
}

uint32_t SystemConfiguration::calculateElapsedTime(uint32_t customMark)
{
	uint32_t current = this->stopWatchValue_;
	uint32_t elapsed = 0;

	elapsed = (current > customMark) ? (current - customMark) : (((0xFFFFFFFF - customMark)  + current) + 1);

	return elapsed;
}

void SystemConfiguration::announceError(error_e error)
{
	this->lastError_ = error;
}

// -----------------------------------------------------------------------------
// SystemConfiguration class - Private methods definitions ---------------------

/* NONE */

// -----------------------------------------------------------------------------
// SystemConfiguration class - Protected methods definitions -------------------

/* NONE */

// -----------------------------------------------------------------------------
// General public functions definitions ----------------------------------------

weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year)
{
	uint16_t aux16;
	year -= month < 3;
	aux16 = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + monthDay) % 7;
	aux16++;

	return (weekDay_t)aux16;
}
