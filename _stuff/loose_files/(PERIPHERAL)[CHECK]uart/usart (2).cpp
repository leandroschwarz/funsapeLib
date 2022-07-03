/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	usart.cpp
 * Module:		USART0 Module Interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 16, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "usart.hpp"
#if __USART_HPP != 1002
#	error [usart0.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// *INDENT-OFF*
// =============================================================================

#if USART0_EXISTS
	FILE usartStream;
	Usart0 usart0;
	Usart0 *usartDefaultHandler;
#endif

#if USART1_EXISTS
//	FILE usart1Stream;
//	Usart1 usart1;
#endif
// *INDENT-ON*

// =============================================================================
// Static functions definitions
// =============================================================================

int usartTransmitStdWrapper(char data, FILE *stream);

// =============================================================================
// Class constructors
// =============================================================================

// -----------------------------------------------------------------------------
// Usart0 ----------------------------------------------------------------------

#if USART0_EXISTS
Usart0::Usart0(void)
{
	uint64_t aux64 = 0;

	// Reset all data members
	this->mode_ = USART_MODE_ASYNCHRONOUS;
	this->baudRate_ = USART_BAUD_9600;
	this->parity_ = USART_PARITY_NONE;
	this->stopBit_ = USART_STOP_BIT_SINGLE;
	this->dataBits_ = USART_DATA_BITS_8;
	this->initialized_ = false;
	this->configFlags_ = USART_CONFIG_FLAGS_NONE;
	this->receptionErrors_ = USART_RECEPTION_ERROR_NONE;

	// Register configuration
	USART0_CONTROL_STATUS_A_REG =	(0 << USART0_DOUBLE_SPEED_BIT)		|
									(0 << USART0_MULTI_PROCESSOR_BIT);

	USART0_CONTROL_STATUS_B_REG =	(0 << USART0_RECEP_COMP_INT_ENABLE_BIT)						|
									(0 << USART0_TRANS_COMP_INT_ENABLE_BIT)						|
									(0 << USART0_DATA_READY_INT_ENABLE_BIT)						|
									(0 << USART0_RECEIVER_ENABLE_BIT)							|
									(0 << USART0_TRANSMITTER_ENABLE_BIT)						|
									(isBitSet(this->dataBits_, 2) << USART0_CHAR_DATA_BIT_2);
	USART0_CONTROL_STATUS_C_REG =	(this->mode_ << USART0_MODE_SELECTION_BIT)					|
									(this->parity_ << USART0_PARITY_BIT)						|
									(this->stopBit_ << USART0_STOP_BIT)							|
									(isBitSet(this->dataBits_, 1) << USART0_CHAR_DATA_BIT_1)	|
									(isBitSet(this->dataBits_, 0) << USART0_CHAR_DATA_BIT_0)	|
									(0 << USART0_CLOCK_POLARITY_BIT);

	// Configure baud rate
	switch (this->mode_) {
	case USART_MODE_ASYNCHRONOUS:
		aux64 = (systemStatus.getCpuClock() / 16 / this->baudRate_) - 1;
		break;
	case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
		aux64 = (systemStatus.getCpuClock() / 8 / this->baudRate_) - 1;
		break;
	case USART_MODE_SYNCHRONOUS_SPI:
		aux64 = (systemStatus.getCpuClock() / 2 / this->baudRate_) - 1;
		break;
	default:
		// TODO
		break;
	}
	USART0_BAUD_RATE_H_REG = (uint8_t)(0x0F & (aux64 >> 8));
	USART0_BAUD_RATE_L_REG = (uint8_t)(0xFF & aux64);

	fdev_setup_stream(&usartStream, usartTransmitStdWrapper, NULL, _FDEV_SETUP_WRITE);

	this->lastError_ = ERROR_NONE;
	return;
}
#endif

// -----------------------------------------------------------------------------
// Usart1 ----------------------------------------------------------------------

#if USART1_EXISTS
#endif

// =============================================================================
// Class public methods
// =============================================================================

// -----------------------------------------------------------------------------
// Usart0 ----------------------------------------------------------------------

