

#include "peripheral.hpp"
#include "mainDefs.h"

ADC_HandleTypeDef hadc;
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_adc;
DMA_HandleTypeDef hdma_i2c2_rx;
DMA_HandleTypeDef hdma_i2c2_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
I2C_HandleTypeDef hi2c2;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim21;
TIM_HandleTypeDef htim22;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */
uint8_t retUSER;    /* Return value for USER */

/* ADC init function */
void MX_ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	hadc.Instance = ADC1;
	hadc.Init.OversamplingMode = DISABLE;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerFrequencyMode = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	if (HAL_ADC_Init(&hadc) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_1;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_8;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_10;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_11;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_12;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel to be converted.
	*/
	sConfig.Channel = ADC_CHANNEL_13;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (adcHandle->Instance == ADC1) {
		/* USER CODE BEGIN ADC1_MspInit 0 */

		/* USER CODE END ADC1_MspInit 0 */
		/* ADC1 clock enable */
		__HAL_RCC_ADC1_CLK_ENABLE();

		/**ADC GPIO Configuration
		PC0     ------> ADC_IN10
		PC1     ------> ADC_IN11
		PC2     ------> ADC_IN12
		PC3     ------> ADC_IN13
		PA0     ------> ADC_IN0
		PA1     ------> ADC_IN1
		PB0     ------> ADC_IN8
		*/
		GPIO_InitStruct.Pin = ADC3_Pin | ADC4_Pin | ADC5_Pin | ADC6_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = ADC0_Pin | ADC1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = ADC2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC2_GPIO_Port, &GPIO_InitStruct);

		/* ADC1 DMA Init */
		/* ADC Init */
		hdma_adc.Instance = DMA1_Channel1;
		hdma_adc.Init.Request = DMA_REQUEST_0;
		hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc.Init.Mode = DMA_CIRCULAR;
		hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_adc) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc);

		/* ADC1 interrupt Init */
		HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
		/* USER CODE BEGIN ADC1_MspInit 1 */

		/* USER CODE END ADC1_MspInit 1 */
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

	if (adcHandle->Instance == ADC1) {
		/* USER CODE BEGIN ADC1_MspDeInit 0 */

		/* USER CODE END ADC1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ADC1_CLK_DISABLE();

		/**ADC GPIO Configuration
		PC0     ------> ADC_IN10
		PC1     ------> ADC_IN11
		PC2     ------> ADC_IN12
		PC3     ------> ADC_IN13
		PA0     ------> ADC_IN0
		PA1     ------> ADC_IN1
		PB0     ------> ADC_IN8
		*/
		HAL_GPIO_DeInit(GPIOC, ADC3_Pin | ADC4_Pin | ADC5_Pin | ADC6_Pin);

		HAL_GPIO_DeInit(GPIOA, ADC0_Pin | ADC1_Pin);

		HAL_GPIO_DeInit(ADC2_GPIO_Port, ADC2_Pin);

		/* ADC1 DMA DeInit */
		HAL_DMA_DeInit(adcHandle->DMA_Handle);

		/* ADC1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
		/* USER CODE BEGIN ADC1_MspDeInit 1 */

		/* USER CODE END ADC1_MspDeInit 1 */
	}
}

/* DAC init function */
void MX_DAC_Init(void)
{
	DAC_ChannelConfTypeDef sConfig;

	/**DAC Initialization
	*/
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**DAC channel OUT1 config
	*/
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (dacHandle->Instance == DAC) {
		/* USER CODE BEGIN DAC_MspInit 0 */

		/* USER CODE END DAC_MspInit 0 */
		/* DAC clock enable */
		__HAL_RCC_DAC_CLK_ENABLE();

		/**DAC GPIO Configuration
		PA4     ------> DAC_OUT1
		*/
		GPIO_InitStruct.Pin = DAC_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(DAC_GPIO_Port, &GPIO_InitStruct);

		/* DAC interrupt Init */
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
		/* USER CODE BEGIN DAC_MspInit 1 */

		/* USER CODE END DAC_MspInit 1 */
	}
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

	if (dacHandle->Instance == DAC) {
		/* USER CODE BEGIN DAC_MspDeInit 0 */

		/* USER CODE END DAC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_DAC_CLK_DISABLE();

		/**DAC GPIO Configuration
		PA4     ------> DAC_OUT1
		*/
		HAL_GPIO_DeInit(DAC_GPIO_Port, DAC_Pin);

		/* DAC interrupt Deinit */
		/* USER CODE BEGIN DAC:TIM6_DAC_IRQn disable */
		/**
		* Uncomment the line below to disable the "TIM6_DAC_IRQn" interrupt
		* Be aware, disabling shared interrupt may affect other IPs
		*/
		/* HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn); */
		/* USER CODE END DAC:TIM6_DAC_IRQn disable */

		/* USER CODE BEGIN DAC_MspDeInit 1 */

		/* USER CODE END DAC_MspDeInit 1 */
	}
}

