
#include "tim.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;

void Error_Handler(uint16_t errorCode_p);

// -----------------------------------------------------------------------------
// TIM1 init function ----------------------------------------------------------

void MX_TIM1_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 23;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler(ERROR_CODE_1015);
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1016);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1017);
	}
}

// -----------------------------------------------------------------------------
// TIM3 init function ----------------------------------------------------------

void MX_TIM3_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 31;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 22499;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler(ERROR_CODE_1018);
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_1019);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_101A);
	}
}

// -----------------------------------------------------------------------------
// TIM7 init function ----------------------------------------------------------

void MX_TIM7_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 71;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 65535;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler(ERROR_CODE_101B);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_101C);
	}
}

// -----------------------------------------------------------------------------
// TIM8 init function ----------------------------------------------------------

void MX_TIM8_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim8.Instance = TIM8;
	htim8.Init.Prescaler = 23999;
	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim8.Init.Period = 23999;
	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim8.Init.RepetitionCounter = 0;
	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htim8) != HAL_OK) {
		Error_Handler(ERROR_CODE_101D);
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_101E);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if(HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK) {
		Error_Handler(ERROR_CODE_101F);
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{

	if(tim_baseHandle->Instance == TIM1) {
		// TIM1 clock enable
		__HAL_RCC_TIM1_CLK_ENABLE();
	} else if(tim_baseHandle->Instance == TIM3) {
		// TIM3 clock enable
		__HAL_RCC_TIM3_CLK_ENABLE();

		// TIM3 interrupt Init
		HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	} else if(tim_baseHandle->Instance == TIM7) {
		// TIM7 clock enable
		__HAL_RCC_TIM7_CLK_ENABLE();

		// TIM7 interrupt Init
		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	} else if(tim_baseHandle->Instance == TIM8) {
		// TIM8 clock enable
		__HAL_RCC_TIM8_CLK_ENABLE();

		// TIM8 interrupt Init
		HAL_NVIC_SetPriority(TIM8_UP_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{

	if(tim_baseHandle->Instance == TIM1) {
		// Peripheral clock disable
		__HAL_RCC_TIM1_CLK_DISABLE();
	} else if(tim_baseHandle->Instance == TIM3) {
		// Peripheral clock disable
		__HAL_RCC_TIM3_CLK_DISABLE();

		// TIM3 interrupt Deinit
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
	} else if(tim_baseHandle->Instance == TIM7) {
		// Peripheral clock disable
		__HAL_RCC_TIM7_CLK_DISABLE();

		// TIM7 interrupt Deinit
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
	} else if(tim_baseHandle->Instance == TIM8) {
		// Peripheral clock disable
		__HAL_RCC_TIM8_CLK_DISABLE();

		// TIM8 interrupt Deinit
		HAL_NVIC_DisableIRQ(TIM8_UP_IRQn);
	}
}
