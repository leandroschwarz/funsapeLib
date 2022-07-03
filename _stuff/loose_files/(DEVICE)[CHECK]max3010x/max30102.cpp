/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		max30102.cpp
 * Module:			MAX30102 module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "max30102.hpp"
#ifndef __MAX30102_HPP
#	error	[max30102.cpp] Error 1 - Header file (max30102.hpp) is missing or corrupted!
#elif __MAX30102_HPP != __BUILD_TO_REPLACE__
#	error	[max30102.cpp] Error 13 - Build mismatch between source (max30102.cpp) and header (max30102.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define MAX30102_DEVICE_ADDRESS			0x57
#define MAX30102_PART_ID				0x15
#define MAX30102_BYTES_PER_CHANNEL		3
#define MAX30102_FIFO_SIZE_IN_SAMPLES	32

// =============================================================================
// File exclusive - New data types
// =============================================================================

typedef enum max30102Register_e {
	// Status registers
	MAX30102_REG_INTERRUPT_STATUS_1				= 0x00,
	MAX30102_REG_INTERRUPT_STATUS_2				= 0x01,
	MAX30102_REG_INTERRUPT_ENABLE_1				= 0x02,
	MAX30102_REG_INTERRUPT_ENABLE_2				= 0x03,

	// FIFO registers
	MAX30102_REG_FIFO_WRITE_POINTER				= 0x04,
	MAX30102_REG_FIFO_OVERFLOW_COUNTER			= 0x05,
	MAX30102_REG_FIFO_READ_POINTER				= 0x06,
	MAX30102_REG_FIFO_DATA						= 0x07,

	// Configuration registers
	MAX30102_REG_FIFO_CONFIGURATION				= 0x08,
	MAX30102_REG_MODE_CONFIGURATION				= 0x09,
	MAX30102_REG_SPO2_CONFIGURATION				= 0x0A,
	MAX30102_REG_LED1_PULSE_AMPLITUDE			= 0x0C,
	MAX30102_REG_LED2_PULSE_AMPLITUDE			= 0x0D,
	MAX30102_REG_LED3_PULSE_AMPLITUDE			= 0x0E,
	MAX30102_REG_PROXIMITY_LED_AMPLITUDE		= 0x10,
	MAX30102_REG_MULTI_LED_CONTROL_1			= 0x11,
	MAX30102_REG_MULTI_LED_CONTROL_2			= 0x12,

	// Die temperature registers
	MAX30102_REG_DIE_TEMPERATURE_INTEGER		= 0x1F,
	MAX30102_REG_DIE_TEMPERATURE_FRACTION		= 0x20,
	MAX30102_REG_DIE_TEMPERATURE_CONFIG			= 0x21,

	// Proximity function registers
	MAX30102_REG_PROXIMITY_INTERRUPT_THRESHOLD	= 0x30,

	// Part ID registers
	MAX30102_REG_REVISION_ID					= 0xFE,
	MAX30102_REG_PART_ID						= 0xFF
} max30102Register_e;

