/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseAvrSystem.cpp
 * Module:		System definitions for GPDSE++ AVR Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseAvrSystem.hpp>
#if __GPDSE_AVR_SYSTEM_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [gpdseAvrSystem.cpp] Build mismatch on header and source code files.
#endif

#include <util/atomic.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

SystemStatus systemStatus(_GPDSE_CPU_CLOCK);

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

SystemStatus::SystemStatus(uint32_t mainClock)
{
	// Reset data members
	this->_initialized = false;
	this->_cpuClock = 0;
	this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_1;
	this->_cpuClockMainSourceValue = 0;
	this->_stopwatchValue = 0;
	this->_stopwatchMark = 0;
	this->_stopwatchHalted = true;
	this->_lastError = ERROR_NONE;

	// Check for errors - Clock value
	if (mainClock == 0) {
		this->_lastError = ERROR_CLOCK_SPEED_TOO_LOW;
		return;
	}

	// Update data members
	this->_cpuClockMainSourceValue = mainClock;
	this->_cpuClock = this->_cpuClockMainSourceValue / this->_cpuClockPrescaler;
	// Return successfully
	this->_initialized = true;
	this->_lastError = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool SystemStatus::setCpuClockPrescaler(cpuClockPrescaler_e prescaler)
{
	uint8_t auxPrescaler = 0;

	// Decode prescaler
	switch (prescaler) {
	case CPU_CLOCK_PRESCALER_1:
		auxPrescaler = 0;
		break;
	case CPU_CLOCK_PRESCALER_2:
		auxPrescaler = 1;
		break;
	case CPU_CLOCK_PRESCALER_4:
		auxPrescaler = 2;
		break;
	case CPU_CLOCK_PRESCALER_8:
		auxPrescaler = 3;
		break;
	case CPU_CLOCK_PRESCALER_32:
		auxPrescaler = 4;
		break;
	case CPU_CLOCK_PRESCALER_64:
		auxPrescaler = 5;
		break;
	case CPU_CLOCK_PRESCALER_128:
		auxPrescaler = 6;
		break;
	case CPU_CLOCK_PRESCALER_256:
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
	doNop();
	if ((CLKPR & 0x7F) != auxPrescaler) {
		this->_lastError = ERROR_CLOCK_PRESCALER_CHANGE_FAILED;
		return false;
	}

	// Update member data
	this->_cpuClockPrescaler = prescaler;
	this->_cpuClock = this->_cpuClockMainSourceValue / prescaler;

	// Error - Clock value equal zero
	if (this->_cpuClock == 0) {
		this->_lastError = ERROR_CLOCK_MAIN_SOURCE_VALUE_ZERO;
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool SystemStatus::getCpuClock(uint32_t * cpuClock)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	*cpuClock = this->_cpuClockMainSourceValue;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

error_e SystemStatus::getLastError(void)
{
	return this->_lastError;
}

bool SystemStatus::getStopwatch(uint32_t * stopwatch)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	*stopwatch = this->_stopwatchValue;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool SystemStatus::resumeStopwatch(void)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	this->_stopwatchHalted = false;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool SystemStatus::pauseStopwatch(void)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	this->_stopwatchHalted = true;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool SystemStatus::resetStopwatch(void)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	this->_stopwatchValue = 0;
	this->_stopwatchMark = 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool SystemStatus::setStopwatchMark(void)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	this->_stopwatchMark = this->_stopwatchValue;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool SystemStatus::incrementStopwatch(void)
{
	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data
	this->_stopwatchValue++;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

uint32_t SystemStatus::getElapsedTime(bool setNewMark)
{
	uint32_t start = this->_stopwatchMark;
	uint32_t current = this->_stopwatchValue;
	uint32_t elapsed = 0;

	// Check for errors - not initialized
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return 0;
	}

	// Update data
	elapsed = (current > start) ? (current - start) : (((0xFFFFFFFF - start)  + current) + 1);
	if (setNewMark) {
		this->_stopwatchMark = current;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return elapsed;
}

// =============================================================================
// Class private methods
// =============================================================================

void SystemStatus::_getCpuClockPrescaler(void)
{
	uint16_t aux16 = 0;

	// Get prescaler
	aux16 = (CLKPR & 0x7F);

	// Decode prescaler
	switch (aux16) {
	case 0:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_1;
		break;
	case 1:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_2;
		break;
	case 2:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_4;
		break;
	case 3:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_8;
		break;
	case 4:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_32;
		break;
	case 5:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_64;
		break;
	case 6:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_128;
		break;
	case 7:
		this->_cpuClockPrescaler = CPU_CLOCK_PRESCALER_256;
		break;
	default:
		this->_lastError = ERROR_CLOCK_PRESCALER_UNSUPPORTED;
		return;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Interruption handlers
// =============================================================================

// NONE
