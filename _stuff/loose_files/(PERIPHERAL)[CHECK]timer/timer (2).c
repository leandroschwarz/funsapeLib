/**
  ******************************************************************************
  * @file    timer.c
  * @author  Karolina Majstrovic
  * @version 
  * @date    09-May-2015
  * @brief   Delay function.
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* */
TIM_HandleTypeDef TimHandle;
TIM_HandleTypeDef TimHandleCnt;

/**
  * @brief Initializes timer for counting until 32ms, than interrupt occurs if 
	*				 enabled by TIM_Turn_On()
  * @param timx: 
  * @retval 
  */
void TIM_Init(TIM_TypeDef * timx)
{
	int uwPrescalerValue;

  /* Compute the prescaler value to have TIMx counter clock equal to 2MHz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 2000000) - 1;

  /* Set TIMx instance */
  TimHandle.Instance = TIMx;

  /* Initialize TIMx peripheral as follows:
       + Period = 65536 - 1 ... biggest value in ARR register
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = 65536 - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&TimHandle);
		
}

/**
  * @brief Starts counting
  * @param htim: tim handler
  * @retval none
  */
void TIM_Start(TIM_HandleTypeDef *htim){
	htim->Instance->CR1 |= TIM_CR1_CEN;
	HAL_TIM_Base_Start_IT(htim);
}

void TIM_Stop(TIM_HandleTypeDef *htim){
	htim->Instance->CR1 &= ~TIM_CR1_CEN;
	HAL_TIM_Base_Stop(htim);
}

void TIM_Turn_On(TIM_HandleTypeDef *htim)
{
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();
  
  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIMx_IRQn, 3, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);
}

/* Reads and return counter value. Counter is set for system clock 48MHz and 
 * TIMx clock frequency 2MHz.
*/
int TIM_Cnt(TIM_HandleTypeDef *htim){
	return __HAL_TIM_GetCounter(htim);
}

/* Sets counter to zero.
*/
void TIM_Set_Zero(TIM_HandleTypeDef *htim){
	htim->Instance->CNT = 0;	
}

/* Calculated for system clock: 48 MHz and TIMx clock frequency 2MHz.
*
*/
void TIM_Set_Value(TIM_HandleTypeDef *htim, int val, char us){
	if(us == US){
		//
		htim->Instance->ARR = val * 2 - 1;
	}
	else if(us == MS){
			htim->Instance->ARR = val * 2000 - 1;	
	}	
}