typedef enum max30102Bit_e {
	// Interrupt
	MAX30102_BIT_INTERRUPT_FIFO_ALMOST_FULL_FLAG		= 7,		// Interrupt Status 1
	MAX30102_BIT_INTERRUPT_FIFO_NEW_DATA_READY_FLAG		= 6,
	MAX30102_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_FLAG		= 5,
	MAX30102_BIT_INTERRUPT_PROXIMITY_THRESHOLD_FLAG		= 4,
	MAX30102_BIT_INTERRUPT_POWER_READY_FLAG				= 0,
	MAX30102_BIT_INTERRUPT_DIE_TEMPERATURE_READY_FLAG	= 1,		// Interrupt Status 2
	MAX30102_BIT_INTERRUPT_FIFO_ALMOST_FULL_ENABLE		= 7,		// Interrupt Enable 1
	MAX30102_BIT_INTERRUPT_FIFO_NEW_DATA_READY_ENABLE	= 6,
	MAX30102_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_ENABLE	= 5,
	MAX30102_BIT_INTERRUPT_PROXIMITY_THRESHOLD_ENABLE	= 4,
	MAX30102_BIT_INTERRUPT_DIE_TEMPERATURE_READY_ENABLE	= 1,		// Interrupt Enable 2

	// FIFO
	MAX30102_BIT_FIFO_WR_PTR						= 0,		// FIFO Write Pointer
	MAX30102_BIT_FIFO_OVF_COUNTER					= 0,		// FIFO Overflow Counter
	MAX30102_BIT_FIFO_RD_PTR						= 0,		// FIFO Read Pointer
	MAX30102_BIT_FIFO_DATA							= 0,		// FIFO Data Register

	// Configuration
	MAX30102_BIT_CONFIGURATION_SMP_AVE				= 5,		// FIFO Configuration
	MAX30102_BIT_CONFIGURATION_FIFO_ROLLOVER_EN		= 4,
	MAX30102_BIT_CONFIGURATION_FIFO_A_FULL			= 0,
	MAX30102_BIT_CONFIGURATION_SHDN					= 7,		// Mode Configuration
	MAX30102_BIT_CONFIGURATION_RESET				= 6,
	MAX30102_BIT_CONFIGURATION_MODE					= 0,
	MAX30102_BIT_CONFIGURATION_SENSOR_FULL_SCALE	= 5,		// SPO2 Configuration
	MAX30102_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE	= 2,
	MAX30102_BIT_CONFIGURATION_SENSOR_RESOLUTION	= 0,
	MAX30102_BIT_CONFIGURATION_LED1_PA				= 0,		// LED Pulse Amplitude 1
	MAX30102_BIT_CONFIGURATION_LED2_PA				= 0,		// LED Pulse Amplitude 2
	MAX30102_BIT_CONFIGURATION_PILOT_PA				= 0,		// Proximity Mode LED Pulse Amplitude
	MAX30102_BIT_CONFIGURATION_SLOT2				= 4,		// Multi-LED Mode Control Register 1
	MAX30102_BIT_CONFIGURATION_SLOT1				= 0,
	MAX30102_BIT_CONFIGURATION_SLOT4				= 4,		// Multi-LED Mode Control Register 2
	MAX30102_BIT_CONFIGURATION_SLOT3				= 0,

	// Temperature Data
	MAX30102_BIT_TEMPERATURE_TINT					= 0,		// Die Temperature Integer
	MAX30102_BIT_TEMPERATURE_TFRAC					= 0,		// Die Temperature Fraction
	MAX30102_BIT_TEMPERATURE_TEMP_EN				= 0,		// Die Temperature Config

	// Proximity Function
	MAX30102_BIT_PROX_INT_THRESH					= 0,		// Proximity Interrupt Threshold
} max30102Bit_e;

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

