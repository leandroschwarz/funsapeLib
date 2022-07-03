
extern volatile unsigned int Timer;

#define	_BV(bit) (1<<(bit))

#define FCLK_SLOW() { SPIx_CR1 = (SPIx_CR1 & ~0x38) | (5 << 3); }	// Set SCLK = PCLK / 64
#define FCLK_FAST() { SPIx_CR1 = (SPIx_CR1 & ~0x38) | (4 << 3); }	// Set SCLK = PCLK / 2
#define CS_HIGH()	GPIOB->BSRR = _BV(12)
#define CS_LOW()	GPIOB->BSRR = _BV(12+16)
// #define	MMC_CD		!(GPIOD_IDR & _BV(8))	// Card detect (yes:true, no:false, default:true)
#define	MMC_CD		1
#define	MMC_WP		0							// Write protected (yes:true, no:false, default:false)
#define SPIx_CR1	spiMmc.Instance->CR1
#define SPIx_SR		spiMmc.Instance->SR
#define SPIx_DR		spiMmc.Instance->DR

// PB12: MMC_CS
// PB13: MMC_SCLK
// PB14: MMC_DO with pull-up
// PB15: MMC_DI
// PD8: MMC_CD with pull-up
// Enable SPI1
#define	SPIxENABLE() {													\
		__enable_peripheral(SPI2EN);									\
		__enable_peripheral(IOPBEN);									\
		__enable_peripheral(IOPDEN); 									\
		__gpio_conf_bit(GPIOB, 12, OUT_PP);								\
		__gpio_conf_bit(GPIOB, 13, ALT_PP);								\
		GPIOB_BSRR = _BV(14); __gpio_conf_bit(GPIOB, 14, IN_PUL);		\
		__gpio_conf_bit(GPIOB, 15, ALT_PP);								\
		GPIOD_BSRR = _BV(8); __gpio_conf_bit(GPIOD, 8, IN_PUL); 		\
		SPIx_CR1 = _BV(9) | _BV(8) | _BV(6) | _BV(2);					\
	}

//------------------------------------------------------------------------
// Module Private Functions
//--------------------------------------------------------------------------

//#include "STM32F100.h"
#include "funsapeLibGlobalDefines.hpp"
#include "diskio.h"
#include "spi.h"

//-----------------------------------------------------------------------
// SPI controls (Platform dependent)
//-----------------------------------------------------------------------


SPI_HandleTypeDef spiMmc = {0};
HAL_StatusTypeDef spiStatus = HAL_OK;

#define									SPI_BAUD_RATE_LOW_SPEED			SPI_BAUDRATEPRESCALER_64
#define									SPI_BAUD_RATE_HIGH_SPEED		SPI_BAUDRATEPRESCALER_32

