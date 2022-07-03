/* 
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibSpi.cpp
 * Module:			Spi Peripheral Module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Dependencies
// 

#include "funsapeLibSpi.hpp"
#ifndef __FUNSAPE_LIB_SPI_HPP
#	error	[funsapeLibSpi.cpp] Error 1 - Header file (funsapeLibSpi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_SPI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibSpi.cpp] Error 13 - Build mismatch between source (funsapeLibSpi.cpp) and header (funsapeLibSpi.hpp) files!
#endif

// 
// File exclusive - Constants
// 

#define		SPI_DEFAULT_TIME_OUT				30

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

Spi::Spi(SpiInstance spiInstance_p)
{
	// Resets data members
	this->_initialized					= false;
	this->_timeOut						= SPI_DEFAULT_TIME_OUT;

	// Configure instance
	switch(spiInstance_p) {
	case SPI_1:
		this->_spiHandler.Instance = SPI1;
		break;
	case SPI_2:
		this->_spiHandler.Instance = SPI2;
		break;
	case SPI_3:
		this->_spiHandler.Instance = SPI3;
		break;
	default:
		this->_lastError = Error::INSTANCE_INVALID;
		return;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

Spi::~Spi(void)
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

bool Spi::setTimeOut(uint16_t timeOut_p)
{
	// Update data members
	this->_timeOut = timeOut_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

Error Spi::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

bool Spi::init(void)
{
	this->_initialized = false;

//	this->_spiHandler.Instance->CR1 =

	return false;
}

// 
// Class own methods - Private
// 

// NONE

// 
// Class own methods - Protected
// 

// NONE
