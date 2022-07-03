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
#define ADC1_BAT_GPIO_Port 				GPIOB
#define ADC1_BAT_Pin 					GPIO_PIN_1
#define ADC1_BELT0_GPIO_Port 			GPIOA
#define ADC1_BELT0_Pin 					GPIO_PIN_3
#define ADC1_BELT1_GPIO_Port 			GPIOA
#define ADC1_BELT1_Pin 					GPIO_PIN_4
#define ADC1_ECG0_GPIO_Port 			GPIOA
#define ADC1_ECG0_Pin 					GPIO_PIN_0
#define ADC1_ECG1_GPIO_Port 			GPIOA
#define ADC1_ECG1_Pin 					GPIO_PIN_1
#define ADC1_ECG2_GPIO_Port 			GPIOA
#define ADC1_ECG2_Pin 					GPIO_PIN_2
#define BOOT1_GPIO_Port 				GPIOB
#define BOOT1_Pin 						GPIO_PIN_2
#define DS1302_CE_GPIO_Port 			GPIOB
#define DS1302_CE_Pin 					GPIO_PIN_3
#define DS1302_DIO_GPIO_Port 			GPIOB
#define DS1302_DIO_Pin 					GPIO_PIN_11
#define DS1302_SCK_GPIO_Port 			GPIOB
#define DS1302_SCK_Pin 					GPIO_PIN_10
#define NLED_GPIO_Port 					GPIOC
#define NLED_Pin 						GPIO_PIN_13
#define SPI1_CS_SD_GPIO_Port 			GPIOB
#define SPI1_CS_SD_Pin 					GPIO_PIN_0
#define SPI2_CS_MPU0_GPIO_Port 			GPIOB
#define SPI2_CS_MPU0_Pin 				GPIO_PIN_12
#define SPI2_CS_MPU1_GPIO_Port 			GPIOA
#define SPI2_CS_MPU1_Pin 				GPIO_PIN_9
#define SPI2_CS_MPU2_GPIO_Port 			GPIOA
#define SPI2_CS_MPU2_Pin 				GPIO_PIN_10
#define SPI2_CS_MPU3_GPIO_Port 			GPIOA
#define SPI2_CS_MPU3_Pin 				GPIO_PIN_15
#define USART1_BT_EN_GPIO_Port 			GPIOB
#define USART1_BT_EN_Pin 				GPIO_PIN_5
#define USART1_BT_RX_GPIO_Port 			GPIOB
#define USART1_BT_RX_Pin 				GPIO_PIN_7
#define USART1_BT_STATE_GPIO_Port 		GPIOB
#define USART1_BT_STATE_Pin 			GPIO_PIN_4
#define USART1_BT_TX_GPIO_Port 			GPIOB
#define USART1_BT_TX_Pin 				GPIO_PIN_6
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
