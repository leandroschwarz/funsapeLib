
// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1000
	#error [spiBitBang.hpp] Error 100 - Build mismatch (globalDefines must be build 1000).
#endif

#ifndef __SPI_BIT_BANG_HPP
#define __SPI_BIT_BANG_HPP 1000

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Extern variable declarations ------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

class SpiBitBangDevice;

class SpiBitBang
{
	friend class SpiBitBangDevice;
public:
	SpiBitBang();
	void initialize(reg_t * spiDdr, uint8_t misoBit, uint8_t mosiBit, uint8_t sclkBit);
	void initialize(reg_t * misoDdr, uint8_t misoBit, reg_t * mosiDdr, uint8_t mosiBit, reg_t * sclkDdr, uint8_t sclkBit);

private:

	void turnOnGpio(void);
	void turnOffGpio(void);

	reg_t				* sclkDir_;
	reg_t				* sclkOut_;
	uint8_t				sclkBit_;
	reg_t				* misoDir_;
	reg_t				* misoIn_;
	reg_t				* misoOut_;
	uint8_t				misoBit_;
	reg_t				* mosiDir_;
	reg_t				* mosiOut_;
	uint8_t				mosiBit_;
	bool_t				initialized_	: 1;
	bool_t				turnedOn_		: 1;
	uint8_t				unusedFlags_	: 6;

protected:

};


class SpiBitBangDevice
{

public:
	SpiBitBangDevice(reg_t * ssDir, uint8_t ssBit);
	void defineDriver(SpiBitBang * driver);
	void sendByte(uint8_t dataByte);
	void sendBuffer(uint8_t * bufferData, uint8_t bufferSize);
	void sendWord(uint16_t dataWord);

private:
	reg_t		* ssDir_;
	reg_t		* ssOut_;
	uint8_t		ssBit_;
	SpiBitBang	* spiDriver_;

	void send(void * dataBuff, uint8_t bytes);
protected:
};




#endif /* SPIBITBANG_H_ */