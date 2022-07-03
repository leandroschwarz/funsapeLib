/* 
 * Project:		FunSAPE++ Library
 * File name:	funsapeLibSystemStatus.cpp
 * Module:		System status Class for FunSAPE++ Library project
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// 
// System file dependencies
// 

#include "funsapeLibSystemStatus.hpp"
#if __FUNSAPE_LIB_SYSTEM_STATUS_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [funsapeLibSystemStatus.cpp] Build mismatch on header and source code files.
#endif

#include <util/atomic.h>

// 
// File exclusive - Constants
// 

#define		TWI_DEFAULT_TIME_OUT			20

// 
// File exclusive - New data types
// 

// NONE

// 
// File exclusive - Macro-functions
// 

// NONE

// 
// Global variables
// 

SystemStatus systemStatus;

// 
// Class constructors
// 

SystemStatus::SystemStatus(void)
{
	// Reset data members
	this->_cpuClockValue	= F_CPU;
	this->_sourceClockValue	= F_CPU;
	this->_clockPrescaler	= SYSTEM_CLOCK_PRESCALER_1;
	this->_stopwatchMark	= 0;
	this->_stopwatchHalted	= false;

	// Return successfully
	this->_lastError		= ERROR_NONE;
	return;
}

SystemStatus::~SystemStatus(void)
{
	// Return successfully
	return;
}

// 
// Class public methods
// 

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
		this->_lastError = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
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
		this->_lastError = ERROR_CLOCK_PRESCALER_CHANGE_FAILED;
		return false;
	}

	// Update member data
	this->_clockPrescaler = prescaler;
	this->_cpuClockValue = this->_sourceClockValue / prescaler;
	this->_lastError = ERROR_NONE;

	return true;
}
#endif

uint32_t SystemStatus::getCpuClock(void)
{
	return this->_cpuClockValue;
}

error_e SystemStatus::getLastError(void)
{
	return this->_lastError;
}

uint32_t SystemStatus::readStopwatch(void)
{
	return this->_stopwatchValue;
}

void SystemStatus::resumeStopwatch(void)
{
	this->_stopwatchHalted = false;
}

void SystemStatus::pauseStopwatch(void)
{
	this->_stopwatchHalted = true;
}

void SystemStatus::resetStopwatch(void)
{
	this->_stopwatchValue = 0;
	this->_stopwatchMark = 0;
}

void SystemStatus::setStopwatchMark(void)
{
	this->_stopwatchMark = this->_stopwatchValue;
}

void SystemStatus::incrementStopwatch(void)
{
	this->_stopwatchValue++;
}

uint32_t SystemStatus::getElapsedTime(bool setNewMark)
{
	uint32_t start = this->_stopwatchMark;
	uint32_t current = this->_stopwatchValue;
	uint32_t elapsed = 0;

	elapsed = (current > start) ? (current - start) : (((0xFFFFFFFF - start)  + current) + 1);
	if (setNewMark) {
		this->_stopwatchMark = current;
	}

	return elapsed;
}

// 
// Class private methods
// 

/* NONE */

// 
// Class public methods
// 

/* NONE */