// Initialize MMC interface
void init_spi(void)
{
	spiMmc.Instance						= SPI2;
	spiMmc.Init.Mode					= SPI_MODE_MASTER;
	spiMmc.Init.Direction				= SPI_DIRECTION_2LINES;
	spiMmc.Init.DataSize				= SPI_DATASIZE_8BIT;
	spiMmc.Init.CLKPolarity				= SPI_POLARITY_LOW;
	spiMmc.Init.CLKPhase				= SPI_PHASE_1EDGE;
	spiMmc.Init.NSS						= SPI_NSS_SOFT;
	spiMmc.Init.BaudRatePrescaler		= SPI_BAUD_RATE_LOW_SPEED;
	spiMmc.Init.FirstBit				= SPI_FIRSTBIT_MSB;
	spiMmc.Init.TIMode					= SPI_TIMODE_DISABLE;
	spiMmc.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLE;
	spiMmc.Init.CRCPolynomial			= 10;
	spiMmc.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLE;
	spiMmc.Lock							= HAL_UNLOCKED;
	spiMmc.State						= HAL_SPI_STATE_BUSY;

	clrBit(spiMmc.Instance->CR1, SPI_CR1_SPE_Pos);
	spiMmc.Instance->CR1 = (
					spiMmc.Init.Mode | spiMmc.Init.Direction | spiMmc.Init.DataSize |
					spiMmc.Init.CLKPolarity | spiMmc.Init.CLKPhase | (spiMmc.Init.NSS & SPI_CR1_SSM) |
					spiMmc.Init.BaudRatePrescaler | spiMmc.Init.FirstBit | spiMmc.Init.CRCCalculation
			);
	spiMmc.Instance->CR2 = ((spiMmc.Init.NSS >> 16U) & SPI_CR2_SSOE);
#if defined(SPI_I2SCFGR_I2SMOD)
	clrBit(spiMmc.Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD_Pos);
#endif
	spiMmc.ErrorCode					= HAL_SPI_ERROR_NONE;
	spiMmc.State						= HAL_SPI_STATE_READY;
	__HAL_RCC_SPI2_CLK_ENABLE();
	setBit(spiMmc.Instance->CR1, SPI_CR1_SPE_Pos);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	// SPI2 GPIO Configuration
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// PB13     ------> SPI2_SCK
	// PB15     ------> SPI2_MOSI

	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// PB14     ------> SPI2_MISO

	HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);
	CS_HIGH();
	HAL_Delay(10);

	return;
}

// Exchange a byte
BYTE xchg_spi(BYTE dat)
{
	SPIx_DR = dat;						// Start an SPI transaction
	while((SPIx_SR & 0x83) != 0x03) ;	// Wait for end of the transaction
	return (BYTE)SPIx_DR;				// Return received byte
}

// Receive multiple byte
// Number of bytes to receive (even number)
void rcvr_spi_multi(BYTE *buff, UINT btr)
{
	WORD d;

	SPIx_CR1 &= ~_BV(6);
	SPIx_CR1 |= (_BV(6) | _BV(11));	// Put SPI into 16-bit mode

	SPIx_DR = 0xFFFF;		// Start the first SPI transaction
	btr -= 2;
	do {					// Receive the data block into buffer
		while((SPIx_SR & 0x83) != 0x03) ;	// Wait for end of the SPI transaction
		d = SPIx_DR;						// Get received word
		SPIx_DR = 0xFFFF;					// Start next transaction
		buff[1] = d; buff[0] = d >> 8; 		// Store received data
		buff += 2;
	} while(btr -= 2);
	while((SPIx_SR & 0x83) != 0x03) ;		// Wait for end of the SPI transaction
	d = SPIx_DR;							// Get last word received
	buff[1] = d; buff[0] = d >> 8;			// Store it

	SPIx_CR1 &= ~(_BV(6) | _BV(11));	// Put SPI into 8-bit mode
	SPIx_CR1 |= _BV(6);
}

#if FF_FS_READONLY == 0
// Send multiple byte
// Number of bytes to send (even number)
void xmit_spi_multi(const BYTE *buff, UINT btx)
{
	WORD d;

	SPIx_CR1 &= ~_BV(6);
	SPIx_CR1 |= (_BV(6) | _BV(11));		// Put SPI into 16-bit mode

	d = buff[0] << 8 | buff[1]; buff += 2;
	SPIx_DR = d;	// Send the first word
	btx -= 2;
	do {
		d = buff[0] << 8 | buff[1]; buff += 2;	// Word to send next
		while((SPIx_SR & 0x83) != 0x03) ;	// Wait for end of the SPI transaction
		SPIx_DR;							// Discard received word
		SPIx_DR = d;						// Start next transaction
	} while(btx -= 2);
	while((SPIx_SR & 0x83) != 0x03) ;	// Wait for end of the SPI transaction
	SPIx_DR;							// Discard received word

	SPIx_CR1 &= ~(_BV(6) | _BV(11));	// Put SPI into 8-bit mode
	SPIx_CR1 |= _BV(6);
}
#endif
