/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "STM32L0xx_HAL_Driver/stm32l0xx_hal.h"
#include "CMSIS\Device\ST\STM32L0xx\Include\stm32l0xx.h"
#include "stm32l0xx_it.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc;
extern ADC_HandleTypeDef hadc;
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_i2c2_rx;
extern DMA_HandleTypeDef hdma_i2c2_tx;
extern I2C_HandleTypeDef hi2c2;
extern RTC_HandleTypeDef hrtc;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/******************************************************************************/
/*            Cortex-M0+ Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles RTC global interrupt through EXTI lines 17, 19 and 20 and LSE CSS interrupt through EXTI line 19.
*/
void RTC_IRQHandler(void)
{
	/* USER CODE BEGIN RTC_IRQn 0 */

	/* USER CODE END RTC_IRQn 0 */
	HAL_RTC_AlarmIRQHandler(&hrtc);
	/* USER CODE BEGIN RTC_IRQn 1 */

	/* USER CODE END RTC_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel 1 interrupt.
*/
void DMA1_Channel1_IRQHandler(void)
{
	/* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

	/* USER CODE END DMA1_Channel1_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_adc);
	/* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

	/* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel 2 and channel 3 interrupts.
*/
void DMA1_Channel2_3_IRQHandler(void)
{
	/* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

	/* USER CODE END DMA1_Channel2_3_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_spi1_rx);
	HAL_DMA_IRQHandler(&hdma_spi1_tx);
	/* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

	/* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel 4, channel 5, channel 6 and channel 7 interrupts.
*/
void DMA1_Channel4_5_6_7_IRQHandler(void)
{
	/* USER CODE BEGIN DMA1_Channel4_5_6_7_IRQn 0 */

	/* USER CODE END DMA1_Channel4_5_6_7_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_i2c2_tx);
	HAL_DMA_IRQHandler(&hdma_i2c2_rx);
	HAL_DMA_IRQHandler(&hdma_usart2_rx);
	HAL_DMA_IRQHandler(&hdma_usart2_tx);
	/* USER CODE BEGIN DMA1_Channel4_5_6_7_IRQn 1 */

	/* USER CODE END DMA1_Channel4_5_6_7_IRQn 1 */
}

/**
* @brief This function handles ADC1, COMP1 and COMP2 interrupts (COMP interrupts through EXTI lines 21 and 22).
*/
void ADC1_COMP_IRQHandler(void)
{
	/* USER CODE BEGIN ADC1_COMP_IRQn 0 */

	/* USER CODE END ADC1_COMP_IRQn 0 */
	HAL_ADC_IRQHandler(&hadc);
	/* USER CODE BEGIN ADC1_COMP_IRQn 1 */

	/* USER CODE END ADC1_COMP_IRQn 1 */
}

/**
* @brief This function handles TIM6 global interrupt and DAC1/DAC2 underrun error interrupts.
*/
void TIM6_DAC_IRQHandler(void)
{
	/* USER CODE BEGIN TIM6_DAC_IRQn 0 */

	/* USER CODE END TIM6_DAC_IRQn 0 */
	HAL_TIM_IRQHandler(&htim6);
	HAL_DAC_IRQHandler(&hdac);
	/* USER CODE BEGIN TIM6_DAC_IRQn 1 */

	/* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
* @brief This function handles I2C2 interrupt.
*/
void I2C2_IRQHandler(void)
{
	/* USER CODE BEGIN I2C2_IRQn 0 */

	/* USER CODE END I2C2_IRQn 0 */
	if (hi2c2.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
		HAL_I2C_ER_IRQHandler(&hi2c2);
	} else {
		HAL_I2C_EV_IRQHandler(&hi2c2);
	}
	/* USER CODE BEGIN I2C2_IRQn 1 */

	/* USER CODE END I2C2_IRQn 1 */
}

/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
	/* USER CODE BEGIN SPI1_IRQn 0 */

	/* USER CODE END SPI1_IRQn 0 */
	HAL_SPI_IRQHandler(&hspi1);
	/* USER CODE BEGIN SPI1_IRQn 1 */

	/* USER CODE END SPI1_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
*/
void USART1_IRQHandler(void)
{
	/* USER CODE BEGIN USART1_IRQn 0 */

	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */

	/* USER CODE END USART1_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
void USART2_IRQHandler(void)
{
	/* USER CODE BEGIN USART2_IRQn 0 */

	/* USER CODE END USART2_IRQn 0 */
	HAL_UART_IRQHandler(&huart2);
	/* USER CODE BEGIN USART2_IRQn 1 */

	/* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
