/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		max30105.cpp
 * Module:			MAX30105 module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "max30105.hpp"
#ifndef __MAX30105_HPP
#	error	[max30105.cpp] Error 1 - Header file (max30105.hpp) is missing or corrupted!
#elif __MAX30105_HPP != __BUILD_TO_REPLACE__
#	error	[max30105.cpp] Error 13 - Build mismatch between source (max30105.cpp) and header (max30105.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define MAX30105_BYTES_PER_CHANNEL				3
#define MAX30105_DEVICE_ADDRESS					0x57
#define MAX30105_FIFO_SIZE_IN_SAMPLES			32
#define MAX30105_OPERATION_TIMEOUT				100
#define MAX30105_PART_ID						0x15

// =============================================================================
// File exclusive - New data types
// =============================================================================

// -----------------------------------------------------------------------------
// MAX30105 register bit position
// -----------------------------------------------------------------------------

typedef enum max30105Bit_e {
	// Interrupt
	MAX30105_BIT_INTERRUPT_FIFO_ALMOST_FULL_FLAG		= 7,	// Interrupt status 1
	MAX30105_BIT_INTERRUPT_FIFO_NEW_DATA_READY_FLAG		= 6,
	MAX30105_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_FLAG		= 5,
	MAX30105_BIT_INTERRUPT_PROXIMITY_THRESHOLD_FLAG		= 4,
	MAX30105_BIT_INTERRUPT_POWER_READY_FLAG				= 0,
	MAX30105_BIT_INTERRUPT_DIE_TEMPERATURE_READY_FLAG	= 1,	// Interrupt status 2
	MAX30105_BIT_INTERRUPT_FIFO_ALMOST_FULL_ENABLE		= 7,	// Interrupt enable 1
	MAX30105_BIT_INTERRUPT_FIFO_NEW_DATA_READY_ENABLE	= 6,
	MAX30105_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_ENABLE	= 5,
	MAX30105_BIT_INTERRUPT_PROXIMITY_THRESHOLD_ENABLE	= 4,
	MAX30105_BIT_INTERRUPT_DIE_TEMPERATURE_READY_ENABLE	= 1,	// Interrupt enable 2

	// FIFO
	MAX30105_BIT_FIFO_WR_PTR							= 0,	// FIFO write pointer
	MAX30105_BIT_FIFO_OVF_COUNTER						= 0,	// FIFO overflow counter
	MAX30105_BIT_FIFO_RD_PTR							= 0,	// FIFO read pointer
	MAX30105_BIT_FIFO_DATA								= 0,	// FIFO data register

	// Configuration
	MAX30105_BIT_CONFIGURATION_SMP_AVE					= 5,	// FIFO configuration
	MAX30105_BIT_CONFIGURATION_FIFO_ROLLOVER_EN			= 4,
	MAX30105_BIT_CONFIGURATION_FIFO_A_FULL				= 0,
	MAX30105_BIT_CONFIGURATION_SHDN						= 7,	// Mode configuration
	MAX30105_BIT_CONFIGURATION_RESET					= 6,
	MAX30105_BIT_CONFIGURATION_MODE						= 0,
	MAX30105_BIT_CONFIGURATION_SENSOR_FULL_SCALE		= 5,	// SpO2 configuration
	MAX30105_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE		= 2,
	MAX30105_BIT_CONFIGURATION_SENSOR_RESOLUTION		= 0,
	MAX30105_BIT_RED_LED_PULSE_AMPLITUDE				= 0,	// Red LED pulse amplitude
	MAX30105_BIT_IR_LED_PULSE_AMPLITUDE					= 0,	// Infrared LED pulse amplitude
	MAX30105_BIT_GREEN_LED_PULSE_AMPLITUDE				= 0,	// Green LED pulse amplitude
	// MAX30105_BIT_CONFIGURATION_PILOT_PA					= 0,	// Proximity mode LED pulse amplitude
	// MAX30105_BIT_CONFIGURATION_SLOT2					= 4,	// Multi-LED mode control register 1
	// MAX30105_BIT_CONFIGURATION_SLOT1					= 0,
	// MAX30105_BIT_CONFIGURATION_SLOT4					= 4,	// Multi-LED mode control register 2
	// MAX30105_BIT_CONFIGURATION_SLOT3					= 0,

	// Temperature Data
	MAX30105_BIT_TEMPERATURE_INT						= 0,	// Die temperature integer
	MAX30105_BIT_TEMPERATURE_FRAC						= 0,	// Die temperature fraction
	MAX30105_BIT_TEMPERATURE_EN							= 0,	// Die temperature config

	// Proximity Function
	// MAX30105_BIT_PROX_INT_THRESH						= 0,	// Proximity interrupt threshold
} max30105Bit_e;

