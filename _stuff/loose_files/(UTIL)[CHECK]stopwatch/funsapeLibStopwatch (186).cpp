/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibStopwatch.cpp
 * Module:			Stopwatch Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Dependencies
// 

#include "funsapeLibStopwatch.hpp"
#ifndef __FUNSAPE_LIB_STOPWATCH_HPP
#	error	[funsapeLibStopwatch.cpp] Error 16 - Header file (funsapeLibStopwatch.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_STOPWATCH_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.cpp] Error 17 - Build mismatch between source (funsapeLibStopwatch.cpp) and header (funsapeLibStopwatch.hpp) files!
#endif

// 
// File exclusive - Constants
// 

// NONE

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

// NONE

// 
// Static functions declarations
// 

// NONE

// 
// Class constructors
// 

/**
 * @brief Construct a new Stopwatch::Stopwatch object
 * @details
 * @param[in]	timeout_p
 */
Stopwatch::Stopwatch(uint32_t timeout_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::Stopwatch(uint32_t)", DEBUG_CODE_STOPWATCH);

	// Resets data members
	this->_started						= false;
	this->_timeout						= timeout_p;
	this->_valueInitial					= getTick();
	this->_valueMark					= 0;

	// Update data members
	if(timeout_p) {
		this->_started = true;
		this->_valueMark = this->_valueInitial + timeout_p;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief Destroy the Stopwatch::Stopwatch object
 * @details
 */
Stopwatch::~Stopwatch(void)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::~Stopwatch(void)", DEBUG_CODE_STOPWATCH);

	// TODO: Implement destructor function

	// Returns successfully
	return;
}

// 
// Class own methods - Public
// 

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 * @details
 * @return uint32_t
 */
uint32_t Stopwatch::elapsed(void)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::elapsed(void)", DEBUG_CODE_STOPWATCH);

	// Local variables
	uint32_t elapsed = 0;

	// Check for errors
	if(!this->_started) {
		// Returns error
		this->_lastError = Error::STOPWATCH_NOT_STARTED;
		debugMessage(Error::STOPWATCH_NOT_STARTED);
		return 0;
	}

	// Count elapsed time
	elapsed = getTick();
	elapsed -= this->_valueInitial;

	// Returns result
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return elapsed;
}

/**
 * @brief
 * @details
 * @param[in]	stopOnSuccess_p
 * @return true
 * @return false
 */
bool Stopwatch::isTimedOut(bool stopOnSuccess_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::isTimedOut(bool)", DEBUG_CODE_STOPWATCH);

	// Local variables
	uint32_t current = getTick();

	// Check for errors
	if(!this->_started) {
		// Returns error - TRUE is sent to break any possible infinity loop
		this->_lastError = Error::STOPWATCH_NOT_STARTED;
		debugMessage(Error::STOPWATCH_NOT_STARTED);
		return true;
	}

	// Is timed out?
	if(current >= this->_valueMark) {
		if(stopOnSuccess_p) {			// Stop on success
			this->_started = false;
		} else {						// Do not stop on success
			this->_valueMark += this->_timeout;
		}
		// Returns successfully - Yes, it is timed out
		this->_lastError = Error::NONE;
		debugMessage(Error::NONE);
		return true;
	}

	// Returns successfully - No, it isn't timed out
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return false;
}

/**
 * @brief
 * @details
 * @param[in]	timeout_p
 */
void Stopwatch::start(uint32_t timeout_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::start(uint32_t)", DEBUG_CODE_STOPWATCH);

	// Start stopwatch
	this->_started						= true;
	this->_timeout						= timeout_p;
	this->_valueInitial					= getTick();
	this->_valueMark					= this->_valueInitial + timeout_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief
 * @details
 * @param[in]	stopOnSuccess_p
 * @return true
 * @return false
 */
bool Stopwatch::waitUntilTimedOut(bool stopOnSuccess_p)
{
	// Mark passage for debugging purpose
	debugMark("Stopwatch::waitUntilTimedOut(bool)", DEBUG_CODE_STOPWATCH);

	// Check for errors
	if(!this->_started) {
		// Returns error
		this->_lastError = Error::STOPWATCH_NOT_STARTED;
		debugMessage(Error::STOPWATCH_NOT_STARTED);
		return false;
	}

	// Check if already timed out
	while(!this->isTimedOut(stopOnSuccess_p)) {
		doNothing();
	}

	// Returns result
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// 
// Class own methods - Private
// 

// NONE

// 
// Class own methods - Protected
// 

// NONE

// 
// END OF FILE
// 
