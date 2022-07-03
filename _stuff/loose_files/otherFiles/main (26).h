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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

// Channels per sample
#define ECG_ADC_SAMPLE_SIZE				3
// Samples per buffer (Must be even)
#define ECG_ADC_BUFFER_LENGTH			8				// Must be even!
// Channels per buffer
#define ECG_DMA_BUFFER_SIZE				(ECG_ADC_BUFFER_LENGTH * ECG_ADC_SAMPLE_SIZE)
#define ECG_DMA_HALF_BUFFER				(ECG_DMA_BUFFER_SIZE / 2)
#define BAT_DMA_BUFFER_SIZE				6
#define BAT_DMA_HALF_BUFFER				(BAT_DMA_BUFFER_SIZE / 2)

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

// System LED macrofunctions
#define ledTurnOn()						HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET)
#define ledTurnOff()					HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET)
#define ledToggle()						HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin)

// System DEBUG pins macrofunctions
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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NLED_Pin GPIO_PIN_13
#define NLED_GPIO_Port GPIOC
#define ADC3_STEP_UP_Pin GPIO_PIN_0
#define ADC3_STEP_UP_GPIO_Port GPIOC
#define ADC3_BAT_Pin GPIO_PIN_1
#define ADC3_BAT_GPIO_Port GPIOC
#define ADC3_CR2032_Pin GPIO_PIN_2
#define ADC3_CR2032_GPIO_Port GPIOC
#define ADC1_ECG3_Pin GPIO_PIN_3
#define ADC1_ECG3_GPIO_Port GPIOC
#define ADC1_ECG2_Pin GPIO_PIN_4
#define ADC1_ECG2_GPIO_Port GPIOC
#define ADC1_ECG1_Pin GPIO_PIN_5
#define ADC1_ECG1_GPIO_Port GPIOC
#define SPI2_CS_SD_Pin GPIO_PIN_12
#define SPI2_CS_SD_GPIO_Port GPIOB
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

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
