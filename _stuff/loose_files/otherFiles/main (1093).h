/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#define ADC1_ECG1_Pin GPIO_PIN_3
#define ADC1_ECG1_GPIO_Port GPIOC
#define BT_EN_Pin GPIO_PIN_0
#define BT_EN_GPIO_Port GPIOA
#define BT_ST_Pin GPIO_PIN_1
#define BT_ST_GPIO_Port GPIOA
#define SPI1_CS_MPU4_Pin GPIO_PIN_4
#define SPI1_CS_MPU4_GPIO_Port GPIOA
#define ADC1_ECG2_Pin GPIO_PIN_4
#define ADC1_ECG2_GPIO_Port GPIOC
#define ADC1_ECG3_Pin GPIO_PIN_5
#define ADC1_ECG3_GPIO_Port GPIOC
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
#define DEBUG1_Pin GPIO_PIN_10
#define DEBUG1_GPIO_Port GPIOD
#define DEBUG2_Pin GPIO_PIN_11
#define DEBUG2_GPIO_Port GPIOD
#define DEBUG3_Pin GPIO_PIN_12
#define DEBUG3_GPIO_Port GPIOD
#define DEBUG4_Pin GPIO_PIN_13
#define DEBUG4_GPIO_Port GPIOD
#define DEBUG5_Pin GPIO_PIN_14
#define DEBUG5_GPIO_Port GPIOD
#define DEBUG6_Pin GPIO_PIN_15
#define DEBUG6_GPIO_Port GPIOD
#define DEBUG7_Pin GPIO_PIN_6
#define DEBUG7_GPIO_Port GPIOC
#define DEBUG8_Pin GPIO_PIN_7
#define DEBUG8_GPIO_Port GPIOC
#define SWIO_Pin GPIO_PIN_13
#define SWIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define JTDI_Pin GPIO_PIN_15
#define JTDI_GPIO_Port GPIOA
#define SPI1_CS_MPU3_Pin GPIO_PIN_10
#define SPI1_CS_MPU3_GPIO_Port GPIOC
#define SPI1_CS_MPU2_Pin GPIO_PIN_11
#define SPI1_CS_MPU2_GPIO_Port GPIOC
#define SPI1_CS_MPU1_Pin GPIO_PIN_12
#define SPI1_CS_MPU1_GPIO_Port GPIOC
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
#define ENC_A_EXTI_IRQn EXTI9_5_IRQn
#define ENC_BTN_Pin GPIO_PIN_6
#define ENC_BTN_GPIO_Port GPIOD
#define ENC_BTN_EXTI_IRQn EXTI9_5_IRQn
#define ENC_B_Pin GPIO_PIN_7
#define ENC_B_GPIO_Port GPIOD
#define JTDO_Pin GPIO_PIN_3
#define JTDO_GPIO_Port GPIOB
#define JTNRST_Pin GPIO_PIN_4
#define JTNRST_GPIO_Port GPIOB
#define DS1307_SWO_Pin GPIO_PIN_8
#define DS1307_SWO_GPIO_Port GPIOB
#define DS1307_SWO_EXTI_IRQn EXTI9_5_IRQn
#define TFT_DATA0_Pin GPIO_PIN_0
#define TFT_DATA0_GPIO_Port GPIOE
#define TFT_DATA1_Pin GPIO_PIN_1
#define TFT_DATA1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#define ERROR_CODE_1000					0x1000
#define ERROR_CODE_1001					0x1001
#define ERROR_CODE_1002					0x1002
#define ERROR_CODE_1003					0x1003
#define ERROR_CODE_1004					0x1004
#define ERROR_CODE_1005					0x1005
#define ERROR_CODE_1006					0x1006
#define ERROR_CODE_1007					0x1007
#define ERROR_CODE_1008					0x1008
#define ERROR_CODE_1009					0x1009
#define ERROR_CODE_100A					0x100A
#define ERROR_CODE_100B					0x100B
#define ERROR_CODE_100C					0x100C
#define ERROR_CODE_100D					0x100D
#define ERROR_CODE_100E					0x100E
#define ERROR_CODE_100F					0x100F
#define ERROR_CODE_1010					0x1010
#define ERROR_CODE_1011					0x1011
#define ERROR_CODE_1012					0x1012
#define ERROR_CODE_1013					0x1013
#define ERROR_CODE_1014					0x1014
#define ERROR_CODE_1015					0x1015
#define ERROR_CODE_1016					0x1016
#define ERROR_CODE_1017					0x1017
#define ERROR_CODE_1018					0x1018
#define ERROR_CODE_1019					0x1019
#define ERROR_CODE_101A					0x101A
#define ERROR_CODE_101B					0x101B
#define ERROR_CODE_101C					0x101C
#define ERROR_CODE_101D					0x101D
#define ERROR_CODE_101E					0x101E
#define ERROR_CODE_101F					0x101F
#define ERROR_CODE_1020					0x1020
#define ERROR_CODE_1021					0x1021
#define ERROR_CODE_1022					0x1022
#define ERROR_CODE_1023					0x1023
#define ERROR_CODE_1024					0x1024
#define ERROR_CODE_1025					0x1025
#define ERROR_CODE_1026					0x1026
#define ERROR_CODE_1027					0x1027
#define ERROR_CODE_1028					0x1028
#define ERROR_CODE_1029					0x1029
#define ERROR_CODE_102A					0x102A
#define ERROR_CODE_102B					0x102B
#define ERROR_CODE_102C					0x102C
#define ERROR_CODE_102D					0x102D
#define ERROR_CODE_102E					0x102E
#define ERROR_CODE_102F					0x102F
#define ERROR_CODE_1030					0x1030
#define ERROR_CODE_1031					0x1031
#define ERROR_CODE_1032					0x1032
#define ERROR_CODE_1033					0x1033
#define ERROR_CODE_1034					0x1034
#define ERROR_CODE_1035					0x1035
#define ERROR_CODE_1036					0x1036
#define ERROR_CODE_1037					0x1037
#define ERROR_CODE_1038					0x1038
#define ERROR_CODE_1039					0x1039
#define ERROR_CODE_103A					0x103A
#define ERROR_CODE_103B					0x103B
// #define ERROR_CODE_103C					0x103C
// #define ERROR_CODE_103D					0x103D
// #define ERROR_CODE_103E					0x103E
// #define ERROR_CODE_103F					0x103F

// ERROR_CODE_1038
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