#if USART0_EXISTS
bool Usart0::flush(void)
{
	__attribute__((unused)) vuint8_t aux8;

	// Check for errors
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Flushes buffer
	while (this->isReceptionComplete()) {
		aux8 = USART0_DATA_REG;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}


bool Usart0::init(usartBaudRate_e baudRate, usartMode_e mode, usartFrameFormat_e format)
{
	uint8_t aux8 = (uint8_t)format;
	uint64_t aux64 = 0;

	this->configFlags_ = USART_CONFIG_FLAGS_NONE;

	// Error checking - format
	switch (format) {
	case USART_FRAME_FORMAT_8_N_1:
	case USART_FRAME_FORMAT_8_O_1:
	case USART_FRAME_FORMAT_8_E_1:
	case USART_FRAME_FORMAT_8_N_2:
	case USART_FRAME_FORMAT_8_O_2:
	case USART_FRAME_FORMAT_8_E_2:
	case USART_FRAME_FORMAT_9_N_1:
	case USART_FRAME_FORMAT_9_O_1:
	case USART_FRAME_FORMAT_9_E_1:
	case USART_FRAME_FORMAT_9_N_2:
	case USART_FRAME_FORMAT_9_O_2:
	case USART_FRAME_FORMAT_9_E_2:
	case USART_FRAME_FORMAT_5_N_1:
	case USART_FRAME_FORMAT_5_O_1:
	case USART_FRAME_FORMAT_5_E_1:
	case USART_FRAME_FORMAT_5_N_2:
	case USART_FRAME_FORMAT_5_O_2:
	case USART_FRAME_FORMAT_5_E_2:
	case USART_FRAME_FORMAT_6_N_1:
	case USART_FRAME_FORMAT_6_O_1:
	case USART_FRAME_FORMAT_6_E_1:
	case USART_FRAME_FORMAT_6_N_2:
	case USART_FRAME_FORMAT_6_O_2:
	case USART_FRAME_FORMAT_6_E_2:
	case USART_FRAME_FORMAT_7_N_1:
	case USART_FRAME_FORMAT_7_O_1:
	case USART_FRAME_FORMAT_7_E_1:
	case USART_FRAME_FORMAT_7_N_2:
	case USART_FRAME_FORMAT_7_O_2:
	case USART_FRAME_FORMAT_7_E_2:
		break;
	default:
		this->lastError_ = ERROR_USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT;
		return false;
	}
	// Error checking - mode
	switch (mode) {
	case USART_MODE_ASYNCHRONOUS:
	case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
	case USART_MODE_SYNCHRONOUS_XCK_FALLING:
	case USART_MODE_SYNCHRONOUS_XCK_RISING:
	case USART_MODE_SYNCHRONOUS_SPI:
		break;
	default:
		this->lastError_ = ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY;
		return false;
	}
	// Error checking - baud rate
	switch (baudRate) {
	case USART_BAUD_600:
	case USART_BAUD_1200:
	case USART_BAUD_1800:
	case USART_BAUD_2400:
	case USART_BAUD_4800:
	case USART_BAUD_9600:
	case USART_BAUD_14400:
	case USART_BAUD_19200:
	case USART_BAUD_28800:
	case USART_BAUD_38400:
	case USART_BAUD_56000:
	case USART_BAUD_57600:
	case USART_BAUD_115200:
	case USART_BAUD_128000:
	case USART_BAUD_230400:
	case USART_BAUD_256000:
		break;
	default:
		this->lastError_ = ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE;
		return false;
	}

	// Update class members
	this->baudRate_ = baudRate;
	this->mode_ = mode;
	this->stopBit_ = (usartStopBit_e)((aux8 & 0x03) - 1);
	aux8 >>= 2;
	this->parity_ = (usartParity_e)(aux8 & 0x03);
	aux8 >>= 2;
	this->dataBits_ = (usartDataBits_e)((aux8 & 0x0F) - 5);
	this->configFlags_ = USART_CONFIG_FLAGS_ALL;

	// Register configuration
	USART0_CONTROL_STATUS_A_REG =	(0 << USART0_DOUBLE_SPEED_BIT)		|
									(0 << USART0_MULTI_PROCESSOR_BIT);

	USART0_CONTROL_STATUS_B_REG =	(0 << USART0_RECEP_COMP_INT_ENABLE_BIT)						|
									(0 << USART0_TRANS_COMP_INT_ENABLE_BIT)						|
									(0 << USART0_DATA_READY_INT_ENABLE_BIT)						|
									(0 << USART0_RECEIVER_ENABLE_BIT)							|
									(0 << USART0_TRANSMITTER_ENABLE_BIT)						|
									(isBitSet(this->dataBits_, 2) << USART0_CHAR_DATA_BIT_2);
	USART0_CONTROL_STATUS_C_REG =	(this->mode_ << USART0_MODE_SELECTION_BIT)					|
									(this->parity_ << USART0_PARITY_BIT)						|
									(this->stopBit_ << USART0_STOP_BIT)							|
									(isBitSet(this->dataBits_, 1) << USART0_CHAR_DATA_BIT_1)	|
									(isBitSet(this->dataBits_, 0) << USART0_CHAR_DATA_BIT_0)	|
									(0 << USART0_CLOCK_POLARITY_BIT);

	// Configure baud rate
	switch (this->mode_) {
	case USART_MODE_ASYNCHRONOUS:
		aux64 = (systemStatus.getCpuClock() / 16 / this->baudRate_) - 1;
		break;
	case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
		aux64 = (systemStatus.getCpuClock() / 8 / this->baudRate_) - 1;
		break;
	case USART_MODE_SYNCHRONOUS_SPI:
		aux64 = (systemStatus.getCpuClock() / 2 / this->baudRate_) - 1;
		break;
	default:
		// TODO
		break;
	}
	USART0_BAUD_RATE_H_REG = (uint8_t)(0x0F & (aux64 >> 8));
	USART0_BAUD_RATE_L_REG = (uint8_t)(0xFF & aux64);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::setDefault(void)
{
	// Check for errors
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Set default handler
	stdin = stdout = stderr = &usartStream;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Usart0::getLastError(void)
{
	return this->lastError_;
}

bool Usart0::receiveData(uint8_t *data)
{
	__attribute__((unused)) vuint8_t status;

	waitUntilBitIsSet(USART0_CONTROL_STATUS_A_REG, USART0_RECEP_COMP_BIT);	// Waits until last reception ends
	status = USART0_CONTROL_STATUS_A_REG;
	*data = USART0_DATA_REG;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::receiveData9bits(uint16_t *data)
{
	__attribute__((unused)) vuint8_t status;
	__attribute__((unused)) vuint8_t byteh;
	uint8_t bytel;

	waitUntilBitIsSet(USART0_CONTROL_STATUS_A_REG, USART0_RECEP_COMP_BIT);	// Waits until last reception ends
	status = USART0_CONTROL_STATUS_A_REG;
	byteh = USART0_CONTROL_STATUS_B_REG;
	bytel = USART0_DATA_REG;

	*data = isBitSet(USART0_CONTROL_STATUS_B_REG, USART0_DATA_RX_9_BIT);
	*data <<= 8;
	*data |= bytel;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::sendData(uint8_t data)
{
	waitUntilBitIsSet(USART0_CONTROL_STATUS_A_REG, USART0_DATA_READY_BIT);	// Waits until last transmission ends
	USART0_DATA_REG = data;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::sendData9Bits(uint16_t data)
{
	__attribute__((unused)) vuint8_t aux;

	waitUntilBitIsSet(USART0_CONTROL_STATUS_A_REG, USART0_DATA_READY_BIT);	// Waits until last transmission ends
	aux = ((data & 0x100) >> 8);
	if (aux) {
		setBit(USART0_CONTROL_STATUS_B_REG, USART0_DATA_TX_9_BIT);
	} else {
		clrBit(USART0_CONTROL_STATUS_B_REG, USART0_DATA_TX_9_BIT);
	}
	USART0_DATA_REG = (uint8_t)data;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

usartReceptionError_f Usart0::getReceptionErrors(void)
{
	return this->receptionErrors_;
}
#endif

// -----------------------------------------------------------------------------
// Usart1 ----------------------------------------------------------------------

#if USART1_EXISTS
#endif

// =============================================================================
// Class private methods
// =============================================================================

#if USART0_EXISTS
void Usart0::checkReceptionErrors(void)
{
	usartReceptionError_f errors = USART_RECEPTION_ERROR_NONE;

	if (isBitSet(USART0_CONTROL_STATUS_A_REG, USART0_FRAME_ERROR_BIT)) {
		errors = errors | USART_RECEPTION_ERROR_FRAME;
	}
	if (isBitSet(USART0_CONTROL_STATUS_A_REG, USART0_PARITY_ERROR_BIT)) {
		errors = errors | USART_RECEPTION_ERROR_PARITY;
	}
	if (isBitSet(USART0_CONTROL_STATUS_A_REG, USART0_DATA_OVERRUN_ERROR_BIT)) {
		errors = errors | USART_RECEPTION_ERROR_BUFFER_OVERFLOW;
	}

	this->receptionErrors_ = errors;
}

char Usart0::receiveDataStd(FILE *stream)
{
	waitUntilBitIsSet(USART0_CONTROL_STATUS_A_REG, USART0_RECEP_COMP_BIT);	// Waits until last reception ends

	return (int16_t)USART0_DATA_REG;
}

int Usart0::sendDataStd(char data, FILE *stream)
{
	waitUntilBitIsSet(USART0_CONTROL_STATUS_A_REG, USART0_DATA_READY_BIT);	// Waits until last transmission ends
	USART0_DATA_REG = data;

	return 0;
}
#endif

// -----------------------------------------------------------------------------
// Usart1 ----------------------------------------------------------------------

#if USART1_EXISTS
#endif

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */

// =============================================================================
// Static functions definitions
// =============================================================================

int usartTransmitStdWrapper(char c, FILE *f)
{
	return usartDefaultHandler->sendDataStd(c, f);
}