void MX_DMA_Init(void)
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel2_3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	/* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

}

void MX_FATFS_Init(void)
{
	/*## FatFS: Link the USER driver ###########################*/
	retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

	/* USER CODE BEGIN Init */
	/* additional user code for init */
	/* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
	/* USER CODE BEGIN get_fattime */
	return 0;
	/* USER CODE END get_fattime */
}

void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, SPI1_SS5_Pin | SPI1_SS3_Pin | SPI1_SS6_Pin | SPI1_SS1_Pin
					  | SPI1_SS0_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, SPI1_SS2_Pin | SPI1_SS4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = BUTTON_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : PC4 PC10 PC11 PC12 */
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PCPin PCPin PCPin PCPin
	                         PCPin */
	GPIO_InitStruct.Pin = SPI1_SS5_Pin | SPI1_SS3_Pin | SPI1_SS6_Pin | SPI1_SS1_Pin
						  | SPI1_SS0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PB1 PB2 PB12 PB13
	                         PB14 PB15 PB4 PB5
	                         PB6 PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_12 | GPIO_PIN_13
						  | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_4 | GPIO_PIN_5
						  | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PA8 PA11 PA12 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PD2 */
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PBPin PBPin */
	GPIO_InitStruct.Pin = SPI1_SS2_Pin | SPI1_SS4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* I2C2 init function */
void MX_I2C2_Init(void)
{

	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00707CBB;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (i2cHandle->Instance == I2C2) {
		/* USER CODE BEGIN I2C2_MspInit 0 */

		/* USER CODE END I2C2_MspInit 0 */

		/**I2C2 GPIO Configuration
		PB10     ------> I2C2_SCL
		PB11     ------> I2C2_SDA
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* I2C2 clock enable */
		__HAL_RCC_I2C2_CLK_ENABLE();

		/* I2C2 DMA Init */
		/* I2C2_RX Init */
		hdma_i2c2_rx.Instance = DMA1_Channel5;
		hdma_i2c2_rx.Init.Request = DMA_REQUEST_7;
		hdma_i2c2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_i2c2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_i2c2_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_i2c2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_i2c2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_i2c2_rx.Init.Mode = DMA_CIRCULAR;
		hdma_i2c2_rx.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_i2c2_rx) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(i2cHandle, hdmarx, hdma_i2c2_rx);

		/* I2C2_TX Init */
		hdma_i2c2_tx.Instance = DMA1_Channel4;
		hdma_i2c2_tx.Init.Request = DMA_REQUEST_7;
		hdma_i2c2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_i2c2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_i2c2_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_i2c2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_i2c2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_i2c2_tx.Init.Mode = DMA_CIRCULAR;
		hdma_i2c2_tx.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_i2c2_tx) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(i2cHandle, hdmatx, hdma_i2c2_tx);

		/* I2C2 interrupt Init */
		HAL_NVIC_SetPriority(I2C2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(I2C2_IRQn);
		/* USER CODE BEGIN I2C2_MspInit 1 */

		/* USER CODE END I2C2_MspInit 1 */
	}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

	if (i2cHandle->Instance == I2C2) {
		/* USER CODE BEGIN I2C2_MspDeInit 0 */

		/* USER CODE END I2C2_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_I2C2_CLK_DISABLE();

		/**I2C2 GPIO Configuration
		PB10     ------> I2C2_SCL
		PB11     ------> I2C2_SDA
		*/
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);

		/* I2C2 DMA DeInit */
		HAL_DMA_DeInit(i2cHandle->hdmarx);
		HAL_DMA_DeInit(i2cHandle->hdmatx);

		/* I2C2 interrupt Deinit */
		HAL_NVIC_DisableIRQ(I2C2_IRQn);
		/* USER CODE BEGIN I2C2_MspDeInit 1 */

		/* USER CODE END I2C2_MspDeInit 1 */
	}
}