Max30102::Max30102(Handler *handler_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff[2];

	// Reset data members
	this->_initialized = false;

	// Check for errors
	if(!isPointerValid(handler_p)) {
		this->_handler = nullptr;
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return;
	} else if(handler_p->getHandlerType() != Handler::HandlerType::TWI) {
		this->_handler = nullptr;
		this->_lastError = Error::HANDLER_UNSUPPORTED;
		debugMessage(Error::HANDLER_UNSUPPORTED);
		return;
	}

	// Selects device
	if(!handler_p->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = handler_p->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return;
	}

	// Reads device info
	if(!handler_p->read(MAX30102_REG_REVISION_ID, auxBuff, 2)) {
		// Returns error
		funsapeStatus = handler_p->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return;
	}
	// Checks device ID
	if(auxBuff[1] != MAX30102_PART_ID) {
		// Returns error
		this->_lastError = Error::DEVICE_ID_MATCH_FAILED;
		debugMessage(Error::DEVICE_ID_MATCH_FAILED);
		return;
	}

	// Update data members
	this->_revisionId = auxBuff[0];
	this->_handler = handler_p;
	this->_initialized = true;
	this->_ambientLightCancelationInterruptEnable = false;
	this->_dieTemperatureInterruptEnable = false;
	this->_fifoAlmostFullInterruptEnable = false;
	this->_fifoNewDataInterruptEnable = false;
	this->_ambientLightCancelationInterruptStatus = false;
	this->_dieTemperatureInterruptStatus = false;
	this->_fifoAlmostFullInterruptStatus = false;
	this->_fifoNewDataInterruptStatus = false;
	this->_powerReadyInterruptStatus = false;
	this->_shutDownEnabled = false;
	this->_fullScale = FullScale::FULL_SCALE_2048_NA;
	this->_infraredLedCurrent = LedCurrent::LED_OFF;
	this->_modeControl = ModeControl::HEART_RATE_MODE;
	this->_redLedCurrent = LedCurrent::LED_OFF;
	this->_resolution = Resolution::RESOLUTION_15_BITS;
	this->_sampleRate = SampleRate::SAMPLE_RATE_50_HZ;
	this->_fifoAveraging = FifoAveraging::FIFO_NO_AVERAGE;
	this->_fifoOverflowEnable = false;
	this->_fifoInterruptRemainingSpace = 0;
	this->_fifoWritePointer = 0;
	this->_fifoOverflowCounter = 0;
	this->_fifoReadPointer = 0;
	this->_fifoData = 0;

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

Error Max30102::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

bool Max30102::activateInterrupts(InterruptFlags activationMask_p)
{
	uint8_t auxBuff[2];
	Error funsapeStatus;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Update data members
	if(activationMask_p & InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL) {
		this->_fifoAlmostFullInterruptEnable = true;
	}
	if(activationMask_p & InterruptFlags::INTERRUPT_FIFO_NEW_DATA) {
		this->_fifoNewDataInterruptEnable = true;
	}
	if(activationMask_p & InterruptFlags::INTERRUPT_AMBIENT_LIGHT_CANC) {
		this->_ambientLightCancelationInterruptEnable = true;
	}
	if(activationMask_p & InterruptFlags::INTERRUPT_DIE_TEMPERATURE) {
		this->_dieTemperatureInterruptEnable = true;
	}

	// Mount interrupt mask
	auxBuff[0] = (this->_fifoAlmostFullInterruptEnable << MAX30102_BIT_INTERRUPT_FIFO_ALMOST_FULL_ENABLE) |
			(this->_fifoNewDataInterruptEnable << MAX30102_BIT_INTERRUPT_FIFO_NEW_DATA_READY_ENABLE) |
			(this->_ambientLightCancelationInterruptEnable << MAX30102_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_ENABLE);
	auxBuff[1] = (this->_dieTemperatureInterruptEnable << MAX30102_BIT_INTERRUPT_DIE_TEMPERATURE_READY_ENABLE);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_INTERRUPT_ENABLE_1, auxBuff, 2)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::deactivateInterrupts(InterruptFlags deactivationMask_p)
{
	uint8_t auxBuff[2];
	Error funsapeStatus;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Update data members
	if(deactivationMask_p & InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL) {
		this->_fifoAlmostFullInterruptEnable = false;
	}
	if(deactivationMask_p & InterruptFlags::INTERRUPT_FIFO_NEW_DATA) {
		this->_fifoNewDataInterruptEnable = false;
	}
	if(deactivationMask_p & InterruptFlags::INTERRUPT_AMBIENT_LIGHT_CANC) {
		this->_ambientLightCancelationInterruptEnable = false;
	}
	if(deactivationMask_p & InterruptFlags::INTERRUPT_DIE_TEMPERATURE) {
		this->_dieTemperatureInterruptEnable = false;
	}

	// Mount interrupt mask
	auxBuff[0] = (this->_fifoAlmostFullInterruptEnable << MAX30102_BIT_INTERRUPT_FIFO_ALMOST_FULL_ENABLE) |
			(this->_fifoNewDataInterruptEnable << MAX30102_BIT_INTERRUPT_FIFO_NEW_DATA_READY_ENABLE) |
			(this->_ambientLightCancelationInterruptEnable << MAX30102_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_ENABLE);
	auxBuff[1] = (this->_dieTemperatureInterruptEnable << MAX30102_BIT_INTERRUPT_DIE_TEMPERATURE_READY_ENABLE);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_INTERRUPT_ENABLE_1, auxBuff, 2)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::getInterruptStatus(InterruptFlags *interruptStatus_p)
{
	// Local variables
	Error funsapeStatus;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Reads from device
	uint8_t auxBuff[2];
	if(!this->_handler->read(MAX30102_REG_INTERRUPT_STATUS_1, auxBuff, 2)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Decode response
	this->_fifoAlmostFullInterruptStatus = (auxBuff[0] & MAX30102_BIT_INTERRUPT_FIFO_ALMOST_FULL_FLAG);
	this->_fifoNewDataInterruptStatus = (auxBuff[0] & MAX30102_BIT_INTERRUPT_FIFO_NEW_DATA_READY_FLAG);
	this->_ambientLightCancelationInterruptStatus = (auxBuff[0] & MAX30102_BIT_INTERRUPT_AMBIENT_LIGHT_CANC_FLAG);
	this->_powerReadyInterruptStatus = (auxBuff[0] & MAX30102_BIT_INTERRUPT_POWER_READY_FLAG);
	this->_dieTemperatureInterruptStatus = (auxBuff[1] & MAX30102_BIT_INTERRUPT_DIE_TEMPERATURE_READY_FLAG);

	// Retrieve interrupt status
	*interruptStatus_p = static_cast<InterruptFlags>(0);
	if(this->_fifoAlmostFullInterruptStatus) {
		*interruptStatus_p |= InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL;
	}
	if(this->_fifoNewDataInterruptStatus) {
		*interruptStatus_p |= InterruptFlags::INTERRUPT_FIFO_NEW_DATA;
	}
	if(this->_ambientLightCancelationInterruptStatus) {
		*interruptStatus_p |= InterruptFlags::INTERRUPT_AMBIENT_LIGHT_CANC;
	}
	if(this->_dieTemperatureInterruptStatus) {
		*interruptStatus_p |= InterruptFlags::INTERRUPT_DIE_TEMPERATURE;
	}
	if(this->_powerReadyInterruptStatus) {
		*interruptStatus_p |= InterruptFlags::INTERRUPT_POWER_READY;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::reset(void)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (1 << MAX30102_BIT_CONFIGURATION_RESET);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Updates data members
	this->_ambientLightCancelationInterruptEnable = false;
	this->_dieTemperatureInterruptEnable = false;
	this->_fifoAlmostFullInterruptEnable = false;
	this->_fifoNewDataInterruptEnable = false;
	this->_ambientLightCancelationInterruptStatus = false;
	this->_dieTemperatureInterruptStatus = false;
	this->_fifoAlmostFullInterruptStatus = false;
	this->_fifoNewDataInterruptStatus = false;
	this->_powerReadyInterruptStatus = false;
	this->_shutDownEnabled = false;
	this->_fullScale = FullScale::FULL_SCALE_2048_NA;
	this->_infraredLedCurrent = LedCurrent::LED_OFF;
	this->_modeControl = ModeControl::HEART_RATE_MODE;
	this->_redLedCurrent = LedCurrent::LED_OFF;
	this->_resolution = Resolution::RESOLUTION_15_BITS;
	this->_sampleRate = SampleRate::SAMPLE_RATE_50_HZ;
	this->_fifoAveraging = FifoAveraging::FIFO_NO_AVERAGE;
	this->_fifoOverflowEnable = false;
	this->_fifoInterruptRemainingSpace = 0;
	this->_fifoWritePointer = 0;
	this->_fifoOverflowCounter = 0;
	this->_fifoReadPointer = 0;
	this->_fifoData = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::shutDown(void)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (1 << MAX30102_BIT_CONFIGURATION_SHDN) |
			(this->_modeControl << MAX30102_BIT_CONFIGURATION_MODE);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Updates data members
	this->_shutDownEnabled = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::wakeUp(void)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (0 << MAX30102_BIT_CONFIGURATION_SHDN) |
			(this->_modeControl << MAX30102_BIT_CONFIGURATION_MODE);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Updates data members
	this->_shutDownEnabled = false;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::setHeartRateMode(LedCurrent redLedCurrent_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (this->_shutDownEnabled << MAX30102_BIT_CONFIGURATION_SHDN) |
			(ModeControl::HEART_RATE_MODE << MAX30102_BIT_CONFIGURATION_MODE);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_MODE_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_modeControl = ModeControl::HEART_RATE_MODE;

	// Mount data package
	auxBuff = uint8_t(redLedCurrent_p);

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_LED1_PULSE_AMPLITUDE, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_redLedCurrent = redLedCurrent_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::setSpo2Mode(LedCurrent redLedCurrent_p, LedCurrent infraredLedCurrent_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff[2];

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff[0] = (this->_shutDownEnabled << MAX30102_BIT_CONFIGURATION_SHDN) |
			(ModeControl::SPO2_MODE << MAX30102_BIT_CONFIGURATION_MODE);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_MODE_CONFIGURATION, auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_modeControl = ModeControl::SPO2_MODE;

	// Mount data package
	auxBuff[0] = uint8_t(redLedCurrent_p);
	auxBuff[1] = uint8_t(infraredLedCurrent_p);

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_LED1_PULSE_AMPLITUDE, auxBuff, 2)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_redLedCurrent = redLedCurrent_p;
	this->_infraredLedCurrent = infraredLedCurrent_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// TODO: Implement Multi-LED Mode

// // Mount data package
// auxBuff = (this->_fifoAveraging << MAX30102_BIT_CONFIGURATION_SMP_AVE) |
// 		(this->_fifoOverflowEnable << MAX30102_BIT_CONFIGURATION_FIFO_ROLLOVER_EN) |
// 		(this->_fifoInterruptRemainingSpace << MAX30102_BIT_CONFIGURATION_FIFO_A_FULL) |

bool Max30102::setSampleRate(SampleRate sampleRate_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (this->_fullScale << MAX30102_BIT_CONFIGURATION_SENSOR_FULL_SCALE) |
			(sampleRate_p << MAX30102_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE) |
			(this->_resolution << MAX30102_BIT_CONFIGURATION_SENSOR_RESOLUTION);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_SPO2_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_sampleRate = sampleRate_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::setResolution(Resolution resolution_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (this->_fullScale << MAX30102_BIT_CONFIGURATION_SENSOR_FULL_SCALE) |
			(this->_sampleRate << MAX30102_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE) |
			(resolution_p << MAX30102_BIT_CONFIGURATION_SENSOR_RESOLUTION);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_SPO2_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_resolution = resolution_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::setFullScale(FullScale sensorFullScale_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (sensorFullScale_p << MAX30102_BIT_CONFIGURATION_SENSOR_FULL_SCALE) |
			(this->_sampleRate << MAX30102_BIT_CONFIGURATION_SENSOR_SAMPLE_RATE) |
			(this->_resolution << MAX30102_BIT_CONFIGURATION_SENSOR_RESOLUTION);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_SPO2_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_fullScale = sensorFullScale_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::configFifo(FifoAveraging averages_p, bool overflowEnable_p, uint8_t remainingSpaceInterrupt_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = (averages_p << MAX30102_BIT_CONFIGURATION_SMP_AVE) |
			(overflowEnable_p << MAX30102_BIT_CONFIGURATION_FIFO_ROLLOVER_EN) |
			(remainingSpaceInterrupt_p << MAX30102_BIT_CONFIGURATION_FIFO_A_FULL);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_FIFO_CONFIGURATION, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_fifoAveraging = averages_p;
	this->_fifoOverflowEnable = overflowEnable_p;
	this->_fifoInterruptRemainingSpace = remainingSpaceInterrupt_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::flushFifo(void)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff[3];

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff[0] = 0;
	auxBuff[1] = 0;
	auxBuff[2] = 0;

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_FIFO_WRITE_POINTER, auxBuff, 3)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_fifoWritePointer = 0;
	this->_fifoOverflowCounter = 0;
	this->_fifoReadPointer = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::getFifoOccupation(uint8_t *samplesWaiting_p, uint8_t *samplesLost_p)
{
	// Local variables
	uint8_t samples;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	if(!this->_getFifoInfo()) {
		// Returns error
		return false;
	}

	if(this->_fifoWritePointer >= this->_fifoReadPointer) {
		samples = this->_fifoWritePointer;
		samples -= this->_fifoReadPointer;
	} else {
		samples = MAX30102_FIFO_SIZE_IN_SAMPLES;
		samples += this->_fifoWritePointer;
		samples -= this->_fifoReadPointer;
	}
	*samplesWaiting_p = samples;
	if(isPointerValid(samplesLost_p)) {
		*samplesLost_p = this->_fifoOverflowCounter;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::getNewestSample(uint32_t *redValue_p, uint32_t *infraredValue_p)
{
	uint8_t samples;
	uint32_t *auxBuff1 = nullptr;
	uint32_t *auxBuff2 = nullptr;

	debugMark();

	this->getFifoOccupation(&samples);

	if(samples == 0) {
		// Return error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
//		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}
	auxBuff1 = (uint32_t *)calloc(samples, sizeof(uint32_t));
	if(auxBuff1 == nullptr) {
		// Return error
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return false;
	}
	if(infraredValue_p != nullptr) {
		auxBuff2 = (uint32_t *)calloc(samples, sizeof(uint32_t));
		if(auxBuff2 == nullptr) {
			free(auxBuff1);
			// Return error
			this->_lastError = Error::MEMORY_ALLOCATION;
			debugMessage(Error::MEMORY_ALLOCATION);
			return false;
		}
	}

	if(infraredValue_p == nullptr) {
		_readFifoData(samples, auxBuff1, nullptr, sizeof(uint32_t));
		*redValue_p = auxBuff1[samples - 1];
	} else {
		_readFifoData(samples, auxBuff1, auxBuff2, sizeof(uint32_t));
		*redValue_p = auxBuff1[samples - 1];
		*infraredValue_p = auxBuff2[samples - 1];
	}

	free(auxBuff1);
	if(auxBuff2 != nullptr) {
		free(auxBuff2);
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::getNewestSample(uint16_t *redValue_p, uint16_t *infraredValue_p)
{
	uint8_t samples;
	uint16_t *auxBuff1 = nullptr;
	uint16_t *auxBuff2 = nullptr;

	debugMark();

	this->getFifoOccupation(&samples);

	if(samples == 0) {
		// Return error
		this->_lastError = Error::VALID_DATA_UNAVAILABLE;
		debugMessage(Error::VALID_DATA_UNAVAILABLE);
		return false;
	}
	auxBuff1 = (uint16_t *)calloc(samples, sizeof(uint16_t));
	if(auxBuff1 == nullptr) {
		// Return error
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return false;
	}
	if(infraredValue_p != nullptr) {
		auxBuff2 = (uint16_t *)calloc(samples, sizeof(uint16_t));
		if(auxBuff2 == nullptr) {
			free(auxBuff1);
			// Return error
			this->_lastError = Error::MEMORY_ALLOCATION;
			debugMessage(Error::MEMORY_ALLOCATION);
			return false;
		}
	}

	if(infraredValue_p == nullptr) {
		_readFifoData(samples, auxBuff1, nullptr, sizeof(uint16_t));
		*redValue_p = auxBuff1[samples - 1];
	} else {
		_readFifoData(samples, auxBuff1, auxBuff2, sizeof(uint16_t));
		*redValue_p = auxBuff1[samples - 1];
		*infraredValue_p = auxBuff2[samples - 1];
	}

	free(auxBuff1);
	if(auxBuff2 != nullptr) {
		free(auxBuff2);
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::getLastSample(uint32_t *redValue_p, uint32_t *infraredValue_p)
{
	uint8_t writePointer = 0;
	uint8_t readPointer = 0;
	uint8_t auxBuff[6];
	Error funsapeStatus;

	writePointer = this->getWritePointer();
	readPointer = (writePointer == 0) ? (31) : (writePointer - 1);
	this->setReadPointer(readPointer);

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Reads from device
	if(!this->_handler->read(MAX30102_REG_FIFO_DATA, auxBuff, 6)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	*redValue_p = (uint32_t)auxBuff[0];
	*redValue_p <<= 8;
	*redValue_p |= (uint32_t)auxBuff[1];
	*redValue_p <<= 8;
	*redValue_p |= (uint32_t)auxBuff[2];
	*infraredValue_p = (uint32_t)auxBuff[3];
	*infraredValue_p <<= 8;
	*infraredValue_p |= (uint32_t)auxBuff[4];
	*infraredValue_p <<= 8;
	*infraredValue_p |= (uint32_t)auxBuff[5];

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::getFifoData(uint8_t samplesCount_p, uint16_t *redLedBuffer_p, uint16_t *infraredLedBuffer_p)
{
	// TODO Check for errors
	debugMark();

	this->_readFifoData(samplesCount_p, redLedBuffer_p, infraredLedBuffer_p, sizeof(uint16_t));
	return true;
}

bool Max30102::getFifoData(uint8_t samplesCount_p, uint32_t *redLedBuffer_p, uint32_t *infraredLedBuffer_p)
{
	// TODO Check for errors
	debugMark();

	this->_readFifoData(samplesCount_p, redLedBuffer_p, infraredLedBuffer_p, sizeof(uint32_t));
	return true;
}

uint8_t Max30102::getReadPointer(void)
{
	uint8_t auxBuff;
	Error funsapeStatus;

	debugMark();

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Reads from device
	if(!this->_handler->read(MAX30102_REG_FIFO_READ_POINTER, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	return auxBuff;
}

uint8_t Max30102::getWritePointer(void)
{
	uint8_t auxBuff;
	Error funsapeStatus;

	debugMark();

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Reads from device
	if(!this->_handler->read(MAX30102_REG_FIFO_WRITE_POINTER, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	return auxBuff;
}

bool Max30102::setReadPointer(uint8_t pointer_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = pointer_p;

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_FIFO_READ_POINTER, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_fifoReadPointer = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Max30102::setWritePointer(uint8_t pointer_p)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff;

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Mount data package
	auxBuff = pointer_p;

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Writes at device
	if(!this->_handler->write(MAX30102_REG_FIFO_WRITE_POINTER, &auxBuff, 1)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_fifoWritePointer = 0;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

bool Max30102::_isInitialized(void)
{
	debugMark();

	// Checks if device is initialized
	if(!this->_initialized) {
		this->_lastError = Error::NOT_INITIALIZED;
		return false;
	}

	// Returns successfully
	return true;
}

bool Max30102::_getFifoInfo(void)
{
	// Local variables
	Error funsapeStatus;
	uint8_t auxBuff[3];

	debugMark();

	// Check for errors
	if(!this->_isInitialized()) {
		// Returns error
		return false;
	}

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Reads from device
	if(!this->_handler->read(MAX30102_REG_FIFO_WRITE_POINTER, auxBuff, 3)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}
	this->_fifoWritePointer = auxBuff[0];
	this->_fifoOverflowCounter = auxBuff[1];
	this->_fifoReadPointer = auxBuff[2];

	// Returns successfully
	return true;
}

bool Max30102::_readFifoData(uint8_t nSamples, void *buff1, void *buff2, uint8_t varSize)
{
	// Local variables
	Error funsapeStatus;
	uint8_t *auxBuff = NULL;
	uint8_t aux8 = 0;
	uint16_t aux16 = 0;
	int32_t aux32 = 0;
	uint16_t *buff16ptr1 = (uint16_t *)buff1;
	uint16_t *buff16ptr2 = (uint16_t *)buff2;
	uint32_t *buff32ptr1 = (uint32_t *)buff1;
	uint32_t *buff32ptr2 = (uint32_t *)buff2;

	debugMark();

	// Evaluate buffer size
	switch(this->_modeControl) {
	case ModeControl::HEART_RATE_MODE:
		aux16 = MAX30102_BYTES_PER_CHANNEL * nSamples * 1;
		break;
	case ModeControl::SPO2_MODE:
		aux16 = MAX30102_BYTES_PER_CHANNEL * nSamples * 2;
		break;
	case ModeControl::MULTI_LED_MODE:
		aux16 = MAX30102_BYTES_PER_CHANNEL * nSamples * this->_enabledSlotsCounter;
		break;
	default:
		break;
	}

	// Memory allocation
	auxBuff = (uint8_t *)calloc(aux16, sizeof(uint8_t));
	if(!isPointerValid(auxBuff)) {
		// Returns error
		this->_lastError = Error::MEMORY_ALLOCATION;
		debugMessage(Error::MEMORY_ALLOCATION);
		return false;
	}

	// Selects device
	if(!this->_handler->setTwiAddress(MAX30102_DEVICE_ADDRESS)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Reads from device
	if(!this->_handler->read(MAX30102_REG_FIFO_DATA, auxBuff, aux16)) {
		// Returns error
		funsapeStatus = this->_handler->getLastError();
		this->_lastError = funsapeStatus;
		debugMessage(funsapeStatus);
		return false;
	}

	// Evaluate resolution fix
	switch(this->_resolution) {
	case Resolution::RESOLUTION_15_BITS:
		aux8 = 3;
		break;
	case Resolution::RESOLUTION_16_BITS:
		aux8 = 2;
		break;
	case Resolution::RESOLUTION_17_BITS:
		aux8 = 1;
		break;
	case Resolution::RESOLUTION_18_BITS:
		aux8 = 0;
		break;
	default:
		free(auxBuff);
		// Returns error
		this->_lastError = Error::UNKNOWN;						// TODO: Evaluate error
		debugMessage(Error::UNKNOWN);
		return false;
	}

	// Decode data
	aux16 = 0;
	for(uint8_t i = 0; i < nSamples; i++) {
		switch(this->_modeControl) {
		case ModeControl::HEART_RATE_MODE:
			aux32 = (uint32_t)(auxBuff[aux16++]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[aux16++]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[aux16++]);
			if(varSize == 2) {
				*buff16ptr1++ = (uint16_t)(aux32 >> aux8);
			} else {
				*buff32ptr1++ = (aux32 >> aux8);
			}
			break;

		case ModeControl::SPO2_MODE:
			aux32 = (uint32_t)(auxBuff[aux16++]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[aux16++]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[aux16++]);
			if(varSize == 2) {
				*buff16ptr1++ = (uint16_t)(aux32 >> aux8);
			} else {
				*buff32ptr1++ = (aux32 >> aux8);
			}
			aux32 = (uint32_t)(auxBuff[aux16++]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[aux16++]);
			aux32 <<= 8;
			aux32 |= (uint32_t)(auxBuff[aux16++]);
			if(varSize == 2) {
				*buff16ptr2++ = (uint16_t)(aux32 >> aux8);
			} else {
				*buff32ptr2++ = (aux32 >> aux8);
			}
			break;

		case ModeControl::MULTI_LED_MODE:
			// TODO: Implement this mode
			break;
		}
	}
	free(auxBuff);

	// Update data members
	this->_getFifoInfo();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE







/*
//Given a slot number assign a thing to it
//Devices are SLOT_RED_LED or SLOT_RED_PILOT (proximity)
//Assigning a SLOT_RED_LED will pulse LED
//Assigning a SLOT_RED_PILOT will ??
void Max30102::enableSlot(uint8_t slotNumber, uint8_t device)
{

	uint8_t originalContents;

	switch (slotNumber) {
	case (1):
		bitMask(MAX30102_REG_MULTILEDCONFIG1, MAX30102_REG_SLOT1_MASK, device);
		break;
	case (2):
		bitMask(MAX30102_REG_MULTILEDCONFIG1, MAX30102_REG_SLOT2_MASK, device << 4);
		break;
	case (3):
		bitMask(MAX30102_REG_MULTILEDCONFIG2, MAX30102_REG_SLOT3_MASK, device);
		break;
	case (4):
		bitMask(MAX30102_REG_MULTILEDCONFIG2, MAX30102_REG_SLOT4_MASK, device << 4);
		break;
	default:
		//Shouldn't be here!
		break;
	}
}

//Clears all slot assignments
void Max30102::disableSlots(void)
{
	writeRegister8(_i2caddr, MAX30102_REG_MULTILEDCONFIG1, 0);
	writeRegister8(_i2caddr, MAX30102_REG_MULTILEDCONFIG2, 0);
}

// Die Temperature
// Returns temp in C
float Max30102::readTemperature()
{
	// Step 1: Config die temperature register to take 1 temperature sample
	writeRegister8(_i2caddr, MAX30102_REG_DIETEMPCONFIG, 0x01);

	// Poll for bit to clear, reading is then complete
	// Timeout after 100ms
	unsigned long startTime = millis();
	while (millis() - startTime < 100) {
		uint8_t response = readRegister8(_i2caddr, MAX30102_REG_DIETEMPCONFIG);
		if ((response & 0x01) == 0) { break; } //We're done!
		delay(1); //Let's not over burden the I2C bus
	}
	//TODO How do we want to fail? With what type of error?
	//? if(millis() - startTime >= 100) return(-999.0);

	// Step 2: Read die temperature register (integer)
	int8_t tempInt = readRegister8(_i2caddr, MAX30102_REG_DIETEMPINT);
	uint8_t tempFrac = readRegister8(_i2caddr, MAX30102_REG_DIETEMPFRAC);

	// Step 3: Calculate temperature (datasheet pg. 23)
	return (float)tempInt + ((float)tempFrac * 0.0625);
}

// Returns die temp in F
float Max30102::readTemperatureF()
{
	float temp = readTemperature();

	if (temp != -999.0) { temp = temp * 1.8 + 32.0; }

	return (temp);
}
*/
