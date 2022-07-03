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
#define TFT_DATA2_Pin GPIO_PIN_2
#define TFT_DATA2_GPIO_Port GPIOE
#define TFT_DATA3_Pin GPIO_PIN_3
#define TFT_DATA3_GPIO_Port GPIOE
#define TFT_DATA4_Pin GPIO_PIN_4
#define TFT_DATA4_GPIO_Port GPIOE
#define TFT_DATA5_Pin GPIO_PIN_5
#define TFT_DATA5_GPIO_Port GPIOE
#define TFT_DATA6_Pin GPIO_PIN_6
#define TFT_DATA6_GPIO_Port GPIOE
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
#define BT_EN_Pin GPIO_PIN_0
#define BT_EN_GPIO_Port GPIOA
#define BT_ST_Pin GPIO_PIN_1
#define BT_ST_GPIO_Port GPIOA
#define SPI1_CS_MPU3_Pin GPIO_PIN_4
#define SPI1_CS_MPU3_GPIO_Port GPIOA
#define ADC1_ECG2_Pin GPIO_PIN_4
#define ADC1_ECG2_GPIO_Port GPIOC
#define ADC1_ECG1_Pin GPIO_PIN_5
#define ADC1_ECG1_GPIO_Port GPIOC
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define TFT_DATA7_Pin GPIO_PIN_7
#define TFT_DATA7_GPIO_Port GPIOE
#define TFT_DATA8_Pin GPIO_PIN_8
#define TFT_DATA8_GPIO_Port GPIOE
#define TFT_DATA9_Pin GPIO_PIN_9
#define TFT_DATA9_GPIO_Port GPIOE
#define TFT_DATA10_Pin GPIO_PIN_10
#define TFT_DATA10_GPIO_Port GPIOE
#define TFT_DATA11_Pin GPIO_PIN_11
#define TFT_DATA11_GPIO_Port GPIOE
#define TFT_DATA12_Pin GPIO_PIN_12
#define TFT_DATA12_GPIO_Port GPIOE
#define TFT_DATA13_Pin GPIO_PIN_13
#define TFT_DATA13_GPIO_Port GPIOE
#define TFT_DATA14_Pin GPIO_PIN_14
#define TFT_DATA14_GPIO_Port GPIOE
#define TFT_DATA15_Pin GPIO_PIN_15
#define TFT_DATA15_GPIO_Port GPIOE
#define SPI2_CS_SD_Pin GPIO_PIN_12
#define SPI2_CS_SD_GPIO_Port GPIOB
#define SPI2_SCLK_Pin GPIO_PIN_13
#define SPI2_SCLK_GPIO_Port GPIOB
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
#define SWIO_Pin GPIO_PIN_13
#define SWIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define JTDI_Pin GPIO_PIN_15
#define JTDI_GPIO_Port GPIOA
#define SPI1_CS_MPU2_Pin GPIO_PIN_10
#define SPI1_CS_MPU2_GPIO_Port GPIOC
#define SPI1_CS_MPU1_Pin GPIO_PIN_11
#define SPI1_CS_MPU1_GPIO_Port GPIOC
#define SPI1_CS_MPU0_Pin GPIO_PIN_12
#define SPI1_CS_MPU0_GPIO_Port GPIOC
#define TFT_RD_Pin GPIO_PIN_0
#define TFT_RD_GPIO_Port GPIOD
#define TFT_WR_Pin GPIO_PIN_1
#define TFT_WR_GPIO_Port GPIOD
#define TFT_RS_Pin GPIO_PIN_2
#define TFT_RS_GPIO_Port GPIOD
#define TFT_CS_Pin GPIO_PIN_3
#define TFT_CS_GPIO_Port GPIOD
#define TFT_RST_Pin GPIO_PIN_4
#define TFT_RST_GPIO_Port GPIOD
#define ENC_A_Pin GPIO_PIN_5
#define ENC_A_GPIO_Port GPIOD
#define ENC_BTN_Pin GPIO_PIN_6
#define ENC_BTN_GPIO_Port GPIOD
#define ENC_B_Pin GPIO_PIN_7
#define ENC_B_GPIO_Port GPIOD
#define JTDO_Pin GPIO_PIN_3
#define JTDO_GPIO_Port GPIOB
#define JTNRST_Pin GPIO_PIN_4
#define JTNRST_GPIO_Port GPIOB
#define RTC_SWO_Pin GPIO_PIN_8
#define RTC_SWO_GPIO_Port GPIOB
#define TFT_DATA0_Pin GPIO_PIN_0
#define TFT_DATA0_GPIO_Port GPIOE
#define TFT_DATA1_Pin GPIO_PIN_1
#define TFT_DATA1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
