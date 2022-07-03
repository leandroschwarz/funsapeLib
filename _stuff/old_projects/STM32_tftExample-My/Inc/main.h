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
#define LD2_Pin							GPIO_PIN_13
#define LD2_GPIO_Port					GPIOC
#define LCD_DATA_0_Pin					GPIO_PIN_0
#define LCD_DATA_0_GPIO_Port			GPIOA
#define LCD_DATA_1_Pin					GPIO_PIN_1
#define LCD_DATA_1_GPIO_Port			GPIOA
#define LCD_DATA_2_Pin					GPIO_PIN_2
#define LCD_DATA_2_GPIO_Port			GPIOA
#define LCD_DATA_3_Pin					GPIO_PIN_3
#define LCD_DATA_3_GPIO_Port			GPIOA
#define LCD_DATA_4_Pin					GPIO_PIN_4
#define LCD_DATA_4_GPIO_Port			GPIOA
#define LCD_DATA_5_Pin					GPIO_PIN_5
#define LCD_DATA_5_GPIO_Port			GPIOA
#define LCD_DATA_6_Pin					GPIO_PIN_6
#define LCD_DATA_6_GPIO_Port			GPIOA
#define LCD_DATA_7_Pin					GPIO_PIN_7
#define LCD_DATA_7_GPIO_Port			GPIOA
#define LCD_RS_Pin						GPIO_PIN_0
#define LCD_RS_GPIO_Port				GPIOB
#define LCD_WR_Pin						GPIO_PIN_1
#define LCD_WR_GPIO_Port				GPIOB
#define LCD_CS_Pin						GPIO_PIN_10
#define LCD_CS_GPIO_Port				GPIOB
#define LCD_RST_Pin						GPIO_PIN_11
#define LCD_RST_GPIO_Port				GPIOB
#define LD3_Pin							GPIO_PIN_9
#define LD3_GPIO_Port					GPIOB
/* USER CODE BEGIN Private defines */

#define LCD_ALE_PORT					GPIOB
#define LCD_ALE_PIN						GPIO_PIN_3
#define LCD_RD_PORT						GPIOB
#define LCD_RD_PIN						GPIO_PIN_4

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
