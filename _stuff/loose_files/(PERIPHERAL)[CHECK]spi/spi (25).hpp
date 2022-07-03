/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	spi.hpp
 * Module:		SPI Interface Module for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 *				Fábio Cabral Pacheco
 * Build:		1002
 * Date:		July 04, 2018
 * ========================================================================== */

#ifndef __SPI_HPP
#define __SPI_HPP	1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[spi.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include "system.hpp"

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

typedef enum spiMode_e {
	SPI_MODE_0 = 0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
} spiMode_e;

typedef enum spiOperation_t {
	SPI_WRITE	= 0x00,
	SPI_READ	= 0x80
} spiOperation_t;

typedef enum spiPrescaler_e {
	SPI_PRESCALER_4 = 0,
	SPI_PRESCALER_16,
	SPI_PRESCALER_64,
	SPI_PRESCALER_128,
} spiPrescaler_e;

// =============================================================================
// SPI - Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Spi0 ------------------------------------------------------------------------

#if SPI0_EXISTS
class Spi0
{
	// Constructors
public:
	Spi0();

	// Methods - Public
public:
	bool setDefaultDevice(void actFuncCall(void), void deactFuncCall(void));
	bool sendData(uint8_t *msg, uint8_t msgSize = 1);
	bool sendData(uint8_t reg, uint8_t *msg, uint8_t msgSize = 1);
	bool init(uint32_t maxSpeed = 1000000, uint8_t bufferSize = 20);
	bool setMode(spiMode_e mode = SPI_MODE_0, bool lsbFirst = false);
	bool setInterrupt(controlMode_t controlMode = CONTROL_POOLING);
//	bool setTimeOut(uint8_t timeOut);
	void interruptHandler(void);
	error_e getLastError(void);

	// Methods - Private
private:
	void holdGpio(void);
	void releaseGpio(void);
//	bool waitWhileIsBusy(void);

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	void			(*activateDefaultDevice_)(void);
	void			(*deactivateDefaultDevice_)(void);
	bool			masterInitialized_	: 1;
	bool			slaveInitialized_	: 1;
	bool			useInterrupt_		: 1;
	bool			callbacksReady_		: 1;
	spiPrescaler_e	clockPrescaler_		: 2;
	bool			doubleSpeed_		: 1;
	spiMode_e		mode_				: 2;
	bool			lsbFirst_			: 1;
	uint8_t			*bufferData_;
	uint8_t			bufferMaxSize_		: 7;
	uint8_t			bufferIndex_		: 7;
	uint8_t			bufferLength_		: 7;
	uint8_t			timeOut_;
	error_e			lastError_;
//	uint8_t			tempBytesToSend_	: 7;

// Data members - Protected
protected:

}; // class Spi0
#endif

// -----------------------------------------------------------------------------
// Spi1 ------------------------------------------------------------------------

#if SPI1_EXISTS
class Spi1
{
	// Constructors
public:
	Spi1();

	// Methods - Public
public:
	bool init(uint8_t bufferSize = 20);
	bool setDefaultDevice(void actFuncCall(void), void deactFuncCall(void));
	bool setMasterClock(uint32_t maxSpeed = 1000000);
	bool setMode(controlMode_t controlMode = CONTROL_POOLING, spiMode_e mode = SPI_MODE_0, bool lsbFirst = false);
	bool sendData(uint8_t *msg, uint8_t msgSize = 1);
	bool sendData(uint8_t reg, uint8_t *msg, uint8_t msgSize = 1);
//	bool setTimeOut(uint8_t timeOut);
	void interruptHandler(void);
	error_e getLastError(void);

	// Methods - Private
private:
	void holdGpio(void);
	void releaseGpio(void);
//	bool waitWhileIsBusy(void);

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	void			(*activateDefaultDevice_)(void);
	void			(*deactivateDefaultDevice_)(void);
	bool			initialized_		: 1;
	bool			useInterrupt_		: 1;
	bool			callbacksReady_		: 1;
	spiPrescaler_e	clockPrescaler_		: 2;
	bool			doubleSpeed_		: 1;
	spiMode_e		mode_				: 2;
	bool			lsbFirst_			: 1;
	bool			masterEnable_		: 1;
	uint8_t			*bufferData_;
	uint8_t			bufferMaxSize_		: 7;
	uint8_t			bufferIndex_		: 7;
	uint8_t			bufferLength_		: 7;
	uint8_t			timeOut_;
	error_e			lastError_;
//	uint8_t			tempBytesToSend_	: 7;

// Data members - Protected
protected:

}; // class Spi1
#endif

// =============================================================================
// Spi - Class inline function definitions
// =============================================================================

/* NONE */

// =============================================================================
// Extern global variables
// *INDENT-OFF*
// =============================================================================

#if SPI0_EXISTS
	extern Spi0 spi0;
#endif
#if SPI1_EXISTS
	extern Spi1 spi1;
#endif
// *INDENT-ON*

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __SPI_HPP
