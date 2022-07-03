
Usart0 *usartDefaultHandler;
FILE usartStream;

int usartTransmitStdWrapper(char data, FILE *stream);

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

int usartTransmitStdWrapper(char c, FILE *f)
{
	return usartDefaultHandler->sendDataStd(c, f);
}
