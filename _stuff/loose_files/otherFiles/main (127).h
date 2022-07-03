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
void ledBlinkError(uint8_t errorCode);

/* USER CODE BEGIN EFP */

int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len);
#define CDC_Receive_FS(x, y)			__attribute__((unused)) CDC_OLD_Receive_FS(x, y)
#define SPIx_Write(x)					__attribute__((unused)) SPIx_Write(x)

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_BATTERY_GPIO_Port GPIOA
#define ADC_BATTERY_Pin GPIO_PIN_7
#define ADC_BRAKE_GPIO_Port GPIOA
#define ADC_BRAKE_Pin GPIO_PIN_4
#define ADC_BUFFER_SIZE_BYTES (2 * ADC_CHANNELS_PER_BUFFER)
#define ADC_BUFFER_SIZE_HWORDS ADC_CHANNELS_PER_BUFFER
#define ADC_CHANNELS_PER_BUFFER (ADC_CHANNELS_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER)
#define ADC_CHANNELS_PER_SAMPLE 9
#define ADC_HALF_BUFFER_SIZE_HWORDS (ADC_BUFFER_SIZE_HWORDS / 2)
#define ADC_SAMPLES_PER_BUFFER 50
#define ADC_STEERING_GPIO_Port GPIOA
#define ADC_STEERING_Pin GPIO_PIN_6
#define ADC_SUS_FL_GPIO_Port GPIOA
#define ADC_SUS_FL_Pin GPIO_PIN_0
#define ADC_SUS_FR_GPIO_Port GPIOA
#define ADC_SUS_FR_Pin GPIO_PIN_1
#define ADC_SUS_RL_GPIO_Port GPIOA
#define ADC_SUS_RL_Pin GPIO_PIN_2
#define ADC_SUS_RR_GPIO_Port GPIOA
#define ADC_SUS_RR_Pin GPIO_PIN_3
#define ADC_THROTLLE_GPIO_Port GPIOA
#define ADC_THROTLLE_Pin GPIO_PIN_5
#define BOOT1_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define DEBUG1_GPIO_Port GPIOA
#define DEBUG1_Pin GPIO_PIN_15
#define DEBUG2_GPIO_Port GPIOB
#define DEBUG2_Pin GPIO_PIN_3
#define DEBUG3_GPIO_Port GPIOB
#define DEBUG3_Pin GPIO_PIN_4
#define DEBUG4_GPIO_Port GPIOB
#define DEBUG4_Pin GPIO_PIN_5
#define I2C1_DEFAULT_TIMEOUT 10
#define LED_B_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_10
#define LED_G_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_9
#define LED_R_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_8
#define NLED_GPIO_Port GPIOC
#define NLED_Pin GPIO_PIN_13
#define SPI2_CS_SD_GPIO_Port GPIOB
#define SPI2_CS_SD_Pin GPIO_PIN_12
#define SPI2_DEFAULT_TIMEOUT 10
#define TIMER_DEBUG_FACTOR 1
#define TIMER3_COUNT_VALUE 10000
#define TIMER3_PRESCALER 36
#define USART1_DEFAULT_TIMEOUT 10
#define USART3_DEFAULT_TIMEOUT 10
#define USB_DEFAULT_TIMEOUT 10
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
