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
#define TFT_DATA_2_Pin GPIO_PIN_2
#define TFT_DATA_2_GPIO_Port GPIOE
#define TFT_DATA_3_Pin GPIO_PIN_3
#define TFT_DATA_3_GPIO_Port GPIOE
#define TFT_DATA_4_Pin GPIO_PIN_4
#define TFT_DATA_4_GPIO_Port GPIOE
#define TFT_DATA_5_Pin GPIO_PIN_5
#define TFT_DATA_5_GPIO_Port GPIOE
#define TFT_DATA_6_Pin GPIO_PIN_6
#define TFT_DATA_6_GPIO_Port GPIOE
#define NLED_Pin GPIO_PIN_13
#define NLED_GPIO_Port GPIOC
#define TFT_DATA_7_Pin GPIO_PIN_7
#define TFT_DATA_7_GPIO_Port GPIOE
#define TFT_DATA_8_Pin GPIO_PIN_8
#define TFT_DATA_8_GPIO_Port GPIOE
#define TFT_DATA_9_Pin GPIO_PIN_9
#define TFT_DATA_9_GPIO_Port GPIOE
#define TFT_DATA_10_Pin GPIO_PIN_10
#define TFT_DATA_10_GPIO_Port GPIOE
#define TFT_DATA_11_Pin GPIO_PIN_11
#define TFT_DATA_11_GPIO_Port GPIOE
#define TFT_DATA_12_Pin GPIO_PIN_12
#define TFT_DATA_12_GPIO_Port GPIOE
#define TFT_DATA_13_Pin GPIO_PIN_13
#define TFT_DATA_13_GPIO_Port GPIOE
#define TFT_DATA_14_Pin GPIO_PIN_14
#define TFT_DATA_14_GPIO_Port GPIOE
#define TFT_DATA_15_Pin GPIO_PIN_15
#define TFT_DATA_15_GPIO_Port GPIOE
#define SPI2_CS_SD_Pin GPIO_PIN_12
#define SPI2_CS_SD_GPIO_Port GPIOB
#define DEB_0_Pin GPIO_PIN_10
#define DEB_0_GPIO_Port GPIOD
#define DEB_1_Pin GPIO_PIN_11
#define DEB_1_GPIO_Port GPIOD
#define DEB_2_Pin GPIO_PIN_12
#define DEB_2_GPIO_Port GPIOD
#define DEB_3_Pin GPIO_PIN_13
#define DEB_3_GPIO_Port GPIOD
#define DEB_4_Pin GPIO_PIN_14
#define DEB_4_GPIO_Port GPIOD
#define DEB_5_Pin GPIO_PIN_15
#define DEB_5_GPIO_Port GPIOD
#define DEB_6_Pin GPIO_PIN_6
#define DEB_6_GPIO_Port GPIOC
#define DEB_7_Pin GPIO_PIN_7
#define DEB_7_GPIO_Port GPIOC
#define SPI1_CS_MPU3_Pin GPIO_PIN_15
#define SPI1_CS_MPU3_GPIO_Port GPIOA
#define SPI1_CS_MPU2_Pin GPIO_PIN_10
#define SPI1_CS_MPU2_GPIO_Port GPIOC
#define SPI1_CS_MPU1_Pin GPIO_PIN_11
#define SPI1_CS_MPU1_GPIO_Port GPIOC
#define SPI1_CS_MPU0_Pin GPIO_PIN_12
#define SPI1_CS_MPU0_GPIO_Port GPIOC
#define TFT_CS_Pin GPIO_PIN_0
#define TFT_CS_GPIO_Port GPIOD
#define TFT_RD_Pin GPIO_PIN_1
#define TFT_RD_GPIO_Port GPIOD
#define TFT_RS_Pin GPIO_PIN_2
#define TFT_RS_GPIO_Port GPIOD
#define TFT_RST_Pin GPIO_PIN_3
#define TFT_RST_GPIO_Port GPIOD
#define TFT_WR_Pin GPIO_PIN_4
#define TFT_WR_GPIO_Port GPIOD
#define ENC_A_Pin GPIO_PIN_5
#define ENC_A_GPIO_Port GPIOD
#define ENC_B_Pin GPIO_PIN_6
#define ENC_B_GPIO_Port GPIOD
#define ENC_BTN_Pin GPIO_PIN_7
#define ENC_BTN_GPIO_Port GPIOD
#define TFT_DATA_0_Pin GPIO_PIN_0
#define TFT_DATA_0_GPIO_Port GPIOE
#define TFT_DATA_1_Pin GPIO_PIN_1
#define TFT_DATA_1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#define setDebug0()						HAL_GPIO_WritePin(DEB_0_GPIO_Port, DEB_0_Pin, GPIO_PIN_SET)
#define clrDebug0()						HAL_GPIO_WritePin(DEB_0_GPIO_Port, DEB_0_Pin, GPIO_PIN_RESET)
#define cplDebug0()						HAL_GPIO_TogglePin(DEB_0_GPIO_Port, DEB_0_Pin)
#define setDebug1()						HAL_GPIO_WritePin(DEB_1_GPIO_Port, DEB_1_Pin, GPIO_PIN_SET)
#define clrDebug1()						HAL_GPIO_WritePin(DEB_1_GPIO_Port, DEB_1_Pin, GPIO_PIN_RESET)
#define cplDebug1()						HAL_GPIO_TogglePin(DEB_1_GPIO_Port, DEB_1_Pin)
#define setDebug2()						HAL_GPIO_WritePin(DEB_2_GPIO_Port, DEB_2_Pin, GPIO_PIN_SET)
#define clrDebug2()						HAL_GPIO_WritePin(DEB_2_GPIO_Port, DEB_2_Pin, GPIO_PIN_RESET)
#define cplDebug2()						HAL_GPIO_TogglePin(DEB_2_GPIO_Port, DEB_2_Pin)
#define setDebug3()						HAL_GPIO_WritePin(DEB_3_GPIO_Port, DEB_3_Pin, GPIO_PIN_SET)
#define clrDebug3()						HAL_GPIO_WritePin(DEB_3_GPIO_Port, DEB_3_Pin, GPIO_PIN_RESET)
#define cplDebug3()						HAL_GPIO_TogglePin(DEB_3_GPIO_Port, DEB_3_Pin)
#define setDebug4()						HAL_GPIO_WritePin(DEB_4_GPIO_Port, DEB_4_Pin, GPIO_PIN_SET)
#define clrDebug4()						HAL_GPIO_WritePin(DEB_4_GPIO_Port, DEB_4_Pin, GPIO_PIN_RESET)
#define cplDebug4()						HAL_GPIO_TogglePin(DEB_4_GPIO_Port, DEB_4_Pin)
#define setDebug5()						HAL_GPIO_WritePin(DEB_5_GPIO_Port, DEB_5_Pin, GPIO_PIN_SET)
#define clrDebug5()						HAL_GPIO_WritePin(DEB_5_GPIO_Port, DEB_5_Pin, GPIO_PIN_RESET)
#define cplDebug5()						HAL_GPIO_TogglePin(DEB_5_GPIO_Port, DEB_5_Pin)
#define setDebug6()						HAL_GPIO_WritePin(DEB_6_GPIO_Port, DEB_6_Pin, GPIO_PIN_SET)
#define clrDebug6()						HAL_GPIO_WritePin(DEB_6_GPIO_Port, DEB_6_Pin, GPIO_PIN_RESET)
#define cplDebug6()						HAL_GPIO_TogglePin(DEB_6_GPIO_Port, DEB_6_Pin)
#define setDebug7()						HAL_GPIO_WritePin(DEB_7_GPIO_Port, DEB_7_Pin, GPIO_PIN_SET)
#define clrDebug7()						HAL_GPIO_WritePin(DEB_7_GPIO_Port, DEB_7_Pin, GPIO_PIN_RESET)
#define cplDebug7()						HAL_GPIO_TogglePin(DEB_7_GPIO_Port, DEB_7_Pin)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
