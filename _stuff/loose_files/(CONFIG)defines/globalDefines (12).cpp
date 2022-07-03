/*!
	\file		globalDefines.c
	\brief		Global definitions for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 27, 2018
*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1000
	#error [globalDefines.cpp] Error 101 - Build mismatch on header and source code files.
#endif

// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

SystemStatus systemStatus;

// -----------------------------------------------------------------------------
// Public Functions Definitions ------------------------------------------------



weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year)
{
	uint16_t aux16;
	year -= month < 3;
	aux16 = (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + monthDay) % 7;
	aux16++;

	return (weekDay_t)aux16;
}




/////////////////////////// CLASS METHODS

SystemStatus::SystemStatus(uint32_t mainClock)
{
	this->mainSourceClock_ = mainClock;
	this->clockPrescaler_ = SYSTEM_CLOCK_PRESCALER_1;
	this->cpuClock_ = F_CPU;
	this->lastError_ = ERROR_NONE;
	this->stopWatchMark_ = 0;
	this->stopWatchHalted_ = FALSE;
}

// NOTE: the function WILL NOT evaluate other peripherals clocks. clock changes
//		must take that into account.
error_e SystemStatus::changeClockPrescaler(systemClockPrescaler_e prescaler)
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
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
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

uint32_t SystemStatus::getCpuClock(void)
{
	return this->cpuClock_;
}

error_e SystemStatus::getLastError(void)
{
	return this->lastError_;
}

void SystemStatus::announceError(error_e error)
{
	this->lastError_ = error;
}


uint32_t SystemStatus::readStopWatch(void)
{
	return this->stopWatchValue_;
}

void SystemStatus::resumeStopWatch(void)
{
	this->stopWatchHalted_ = FALSE;
}

void SystemStatus::pauseStopWatch(void)
{
	this->stopWatchHalted_ = TRUE;

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
uint32_t SystemStatus::calculateElapsedTime(bool_t setNewMark)
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

uint32_t SystemStatus::calculateElapsedTime(uint32_t customMark)
{
	uint32_t current = this->stopWatchValue_;
	uint32_t elapsed = 0;

	elapsed = (current > customMark) ? (current - customMark) : (((0xFFFFFFFF - customMark)  + current) + 1);

	return elapsed;
}

void SystemStatus::incrementStopWatch(void)
{
	this->stopWatchValue_++;
}
