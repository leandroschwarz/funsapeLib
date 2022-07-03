/* =============================================================================
 * Project:		FunSAPE++ AVR Library
 * File name:	funsapeAvrSpi.cpp
 * Module:		SPI Peripheral controller class
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "funsapeAvrSpi.hpp"
#if __FUNSAPE_AVR_SPI_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [funsapeAvrSpi.cpp] Build mismatch on header and source code files.
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
// Class constructors
// =============================================================================


Spi0::Spi0(
		uint32_t	maxClockSpeed,
		spiMode_e	mode,
		bool		lsbFirst,
		uint8_t		bufferSize
)
{

}

// =============================================================================
// Class public methods
// =============================================================================

bool Spi0::deinitialize(void)
{
	return true;
}

bool Spi0::initialize(void)
{
	return true;
}

bool Spi0::setBufferSize(uint8_t bufferSize)
{
	return true;
}

bool Spi0::setClock(uint32_t maxClockSpeed)
{
	return true;
}

bool Spi0::setDefaultDevice(void actFuncCall(void), void deactFuncCall(void))
{
	return true;
}

bool Spi0::setMode(spiMode_e mode, bool lsbFirst)
{
	return true;
}

bool Spi0::setTimeOut(uint8_t timeOut)
{
	return true;
}

error_e Spi0::getLastError(void)
{
	return ERROR_NONE;
}

bool Spi0::isReceptionCompleted(void)
{
	return true;
}

bool Spi0::sendData(uint8_t *msg, uint8_t msgSize)
{
	return true;
}

bool Spi0::sendData(uint8_t reg, uint8_t *msg, uint8_t msgSize)
{
	return true;
}

bool Spi0::readData(uint8_t *bufferData, uint8_t bufferSize, uint8_t *bytesReceived)
{
	return true;
}

void Spi0::interruptHandler(void)
{
	return;
}

bool Spi0::setInterrupt(bool enabled)
{
	return true;
}


// =============================================================================
// Class private methods
// =============================================================================

// NONE

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
