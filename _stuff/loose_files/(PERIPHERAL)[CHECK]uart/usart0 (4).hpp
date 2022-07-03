
#include "system.hpp"

typedef enum usartMode_e {
	USART_MODE_ASYNCHRONOUS					= 0,
	USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED	= 1,
	USART_MODE_SYNCHRONOUS_XCK_FALLING		= 2,
	USART_MODE_SYNCHRONOUS_XCK_RISING		= 3,
	USART_MODE_SYNCHRONOUS_SPI				= 4,
} usartMode_e;

typedef enum usartParity_e {
	USART_PARITY_NONE	= 0,
	USART_PARITY_ODD	= 1,
	USART_PARITY_EVEN	= 2
} usartParity_e;

typedef enum usartStopBit_e {
	USART_STOP_BIT_SINGLE	= 0,
	USART_STOP_BIT_DOUBLE	= 1
} usartStopBit_e;

typedef enum usartDataBits_e {
	USART_DATA_BITS_5	= 0,
	USART_DATA_BITS_6	= 1,
	USART_DATA_BITS_7	= 2,
	USART_DATA_BITS_8	= 3,
	USART_DATA_BITS_9	= 4
} usartDataBits_e;

class Usart0
{
	// New data types
public:
	enum usartConfigFlags_f {
		USART_CONFIG_FLAGS_NONE			= 0x00,
		USART_CONFIG_FLAGS_BAUD_RATE	= 0x01,
		USART_CONFIG_FLAGS_DATA_BITS	= 0x02,
		USART_CONFIG_FLAGS_MODE			= 0x04,
		USART_CONFIG_FLAGS_PARITY		= 0x08,
		USART_CONFIG_FLAGS_STOP_BIT		= 0x10,
		USART_CONFIG_FLAGS_ALL			= 0x1F
	};
	enum usartReceptionError_f {
		USART_RECEPTION_ERROR_NONE				= 0,
		USART_RECEPTION_ERROR_FRAME				= 1,
		USART_RECEPTION_ERROR_PARITY			= 2,
		USART_RECEPTION_ERROR_BUFFER_OVERFLOW	= 4
	};

	// Methods
public:
	Usart0(usartBaudRate_e baudRate = USART_BAUD_9600, usartMode_e mode = USART_MODE_ASYNCHRONOUS, usartFrameFormat_e format = USART_FRAME_FORMAT_8_N_1);
	bool setParity(usartParity_e parity);
	bool setMode(usartMode_e mode);
	bool setBaudRate(usartBaudRate_e baudRate);
	bool setDataBits(usartDataBits_e dataBits);
	bool setStopBit(usartStopBit_e stopBit);
	bool initialize(void);
	usartReceptionError_f getReceptionErrors(void);
	void sendData(uint8_t data);
	void sendData9Bits(uint16_t data);
	uint8_t receiveData(void);
	uint16_t receiveData9bits(void);
	void flushReceptionBuffer(void);
	int sendDataStd(char data, FILE *stream);

	void inline __attribute__((always_inline)) enableReceiver(void);
	void inline __attribute__((always_inline)) disableReceiver(void);
	void inline __attribute__((always_inline)) enableTransmitter(void);
	void inline __attribute__((always_inline)) disableTransmitter(void);
	void inline __attribute__((always_inline)) activateReceptionCompleteInterrupt(void);
	void inline __attribute__((always_inline)) deactivateReceptionCompleteInterrupt(void);
	void inline __attribute__((always_inline)) activateTransmissionCompleteInterrupt(void);
	void inline __attribute__((always_inline)) deactivateTransmissionCompleteInterrupt(void);
	void inline __attribute__((always_inline)) activateBufferEmptyInterrupt(void);
	void inline __attribute__((always_inline)) deactivateBufferEmptyInterrupt(void);
	bool inline __attribute__((always_inline)) isReceptionComplete(void);
	bool inline __attribute__((always_inline)) isTransmissionComplete(void);
	bool inline __attribute__((always_inline)) isBufferEmpty(void);
	void inline __attribute__((always_inline)) stdio(void);

// Overloading operators

