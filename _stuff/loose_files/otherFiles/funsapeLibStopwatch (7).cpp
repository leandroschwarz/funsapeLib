/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibStopwatch.cpp
 * Module:			Stopwatch Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibStopwatch.hpp"
#ifndef __FUNSAPE_LIB_STOPWATCH_HPP
#	error	[funsapeLibStopwatch.cpp] Error 1 - Header file (funsapeLibStopwatch.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_STOPWATCH_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.cpp] Error 13 - Build mismatch between source (funsapeLibStopwatch.cpp) and header (funsapeLibStopwatch.hpp) files!
#endif

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

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Stopwatch::Stopwatch object
 *
 * @param[in] timeout_p
 */
Stopwatch::Stopwatch(uint32_t timeout_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::Stopwatch(uint32_t)", 4);

	// Resets data members
	this->_started						= false;
	this->_timeout						= timeout_p;
	this->_valueInitial					= getTick();
	this->_valueMark					= 0;

	if(timeout_p) {
		this->_started = true;
		this->_valueMark = this->_valueInitial + timeout_p;
	}

	// Returns successfully
	return;
}

/**
 * @brief Destroy the Stopwatch::Stopwatch object
 *
 */
Stopwatch::~Stopwatch(void)
{
	// TODO: Implement destructor function

	// Returns successfully
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

/**
 * @brief
 *
 * @param[in] timeout_p
 */
void Stopwatch::start(uint32_t timeout_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::start(uint32_t)", 4);

	// Start stopwatch
	this->_started						= true;
	this->_timeout						= timeout_p;
	this->_valueInitial					= getTick();
	this->_valueMark					= this->_valueInitial + timeout_p;

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * @return uint32_t
 */
uint32_t Stopwatch::elapsed(void)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::elapsed(void)", 4);

	// Local variables
	uint32_t elapsed = 0;

	// Check for errors
	if(!this->_started) {
		return 0;
	}

	// Count elapsed time
	elapsed = getTick();
	elapsed -= this->_valueInitial;

	// Returns result
	return elapsed;
}

/**
 * @brief
 *
 * @param[in] stopOnSuccess_p
 * @return true
 * @return false
 */
bool Stopwatch::isTimedOut(bool stopOnSuccess_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::isTimedOut(bool)", 4);

	// Local variables
	uint32_t current = getTick();
	bool timedOut = false;

	// Check for errors
	if(!this->_started) {
		return false;
	}

	// Check if already timed out
	timedOut = (current >= this->_valueMark) ? true : false;

	// Stop on success
	if(stopOnSuccess_p && timedOut) {
		this->_started = false;
	}

	if((!stopOnSuccess_p) && timedOut) {
		this->_valueMark += this->_timeout;
	}

	// Returns result
	return timedOut;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
