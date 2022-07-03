/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NLED_Pin GPIO_PIN_13
#define NLED_GPIO_Port GPIOC
#define BT_ST_Pin GPIO_PIN_1
#define BT_ST_GPIO_Port GPIOA
#define BT_ST_EXTI_IRQn EXTI1_IRQn
#define DEBUG0_Pin GPIO_PIN_10
#define DEBUG0_GPIO_Port GPIOD
#define DEBUG1_Pin GPIO_PIN_11
#define DEBUG1_GPIO_Port GPIOD
#define DEBUG2_Pin GPIO_PIN_12
#define DEBUG2_GPIO_Port GPIOD
#define DEBUG3_Pin GPIO_PIN_13
#define DEBUG3_GPIO_Port GPIOD
#define DEBUG4_Pin GPIO_PIN_14
#define DEBUG4_GPIO_Port GPIOD
#define DEBUG5_Pin GPIO_PIN_15
#define DEBUG5_GPIO_Port GPIOD
#define DEBUG6_Pin GPIO_PIN_6
#define DEBUG6_GPIO_Port GPIOC
#define DEBUG7_Pin GPIO_PIN_7
#define DEBUG7_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

// =============================================================================
// Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Debug level control constants -----------------------------------------------

#define DEBUG_ALLOW_FULL				1
#define DEBUG_ALLOW_VERBOSE				1
#define DEBUG_DISABLE_SD				1
#define DEBUG_DISABLE_ADC				1
#define DEBUG_DISABLE_MPU				1

// -----------------------------------------------------------------------------
// Timeouts / debounce times ---------------------------------------------------

#define I2C1_TIMEOUT					10				// Value in miliseconds
#define UART1_TIMEOUT					10				// Value in miliseconds
#define UART3_TIMEOUT					10				// Value in miliseconds

// =============================================================================
// Macro-functions
// =============================================================================

// -----------------------------------------------------------------------------
// System LED macrofunctions ---------------------------------------------------

#define ledPulseOff(milliSeconds)		do{ledTurnOff();delayMs(milliSeconds);ledTurnOn();}while(0)
#define ledPulseOn(milliSeconds)		do{ledTurnOn();delayMs(milliSeconds);ledTurnOff();}while(0)
#define ledToggle()						do{HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);}while(0)
#define ledTurnOff()					do{HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);}while(0)
#define ledTurnOn()						do{HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);}while(0)

// -----------------------------------------------------------------------------
// System DEBUG pins macrofunctions --------------------------------------------