	friend inline __attribute__((always_inline)) usartConfigFlags_f operator ~(usartConfigFlags_f a)
	{
		return usartConfigFlags_f(~int(a));
	}

	friend inline __attribute__((always_inline)) usartConfigFlags_f operator&(usartConfigFlags_f a, usartConfigFlags_f b)
	{
		return usartConfigFlags_f(int(a) & int(b));
	}

	friend inline __attribute__((always_inline)) usartConfigFlags_f operator|(usartConfigFlags_f a, usartConfigFlags_f b)
	{
		return usartConfigFlags_f(int(a) | int(b));
	}

	friend inline __attribute__((always_inline)) usartConfigFlags_f operator^(usartConfigFlags_f a, usartConfigFlags_f b)
	{
		return usartConfigFlags_f(int(a) ^ int(b));
	}

	friend inline __attribute__((always_inline)) usartConfigFlags_f operator>>(usartConfigFlags_f a, uint8_t b)
	{
		return usartConfigFlags_f(int(a) >> b);
	}

	friend inline __attribute__((always_inline)) usartConfigFlags_f operator<<(usartConfigFlags_f a, uint8_t b)
	{
		return usartConfigFlags_f(int(a) << b);
	}

	friend inline __attribute__((always_inline)) usartReceptionError_f operator|(usartReceptionError_f a, usartReceptionError_f b)
	{
		return usartReceptionError_f(int(a) | int(b));
	}


private:
	char receiveDataStd(FILE *stream);
	void checkReceptionErrors(void);
	usartMode_e				mode_				: 3;
	usartBaudRate_e			baudRate_			: 18;
	usartParity_e			parity_				: 2;
	usartStopBit_e			stopBit_			: 1;
	usartDataBits_e			dataBits_			: 3;
	bool					initialized_		: 1;
	usartConfigFlags_f		configFlags_		: 5;
	usartReceptionError_f	receptionErrors_	: 3;
	uint8_t					unusedBits_			: 4;
	error_e					lastError_;
protected:
}; // class Usart0

extern FILE usartStream;

void inline __attribute__((always_inline)) Usart0::enableReceiver(void)
{
	setBit(UCSR0B, RXEN0);
	return;
}

void inline __attribute__((always_inline)) Usart0::disableReceiver(void)
{
	clrBit(UCSR0B, RXEN0);
	return;
}

void inline __attribute__((always_inline)) Usart0::enableTransmitter(void)
{
	setBit(UCSR0B, TXEN0);
	return;
}

void inline __attribute__((always_inline)) Usart0::disableTransmitter(void)
{
	clrBit(UCSR0B, TXEN0);
	return;
}

void inline __attribute__((always_inline)) Usart0::activateReceptionCompleteInterrupt(void)
{
	setBit(UCSR0B, RXCIE0);
	return;
}

void inline __attribute__((always_inline)) Usart0::deactivateReceptionCompleteInterrupt(void)
{
	clrBit(UCSR0B, RXCIE0);
	return;
}

void inline __attribute__((always_inline)) Usart0::activateTransmissionCompleteInterrupt(void)
{
	setBit(UCSR0B, TXCIE0);
	return;
}

void inline __attribute__((always_inline)) Usart0::deactivateTransmissionCompleteInterrupt(void)
{
	clrBit(UCSR0B, TXCIE0);
	return;
}

void inline __attribute__((always_inline)) Usart0::activateBufferEmptyInterrupt(void)
{
	setBit(UCSR0B, UDRIE0);
	return;
}

void inline __attribute__((always_inline)) Usart0::deactivateBufferEmptyInterrupt(void)
{
	clrBit(UCSR0B, UDRIE0);
	return;
}

bool inline __attribute__((always_inline)) Usart0::isReceptionComplete(void)
{
	return isBitSet(UCSR0A, RXC0);
}

bool inline __attribute__((always_inline)) Usart0::isTransmissionComplete(void)
{
	return isBitSet(UCSR0A, TXC0);
}

bool inline __attribute__((always_inline)) Usart0::isBufferEmpty(void)
{
	return isBitSet(UCSR0A, UDRE0);
}

void inline __attribute__((always_inline)) Usart0::stdio(void)
{
	stdin = stdout = stderr = &usartStream;

	return;
}
