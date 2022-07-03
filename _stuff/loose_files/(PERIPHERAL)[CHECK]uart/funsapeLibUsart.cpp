/* 
 * Project:		FunSAPE++ Library
 * File name:	funsapeLibUsart.cpp
 * Module:		USART Peripheral Interface Class for FunSAPE++ Library project
 * Author(s):	__AUTHORS_MACRO_TO_BE_CHANGED__
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// 
// System file dependencies
// 

#include "funsapeLibUsart.hpp"
#if __FUNSAPE_LIB_USART_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error [funsapeLibUsart.cpp] Build mismatch on header and source code files.
#endif

#include "funsapeLibSystemStatus.hpp"

// 
// File exclusive - Constants
// 

// NONE

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
// Class constructors
// 

#if USART0_EXISTS

Usart0::Usart0(void)
{
	// Update class members
	this->_synchronousMode = false;
	this->_dataFormat = USART_DATA_8_N_1;
	this->_baudRate = USART_BAUD_9600;

//	fdev_setup_stream(&usartStream, usartTransmitStdWrapper, NULL, _FDEV_SETUP_WRITE);

	// Configure peripheral
	this->_changeDataFormat();
	this->_changeBaudRate();

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

Usart0::~Usart0()
{
}

#endif

// 
// Class public methods
// 

#if USART0_EXISTS

bool Usart0::initialize(uint8_t txBufferSize, uint8_t rxBufferSize)
{
	// Local variables
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


// NONE

#endif

// 
// Class private methods
// 

#if USART0_EXISTS

bool Usart0::_changeDataFormat(void)
{
	// Local variables
	uint8_t dataBits	= 0;
	uint8_t parity		= 0;
	uint8_t stops		= 0;
	uint8_t aux8		= 0;
	uint8_t ucsr0b		= 0;
	uint8_t ucsr0c		= 0;

	// Process usartDataFormat_e enumeration value
	aux8 = (uint8_t)this->_dataFormat;
	stops = (aux8 & 0x03);
	aux8 >>= 2;
	parity = (aux8 & 0x03);
	aux8 >>= 2;
	dataBits = (aux8 & 0x0F);

	// Copy registers
	ucsr0c = UCSR0C;					// Copy register

	// Clear registers old configuration
	ckrMask(ucsr0c, 0x03, UPM00);		// Parity
	clrBit(ucsr0c, USBS0);				// Stop bits
	ckrMask(ucsr0c, 0x03, UCSZ00);		// Data bits
	clrBit(ucsr0b, UCSZ02);				// ...

	// Process stop bits selection
	if (stops == 2) {
		setBit(ucsr0c, USBS0);
	}

	// Process parity selection
	switch (parity) {
	case 0:								// Parity none
		// Nothing to do
		break;
	case 1:								// Parity odd
		setMask(ucsr0c, 0x03, UPM00);
		break;
	case 2:								// Parity even
		setMask(ucsr0c, 0x02, UPM00);
		break;
	default:
		this->_lastError = ERROR_UNKNOWN;
		return false;
	}

	// Process data bits selection
	switch (dataBits) {
	case 5:
	case 6:
	case 7:
	case 8:
		dataBits -= 5;
		setMask(ucsr0c, dataBits, UCSZ00);
		break;
	case 9:
		setBit(ucsr0b, UCSZ20);
		setMask(ucsr0c, 0x03, UCSZ00);
		break;
	default:
		this->_lastError = ERROR_UNKNOWN;
		return false;
	}

	// Update registers
	UCSR0C = ucsr0c;
	UCSR0B = ucsr0b;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Usart0::_changeBaudRate(void)
{
	// Local variables
	uint32_t aux32 = systemStatus.getCpuClock();
	bool doubleSpeedEnabled = false;

	// Evaluate value based on operation mode
	if (!this->_synchronousMode) {
		aux32 /= (uint32_t)this->_baudRate;
		// Test double speed mode
		aux32 /= 8;
		if (aux32 <= 4096) {
			doubleSpeedEnabled = true;
		} else {
			aux32 /= 2;
		}
		aux32--;
	} else {
		// TODO
	}

	// Update registers
	if (doubleSpeedEnabled) {
		setBit(UCSR0A, U2X0);
	} else {
		clrBit(UCSR0A, U2X0);
	}
	UBRR0H = (uint8_t)(aux32 >> 8);
	UBRR0L = (uint8_t)(aux32 & 0xFF);

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

#endif

// 
// Class protected methods
// 

#if USART0_EXISTS

// NONE

#endif

// 
// General public functions definitions
// 

// NONE

// 
// Public functions definitions
// 

// NONE

// 
// Interruption handlers
// 

// NONE