// -----------------------------------------------------------------------------
// MAX30105 register addresses
// -----------------------------------------------------------------------------

typedef enum max30105Register_e {
	// Status registers
	MAX30105_REG_INTERRUPT_STATUS_1						= 0x00,
	MAX30105_REG_INTERRUPT_STATUS_2						= 0x01,
	MAX30105_REG_INTERRUPT_ENABLE_1						= 0x02,
	MAX30105_REG_INTERRUPT_ENABLE_2						= 0x03,

	// FIFO registers
	MAX30105_REG_FIFO_WRITE_POINTER						= 0x04,
	MAX30105_REG_FIFO_OVERFLOW_COUNTER					= 0x05,
	MAX30105_REG_FIFO_READ_POINTER						= 0x06,
	MAX30105_REG_FIFO_DATA								= 0x07,

	// Configuration registers
	MAX30105_REG_FIFO_CONFIGURATION						= 0x08,
	MAX30105_REG_MODE_CONFIGURATION						= 0x09,
	MAX30105_REG_SPO2_CONFIGURATION						= 0x0A,
	MAX30105_REG_RED_LED_PULSE_AMPLITUDE				= 0x0C,
	MAX30105_REG_IR_LED_PULSE_AMPLITUDE					= 0x0D,
	MAX30105_REG_GREEN_LED_PULSE_AMPLITUDE				= 0x0E,
	MAX30105_REG_PROXIMITY_LED_AMPLITUDE				= 0x10,
	MAX30105_REG_MULTI_LED_CONTROL_1					= 0x11,
	MAX30105_REG_MULTI_LED_CONTROL_2					= 0x12,

	// Die temperature registers
	MAX30105_REG_DIE_TEMPERATURE_INTEGER				= 0x1F,
	MAX30105_REG_DIE_TEMPERATURE_FRACTION				= 0x20,
	MAX30105_REG_DIE_TEMPERATURE_CONFIG					= 0x21,

	// Proximity function registers
	MAX30105_REG_PROXIMITY_INTERRUPT_THRESHOLD			= 0x30,

	// Part ID registers
	MAX30105_REG_REVISION_ID							= 0xFE,
	MAX30105_REG_PART_ID								= 0xFF
} max30105Register_e;

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

