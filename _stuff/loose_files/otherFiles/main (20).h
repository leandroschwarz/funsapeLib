/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BUTTON_Pin GPIO_PIN_13
#define BUTTON_GPIO_Port GPIOC
#define LCD_D8_Pin GPIO_PIN_0
#define LCD_D8_GPIO_Port GPIOC
#define LCD_D9_Pin GPIO_PIN_1
#define LCD_D9_GPIO_Port GPIOC
#define LCD_D10_Pin GPIO_PIN_2
#define LCD_D10_GPIO_Port GPIOC
#define LCD_D11_Pin GPIO_PIN_3
#define LCD_D11_GPIO_Port GPIOC
#define ADC_ECG1_Pin GPIO_PIN_0
#define ADC_ECG1_GPIO_Port GPIOA
#define ADC_ECG2_Pin GPIO_PIN_1
#define ADC_ECG2_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define ADC_BAT_Pin GPIO_PIN_4
#define ADC_BAT_GPIO_Port GPIOA
#define ADC_CHARGER_Pin GPIO_PIN_5
#define ADC_CHARGER_GPIO_Port GPIOA
#define BLE_MISO_Pin GPIO_PIN_6
#define BLE_MISO_GPIO_Port GPIOA
#define BLE_MOSI_Pin GPIO_PIN_7
#define BLE_MOSI_GPIO_Port GPIOA
#define LCD_D12_Pin GPIO_PIN_4
#define LCD_D12_GPIO_Port GPIOC
#define LCD_D13_Pin GPIO_PIN_5
#define LCD_D13_GPIO_Port GPIOC
#define TOUCH_CLK_Pin GPIO_PIN_0
#define TOUCH_CLK_GPIO_Port GPIOB
#define TOUCH_DO_Pin GPIO_PIN_1
#define TOUCH_DO_GPIO_Port GPIOB
#define TOUCH_DI_Pin GPIO_PIN_2
#define TOUCH_DI_GPIO_Port GPIOB
#define TOUCH_CS_Pin GPIO_PIN_10
#define TOUCH_CS_GPIO_Port GPIOB
#define TOUCH_BUSY_Pin GPIO_PIN_11
#define TOUCH_BUSY_GPIO_Port GPIOB
#define TOUCH_IRQ_Pin GPIO_PIN_12
#define TOUCH_IRQ_GPIO_Port GPIOB
#define CARD_SCLK_Pin GPIO_PIN_13
#define CARD_SCLK_GPIO_Port GPIOB
#define CARD_MISO_Pin GPIO_PIN_14
#define CARD_MISO_GPIO_Port GPIOB
#define CARD_MOSI_Pin GPIO_PIN_15
#define CARD_MOSI_GPIO_Port GPIOB
#define LCD_D14_Pin GPIO_PIN_6
#define LCD_D14_GPIO_Port GPIOC
#define LCD_D15_Pin GPIO_PIN_7
#define LCD_D15_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_8
#define LCD_CS_GPIO_Port GPIOC
#define LCD_RD_Pin GPIO_PIN_9
#define LCD_RD_GPIO_Port GPIOC
#define BLE_RST_Pin GPIO_PIN_8
#define BLE_RST_GPIO_Port GPIOA
#define BLE_NS_Pin GPIO_PIN_9
#define BLE_NS_GPIO_Port GPIOA
#define BLE_CS_Pin GPIO_PIN_10
#define BLE_CS_GPIO_Port GPIOA
#define BLE_IRQ_Pin GPIO_PIN_11
#define BLE_IRQ_GPIO_Port GPIOA
#define CARD_CS_Pin GPIO_PIN_12
#define CARD_CS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LCD_BACK_Pin GPIO_PIN_15
#define LCD_BACK_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_10
#define LCD_RS_GPIO_Port GPIOC
#define LCD_WR_Pin GPIO_PIN_11
#define LCD_WR_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOC
#define CHARGER_CS_Pin GPIO_PIN_2
#define CHARGER_CS_GPIO_Port GPIOD
#define BLE_SCLK_Pin GPIO_PIN_3
#define BLE_SCLK_GPIO_Port GPIOB
#define MPU1_CS_Pin GPIO_PIN_4
#define MPU1_CS_GPIO_Port GPIOB
#define MPU2_CS_Pin GPIO_PIN_5
#define MPU2_CS_GPIO_Port GPIOB
#define MPU3_CS_Pin GPIO_PIN_6
#define MPU3_CS_GPIO_Port GPIOB
#define MPU4_CS_Pin GPIO_PIN_7
#define MPU4_CS_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
