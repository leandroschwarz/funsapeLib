/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	usart0.cpp
 * Module:		USART0 Module Interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 16, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "usart0.hpp"
#if __USART0_HPP != 1002
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
// =============================================================================

Usart0 *usartDefaultHandler;
FILE usartStream;

// =============================================================================
// Static functions definitions
// =============================================================================

int usartTransmitStdWrapper(char data, FILE *stream);

// =============================================================================
// Class constructors
// =============================================================================

Usart0::Usart0(usartBaudRate_e baudRate, usartMode_e mode, usartFrameFormat_e frameFormat)
{
	uint8_t aux8 = (uint8_t)frameFormat;

	this->configFlags_ = USART_CONFIG_FLAGS_NONE;

	// Error checking - format
	switch (frameFormat) {
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
		return;
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
		return;
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
		return;
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

	fdev_setup_stream(&usartStream, usartTransmitStdWrapper, NULL, _FDEV_SETUP_WRITE);

	this->lastError_ = ERROR_NONE;
}

// =============================================================================
// Class public methods
// =============================================================================

bool Usart0::setParity(usartParity_e parity)
{
	// Error checking
	switch (parity) {
	case USART_PARITY_NONE:
	case USART_PARITY_ODD:
	case USART_PARITY_EVEN:
		break;
	default:
		this->lastError_ = ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY;
		return false;
	}

	// Update class member
	this->parity_ = parity;
	this->configFlags_ = this->configFlags_ | USART_CONFIG_FLAGS_PARITY;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::setMode(usartMode_e mode)
{
	// Error checking
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

	// Updating class member
	this->mode_ = mode;
	this->configFlags_ = this->configFlags_ | USART_CONFIG_FLAGS_MODE;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::setBaudRate(usartBaudRate_e baudRate)
{
	// Error checking
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

	// Update class member
	this->baudRate_ = baudRate;
	this->configFlags_ = this->configFlags_ | USART_CONFIG_FLAGS_BAUD_RATE;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::setStopBit(usartStopBit_e stopBit)
{
	// Error checking
	switch (stopBit) {
	case USART_STOP_BIT_SINGLE:
	case USART_STOP_BIT_DOUBLE:
		break;
	default:
		this->lastError_ = ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT;
		return false;
	}

	// Update class member
	this->stopBit_ = stopBit;
	this->configFlags_ = this->configFlags_ | USART_CONFIG_FLAGS_STOP_BIT;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::setDataBits(usartDataBits_e dataBits)
{
	// Error checking
	switch (dataBits) {
	case USART_DATA_BITS_5:
	case USART_DATA_BITS_6:
	case USART_DATA_BITS_7:
	case USART_DATA_BITS_8:
	case USART_DATA_BITS_9:
		break;
	default:
		this->lastError_ = ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS;
		return false;
	}

	// Update class member
	this->dataBits_ = dataBits;
	this->configFlags_ = this->configFlags_ | USART_CONFIG_FLAGS_DATA_BITS;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Usart0::initialize(void)
{
	uint8_t reg1 = UCSR0A;
	uint8_t reg2 = UCSR0B;
	uint8_t reg3 = UCSR0C;
	uint64_t aux64 = (uint64_t)UBRR0;

	// Error checking - parameters configured
	if (this->configFlags_ != USART_CONFIG_FLAGS_ALL) {
		this->lastError_ = ERROR_USART_CONFIGURATION_PARAMETER_WRONG;
		return false;
	}

	// Clear errors
	reg1 &= ~((1 << FE0) | (1 << DOR0) | (1 << UPE0));

	// Configure stop bit
	switch (this->stopBit_) {
	case USART_STOP_BIT_SINGLE:
		clrBit(reg3, USBS0);
		break;
	case USART_STOP_BIT_DOUBLE:
		setBit(reg3, USBS0);
		break;
	}

	// Configure parity
	reg3 &= ~(0x03 << UPM00);
	switch (this->parity_) {
	case USART_PARITY_NONE:						// Parity NONE
		break;
	case USART_PARITY_ODD:						// Parity ODD
		reg3 |= (0x03 << UPM00);
		break;
	case USART_PARITY_EVEN:						// Parity EVEN
		reg3 |= (0x02 << UPM00);
		break;
	}

	// Configure data bits
	clrBit(reg2, UCSZ02);
	reg3 &= ~(0x03 << UCSZ00);
	switch (this->dataBits_) {
	case USART_DATA_BITS_5:						// 5 data bits
		break;
	case USART_DATA_BITS_6:						// 6 data bits
		reg3 |= (0x01 << UCSZ00);
		break;
	case USART_DATA_BITS_7:						// 7 data bits
		reg3 |= (0x02 << UCSZ00);
		break;
	case USART_DATA_BITS_8:						// 8 data bits
		reg3 |= (0x03 << UCSZ00);
		break;
	case USART_DATA_BITS_9:						// 9 data bits
		setBit(reg2, UCSZ02);
		reg3 |= (0x03 << UCSZ00);
		break;
	}

	// Configure mode
	reg3 &= ~(0x03 << UMSEL00);
	clrBit(reg1, U2X0);				// Single Speed
	clrBit(reg3, UCPOL0);			// Polarity
	switch (this->mode_) {
	case USART_MODE_ASYNCHRONOUS:
		break;
	case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:
		setBit(reg1, U2X0);			// Double Speed
		break;
	case USART_MODE_SYNCHRONOUS_XCK_FALLING:
		reg3 |= (1 << UMSEL00);		// Synchronous Mode falling edge
		break;
	case USART_MODE_SYNCHRONOUS_XCK_RISING:
		reg3 |= (1 << UMSEL00);		// Synchronous Mode rising edge
		break;
	case USART_MODE_SYNCHRONOUS_SPI:
		reg3 |= (3 << UMSEL00);		// Synchronous SPI Mode
		setBit(reg3, UCPOL0);		// Polarity
		break;
	}

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

	// Configures USART registers
	UCSR0A = reg1;
	UCSR0B = reg2;
	UCSR0C = reg3;
	UBRR0H = (uint8_t)(0x0F & (aux64 >> 8));
	UBRR0L = (uint8_t)(0xFF & aux64);

	this->lastError_ = ERROR_NONE;
	return true;
}

Usart0::usartReceptionError_f Usart0::getReceptionErrors(void)
{
	return this->receptionErrors_;
}

void Usart0::sendData(uint8_t data)
{
	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	UDR0 = data;
	return;
}

uint8_t Usart0::receiveData(void)
{
	__attribute__((unused)) vuint8_t status;

	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends
	status = UCSR0A;
	return UDR0;
}

void Usart0::sendData9Bits(uint16_t data)
{
	__attribute__((unused)) vuint8_t aux;

	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	aux = ((data & 0x100) >> 8);
	if (aux) {
		setBit(UCSR0B, TXB80);
	} else {
		clrBit(UCSR0B, TXB80);
	}
	UDR0 = (uint8_t)data;

	return;
}

uint16_t Usart0::receiveData9bits(void)
{
	__attribute__((unused)) vuint8_t status;
	uint8_t byteh;
	uint8_t bytel;
	uint16_t byte;

	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends
	status = UCSR0A;
	byteh = UCSR0B;
	bytel = UDR0;

	byte = (uint16_t)(byteh & 0x02) << 7;
	byte |= bytel;

	return  byte;
}

void Usart0::flushReceptionBuffer(void)
{
	__attribute__((unused)) vuint8_t aux8;

	while (this->isReceptionComplete()) {
		aux8 = UDR0;
	}

	return;
}

// =============================================================================
// Class private methods
// =============================================================================

void Usart0::checkReceptionErrors(void)
{
	usartReceptionError_f errors = USART_RECEPTION_ERROR_NONE;

	if (isBitSet(UCSR0A, FE0)) {
		errors = errors | USART_RECEPTION_ERROR_FRAME;
	}
	if (isBitSet(UCSR0A, DOR0)) {
		errors = errors | USART_RECEPTION_ERROR_PARITY;
	}
	if (isBitSet(UCSR0A, UPE0)) {
		errors = errors | USART_RECEPTION_ERROR_BUFFER_OVERFLOW;
	}

	this->receptionErrors_ = errors;
}

char Usart0::receiveDataStd(FILE *stream)
{
	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends

	return (int16_t)UDR0;
}

int16_t Usart0::sendDataStd(char data, FILE *stream)
{
	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	UDR0 = data;

	return 0;
}

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