#define debug0High()					HAL_GPIO_WritePin(DEBUG0_GPIO_Port, DEBUG0_Pin, GPIO_PIN_SET)
#define debug0Low()						HAL_GPIO_WritePin(DEBUG0_GPIO_Port, DEBUG0_Pin, GPIO_PIN_RESET)
#define debug0Toggle()					HAL_GPIO_TogglePin(DEBUG0_GPIO_Port, DEBUG0_Pin)
#define debug1High()					HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_SET)
#define debug1Low()						HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_RESET)
#define debug1Toggle()					HAL_GPIO_TogglePin(DEBUG1_GPIO_Port, DEBUG1_Pin)
#define debug2High()					HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_SET)
#define debug2Low()						HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_RESET)
#define debug2Toggle()					HAL_GPIO_TogglePin(DEBUG2_GPIO_Port, DEBUG2_Pin)
#define debug3High()					HAL_GPIO_WritePin(DEBUG3_GPIO_Port, DEBUG3_Pin, GPIO_PIN_SET)
#define debug3Low()						HAL_GPIO_WritePin(DEBUG3_GPIO_Port, DEBUG3_Pin, GPIO_PIN_RESET)
#define debug3Toggle()					HAL_GPIO_TogglePin(DEBUG3_GPIO_Port, DEBUG3_Pin)
#define debug4High()					HAL_GPIO_WritePin(DEBUG4_GPIO_Port, DEBUG4_Pin, GPIO_PIN_SET)
#define debug4Low()						HAL_GPIO_WritePin(DEBUG4_GPIO_Port, DEBUG4_Pin, GPIO_PIN_RESET)
#define debug4Toggle()					HAL_GPIO_TogglePin(DEBUG4_GPIO_Port, DEBUG4_Pin)
#define debug5High()					HAL_GPIO_WritePin(DEBUG5_GPIO_Port, DEBUG5_Pin, GPIO_PIN_SET)
#define debug5Low()						HAL_GPIO_WritePin(DEBUG5_GPIO_Port, DEBUG5_Pin, GPIO_PIN_RESET)
#define debug5Toggle()					HAL_GPIO_TogglePin(DEBUG5_GPIO_Port, DEBUG5_Pin)
#define debug6High()					HAL_GPIO_WritePin(DEBUG6_GPIO_Port, DEBUG6_Pin, GPIO_PIN_SET)
#define debug6Low()						HAL_GPIO_WritePin(DEBUG6_GPIO_Port, DEBUG6_Pin, GPIO_PIN_RESET)
#define debug6Toggle()					HAL_GPIO_TogglePin(DEBUG6_GPIO_Port, DEBUG6_Pin)
#define debug7High()					HAL_GPIO_WritePin(DEBUG7_GPIO_Port, DEBUG7_Pin, GPIO_PIN_SET)
#define debug7Low()						HAL_GPIO_WritePin(DEBUG7_GPIO_Port, DEBUG7_Pin, GPIO_PIN_RESET)
#define debug7Toggle()					HAL_GPIO_TogglePin(DEBUG7_GPIO_Port, DEBUG7_Pin)
#define debugTriggerPulse()				do {debug7Low();delayMs(10);debug7High();delayMs(10);debug7Low();} while(0);
#define delayUs(time)		do {										\
		htim7.Instance->CR1  = 0x0000;									\
		htim7.Instance->CNT  = 0x0000;									\
		htim7.Instance->PSC  = (time - 1);								\
		htim7.Instance->ARR  = (SystemCoreClock / 1000000UL) - 1;		\
		htim7.Instance->DIER = 0x0000;									\
		htim7.Instance->CR2  = 0x0020;									\
		htim7.Instance->CR1  = 0x0001;									\
		do {} while(!((htim7.Instance->SR) & 0x0001));					\
		htim7.Instance->CR1  = 0x0000;									\
		htim7.Instance->SR   = 0x0000;									\
	} while(0)

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// System error codes ----------------------------------------------------------

typedef enum {
	HAL_INTERNAL_ERROR					= 1,
	PPG_CIRC_BUFF_POP_DATA				= 2,	//
	PPG_CIRC_BUFF_PUSH_DATA				= 3,	//
	PPG_SENSOR_1_CONFIG_FIFO			= 4,
	PPG_SENSOR_1_CONFIG_INIT			= 5,
	PPG_SENSOR_1_ENABLE_INTERRUPT		= 6,
	PPG_SENSOR_1_FIFO_FLUSH				= 7,
	PPG_SENSOR_1_GET_TEMPERATURE		= 8,
	PPG_SENSOR_1_SET_FULL_SCALE			= 9,
	PPG_SENSOR_1_SET_INTERRUPT_CALLBACK	= 10,
	PPG_SENSOR_1_SET_RESOLUTION			= 11,
	PPG_SENSOR_1_SET_SAMPLE_RATE		= 12,
	PPG_SENSOR_1_SET_SPO2_MODE			= 13,
	TIMER1_CONFIG						= 14,
	TWI_CLASS_INIT						= 15,
	UART1_DMA_RX_CONFIG					= 16,	//
	UART1_RX_CIRC_BUFFER_PUSH_DATA		= 17,	//
	UART1_TX_CIRC_BUFFER_PUSH_DATA		= 18,	//
	UART1_TX_TRANSFER					= 19,	//
	UART3_DMA_RX_CONFIG					= 20,	//
	UART3_RX_CIRC_BUFFER_PUSH_DATA		= 21,	//
	UART3_TX_CIRC_BUFFER_PUSH_DATA		= 22,	//
	UART3_TX_TRANSFER					= 23,	//
} systemError_e;

// =============================================================================
// Global variables declarations
// =============================================================================

extern __IO uint32_t					uwTick;
extern HAL_TickFreqTypeDef				uwTickFreq;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
