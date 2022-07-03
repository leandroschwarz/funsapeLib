/* -----------------------------------------------------------------------------
 * File:			function.c
 * Author:			Leandro Schwarz
 * Last edition:	May 1, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include "main.h"
#include "stm32l0xx_hal.h"

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

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
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim21;
TIM_HandleTypeDef htim22;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

// -----------------------------------------------------------------------------
// Public functions definitions ------------------------------------------------

/* -----------------------------------------------------------------------------
 * weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year)
 * Returns the day of the week
 * --------------------------------------------------------------------------- */
weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year)
{
	weekDay_t weekDay;
	year -= month < 3;
	weekDay = (weekDay_t)((year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + monthDay) % 7);
	weekDay++;

	return weekDay;
}

/* -----------------------------------------------------------------------------
 * void SystemClock_Config(void)
 * Configures system clocks
 * --------------------------------------------------------------------------- */
void SystemClock_Config(void)
{
	// Local variables
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	// Configures the main internal regulator output voltage
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Initializes the CPU, AHB and APB busses clocks
	RCC_OscInitStruct.OscillatorType = (	RCC_OSCILLATORTYPE_HSI	|
											RCC_OSCILLATORTYPE_LSI	);
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_6;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Initializes the CPU, AHB and APB busses clocks
	RCC_ClkInitStruct.ClockType	= ( RCC_CLOCKTYPE_HCLK		|
									RCC_CLOCKTYPE_SYSCLK	|
									RCC_CLOCKTYPE_PCLK1		|
									RCC_CLOCKTYPE_PCLK2		);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Initializes the RCC extended peripherals clocks
	PeriphClkInit.PeriphClockSelection = (	RCC_PERIPHCLK_USART1	|
											RCC_PERIPHCLK_USART2	|
											RCC_PERIPHCLK_RTC		);
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the Systick interrupt time
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	// Configures the Systick
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_ADC_Init(void)
 * Initializes the ADC module
 * --------------------------------------------------------------------------- */
void MX_ADC_Init(void)
{
	// Local variables
	ADC_ChannelConfTypeDef sConfig;

	// Configures the global features of the ADC
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

	// Configures the ADC channels to be converted
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_1;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_8;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_10;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_11;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_12;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sConfig.Channel = ADC_CHANNEL_13;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_DAC_Init(void)
 * Initializes the DAC module
 * --------------------------------------------------------------------------- */
void MX_DAC_Init(void)
{
	// Local variables
	DAC_ChannelConfTypeDef sConfig;

	// DAC initialization
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures DAC output channel
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_I2C2_Init(void)
 * Initializes the I2C2 module
 * --------------------------------------------------------------------------- */
void MX_I2C2_Init(void)
{
	// Configures the global features of the I2C2
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

	// Configures the analogue filter
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the digital filter
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_SPI1_Init(void)
 * Initializes the SPI1 module
 * --------------------------------------------------------------------------- */
void MX_SPI1_Init(void)
{
	// Configures the global features of the SPI1
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

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_TIM2_Init(void)
 * Initializes the TIM2 module
 * --------------------------------------------------------------------------- */
void MX_TIM2_Init(void)
{
	// Local variables
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	// Configures the global features of the TIM2
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the clock source of the TIM2
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the TIM2 master/slave mode
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_TIM6_Init(void)
 * Initializes the TIM6 module
 * --------------------------------------------------------------------------- */
void MX_TIM6_Init(void)
{
	// Local variables
	TIM_MasterConfigTypeDef sMasterConfig;

	// Configures the global features of the TIM6
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 0;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 0;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the TIM6 master/slave mode
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_TIM21_Init(void)
 * Initializes the TIM21 module
 * --------------------------------------------------------------------------- */
void MX_TIM21_Init(void)
{
	// Local variables
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	// Configures the global features of the TIM21
	htim21.Instance = TIM21;
	htim21.Init.Prescaler = 0;
	htim21.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim21.Init.Period = 0;
	htim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim21) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the clock source of the TIM21
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim21, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the TIM21 master/slave mode
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim21, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_TIM22_Init(void)
 * Initializes the TIM22 module
 * --------------------------------------------------------------------------- */
void MX_TIM22_Init(void)
{
	// Local variables
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	// Configures the clock source of the TIM22
	htim22.Instance = TIM22;
	htim22.Init.Prescaler = 0;
	htim22.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim22.Init.Period = 0;
	htim22.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim22) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the clock source of the TIM22
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim22, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configures the TIM22 master/slave mode
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim22, &sMasterConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_USART1_UART_Init(void)
 * Initializes the USART1 module
 * --------------------------------------------------------------------------- */
void MX_USART1_UART_Init(void)
{
	// Configures the global features of the USART1
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

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_USART2_UART_Init(void)
 * Initializes the USART2 module
 * --------------------------------------------------------------------------- */
void MX_USART2_UART_Init(void)
{
	// Configures the global features of the USART2
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

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_DMA_Init(void)
 * Initializes the DMA module
 * --------------------------------------------------------------------------- */
void MX_DMA_Init(void) 
{
	// DMA controller clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();

	// DMA channel 1 configuration
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	// DMA channels 2-3 configuration
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	// DMA channel 4-7 configuration
	HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_GPIO_Init(void)
 * Initializes the GPIO module
 * --------------------------------------------------------------------------- */
void MX_GPIO_Init(void)
{
	// Local variables
	GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO ports clock enable
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	// Configure GPIO pin output level
	HAL_GPIO_WritePin(GPIOC,	SPI1_SS5_Pin |
								SPI1_SS3_Pin |
								SPI1_SS6_Pin |
								SPI1_SS1_Pin |
								SPI1_SS0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,	SPI1_SS2_Pin |
								SPI1_SS4_Pin, GPIO_PIN_SET);

	// Configures GPIOA analogs
	GPIO_InitStruct.Pin = (	GPIO_PIN_8	|
							GPIO_PIN_11	|
							GPIO_PIN_12	|
							GPIO_PIN_15	);
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Configures GPIOB analogs
	GPIO_InitStruct.Pin = (	GPIO_PIN_1	|
							GPIO_PIN_2	|
							GPIO_PIN_3	|
							GPIO_PIN_4	|
							GPIO_PIN_5	|
							GPIO_PIN_6	|
							GPIO_PIN_7	|
							GPIO_PIN_12	|
							GPIO_PIN_13	|
							GPIO_PIN_14	|
							GPIO_PIN_15	);
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Configures GPIOB outputs
	GPIO_InitStruct.Pin = (	SPI1_SS2_Pin	|
							SPI1_SS4_Pin	);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Configures GPIOC analogs
	GPIO_InitStruct.Pin = (	GPIO_PIN_4	|
							GPIO_PIN_10	|
							GPIO_PIN_11	|
							GPIO_PIN_12	);
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Configures GPIOC inputs
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	// Configures GPIOC outputs
	GPIO_InitStruct.Pin = (	SPI1_SS5_Pin	|
							SPI1_SS3_Pin	|
							SPI1_SS6_Pin	|
							SPI1_SS1_Pin	|
							SPI1_SS0_Pin	);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Configures GPIOD analogs
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	return;
}

/* -----------------------------------------------------------------------------
 * void MX_RTC_Init(void)
 * Initializes the RTC module
 * --------------------------------------------------------------------------- */
void MX_RTC_Init(void)
{
	// Local variables
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	RTC_AlarmTypeDef sAlarm;

	// Initializes RTC only
	hrtc.Instance = RTC;
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2) {
		hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
		hrtc.Init.AsynchPrediv = 127;
		hrtc.Init.SynchPrediv = 255;
		hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
		hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
		hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
		hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
		if (HAL_RTC_Init(&hrtc) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		// Initializes RTC and sets the time and date
		sTime.Hours = RTC_DEFAULT_HOURS;
		sTime.Minutes = RTC_DEFAULT_MINUTES;
		sTime.Seconds = RTC_DEFAULT_SECONDS;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}
		sDate.WeekDay = RTC_DEFAULT_WEEKDAY;
		sDate.Month = RTC_DEFAULT_MONTH;
		sDate.Date = RTC_DEFAULT_DATE;
		sDate.Year = RTC_DEFAULT_YEAR;
		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}
/*
		// Enables the alarm A
		sAlarm.AlarmTime.Hours = 0;
		sAlarm.AlarmTime.Minutes = 0;
		sAlarm.AlarmTime.Seconds = 0;
		sAlarm.AlarmTime.SubSeconds = 0;
		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
		sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
		sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
		sAlarm.AlarmDateWeekDay = 1;
		sAlarm.Alarm = RTC_ALARM_A;
		if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}
*/
		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
	}

	return;
}

/* -----------------------------------------------------------------------------
 * void _Error_Handler(char *file, int line)
 * Handles errors
 * --------------------------------------------------------------------------- */
void _Error_Handler(char *file, int line)
{
	while(1){
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 

	return;
}

#endif