Max30105::Max30105(void)
{
	// Reset data members
	this->_clearData();

	// Mark passage for debug
	debugMark();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
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

// -----------------------------------------------------------------------------
// Initialization methods
// -----------------------------------------------------------------------------

bool Max30105::init(Handler *handler_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debug
	debugMark();

	// Reset data members
	this->_initialized = false;
	this->_handler = nullptr;

	// Check for errors
	if(!isPointerValid(handler_p)) {
		this->_handler = nullptr;
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	} else if(handler_p->getHandlerType() != Handler::HandlerType::TWI) {
		this->_handler = nullptr;
		this->_lastError = Error::HANDLER_UNSUPPORTED;
		debugMessage(Error::HANDLER_UNSUPPORTED);
		return false;
	}

	// Read data from device
	this->_handler = handler_p;
	if(!this->_read(MAX30105_REG_REVISION_ID, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check device ID
	if(auxBuff[1] != MAX30105_PART_ID) {
		// Returns error
		this->_lastError = Error::DEVICE_ID_MATCH_FAILED;
		debugMessage(Error::DEVICE_ID_MATCH_FAILED);
		return false;
	}

	// Reset device
	if(!this->reset()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_revisionId = auxBuff[0];
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::reset(void)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!isPointerValid(this->_handler)) {
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	} else if(this->_handler->getHandlerType() != Handler::HandlerType::TWI) {
		this->_lastError = Error::HANDLER_UNSUPPORTED;
		debugMessage(Error::HANDLER_UNSUPPORTED);
		return false;
	}

	// Mount data package
	auxBuff = (1 << MAX30105_BIT_CONFIGURATION_RESET);

	// Write data to device
	if(!this->_write(MAX30105_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Test until reset flag is cleared by hardware
	delayMs(10);			// TODO: Pool for RESET FLAG until its cleared by hardware

	// Reset data members
	this->_clearData();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::shutDown(void)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff = (1 << MAX30105_BIT_CONFIGURATION_SHDN) |
			(this->_modeControl << MAX30105_BIT_CONFIGURATION_MODE);

	// Write data to device
	if(!this->_write(MAX30105_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_shutDownEnabled = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::wakeUp(void)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff = (0 << MAX30105_BIT_CONFIGURATION_SHDN) |
			(this->_modeControl << MAX30105_BIT_CONFIGURATION_MODE);

	// Write data to device
	if(!this->_write(MAX30105_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_shutDownEnabled = false;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// -----------------------------------------------------------------------------
// Sensor configuration methods
// -----------------------------------------------------------------------------

bool Max30105::setCustomMode(LedCurrent redLedCurrent_p, LedCurrent infraredLedCurrent_p, LedCurrent auxLedCurrent_p)
{
	// TODO: Implement this function

	// Mark passage for debug
	debugMark();

	// Returns error
	this->_lastError = Error::UNDER_DEVELOPMENT;
	debugMessage(Error::UNDER_DEVELOPMENT);
	return false;
}

bool Max30105::setFullScale(FullScale sensorFullScale_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff = (sensorFullScale_p << MAX30105_BIT_CONFIGURATION_SENSOR_FULL_SCALE) |
			(this->_sampleRate << MAX30105_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE) |
			(this->_resolution << MAX30105_BIT_CONFIGURATION_SENSOR_RESOLUTION);

	// Write data to device
	if(!this->_write(MAX30105_REG_SPO2_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fullScale = sensorFullScale_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::setHeartRateMode(LedCurrent redLedCurrent_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff = (this->_shutDownEnabled << MAX30105_BIT_CONFIGURATION_SHDN) |
			(ModeControl::HEART_RATE_MODE << MAX30105_BIT_CONFIGURATION_MODE);

	// Write data to device
	if(!this->_write(MAX30105_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_modeControl = ModeControl::HEART_RATE_MODE;
	this->_activeLEDs = 1;

	// Mount data package
	auxBuff = (uint8_t(redLedCurrent_p) << MAX30105_BIT_RED_LED_PULSE_AMPLITUDE);

	// Write data to device
	if(!this->_write(MAX30105_REG_RED_LED_PULSE_AMPLITUDE, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_redLedCurrent = redLedCurrent_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::setResolution(Resolution resolution_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff = (this->_fullScale << MAX30105_BIT_CONFIGURATION_SENSOR_FULL_SCALE) |
			(this->_sampleRate << MAX30105_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE) |
			(resolution_p << MAX30105_BIT_CONFIGURATION_SENSOR_RESOLUTION);

	// Write data to device
	if(!this->_write(MAX30105_REG_SPO2_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_resolution = resolution_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::setSampleRate(SampleRate sampleRate_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff = (this->_fullScale << MAX30105_BIT_CONFIGURATION_SENSOR_FULL_SCALE) |
			(sampleRate_p << MAX30105_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE) |
			(this->_resolution << MAX30105_BIT_CONFIGURATION_SENSOR_RESOLUTION);

	// Write data to device
	if(!this->_write(MAX30105_REG_SPO2_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_sampleRate = sampleRate_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::setSpo2Mode(LedCurrent redLedCurrent_p, LedCurrent infraredLedCurrent_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff[0] = (this->_shutDownEnabled << MAX30105_BIT_CONFIGURATION_SHDN) |
			(ModeControl::SPO2_MODE << MAX30105_BIT_CONFIGURATION_MODE);

	// Write data to device
	if(!this->_write(MAX30105_REG_MODE_CONFIGURATION, auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_modeControl = ModeControl::SPO2_MODE;
	this->_activeLEDs = 2;

	// Mount data package
	auxBuff[0] = (uint8_t(redLedCurrent_p) >> MAX30105_BIT_RED_LED_PULSE_AMPLITUDE);
	auxBuff[1] = (uint8_t(infraredLedCurrent_p) >> MAX30105_BIT_IR_LED_PULSE_AMPLITUDE);

	// Write data to device
	if(!this->_write(MAX30105_REG_RED_LED_PULSE_AMPLITUDE, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_redLedCurrent = redLedCurrent_p;
	this->_infraredLedCurrent = infraredLedCurrent_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// -----------------------------------------------------------------------------
// Interrupt methods
// -----------------------------------------------------------------------------

bool Max30105::activateInterrupts(InterruptFlags activationMask_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Update data members
	this->_fifoAlmostFullInterruptEnable			= (activationMask_p & InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL) ? true : this->_fifoAlmostFullInterruptEnable;
	this->_fifoNewDataInterruptEnable				= (activationMask_p & InterruptFlags::INTERRUPT_FIFO_NEW_DATA) ? true : this->_fifoNewDataInterruptEnable;
	this->_ambientLightCancelationInterruptEnable	= (activationMask_p & InterruptFlags::INTERRUPT_AMBIENT_LIGHT_CANC) ? true : this->_ambientLightCancelationInterruptEnable;
	this->_proximityThresholdInterruptEnable		= (activationMask_p & InterruptFlags::INTERRUPT_PROXIMITY) ? true : this->_proximityThresholdInterruptEnable;
	this->_dieTemperatureInterruptEnable			= (activationMask_p & InterruptFlags::INTERRUPT_DIE_TEMPERATURE) ? true : this->_dieTemperatureInterruptEnable;

	// Mount data package
	auxBuff[0] = (this->_fifoAlmostFullInterruptEnable << MAX30105_BIT_INTERRUPT_FIFO_ALMOST_FULL_ENABLE) |
			(this->_fifoNewDataInterruptEnable << MAX30105_BIT_INTERRUPT_FIFO_NEW_DATA_READY_ENABLE) |
			(this->_ambientLightCancelationInterruptEnable << MAX30105_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_ENABLE) |
			(this->_proximityThresholdInterruptEnable << MAX30105_BIT_INTERRUPT_PROXIMITY_THRESHOLD_ENABLE);
	auxBuff[1] = (this->_dieTemperatureInterruptEnable << MAX30105_BIT_INTERRUPT_DIE_TEMPERATURE_READY_ENABLE);

	// Write data to device
	if(!this->_write(MAX30105_REG_INTERRUPT_ENABLE_1, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::deactivateInterrupts(InterruptFlags deactivationMask_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Update data members
	this->_fifoAlmostFullInterruptEnable			= (deactivationMask_p & InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL) ? false : this->_fifoAlmostFullInterruptEnable;
	this->_fifoNewDataInterruptEnable				= (deactivationMask_p & InterruptFlags::INTERRUPT_FIFO_NEW_DATA) ? false : this->_fifoNewDataInterruptEnable;
	this->_ambientLightCancelationInterruptEnable	= (deactivationMask_p & InterruptFlags::INTERRUPT_AMBIENT_LIGHT_CANC) ? false : this->_ambientLightCancelationInterruptEnable;
	this->_proximityThresholdInterruptEnable		= (deactivationMask_p & InterruptFlags::INTERRUPT_PROXIMITY) ? false : this->_proximityThresholdInterruptEnable;
	this->_dieTemperatureInterruptEnable			= (deactivationMask_p & InterruptFlags::INTERRUPT_DIE_TEMPERATURE) ? false : this->_dieTemperatureInterruptEnable;

	// Mount data package
	auxBuff[0] = (this->_fifoAlmostFullInterruptEnable << MAX30105_BIT_INTERRUPT_FIFO_ALMOST_FULL_ENABLE) |
			(this->_fifoNewDataInterruptEnable << MAX30105_BIT_INTERRUPT_FIFO_NEW_DATA_READY_ENABLE) |
			(this->_ambientLightCancelationInterruptEnable << MAX30105_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_ENABLE) |
			(this->_proximityThresholdInterruptEnable << MAX30105_BIT_INTERRUPT_PROXIMITY_THRESHOLD_ENABLE);
	auxBuff[1] = (this->_dieTemperatureInterruptEnable << MAX30105_BIT_INTERRUPT_DIE_TEMPERATURE_READY_ENABLE);

	// Write data to device
	if(!this->_write(MAX30105_REG_INTERRUPT_ENABLE_1, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getInterruptStatus(InterruptFlags *interruptStatus_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(interruptStatus_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Read data from device
	if(!this->_read(MAX30105_REG_INTERRUPT_STATUS_1, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fifoAlmostFullInterruptStatus = (bool)(auxBuff[0] & MAX30105_BIT_INTERRUPT_FIFO_ALMOST_FULL_FLAG);
	this->_fifoNewDataInterruptStatus = (bool)(auxBuff[0] & MAX30105_BIT_INTERRUPT_FIFO_NEW_DATA_READY_FLAG);
	this->_ambientLightCancelationInterruptStatus = (bool)(auxBuff[0] & MAX30105_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_FLAG);
	this->_proximityThresholdInterruptStatus = (bool)(auxBuff[0] & MAX30105_BIT_INTERRUPT_PROXIMITY_THRESHOLD_FLAG);
	this->_powerReadyInterruptStatus = (bool)(auxBuff[0] & MAX30105_BIT_INTERRUPT_POWER_READY_FLAG);
	this->_dieTemperatureInterruptStatus = (bool)(auxBuff[1] & MAX30105_BIT_INTERRUPT_DIE_TEMPERATURE_READY_FLAG);

	// Update function arguments data
	*interruptStatus_p = static_cast<InterruptFlags>(0);
	*interruptStatus_p |= (this->_fifoAlmostFullInterruptStatus) ? InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL : static_cast<InterruptFlags>(0);
	*interruptStatus_p |= (this->_fifoNewDataInterruptStatus) ? InterruptFlags::INTERRUPT_FIFO_NEW_DATA : static_cast<InterruptFlags>(0);
	*interruptStatus_p |= (this->_ambientLightCancelationInterruptStatus) ? InterruptFlags::INTERRUPT_AMBIENT_LIGHT_CANC : static_cast<InterruptFlags>(0);
	*interruptStatus_p |= (this->_proximityThresholdInterruptStatus) ? InterruptFlags::INTERRUPT_PROXIMITY : static_cast<InterruptFlags>(0);
	*interruptStatus_p |= (this->_dieTemperatureInterruptStatus) ? InterruptFlags::INTERRUPT_DIE_TEMPERATURE : static_cast<InterruptFlags>(0);
	*interruptStatus_p |= (this->_powerReadyInterruptStatus) ? InterruptFlags::INTERRUPT_POWER_READY : static_cast<InterruptFlags>(0);

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// -----------------------------------------------------------------------------
// FIFO methods
// -----------------------------------------------------------------------------

bool Max30105::configFifo(FifoAveraging averages_p, bool overflowEnable_p, uint8_t remainingSpaceInterrupt_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(remainingSpaceInterrupt_p > 0x0F) {
		// Returns error
		this->_lastError = Error::ARGUMENT_VALUE_INVALID;
		debugMessage(Error::ARGUMENT_VALUE_INVALID);
		return false;
	}

	// Mount data package
	auxBuff = (averages_p << MAX30105_BIT_CONFIGURATION_SMP_AVE) |
			(overflowEnable_p << MAX30105_BIT_CONFIGURATION_FIFO_ROLLOVER_EN) |
			(remainingSpaceInterrupt_p << MAX30105_BIT_CONFIGURATION_FIFO_A_FULL);

	// Write data to device
	if(!this->_write(MAX30105_REG_FIFO_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fifoAveraging = averages_p;
	this->_fifoOverflowEnable = overflowEnable_p;
	this->_fifoInterruptRemainingSpace = remainingSpaceInterrupt_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::flushFifo(void)
{
	// Local variables
	uint8_t auxBuff[3];

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Mount data package
	auxBuff[0] = (0 << MAX30105_BIT_FIFO_WR_PTR);
	auxBuff[1] = (0 << MAX30105_BIT_FIFO_OVF_COUNTER);
	auxBuff[2] = (0 << MAX30105_BIT_FIFO_RD_PTR);

	// Write data to device
	if(!this->_write(MAX30105_REG_FIFO_WRITE_POINTER, auxBuff, 3)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fifoWritePointer = 0;
	this->_fifoOverflowCounter = 0;
	this->_fifoReadPointer = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoData(uint8_t samplesCount_p, uint32_t *redLedBuffer_p, uint32_t *infraredLedBuffer_p)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(redLedBuffer_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Read data from FIFO
	if(!this->_getFifoData(samplesCount_p, redLedBuffer_p, infraredLedBuffer_p)) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoData(uint8_t samplesCount_p, uint32_t *ledBuff1, uint32_t *ledBuff2, uint32_t *ledBuff3, uint32_t *ledBuff4)
{
	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(ledBuff1)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Read data from FIFO
	if(!this->_getFifoData(samplesCount_p, ledBuff1, ledBuff2, ledBuff3, ledBuff4)) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoLastestSample(uint32_t *redValue_p, uint32_t *infraredValue_p)
{
	// Local variables
	uint8_t samples = 0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(redValue_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get FIFO occupation
	if(!this->getFifoOccupation(&samples)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// If there are no samples in FIFO
	if(samples == 0) {
		// Returns error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}

	// Move FIFO read pointer to last sample
	this->_fifoReadPointer = (this->_fifoWritePointer == 0) ? (0x0F) : (this->_fifoWritePointer - 1);
	if(!this->_setFifoReadPointer(this->_fifoReadPointer)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Read data from FIFO
	if(!this->_getFifoData(1, redValue_p, infraredValue_p)) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoLastestSample(uint32_t *ledBuff1, uint32_t *ledBuff2, uint32_t *ledBuff3, uint32_t *ledBuff4)
{
	// Local variables
	uint8_t samples = 0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(ledBuff1)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get FIFO occupation
	if(!this->getFifoOccupation(&samples)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// If there are no samples in FIFO
	if(samples == 0) {
		// Returns error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}

	// Move FIFO read pointer to last sample
	this->_fifoReadPointer = (this->_fifoWritePointer == 0) ? (0x0F) : (this->_fifoWritePointer - 1);
	if(!this->_setFifoReadPointer(this->_fifoReadPointer)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Read data from FIFO
	if(!this->_getFifoData(1, ledBuff1, ledBuff2, ledBuff3, ledBuff4)) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoNewestSample(uint32_t *redValue_p, uint32_t *infraredValue_p)
{
	// Local variables
	uint8_t samples = 0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(redValue_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get FIFO occupation
	if(!this->getFifoOccupation(&samples)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// If there are no samples in FIFO
	if(samples == 0) {
		// Returns error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}

	// Read data from FIFO
	if(!this->_getFifoData(1, redValue_p, infraredValue_p)) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoNewestSample(uint32_t *ledBuff1, uint32_t *ledBuff2, uint32_t *ledBuff3, uint32_t *ledBuff4)
{
	// Local variables
	uint8_t samples = 0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(ledBuff1)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get FIFO occupation
	if(!this->getFifoOccupation(&samples)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// If there are no samples in FIFO
	if(samples == 0) {
		// Returns error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}

	// Read data from FIFO
	if(!this->_getFifoData(1, ledBuff1, ledBuff2, ledBuff3, ledBuff4)) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::getFifoOccupation(uint8_t *samplesWaiting_p, uint8_t *samplesLost_p)
{
	// Local variables
	uint8_t samples = 0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(samplesWaiting_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Retrieve lastest FIFO information
	if(!this->_getFifoInfo()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Calculate the number of samples in FIFO
	samples = MAX30105_FIFO_SIZE_IN_SAMPLES;
	samples += this->_fifoWritePointer;
	samples -= this->_fifoReadPointer;
	samples %= MAX30105_FIFO_SIZE_IN_SAMPLES;

	// Update function arguments data
	*samplesWaiting_p = samples;
	if(isPointerValid(samplesLost_p)) {
		*samplesLost_p = this->_fifoOverflowCounter;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// -----------------------------------------------------------------------------
// Status methods
// -----------------------------------------------------------------------------

Error Max30105::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

bool Max30105::getTemperature(float *tempValue_p, TermometricScale tempScale_p)
{
	// Local variables
	uint8_t auxBuff[2];
	float tempAux = 0.0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(tempValue_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Mount data package
	auxBuff[0] = (1 << MAX30105_BIT_TEMPERATURE_EN);

	// Write data to device
	if(!this->_write(MAX30105_REG_DIE_TEMPERATURE_CONFIG, auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Test until temperature enable flag is cleared by hardware
	delayMs(10);			// TODO: Pool for TEMP ENABLE until its cleared by hardware

	// Read data from device
	if(!this->_read(MAX30105_REG_DIE_TEMPERATURE_INTEGER, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_dieTemperatureInteger = auxBuff[0];
	this->_dieTemperatureFraction = auxBuff[1];

	// Calculate temperature in Celsius
	tempAux = (float)this->_dieTemperatureInteger;
	tempAux += ((float)this->_dieTemperatureFraction * 0.0625);

	// Convert to desired temometric scale
	switch(tempScale_p) {
	case TermometricScale::CELSIUS:
		*tempValue_p = tempAux;
		break;
	case TermometricScale::DELISLE:
		*tempValue_p = (100 - tempAux) * 1.5;
		break;
	case TermometricScale::FAHRENHEIT:
		*tempValue_p = tempAux * 1.8 + 32;
		break;
	case TermometricScale::KELVIN:
		*tempValue_p = tempAux + 273.15;
		break;
	case TermometricScale::NEWTON:
		*tempValue_p = (tempAux * 33) / 100;
		break;
	case TermometricScale::RANKINE:
		*tempValue_p = (tempAux + 273.15) * 1.8;
		break;
	case TermometricScale::REAUMUR:
		*tempValue_p = (tempAux * 4) / 5;
		break;
	case TermometricScale::ROMER:
		*tempValue_p = ((tempAux * 21) / 40) + 7.5;
		break;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

// -----------------------------------------------------------------------------
// Status methods
// -----------------------------------------------------------------------------

void Max30105::_clearData(void)
{
	// Mark passage for debug
	debugMark();

	// Interrupt configuration
	this->_ambientLightCancelationInterruptEnable		= false;
	this->_dieTemperatureInterruptEnable				= false;
	this->_fifoAlmostFullInterruptEnable				= false;
	this->_fifoNewDataInterruptEnable					= false;
	this->_proximityThresholdInterruptEnable			= false;

	// Interrupt status
	this->_ambientLightCancelationInterruptStatus		= false;
	this->_dieTemperatureInterruptStatus				= false;
	this->_fifoAlmostFullInterruptStatus				= false;
	this->_fifoNewDataInterruptStatus					= false;
	this->_powerReadyInterruptStatus					= false;
	this->_proximityThresholdInterruptStatus			= false;

	// Control and status
	this->_initialized 									= false;
	this->_revisionId									= 0;
	this->_shutDownEnabled								= false;

	// Sensor configuration
	this->_activeLEDs									= 1;
	this->_fullScale									= FullScale::FULL_SCALE_2048_NA;
	// this->_greenLedCurrent								= LedCurrent::LED_OFF;
	this->_infraredLedCurrent							= LedCurrent::LED_OFF;
	this->_modeControl									= ModeControl::HEART_RATE_MODE;
	this->_redLedCurrent								= LedCurrent::LED_OFF;
	this->_resolution									= Resolution::RESOLUTION_15_BITS;
	this->_sampleRate									= SampleRate::SAMPLE_RATE_50_HZ;

	// FIFO configuration
	this->_fifoAveraging								= FifoAveraging::FIFO_NO_AVERAGE;
	this->_fifoData										= 0;
	this->_fifoInterruptRemainingSpace					= 0;
	this->_fifoOverflowCounter							= 0;
	this->_fifoOverflowEnable							= false;
	this->_fifoReadPointer								= 0;
	this->_fifoWritePointer								= 0;

	// Temperature data
	this->_dieTemperatureFraction						= 0;
	this->_dieTemperatureInteger						= 0;

	// Custom slots configuration
	this->_enabledSlotsCounter							= false;
	// this->_multiLedSlot1Control							= 0;
	// this->_multiLedSlot2Control							= 0;
	// this->_multiLedSlot3Control							= 0;
	// this->_multiLedSlot4Control							= 0;

	// Returns successfuly
	return;
}

bool Max30105::_isInitialized(void)
{
	// Mark passage for debug
	debugMark();

	// Check if device is initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		return false;
	}

	// Returns successfully
	return true;
}

// -----------------------------------------------------------------------------
// Communication methods
// -----------------------------------------------------------------------------

bool Max30105::_read(uint8_t regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToRead_p)
{
	// Local variables
	Error funsapeStatus;

	// Mark passage for debug
	debugMark();

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30105_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Read data from device
	if(!this->_handler->read(regAddr_p, dataBuffer_p, bytesToRead_p)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Returns succesfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::_write(uint8_t regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToWrite_p)
{
	// Local variables
	Error funsapeStatus;

	// Mark passage for debug
	debugMark();

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30105_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Write data to device
	if(!this->_handler->write(regAddr_p, dataBuffer_p, bytesToWrite_p)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Returns succesfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// -----------------------------------------------------------------------------
// FIFO methods
// -----------------------------------------------------------------------------

bool Max30105::_getFifoData(uint8_t nSamples_p, uint32_t *buff1_p, uint32_t *buff2_p, uint32_t *buff3_p, uint32_t *buff4_p)
{
	// Local variables
	uint8_t sampleSize = 0;
	uint8_t dataOffset = 0;
	uint8_t auxBuff[4 * MAX30105_BYTES_PER_CHANNEL];
	uint8_t aux8 = 0;
	uint32_t aux32 = 0;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(!isPointerValid(buff1_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}
	if(nSamples_p > MAX30105_FIFO_SIZE_IN_SAMPLES) {
		// Returns error
		this->_lastError = Error::BUFFER_NOT_ENOUGH_ELEMENTS;
		debugMessage(Error::BUFFER_NOT_ENOUGH_ELEMENTS);
		return false;
	}

	// Get number of samples in buffer
	if(!this->getFifoOccupation(&aux8)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	if(nSamples_p > aux8) {
		// Returns error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}

	// Calculate sample size
	sampleSize = MAX30105_BYTES_PER_CHANNEL * this->_activeLEDs;

	// Calculate resolution offset
	switch(this->_resolution) {
	case Resolution::RESOLUTION_15_BITS:
		dataOffset = 3;
		break;
	case Resolution::RESOLUTION_16_BITS:
		dataOffset = 2;
		break;
	case Resolution::RESOLUTION_17_BITS:
		dataOffset = 1;
		break;
	case Resolution::RESOLUTION_18_BITS:
		dataOffset = 0;
		break;
	default:
		// Returns error
		this->_lastError = Error::UNKNOWN;						// TODO: Create specific erro code
		debugMessage(Error::UNKNOWN);
		return false;
	}

	// Get data from FIFO
	for(uint8_t i = 0; i < nSamples_p; i++) {
		// Read data from device
		if(!this->_read(MAX30105_REG_FIFO_DATA, auxBuff, sampleSize)) {
			// Returns with error
			debugMessage(this->_lastError);
			return false;
		}
		// Decode data - first data in sample
		aux32 = (uint32_t)(auxBuff[0]);
		aux32 <<= 8;
		aux32 |= (uint32_t)(auxBuff[1]);
		aux32 <<= 8;
		aux32 |= (uint32_t)(auxBuff[2]);
		aux32 >>= dataOffset;
		buff1_p[i] = aux32;
		// Decode data - second data in sample
		if(isPointerValid(buff2_p) && (this->_activeLEDs > 1)) {
			aux32 = (uint32_t)(auxBuff[3]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[4]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[5]);
			aux32 >>= dataOffset;
			buff2_p[i] = aux32;
		}
		// Decode data - third data in sample
		if(isPointerValid(buff3_p) && (this->_activeLEDs > 2)) {
			aux32 = (uint32_t)(auxBuff[6]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[7]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[8]);
			aux32 >>= dataOffset;
			buff3_p[i] = aux32;
		}
		// Decode data - fourth data in sample
		if(isPointerValid(buff4_p) && (this->_activeLEDs > 3)) {
			aux32 = (uint32_t)(auxBuff[9]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[10]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[11]);
			aux32 >>= dataOffset;
			buff4_p[i] = aux32;
		}
	}

	// Update data members
	if(!this->_getFifoInfo()) {
		// Returns with error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::_getFifoInfo(void)
{
	// Local variables
	uint8_t auxBuff[3];

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Read data from device
	if(!this->_read(MAX30105_REG_FIFO_WRITE_POINTER, auxBuff, 3)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fifoWritePointer = auxBuff[0];
	this->_fifoOverflowCounter = auxBuff[1];
	this->_fifoReadPointer = auxBuff[2];

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

uint8_t Max30105::_getFifoReadPointer(void)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Read data from device
	if(!this->_read(MAX30105_REG_FIFO_READ_POINTER, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return auxBuff;
}

uint8_t Max30105::_getFifoWritePointer(void)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}

	// Read data from device
	if(!this->_read(MAX30105_REG_FIFO_WRITE_POINTER, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return auxBuff;
}

bool Max30105::_setFifoReadPointer(uint8_t pointer_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(pointer_p > 0x0F) {
		// Returns error
		this->_lastError = Error::ARGUMENT_VALUE_INVALID;
		debugMessage(Error::ARGUMENT_VALUE_INVALID);
		return false;
	}

	// Mount data package
	auxBuff = (pointer_p << MAX30105_BIT_FIFO_RD_PTR);

	// Write data to device
	if(!this->_write(MAX30105_REG_FIFO_READ_POINTER, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fifoReadPointer = pointer_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30105::_setFifoWritePointer(uint8_t pointer_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debug
	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(Error::NOT_INITIALIZED);
		return false;
	}
	if(pointer_p > 0x0F) {
		// Returns error
		this->_lastError = Error::ARGUMENT_VALUE_INVALID;
		debugMessage(Error::ARGUMENT_VALUE_INVALID);
		return false;
	}

	// Mount data package
	auxBuff = (pointer_p << MAX30105_BIT_FIFO_WR_PTR);

	// Write data to device
	if(!this->_write(MAX30105_REG_FIFO_WRITE_POINTER, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_fifoWritePointer = pointer_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE
