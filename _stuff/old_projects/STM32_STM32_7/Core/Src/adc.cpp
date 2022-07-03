
#include "adc.h"

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc3;

void Error_Handler(uint16_t errorCode_p);

// -----------------------------------------------------------------------------
// ADC1 init function ----------------------------------------------------------

void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	// Common config
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 3;
	if(HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler(ERROR_CODE_1000);
	}

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1001);
	}

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1002);
	}

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1003);
	}
}

// -----------------------------------------------------------------------------
// ADC3 init function ----------------------------------------------------------

void MX_ADC3_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	// Common config
	hadc3.Instance = ADC3;
	hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 3;
	if(HAL_ADC_Init(&hadc3) != HAL_OK) {
		Error_Handler(ERROR_CODE_1004);
	}

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1005);
	}

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1006);
	}

	// Configure Regular Channel
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if(HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1007);
	}
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(adcHandle->Instance == ADC1) {
		// ADC1 clock enable
		__HAL_RCC_ADC1_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();

		// ADC1 GPIO Configuration
		// PC3     ------> ADC1_IN13
		// PC4     ------> ADC1_IN14
		// PC5     ------> ADC1_IN15
		GPIO_InitStruct.Pin = ADC1_ECG1_Pin | ADC1_ECG2_Pin | ADC1_ECG3_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		// ADC1 DMA Init
		hdma_adc1.Instance = DMA1_Channel1;
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc1.Init.Mode = DMA_CIRCULAR;
		hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
		if(HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
			Error_Handler(ERROR_CODE_1008);
		}
		__HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);

		// ADC1 interrupt Init
		HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	} else if(adcHandle->Instance == ADC3) {
		// ADC3 clock enable
		__HAL_RCC_ADC3_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();

		// ADC3 GPIO Configuration
		// PC0     ------> ADC3_IN10
		// PC1     ------> ADC3_IN11
		// PC2     ------> ADC3_IN12
		GPIO_InitStruct.Pin = ADC3_STEP_UP_Pin | ADC3_BAT_Pin | ADC3_CR2032_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		// ADC3 DMA Init
		hdma_adc3.Instance = DMA2_Channel5;
		hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc3.Init.Mode = DMA_CIRCULAR;
		hdma_adc3.Init.Priority = DMA_PRIORITY_LOW;
		if(HAL_DMA_Init(&hdma_adc3) != HAL_OK) {
			Error_Handler(ERROR_CODE_1009);
		}
		__HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc3);

		// ADC3 interrupt Init
		HAL_NVIC_SetPriority(ADC3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC3_IRQn);
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle)
{
	if(adcHandle->Instance == ADC1) {
		// Peripheral clock disable
		__HAL_RCC_ADC1_CLK_DISABLE();

		// ADC1 GPIO Configuration
		// PC3     ------> ADC1_IN13
		// PC4     ------> ADC1_IN14
		// PC5     ------> ADC1_IN15
		HAL_GPIO_DeInit(GPIOC, ADC1_ECG1_Pin | ADC1_ECG2_Pin | ADC1_ECG3_Pin);

		// ADC1 DMA DeInit
		HAL_DMA_DeInit(adcHandle->DMA_Handle);

		// ADC1 interrupt Deinit
		HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
	} else if(adcHandle->Instance == ADC3) {
		// Peripheral clock disable
		__HAL_RCC_ADC3_CLK_DISABLE();

		// ADC3 GPIO Configuration
		// PC0     ------> ADC3_IN10
		// PC1     ------> ADC3_IN11
		// PC2     ------> ADC3_IN12
		HAL_GPIO_DeInit(GPIOC, ADC3_STEP_UP_Pin | ADC3_BAT_Pin | ADC3_CR2032_Pin);

		// ADC3 DMA DeInit
		HAL_DMA_DeInit(adcHandle->DMA_Handle);

		// ADC3 interrupt Deinit
		HAL_NVIC_DisableIRQ(ADC3_IRQn);
	}
}
