/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseAvrUsart.cpp
 * Module:		USART Module Interface for GPDSE++ AVR Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseAvrUsart.hpp>
#if __GPDSE_AVR_USART_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [gpdseAvrUsart.cpp] Build mismatch on header and source code files.
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

Usart0 *usartDefaultHandler;
FILE usartStream;

#if USART0_EXISTS
Usart0 usart0(USART_BAUD_RATE_57600);
#endif // USART0_EXISTS

#if USART1_EXISTS
Usart1 usart1;
#endif // USART1_EXISTS

// =============================================================================
// Static functions declarations
// =============================================================================

int usartTransmitStdWrapper(char data, FILE *stream);

// =============================================================================
// Class constructors
// =============================================================================

Usart0::Usart0(void)
{
	// Reset data members
	this->_resetDataMembers();

	// Return successfull
	this->_lastError = ERROR_NONE;
	return;
}

Usart0::Usart0(	usartBaudRate_e baudRate,
				usartMode_e mode,
				usartFrameFormat_e frameFormat)
{
	uint16_t aux16 = 0;
	usartStopBits_e stopBits;
	usartParity_e parity;
	usartDataLength_e dataLength;

	// Reset data members
	this->_resetDataMembers();

	// Decode frame
	aux16 = (uint16_t)frameFormat;
	stopBits = (usartStopBits_e)(aux16 & 0x03);
	aux16 >>= 2;
	parity = (usartParity_e)(aux16 & 0x07);
	aux16 >>= 3;
	dataLength = (usartDataLength_e)(aux16 & 0x0F);

	// Check for errors
	if (!this->_validateDataLength(dataLength)) {
		// Error
		return;
	}
	if (!this->_validateModeAndBaudRate(mode, baudRate)) {
		// Error
		return;
	}
	if (!this->_validateParity(parity)) {
		// Error
		return;
	}
	if (!this->_validateStopBits(stopBits)) {
		// Error
		return;
	}

	// Update data members
	this->_baudRate = baudRate;
	this->_dataLength = dataLength;
	this->_mode = mode;
	this->_parity = parity;
	this->_stopBits = stopBits;

	// Standard input/output stream
	fdev_setup_stream(&usartStream, usartTransmitStdWrapper, NULL, _FDEV_SETUP_WRITE);

	// Initialize
	if (!this->initialize()) {
		// Error
		return;
	}

	// Return successfull
	this->_lastError = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool Usart0::deinitialize(void)
{
	// Deactivates USART interface
	this->_initialized = false;
	this->flushDataReceptionBuffer();
	UCSR0A = 0x00;
	UCSR0B = 0x00;
	UCSR0C = 0x00;
	UBRR0 = 0x0000;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::initialize(void)
{
	// Register values
	uint8_t ucsraReg = 0;
	uint8_t ucsrbReg = 0;
	uint8_t ucsrcReg = 0;
	uint16_t ubrrReg = 0;
	uint32_t aux32 = 0;

	// Deactivates USART interface
	this->deinitialize();

	// -------------------------------------------------------------------------
	// Calculate baud rate and mode values -------------------------------------

	switch (this->_mode) {
	// Assynchronous Mode
	case USART_MODE_ASYNCHRONOUS:
		// Calculate baud rate using double speed mode
		aux32 = (_GPDSE_CPU_CLOCK / (USART_PRESCALER(USART_MODE_ASYNCHRONOUS) / 2));
		aux32 /= (uint32_t)(this->_baudRate);
		aux32--;
		if (aux32 < 4096) {								// Double speed may be used
			this->_doubleSpeed = true;
			ubrrReg = (uint16_t)aux32;
		} else {
			// Calculate baud rate using normal speed mode
			this->_doubleSpeed = false;
			aux32 = (_GPDSE_CPU_CLOCK / USART_PRESCALER(USART_MODE_ASYNCHRONOUS));
			aux32 /= (uint32_t)(this->_baudRate);
			aux32--;
			if (aux32 < 4096) {							// Normal speed may be used
				ubrrReg = (uint16_t)aux32;
			} else {									// Unreachable baud rate
				this->_lastError = ERROR_USART_BAUD_RATE_TOO_LOW;
				return false;
			}
		}
		break;
	// Synchronous Mode Slave Role
	case USART_MODE_SYNCHRONOUS_SLAVE:
		setBit(ucsrcReg, UMSEL00);						// Set operation mode
		break;
	// Synchronous Mode Master Role
	case USART_MODE_SYNCHRONOUS_MASTER:
		setBit(ucsrcReg, UMSEL00);						// Set operation mode
		this->_masterRoleEnabled = true;				// Required in this mode
		// Calculate baud rate
		aux32 = (_GPDSE_CPU_CLOCK / (USART_PRESCALER(USART_MODE_SYNCHRONOUS_MASTER)));
		aux32 /= (uint32_t)(this->_baudRate);
		aux32--;
		if (aux32 < 4096) {								// Value may be used
			ubrrReg = (uint16_t)aux32;
		} else {										// Unreachable baud rate
			this->_lastError = ERROR_USART_BAUD_RATE_TOO_LOW;
			return false;
		}
		break;
	// SPI Emulation Mode
	case USART_MODE_SPI_EMULATION_MASTER:
		setBit(ucsrcReg, UMSEL01);						// Set operation mode
		setBit(ucsrcReg, UMSEL00);						// ...
		this->_multiProcessorEnabled = false;			// Not allowed in this mode
		this->_transmitterEnabled = true;				// Required in this mode
		this->_masterRoleEnabled = true;				// Required in this mode
		this->_dataLength = USART_DATA_LENGTH_8_BITS;	// Required in this mode
		// Calculate baud rate
		aux32 = (_GPDSE_CPU_CLOCK / (USART_PRESCALER(USART_MODE_SPI_EMULATION_MASTER)));
		aux32 /= (uint32_t)(this->_baudRate);
		aux32--;
		if (aux32 < 4096) {								// Value may be used
			ubrrReg = (uint16_t)aux32;
		} else {
			this->_lastError = ERROR_USART_BAUD_RATE_TOO_LOW;
			return false;
		}
		break;
	// Mode Unsupported
	default:
		this->_lastError = ERROR_UNSUPPORTED_OPERATION_MODE;
		return false;
	}

	// -------------------------------------------------------------------------
	// Parity mode -------------------------------------------------------------

	switch (this->_parity) {
	case USART_PARITY_ODD:
		setBit(ucsrcReg,  UPM01);				// Set parity mode
		setBit(ucsrcReg,  UPM00);				// ...
		break;
	case USART_PARITY_EVEN:
		setBit(ucsrcReg,  UPM01);				// Set parity mode
		break;
	case USART_PARITY_NONE:
		doNothing();
		break;
	case USART_PARITY_MARK:
	case USART_PARITY_SPACE:
	default:
		this->_lastError = ERROR_UNSUPPORTED_PARITY_MODE;
		return false;
	}

	// -------------------------------------------------------------------------
	// Stop bits mode ----------------------------------------------------------

	switch (this->_stopBits) {
	case USART_STOP_BIT_DOUBLE:
		setBit(ucsrcReg, USBS0);				// Set stop bits mode
		break;
	case USART_STOP_BIT_SINGLE:
		doNothing();
		break;
	case USART_STOP_BIT_ONE_HALF:
	default:
		this->_lastError = ERROR_UNSUPPORTED_STOP_BITS;
		return false;
	}

	// -------------------------------------------------------------------------
	// Data length -------------------------------------------------------------

	switch (this->_dataLength) {
	case USART_DATA_LENGTH_5_BITS:
		doNothing();
		break;
	case USART_DATA_LENGTH_6_BITS:
		setBit(ucsrcReg, UCSZ00);				// Set 6 bits
		break;
	case USART_DATA_LENGTH_7_BITS:
		setBit(ucsrcReg, UCSZ01);				// Set 7 bits
		break;
	case USART_DATA_LENGTH_8_BITS:
		setBit(ucsrcReg, UCSZ01);				// Set 8 bits
		setBit(ucsrcReg, UCSZ00);				// ...
		break;
	case USART_DATA_LENGTH_9_BITS:
		setBit(ucsrbReg, UCSZ02);				// Set 9 bits
		setBit(ucsrcReg, UCSZ01);				// ...
		setBit(ucsrcReg, UCSZ00);				// ...
		break;
	case USART_DATA_LENGTH_10_BITS:
	case USART_DATA_LENGTH_11_BITS:
	case USART_DATA_LENGTH_12_BITS:
	case USART_DATA_LENGTH_13_BITS:
	case USART_DATA_LENGTH_14_BITS:
	case USART_DATA_LENGTH_15_BITS:
	case USART_DATA_LENGTH_16_BITS:
	case USART_DATA_LENGTH_4_BITS:
	default:
		this->_lastError = ERROR_UNSUPPORTED_DATA_LENGTH;
		return false;
	}

	// -------------------------------------------------------------------------
	// Other configurations ----------------------------------------------------

	if (this->_doubleSpeed) {					// Double speed mode
		setBit(ucsraReg, U2X0);
	}
	if (this->_multiProcessorEnabled) {			// Multi-processor mode
		setBit(ucsraReg, MPCM0);
	}
	if (this->_receiverEnabled) {				// Enable receiver
		setBit(ucsrbReg, RXEN0);
	}
	if (this->_transmitterEnabled) {			// Enable transmitter
		setBit(ucsrbReg, TXEN0);
	}
	if (this->_intRecepComplEnabled) {			// Reception complete interrupt
		setBit(ucsrbReg, RXCIE0);
	}
	if (this->_intTransComplEnabled) {			// Transmission complete interrupt
		setBit(ucsrbReg, TXCIE0);
	}
	if (this->_intDataBufEmptyEnabled) {		// Data register empty
		setBit(ucsrbReg, UDRIE0);
	}
	if (this->_clockPolarityInverted) {			// Invert clock polarity
		setBit(ucsrbReg, UCPOL0);
	}
	if (this->_masterRoleEnabled) {				// Configure XCK pin
		switch (this->_mode) {
		case USART_MODE_ASYNCHRONOUS:
			doNothing();
			break;
		case USART_MODE_SYNCHRONOUS_SLAVE:
			clrBit(DDRD, PD4);
			break;
		case USART_MODE_SYNCHRONOUS_MASTER:
		case USART_MODE_SPI_EMULATION_MASTER:
			setBit(DDRD, PD4);
			break;
		}
	}

	// Update registers
	UCSR0A = ucsraReg;
	UCSR0B = ucsrbReg;
	UCSR0C = ucsrcReg;
	UBRR0H = (uint8_t)(ubrrReg >> 8);
	UBRR0L = (uint8_t)(ubrrReg & 0xFF);

	// Return successfull
	this->_initialized = true;
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::setBaudRate(usartBaudRate_e baudRate)
{
	// Check for errors
	if (!this->_validateModeAndBaudRate(this->_mode, baudRate)) {
		// Error
		return false;
	}

	// Update class member
	this->_initialized = false;
	this->_doubleSpeed = false;
	this->_baudRate = baudRate;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::setDataLength(usartDataLength_e length)
{
	// Check for errors
	if (!this->_validateDataLength(length)) {
		// Error
		return false;
	}

	// Update class member
	this->_initialized = false;
	this->_dataLength = length;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::setMode(usartMode_e mode, usartBaudRate_e baudRate)
{
	// Check for errors
	if (!this->_validateModeAndBaudRate(mode, baudRate)) {
		// Error
		return false;
	}

	// Update class member
	this->_initialized = false;
	this->_doubleSpeed = false;
	this->_baudRate = baudRate;
	this->_mode = mode;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::setParity(usartParity_e parity)
{
	// Check for errors
	if (!this->_validateParity(parity)) {
		// Error
		return false;
	}

	// Update class member
	this->_initialized = false;
	this->_parity = parity;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::setStopBits(usartStopBits_e stopBit)
{
	// Check for errors
	if (!this->_validateStopBits(stopBit)) {
		// Error
		return false;
	}

	// Update class member
	this->_initialized = false;
	this->_stopBits = stopBit;

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

void Usart0::flushDataReceptionBuffer(void)
{
	__attribute__((unused)) vuint8_t aux8;

	while (this->isReceptionComplete()) {
		aux8 = UDR0;
	}

	return;
}

bool Usart0::receiveData(uint8_t * data)
{
	__attribute__((unused)) vuint8_t status;

	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	waitUntilBitIsSet(UCSR0A, RXC0);    // Waits until last reception ends
	status = UCSR0A;
	*data = UDR0;

	return true;
}

bool Usart0::receiveData(uint16_t * data)
{
	__attribute__((unused)) vuint8_t status;
	uint8_t byteh;
	uint8_t bytel;
	uint16_t word;

	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends
	status = UCSR0A;
	byteh = UCSR0B;
	bytel = UDR0;

	word = (uint16_t)(byteh & 0x02) << 7;
	word |= bytel;

	*data = word;
	return true;
}

bool Usart0::sendData(uint8_t data)
{
	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	UDR0 = data;
	return true;
}

bool Usart0::sendData(uint16_t data)
{
	__attribute__((unused)) vuint8_t aux;

	if (!this->_initialized) {
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	aux = ((data & 0x100) >> 8);
	if (aux) {
		setBit(UCSR0B, TXB80);
	} else {
		clrBit(UCSR0B, TXB80);
	}
	UDR0 = (uint8_t)data;

	return true;
}

error_e Usart0::getLastError(void)
{
	return this->_lastError;
}

bool Usart0::getLastReceptionError(usartError_f * receptionError)
{

	*receptionError = this->_lastReceptionErrors;

	if (this->_lastReceptionErrors) {
		return false;
	}

	// Return successfull
	this->_lastError = ERROR_NONE;
	return true;
}

char Usart0::receiveDataStd(FILE * stream)
{
	waitUntilBitIsSet(UCSR0A, RXC0);	// Waits until last reception ends

	return (int16_t)UDR0;
}

int_t Usart0::sendDataStd(char data, FILE * stream)
{
	waitUntilBitIsSet(UCSR0A, UDRE0);	// Waits until last transmission ends
	UDR0 = data;

	return 0;
}

// =============================================================================
// Class private methods
// =============================================================================

bool Usart0::_validateDataLength(usartDataLength_e dataLength)
{
	// Check error - verify if data length exists
	switch (dataLength) {
	case USART_DATA_LENGTH_5_BITS:
	case USART_DATA_LENGTH_6_BITS:
	case USART_DATA_LENGTH_7_BITS:
	case USART_DATA_LENGTH_8_BITS:
	case USART_DATA_LENGTH_9_BITS:
		break;
	case USART_DATA_LENGTH_4_BITS:
	case USART_DATA_LENGTH_10_BITS:
	case USART_DATA_LENGTH_11_BITS:
	case USART_DATA_LENGTH_12_BITS:
	case USART_DATA_LENGTH_13_BITS:
	case USART_DATA_LENGTH_14_BITS:
	case USART_DATA_LENGTH_15_BITS:
	case USART_DATA_LENGTH_16_BITS:
	default:
		// Return error
		this->_lastError = ERROR_UNSUPPORTED_DATA_LENGTH;
		return false;
	}

	// Return successfully
	return true;
}

bool Usart0::_validateModeAndBaudRate(usartMode_e mode, usartBaudRate_e baudRate)
{
	// Check error - verify if mode exists
	switch (mode) {
	case USART_MODE_ASYNCHRONOUS:
	case USART_MODE_SYNCHRONOUS_SLAVE:
	case USART_MODE_SYNCHRONOUS_MASTER:
	case USART_MODE_SPI_EMULATION_MASTER:
		break;
	default:
		// Return error
		this->_lastError = ERROR_UNSUPPORTED_OPERATION_MODE;
		return false;
	}

	// Check error - verify if baud rate exists
	switch (baudRate) {
	case USART_BAUD_RATE_60:
	case USART_BAUD_RATE_75:
	case USART_BAUD_RATE_110:
	case USART_BAUD_RATE_150:
	case USART_BAUD_RATE_300:
	case USART_BAUD_RATE_600:
	case USART_BAUD_RATE_1200:
	case USART_BAUD_RATE_1600:
	case USART_BAUD_RATE_1800:
	case USART_BAUD_RATE_2400:
	case USART_BAUD_RATE_3200:
	case USART_BAUD_RATE_3429:
	case USART_BAUD_RATE_4800:
	case USART_BAUD_RATE_8000:
	case USART_BAUD_RATE_9600:
	case USART_BAUD_RATE_14400:
	case USART_BAUD_RATE_19200:
	case USART_BAUD_RATE_28800:
	case USART_BAUD_RATE_38400:
	case USART_BAUD_RATE_56000:
	case USART_BAUD_RATE_57600:
	case USART_BAUD_RATE_115200:
	case USART_BAUD_RATE_128000:
	case USART_BAUD_RATE_230400:
	case USART_BAUD_RATE_250000:
	case USART_BAUD_RATE_256000:
	case USART_BAUD_RATE_460800:
	case USART_BAUD_RATE_500000:
	case USART_BAUD_RATE_921600:
	case USART_BAUD_RATE_1000000:
		break;
	default:
		// Return error
		this->_lastError = ERROR_UNSUPPORTED_BAUD_RATE;
		return false;
	}

	// Check errors - baud rate (min value)
	if (this->_baudRate <= USART_MIN_BAUD_RATE(this->_mode)) {
		// Return error
		this->_lastError = ERROR_USART_BAUD_RATE_TOO_LOW;
		return false;
	}

	// Check errors - baud rate (max value)
	if (this->_baudRate >= USART_MAX_BAUD_RATE(this->_mode)) {
		// Return error
		this->_lastError = ERROR_USART_BAUD_RATE_TOO_HIGH;
		return false;
	}

	// Return successfully
	return true;
}

bool Usart0::_validateParity(usartParity_e parity)
{
	// Check error - verify if parity exists
	switch (parity) {
	case USART_PARITY_NONE:
	case USART_PARITY_ODD:
	case USART_PARITY_EVEN:
		break;
	case USART_PARITY_MARK:
	case USART_PARITY_SPACE:
	default:
		// Return error
		this->_lastError = ERROR_UNSUPPORTED_PARITY_MODE;
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::_validateStopBits(usartStopBits_e stopBits)
{
	// Check error - verify if stop bits exists
	switch (stopBits) {
	case USART_STOP_BIT_SINGLE:
	case USART_STOP_BIT_DOUBLE:
		break;
	case USART_STOP_BIT_ONE_HALF:
	default:
		// Return error
		this->_lastError = ERROR_UNSUPPORTED_STOP_BITS;
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::_checkReceptionErrors(void)
{
	usartError_f errors = USART_ERROR_NONE;

	if (isBitSet(UCSR0A, FE0)) {
		errors = errors | USART_ERROR_FRAME;
	}
	if (isBitSet(UCSR0A, DOR0)) {
		errors = errors | USART_ERROR_PARITY;
	}
	if (isBitSet(UCSR0A, UPE0)) {
		errors = errors | USART_ERROR_BUFFER_OVERFLOW;
	}

	// Update data members
	this->_lastReceptionErrors = errors;

	if (errors) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

void Usart0::_resetDataMembers(void)
{
	// Reset data members - Initialization
	this->_initialized = false;
	this->_receiverEnabled = false;
	this->_transmitterEnabled = false;
	// Reset data members - Protocol configuration
	this->_baudRate = USART_BAUD_RATE_57600;
	this->_dataLength = USART_DATA_LENGTH_8_BITS;
	this->_doubleSpeed = false;
	this->_mode = USART_MODE_ASYNCHRONOUS;
	this->_parity = USART_PARITY_NONE;
	this->_stopBits = USART_STOP_BIT_SINGLE;
	// Reset data members - Error messages
	this->_lastError = ERROR_NONE;
	this->_lastReceptionErrors = USART_ERROR_NONE;
	// Reset data members - Interrupts
	this->_intDataBufEmptyEnabled = false;
	this->_intRecepComplEnabled = false;
	this->_intTransComplEnabled = false;
	// Reset data members - To Do
	this->_multiProcessorEnabled = false;
	this->_clockPolarityInverted = false;
	this->_masterRoleEnabled = false;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

int usartTransmitStdWrapper(char c, FILE * f)
{
	return usartDefaultHandler->sendDataStd(c, f);
}
