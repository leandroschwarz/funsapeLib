
#include "dma.h"

void Error_Handler(uint16_t errorCode_p);

void MX_DMA_Init(void)
{
	// DMA controller clock enable
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	// DMA1_Channel1_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	// DMA1_Channel3_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	// DMA1_Channel6_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	// DMA1_Channel7_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
	// DMA2_Channel4_5_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
}
