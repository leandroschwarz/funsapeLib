/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	gpdseArmI2c.h
 * Module:		I2C interface module for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_ARM_I2C_HPP
#define __GPDSE_ARM_I2C_HPP						__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (gpdseArmGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

#include "stm32f0xx_hal.h"

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constants
// =============================================================================

// NONE

// =============================================================================
// Macro-functions
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================
/*
typedef enum twiState_e {
	TWI_START					= 0x08,	// START has been transmitted
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been transmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been transmitted and NACK received
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been transmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been transmitted and NACK received
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been transmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been transmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK transmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK transmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = 0
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_e;
 */

typedef enum twiOperation_t {
	TWI_WRITE = 0,
	TWI_READ
} twiOperation_t;

// =============================================================================
// I2c - Class declaration
// =============================================================================

class I2c
{
	// Constructors
public:
	I2c(I2C_HandleTypeDef * i2cHandler, uint8_t buffSize = 20);

	// Methods - Public
public:
	// bool init(uint32_t clockSpeed = 10000, uint8_t bufferSize = 20);
	bool sendData(uint8_t devAddress, twiOperation_t readWrite, uint8_t reg,
	        uint8_t *msg = NULL, uint8_t msgSize = 0);
	bool sendData(uint8_t devAddress, uint8_t *msg = NULL, uint8_t msgSize = 0);

	// bool setTimeOut(uint8_t timeOut);
	error_e getLastError(uint16_t * details = NULL);
	uint32_t getHandle(void);
	uint32_t getBufAdd(void);

	// Methods - Private
private:
	// bool waitWhileIsBusy(void);
	// bool startTrasmission(void);
	// bool readFromBuffer(uint8_t *msg, uint8_t msgSize);

	// Data members - Private
private:
	I2C_HandleTypeDef *i2cHandler;
	bool initialized_ :1;
	uint8_t *bufferData_;
	uint8_t bufferMaxSize_;
	uint8_t bufferIndex_;
	uint8_t bufferLength_;
	uint8_t timeOut_;
	error_e lastError_;
	uint16_t lastErrorDetails_;
	uint32_t bufAdd;
}; // class I2c

// =============================================================================
// I2c - Class inline function definitions
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
extern I2C_HandleTypeDef hi2c1;
// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

#endif // __GPDSE_ARM_I2C_HPP
