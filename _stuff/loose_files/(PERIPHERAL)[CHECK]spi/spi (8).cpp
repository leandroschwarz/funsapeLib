
#include "spi.h"

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

void Error_Handler(uint16_t errorCode_p);

// -----------------------------------------------------------------------------
// SPI1 init function ----------------------------------------------------------

void MX_SPI1_Init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if(HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler(ERROR_CODE_1013);
	}
}

// -----------------------------------------------------------------------------
// SPI2 init function ----------------------------------------------------------

void MX_SPI2_Init(void)
{
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if(HAL_SPI_Init(&hspi2) != HAL_OK) {
		Error_Handler(ERROR_CODE_1014);
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(spiHandle->Instance == SPI1) {
		// SPI1 clock enable
		__HAL_RCC_SPI1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		// SPI1 GPIO Configuration
		// PA5     ------> SPI1_SCK
		// PA6     ------> SPI1_MISO
		// PA7     ------> SPI1_MOSI
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// SPI1 interrupt Init
		HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
	} else if(spiHandle->Instance == SPI2) {
		// SPI2 clock enable
		__HAL_RCC_SPI2_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		// SPI2 GPIO Configuration
		// PB13     ------> SPI2_SCK
		// PB14     ------> SPI2_MISO
		// PB15     ------> SPI2_MOSI
		GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		// SPI2 interrupt Init
		HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI2_IRQn);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{
	if(spiHandle->Instance == SPI1) {
		// Peripheral clock disable
		__HAL_RCC_SPI1_CLK_DISABLE();

		// SPI1 GPIO Configuration
		// PA5     ------> SPI1_SCK
		// PA6     ------> SPI1_MISO
		// PA7     ------> SPI1_MOSI
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

		// SPI1 interrupt Deinit
		HAL_NVIC_DisableIRQ(SPI1_IRQn);
	} else if(spiHandle->Instance == SPI2) {
		// Peripheral clock disable
		__HAL_RCC_SPI2_CLK_DISABLE();

		// SPI2 GPIO Configuration
		// PB13     ------> SPI2_SCK
		// PB14     ------> SPI2_MISO
		// PB15     ------> SPI2_MOSI
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

		// SPI2 interrupt Deinit
		HAL_NVIC_DisableIRQ(SPI2_IRQn);
	}
}