/* RTC init function */
void MX_RTC_Init(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	RTC_AlarmTypeDef sAlarm;

	/**Initialize RTC Only
	*/
	hrtc.Instance = RTC;
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2) {
		hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
		hrtc.Init.AsynchPrediv = 37;
		hrtc.Init.SynchPrediv = 1000;
		hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
		hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
		hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
		hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
		if (HAL_RTC_Init(&hrtc) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		/**Initialize RTC and set the Time and Date
		*/
		sTime.Hours = 0;
		sTime.Minutes = 0;
		sTime.Seconds = 0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		sDate.WeekDay = RTC_WEEKDAY_MONDAY;
		sDate.Month = RTC_MONTH_JANUARY;
		sDate.Date = 1;
		sDate.Year = 0;

		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		/**Enable the Alarm A
		*/
		sAlarm.AlarmTime.Hours = 0;
		sAlarm.AlarmTime.Minutes = 0;
		sAlarm.AlarmTime.Seconds = 0;
		sAlarm.AlarmTime.SubSeconds = 0;
		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
		sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
		sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
		sAlarm.AlarmDateWeekDay = 1;
		sAlarm.Alarm = RTC_ALARM_A;
		if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x32F2);
	}

}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle)
{

	if (rtcHandle->Instance == RTC) {
		/* USER CODE BEGIN RTC_MspInit 0 */

		/* USER CODE END RTC_MspInit 0 */
		/* RTC clock enable */
		__HAL_RCC_RTC_ENABLE();

		/* RTC interrupt Init */
		HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(RTC_IRQn);
		/* USER CODE BEGIN RTC_MspInit 1 */

		/* USER CODE END RTC_MspInit 1 */
	}
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle)
{

	if (rtcHandle->Instance == RTC) {
		/* USER CODE BEGIN RTC_MspDeInit 0 */

		/* USER CODE END RTC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();

		/* RTC interrupt Deinit */
		HAL_NVIC_DisableIRQ(RTC_IRQn);
		/* USER CODE BEGIN RTC_MspDeInit 1 */

		/* USER CODE END RTC_MspDeInit 1 */
	}
}

/* SPI1 init function */
void MX_SPI1_Init(void)
{

	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (spiHandle->Instance == SPI1) {
		/* USER CODE BEGIN SPI1_MspInit 0 */

		/* USER CODE END SPI1_MspInit 0 */
		/* SPI1 clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		/**SPI1 GPIO Configuration
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PB3     ------> SPI1_SCK
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* SPI1 DMA Init */
		/* SPI1_RX Init */
		hdma_spi1_rx.Instance = DMA1_Channel2;
		hdma_spi1_rx.Init.Request = DMA_REQUEST_1;
		hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_spi1_rx.Init.Mode = DMA_CIRCULAR;
		hdma_spi1_rx.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(spiHandle, hdmarx, hdma_spi1_rx);

		/* SPI1_TX Init */
		hdma_spi1_tx.Instance = DMA1_Channel3;
		hdma_spi1_tx.Init.Request = DMA_REQUEST_1;
		hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_spi1_tx.Init.Mode = DMA_CIRCULAR;
		hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(spiHandle, hdmatx, hdma_spi1_tx);

		/* SPI1 interrupt Init */
		HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
		/* USER CODE BEGIN SPI1_MspInit 1 */

		/* USER CODE END SPI1_MspInit 1 */
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

	if (spiHandle->Instance == SPI1) {
		/* USER CODE BEGIN SPI1_MspDeInit 0 */

		/* USER CODE END SPI1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_SPI1_CLK_DISABLE();

		/**SPI1 GPIO Configuration
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PB3     ------> SPI1_SCK
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_7);

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

		/* SPI1 DMA DeInit */
		HAL_DMA_DeInit(spiHandle->hdmarx);
		HAL_DMA_DeInit(spiHandle->hdmatx);

		/* SPI1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(SPI1_IRQn);
		/* USER CODE BEGIN SPI1_MspDeInit 1 */

		/* USER CODE END SPI1_MspDeInit 1 */
	}
}

/* TIM2 init function */
void MX_TIM2_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}
/* TIM6 init function */
void MX_TIM6_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 0;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 0;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}
/* TIM21 init function */
void MX_TIM21_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim21.Instance = TIM21;
	htim21.Init.Prescaler = 0;
	htim21.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim21.Init.Period = 0;
	htim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim21) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim21, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim21, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}
