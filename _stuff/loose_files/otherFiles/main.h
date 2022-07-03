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

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
