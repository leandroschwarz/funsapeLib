/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	gpdseArmSpi.h
 * Module:		SPI interface module for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_ARM_SPI_H
#define __GPDSE_ARM_SPI_H						__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "gpdseArmGlobalDefines.h"
#if __GPDSE_ARM_GLOBAL_DEFINES_H != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (gpdseArmGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constants
// =============================================================================

#define SPI_MIN_BUFFER_SIZE				10
#define SPI_MAX_BUFFER_SIZE				255

// =============================================================================
// Macro-functions
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

typedef enum spiOperation_t {
	SPI_WRITE	= 0x00,
	SPI_READ	= 0x80
} spiOperation_t;

// =============================================================================
// Spi - Class declaration
// =============================================================================

class Spi
{
	// Constructors
public:
	Spi();

	// Methods - Public
public:
	bool setDefaultDevice(void actFuncCall(void), void deactFuncCall(void));
	bool sendData(uint8_t *msg, uint16_t msgSize = 1);
	bool sendData(uint8_t reg, uint8_t *msg, uint16_t msgSize = 1);
	bool init(SPI_HandleTypeDef *halSpiHandler, uint8_t bufferSize = 20);
// 	bool setMode(spiMode_e mode = SPI_MODE_0, bool lsbFirst = false);
// 	bool setInterrupt(controlMode_t controlMode = CONTROL_POOLING);
//	bool setTimeOut(uint8_t timeOut);
// 	void interruptHandler(void);
	error_e getLastError(void);

	// Methods - Private
private:
// //	bool waitWhileIsBusy(void);

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	SPI_HandleTypeDef	*spiHandler_;
	void				(*activateDefaultDevice_)(void);
	void				(*deactivateDefaultDevice_)(void);
	bool				initialized_			: 1;
	bool				callbacksReady_			: 1;
	uint8_t				*bufferData_;
	uint32_t			bufferMaxSize_			: 7;
	uint32_t			bufferIndex_			: 7;
	uint32_t			bufferLength_			: 7;
	uint32_t			timeOut_				: 8;
	error_e				lastError_;

	// bool				useInterrupt_		: 1;
	// bool				lsbFirst_			: 1;
	//	uint8_t			tempBytesToSend_	: 7;

// Data members - Protected
protected:

}; // class Spi

// =============================================================================
// Spi - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

#endif // __GPDSE_ARM_SPI_H