/* TIM22 init function */
void MX_TIM22_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim22.Instance = TIM22;
	htim22.Init.Prescaler = 0;
	htim22.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim22.Init.Period = 0;
	htim22.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim22) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim22, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim22, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if (tim_baseHandle->Instance == TIM2) {
		/* USER CODE BEGIN TIM2_MspInit 0 */

		/* USER CODE END TIM2_MspInit 0 */
		/* TIM2 clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
		/* USER CODE BEGIN TIM2_MspInit 1 */

		/* USER CODE END TIM2_MspInit 1 */
	} else if (tim_baseHandle->Instance == TIM6) {
		/* USER CODE BEGIN TIM6_MspInit 0 */

		/* USER CODE END TIM6_MspInit 0 */
		/* TIM6 clock enable */
		__HAL_RCC_TIM6_CLK_ENABLE();

		/* TIM6 interrupt Init */
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
		/* USER CODE BEGIN TIM6_MspInit 1 */

		/* USER CODE END TIM6_MspInit 1 */
	} else if (tim_baseHandle->Instance == TIM21) {
		/* USER CODE BEGIN TIM21_MspInit 0 */

		/* USER CODE END TIM21_MspInit 0 */
		/* TIM21 clock enable */
		__HAL_RCC_TIM21_CLK_ENABLE();
		/* USER CODE BEGIN TIM21_MspInit 1 */

		/* USER CODE END TIM21_MspInit 1 */
	} else if (tim_baseHandle->Instance == TIM22) {
		/* USER CODE BEGIN TIM22_MspInit 0 */

		/* USER CODE END TIM22_MspInit 0 */
		/* TIM22 clock enable */
		__HAL_RCC_TIM22_CLK_ENABLE();
		/* USER CODE BEGIN TIM22_MspInit 1 */

		/* USER CODE END TIM22_MspInit 1 */
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if (tim_baseHandle->Instance == TIM2) {
		/* USER CODE BEGIN TIM2_MspDeInit 0 */

		/* USER CODE END TIM2_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();
		/* USER CODE BEGIN TIM2_MspDeInit 1 */

		/* USER CODE END TIM2_MspDeInit 1 */
	} else if (tim_baseHandle->Instance == TIM6) {
		/* USER CODE BEGIN TIM6_MspDeInit 0 */

		/* USER CODE END TIM6_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM6_CLK_DISABLE();

		/* TIM6 interrupt Deinit */
		/* USER CODE BEGIN TIM6:TIM6_DAC_IRQn disable */
		/**
		* Uncomment the line below to disable the "TIM6_DAC_IRQn" interrupt
		* Be aware, disabling shared interrupt may affect other IPs
		*/
		/* HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn); */
		/* USER CODE END TIM6:TIM6_DAC_IRQn disable */

		/* USER CODE BEGIN TIM6_MspDeInit 1 */

		/* USER CODE END TIM6_MspDeInit 1 */
	} else if (tim_baseHandle->Instance == TIM21) {
		/* USER CODE BEGIN TIM21_MspDeInit 0 */

		/* USER CODE END TIM21_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM21_CLK_DISABLE();
		/* USER CODE BEGIN TIM21_MspDeInit 1 */

		/* USER CODE END TIM21_MspDeInit 1 */
	} else if (tim_baseHandle->Instance == TIM22) {
		/* USER CODE BEGIN TIM22_MspDeInit 0 */

		/* USER CODE END TIM22_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM22_CLK_DISABLE();
		/* USER CODE BEGIN TIM22_MspDeInit 1 */

		/* USER CODE END TIM22_MspDeInit 1 */
	}
}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	} else if (uartHandle->Instance == USART2) {
		/* USER CODE BEGIN USART2_MspInit 0 */

		/* USER CODE END USART2_MspInit 0 */
		/* USART2 clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();

		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = USART_TX_Pin | USART_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART2 DMA Init */
		/* USART2_RX Init */
		hdma_usart2_rx.Instance = DMA1_Channel6;
		hdma_usart2_rx.Init.Request = DMA_REQUEST_4;
		hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
		hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(uartHandle, hdmarx, hdma_usart2_rx);

		/* USART2_TX Init */
		hdma_usart2_tx.Instance = DMA1_Channel7;
		hdma_usart2_tx.Init.Request = DMA_REQUEST_4;
		hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart2_tx.Init.Mode = DMA_CIRCULAR;
		hdma_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		__HAL_LINKDMA(uartHandle, hdmatx, hdma_usart2_tx);

		/* USART2 interrupt Init */
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		/* USER CODE BEGIN USART2_MspInit 1 */

		/* USER CODE END USART2_MspInit 1 */
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

	if (uartHandle->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	} else if (uartHandle->Instance == USART2) {
		/* USER CODE BEGIN USART2_MspDeInit 0 */

		/* USER CODE END USART2_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		HAL_GPIO_DeInit(GPIOA, USART_TX_Pin | USART_RX_Pin);

		/* USART2 DMA DeInit */
		HAL_DMA_DeInit(uartHandle->hdmarx);
		HAL_DMA_DeInit(uartHandle->hdmatx);

		/* USART2 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART2_IRQn);
		/* USER CODE BEGIN USART2_MspDeInit 1 */

		/* USER CODE END USART2_MspDeInit 1 */
	}
}
