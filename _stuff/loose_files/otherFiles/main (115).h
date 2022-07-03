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
#define BUTTON_EXTI_IRQn EXTI4_15_IRQn
#define LCD_DATA8_Pin GPIO_PIN_0
#define LCD_DATA8_GPIO_Port GPIOC
#define LCD_DATA9_Pin GPIO_PIN_1
#define LCD_DATA9_GPIO_Port GPIOC
#define LCD_DATA10_Pin GPIO_PIN_2
#define LCD_DATA10_GPIO_Port GPIOC
#define LCD_DATA11_Pin GPIO_PIN_3
#define LCD_DATA11_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LCD_DATA12_Pin GPIO_PIN_4
#define LCD_DATA12_GPIO_Port GPIOC
#define LCD_DATA13_Pin GPIO_PIN_5
#define LCD_DATA13_GPIO_Port GPIOC
#define LCD_DATA0_Pin GPIO_PIN_0
#define LCD_DATA0_GPIO_Port GPIOB
#define LCD_DATA1_Pin GPIO_PIN_1
#define LCD_DATA1_GPIO_Port GPIOB
#define LCD_DATA2_Pin GPIO_PIN_2
#define LCD_DATA2_GPIO_Port GPIOB
#define SN74HC595_SI_Pin GPIO_PIN_11
#define SN74HC595_SI_GPIO_Port GPIOB
#define SN74HC595_SRCLK_Pin GPIO_PIN_12
#define SN74HC595_SRCLK_GPIO_Port GPIOB
#define SN74HC595_RCLK_Pin GPIO_PIN_13
#define SN74HC595_RCLK_GPIO_Port GPIOB
#define LCD_DATA14_Pin GPIO_PIN_6
#define LCD_DATA14_GPIO_Port GPIOC
#define LCD_DATA15_Pin GPIO_PIN_7
#define LCD_DATA15_GPIO_Port GPIOC
#define LCD_TOUCH_CS_Pin GPIO_PIN_8
#define LCD_TOUCH_CS_GPIO_Port GPIOC
#define LCD_TOUCH_CLK_Pin GPIO_PIN_9
#define LCD_TOUCH_CLK_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_8
#define LCD_RS_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_11
#define LCD_RW_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LCD_RST_Pin GPIO_PIN_15
#define LCD_RST_GPIO_Port GPIOA
#define LCD_TOUCH_DO_Pin GPIO_PIN_10
#define LCD_TOUCH_DO_GPIO_Port GPIOC
#define LCD_TOUCH_DI_Pin GPIO_PIN_11
#define LCD_TOUCH_DI_GPIO_Port GPIOC
#define LCD_TOUCH_IRQ_Pin GPIO_PIN_12
#define LCD_TOUCH_IRQ_GPIO_Port GPIOC
#define LCD_TOUCH_IRQ_EXTI_IRQn EXTI4_15_IRQn
#define LCD_CSD2_Pin GPIO_PIN_2
#define LCD_CSD2_GPIO_Port GPIOD
#define LCD_DATA3_Pin GPIO_PIN_3
#define LCD_DATA3_GPIO_Port GPIOB
#define LCD_DATA4_Pin GPIO_PIN_4
#define LCD_DATA4_GPIO_Port GPIOB
#define LCD_DATA5_Pin GPIO_PIN_5
#define LCD_DATA5_GPIO_Port GPIOB
#define LCD_DATA6_Pin GPIO_PIN_6
#define LCD_DATA6_GPIO_Port GPIOB
#define LCD_DATA7_Pin GPIO_PIN_7
#define LCD_DATA7_GPIO_Port GPIOB

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
