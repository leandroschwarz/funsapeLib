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
#	error	[funsapeLibStopwatch.cpp] Error 1 - Header file (funsapeLibStopwatch.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_STOPWATCH_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.cpp] Error 13 - Build mismatch between source (funsapeLibStopwatch.cpp) and header (funsapeLibStopwatch.hpp) files!
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

Stopwatch::Stopwatch(uint32_t timeout_p)
{
	// Resets data members
	this->_started = false;
	this->_valueInitial = 0;
	this->_valueMark = 0;
	this->_timeout = timeout_p;

	if(timeout_p) {
		this->_started = true;
		this->_valueMark = timeout_p;
		this->_timeout = timeout_p;
	}

	// Returns successfully
	return;
}

Stopwatch::~Stopwatch(void)
{
	// Returns successfully
	return;
}

// 
// Inherited methods - Public
// 

// NONE

// 
// Inherited methods - Protected
// 

// NONE

// 
// Class own methods - Public
// 

void Stopwatch::start(uint32_t timeout_p)
{
	// Start stopwatch
	this->_started = true;
	this->_valueInitial = getTick();
	this->_valueMark = this->_valueInitial + timeout_p;
	this->_timeout = timeout_p;

	// Returns successfully
	return;
}

uint32_t Stopwatch::elapsed(void)
{
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

bool Stopwatch::isTimedOut(bool stopOnSuccess_p)
{
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

	if(timedOut) {
		this->_valueMark += this->_timeout;
	}

	// Returns result
	return timedOut;
}

// 
// Class own methods - Private
// 

// NONE

// 
// Class own methods - Protected
// 

// NONE
