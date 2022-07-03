/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern volatile uint8_t errorCode;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define ERROR_CODE_SPI_SET_TIME_OUT				1
#define ERROR_CODE_SPI_SET_DEFAULT_DEVICE		2
#define ERROR_CODE_SPI_INIT						3
#define ERROR_CODE_RTC_UPDATE_VERIFICATION		4
#define ERROR_CODE_RTC_UPDATE_DATA				5
#define ERROR_CODE_RTC_MARK_UPDATED				6
#define ERROR_CODE_MPU_INITIALIZE				7

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define DEBUG_0_Pin GPIO_PIN_3
#define DEBUG_0_GPIO_Port GPIOA
#define DEBUG_1_Pin GPIO_PIN_4
#define DEBUG_1_GPIO_Port GPIOA
#define DEBUG_2_Pin GPIO_PIN_5
#define DEBUG_2_GPIO_Port GPIOA
#define DEBUG_3_Pin GPIO_PIN_6
#define DEBUG_3_GPIO_Port GPIOA
#define SPI2_SS_MPU2_Pin GPIO_PIN_7
#define SPI2_SS_MPU2_GPIO_Port GPIOA
#define SPI2_SS_MPU3_Pin GPIO_PIN_0
#define SPI2_SS_MPU3_GPIO_Port GPIOB
#define SPI2_SS_MPU4_Pin GPIO_PIN_1
#define SPI2_SS_MPU4_GPIO_Port GPIOB
#define SPI2_SS_MPU1_Pin GPIO_PIN_12
#define SPI2_SS_MPU1_GPIO_Port GPIOB
#define SPI1_SS_SD_Pin GPIO_PIN_15
#define SPI1_SS_SD_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define ledTurnOn()						do{HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);}while(0)
#define ledTurnOff()					do{HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);}while(0)
#define ledToggle()						do{HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);}while(0)
#define debug0High()					do{HAL_GPIO_WritePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin, GPIO_PIN_SET);}while(0)
#define debug0Low()						do{HAL_GPIO_WritePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin, GPIO_PIN_RESET);}while(0)
#define debug0Toggle()					do{HAL_GPIO_TogglePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin);}while(0)
#define debug1High()					do{HAL_GPIO_WritePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin, GPIO_PIN_SET);}while(0)
#define debug1Low()						do{HAL_GPIO_WritePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin, GPIO_PIN_RESET);}while(0)
#define debug1Toggle()					do{HAL_GPIO_TogglePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin);}while(0)
#define debug2High()					do{HAL_GPIO_WritePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin, GPIO_PIN_SET);}while(0)
#define debug2Low()						do{HAL_GPIO_WritePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin, GPIO_PIN_RESET);}while(0)
#define debug2Toggle()					do{HAL_GPIO_TogglePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin);}while(0)
#define debug3High()					do{HAL_GPIO_WritePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin, GPIO_PIN_SET);}while(0)
#define debug3Low()						do{HAL_GPIO_WritePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin, GPIO_PIN_RESET);}while(0)
#define debug3Toggle()					do{HAL_GPIO_TogglePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin);}while(0)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
