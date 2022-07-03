


// -----------------------------------------------------------------------------
// File exclusive - Header files -----------------------------------------------

#include "spiBitBang.hpp"
#if __SPI_BIT_BANG_HPP != 1000
	#error [spiBitBang.cpp] Error 101 - Build mismatch on header and source code files.
#endif

// -----------------------------------------------------------------------------
// File exclusive - Constant definitions ---------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - New data types ---------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - Static functions declarations ------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - Static functions definitions -------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Class SpiBitBangDevice - public functions definitions -----------------------

SpiBitBangDevice::SpiBitBangDevice(reg_t * ssDir, uint8_t ssBit)
{
	this->ssBit_ = ssBit;
	this->ssDir_ = ssDir;
	this->ssOut_ = ssDir + 1;
	setBit(*(this->ssDir_), this->ssBit_);
	setBit(*(this->ssOut_), this->ssBit_);
	printf("1 - 0x%x, 0x%x, %d\r", (uint16_t)(this->ssDir_), (uint16_t)(this->ssOut_), this->ssBit_);
}

void SpiBitBangDevice::defineDriver(SpiBitBang * driver)
{
	this->spiDriver_ = driver;
}

void SpiBitBangDevice::sendByte(uint8_t dataByte)
{
	this->send(&dataByte, 1);
}

void SpiBitBangDevice::sendWord(uint16_t dataWord)
{
	dataWord = wordSwitchBytes(dataWord);
	this->send(&dataWord, 2);
}

void SpiBitBangDevice::sendBuffer(uint8_t * bufferData, uint8_t bufferSize)
{
	this->send(bufferData, bufferSize);
}

// -----------------------------------------------------------------------------
// Class SpiBitBangDevice - private functions definitions ----------------------

/* NONE */

// -----------------------------------------------------------------------------
// Class SpiBitBang - public functions definitions -----------------------------

SpiBitBang::SpiBitBang()
{
	this->sclkDir_ = NULL;
	this->sclkOut_ = NULL;
	this->sclkBit_ = 255;
	this->misoDir_ = NULL;
	this->misoIn_ = NULL;
	this->misoOut_ = NULL;
	this->misoBit_ = 255;
	this->mosiDir_ = NULL;
	this->mosiOut_ = NULL;
	this->mosiBit_ = 255;
	this->turnedOn_ = FALSE;
	this->initialized_ = FALSE;
}

void SpiBitBang::initialize(reg_t * misoDdr, uint8_t misoBit, reg_t * mosiDdr, uint8_t mosiBit, reg_t * sclkDdr, uint8_t sclkBit)
{
	this->misoDir_ = misoDdr;
	this->misoIn_ = misoDdr - 1;
	this->misoOut_ = misoDdr + 1;
	this->misoBit_ = misoBit;
	this->mosiDir_ = mosiDdr;
	this->mosiOut_ = mosiDdr + 1;
	this->mosiBit_ = mosiBit;
	this->sclkDir_ = sclkDdr;
	this->sclkOut_ = sclkDdr + 1;
	this->sclkBit_ = sclkBit;
	this->initialized_ = TRUE;

	this->turnOnGpio();
}

void SpiBitBang::initialize(reg_t * spiDdr, uint8_t misoBit, uint8_t mosiBit, uint8_t sclkBit)
{
	this->initialize(spiDdr, misoBit, spiDdr, mosiBit, spiDdr, sclkBit);
}

// -----------------------------------------------------------------------------
// Class SpiBitBang - private functions definitions ----------------------------

void SpiBitBang::turnOnGpio(void)
{
	// Grab SPI GPIO
	setBit(*(this->sclkDir_), this->sclkBit_);		// SCLK as OUTPUT
	clrBit(*(this->sclkOut_), this->sclkBit_);		// SCLK as LOW
	clrBit(*(this->misoDir_), this->misoBit_);		// MISO as INPUT
	clrBit(*(this->misoOut_), this->misoBit_);		// MISO as HiZ
	setBit(*(this->mosiDir_), this->mosiBit_);		// MOSI as OUTPUT
	clrBit(*(this->mosiOut_), this->mosiBit_);		// MOSI as LOW

	this->turnedOn_ = TRUE;
}

void SpiBitBang::turnOffGpio(void)
{
	// Release SPI GPIO
	clrBit(*(this->sclkDir_), this->sclkBit_);		// SCLK as INPUT
	clrBit(*(this->sclkOut_), this->sclkBit_);		// SCLK as LOW
	clrBit(*(this->misoDir_), this->misoBit_);		// MISO as INPUT
	clrBit(*(this->misoOut_), this->misoBit_);		// MISO as HiZ
	clrBit(*(this->mosiDir_), this->mosiBit_);		// MOSI as INPUT
	clrBit(*(this->mosiOut_), this->mosiBit_);		// MOSI as HiZ}
	this->turnedOn_ = FALSE;
}

void SpiBitBangDevice::send(void * dataBuff, uint8_t bytes)
{
	uint8_t * typeCastedData = (uint8_t *)dataBuff;
	// Select device
	clrBit(*(this->ssOut_), this->ssBit_);

	for(uint8_t j = 0; j < bytes; j++) {
		for(uint8_t i = 0; i < 8; i++) {
			if(isBitSet(*(typeCastedData + j), (7 - i))) {
				setBit(*(this->spiDriver_->mosiOut_), this->spiDriver_->mosiBit_);
			} else {
				clrBit(*(this->spiDriver_->mosiOut_), this->spiDriver_->mosiBit_);
			}
			setBit(*(this->spiDriver_->sclkOut_), this->spiDriver_->sclkBit_);
			clrBit(*(this->spiDriver_->sclkOut_), this->spiDriver_->sclkBit_);
		}
	}

	// SS high
	setBit(*(this->ssOut_), this->ssBit_);
}
