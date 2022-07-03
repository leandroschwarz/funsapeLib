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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define PROBE0_GPIO_Port				GPIOB
#define PROBE0_Pin						GPIO_PIN_7
#define PROBE1_GPIO_Port				GPIOB
#define PROBE1_Pin						GPIO_PIN_6
#define PROBE2_GPIO_Port				GPIOB
#define PROBE2_Pin						GPIO_PIN_5
#define PROBE3_GPIO_Port				GPIOB
#define PROBE3_Pin						GPIO_PIN_4
#define PROBE4_GPIO_Port				GPIOB
#define PROBE4_Pin						GPIO_PIN_3
#define PROBE5_GPIO_Port				GPIOD
#define PROBE5_Pin						GPIO_PIN_2
#define PROBE6_GPIO_Port				GPIOC
#define PROBE6_Pin						GPIO_PIN_12
#define MPU1_GPIO_Port					GPIOC
#define MPU1_Pin						GPIO_PIN_9
#define MPU2_GPIO_Port					GPIOC
#define MPU2_Pin						GPIO_PIN_8
#define MPU3_GPIO_Port					GPIOC
#define MPU3_Pin						GPIO_PIN_7
#define MPU4_GPIO_Port					GPIOC
#define MPU4_Pin						GPIO_PIN_6
/* USER CODE BEGIN Private defines */

#define PROBE0_TOGGLE()					HAL_GPIO_TogglePin(PROBE0_GPIO_Port, PROBE0_Pin)
#define PROBE1_TOGGLE()					HAL_GPIO_TogglePin(PROBE1_GPIO_Port, PROBE1_Pin)
#define PROBE2_TOGGLE()					HAL_GPIO_TogglePin(PROBE2_GPIO_Port, PROBE2_Pin)
#define PROBE3_TOGGLE()					HAL_GPIO_TogglePin(PROBE3_GPIO_Port, PROBE3_Pin)
#define PROBE4_TOGGLE()					HAL_GPIO_TogglePin(PROBE4_GPIO_Port, PROBE4_Pin)
#define PROBE5_TOGGLE()					HAL_GPIO_TogglePin(PROBE5_GPIO_Port, PROBE5_Pin)
#define PROBE6_TOGGLE()					HAL_GPIO_TogglePin(PROBE6_GPIO_Port, PROBE6_Pin)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
