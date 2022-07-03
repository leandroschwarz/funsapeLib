#include "main.h"


// Global variables

lsSystemConfigFlags_f lsSystemConfigFlags;
UART_HandleTypeDef huart2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	/* System interrupt init*/
	/* SVC_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
	/* PendSV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_USART2_UART_Init(UART_HandleTypeDef * huart)
{
	huart->Instance = USART2;
	huart->Init.BaudRate = 115200;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(huart) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(uartHandle->Instance==USART2){
		// USART2 clock enable
		__HAL_RCC_USART2_CLK_ENABLE();
		// USART2 GPIO Configuration
		GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
	if(uartHandle->Instance==USART2){
		// Peripheral clock disable
		__HAL_RCC_USART2_CLK_DISABLE();
		// USART2 GPIO Configuration
		HAL_GPIO_DeInit(GPIOA, USART_TX_Pin|USART_RX_Pin);
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	// Configure the main internal regulator output voltage 
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Initializes the CPU, AHB and APB busses clocks 
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_6;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Initializes the CPU, AHB and APB busses clocks 
	RCC_ClkInitStruct.ClockType =	RCC_CLOCKTYPE_HCLK |
									RCC_CLOCKTYPE_SYSCLK |
									RCC_CLOCKTYPE_PCLK1 |
									RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configure the Systick interrupt time 
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	// Configure the Systick 
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// SysTick_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}





///////////////////////////////// OK

void LS_GPIOA_Clock_Enable(void)
{
	if(!lsSystemConfigFlags.gpioClockAEnabled){
		__HAL_RCC_GPIOA_CLK_ENABLE();
		lsSystemConfigFlags.gpioClockAEnabled = TRUE;
	}
}

void LS_GPIOB_Clock_Enable(void)
{
	if(!lsSystemConfigFlags.gpioClockBEnabled){
		__HAL_RCC_GPIOB_CLK_ENABLE();
		lsSystemConfigFlags.gpioClockBEnabled = TRUE;
	}
}

void LS_GPIOC_Clock_Enable(void)
{
	if(!lsSystemConfigFlags.gpioClockCEnabled){
		__HAL_RCC_GPIOC_CLK_ENABLE();
		lsSystemConfigFlags.gpioClockCEnabled = TRUE;
	}
}

void LS_GPIOD_Clock_Enable(void)
{
	if(!lsSystemConfigFlags.gpioClockDEnabled){
		__HAL_RCC_GPIOD_CLK_ENABLE();
		lsSystemConfigFlags.gpioClockDEnabled = TRUE;
	}
}

void LS_GPIOH_Clock_Enable(void)
{
	if(!lsSystemConfigFlags.gpioClockHEnabled){
		__HAL_RCC_GPIOH_CLK_ENABLE();
		lsSystemConfigFlags.gpioClockHEnabled = TRUE;
	}
}

//void LS_GPIO_Config_Pin(lsGpioPort_e port, lsGpioPinNumber_e pin, lsGpioPinMode_e mode, lsGpioPinPullUp_e pull, lsGpioPinSpeed_e speed)
void LS_GPIO_Config_Pin(GPIO_TypeDef * port, lsGpioPinNumber_e pin, lsGpioPinMode_e mode, lsGpioPinPullUp_e pull, lsGpioPinSpeed_e speed)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	// LED configuration
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = speed;
	if(port == GPIOA) {
		LS_GPIOA_Clock_Enable();
	}
	if(port == GPIOB) {
		LS_GPIOB_Clock_Enable();
	}
	if(port == GPIOC) {
		LS_GPIOC_Clock_Enable();
	}
	if(port == GPIOD) {
		LS_GPIOD_Clock_Enable();
	}
	if(port == GPIOH) {
		LS_GPIOH_Clock_Enable();
	}
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}
