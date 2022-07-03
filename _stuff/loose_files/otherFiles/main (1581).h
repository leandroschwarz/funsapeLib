/**
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/Inc/main.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    05-Dec-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_nucleo.h"
#include "gpio.h"
#include "timer.h"
#include "AM2303.h"
#include "i2c.h"

/* Exported types ------------------------------------------------------------*/
extern volatile int TIM_Status;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Definition for GPIO Output */
#define LED_CLK_ENABLE()							 __GPIOA_CLK_ENABLE()
#define LED_PORT 											 GPIOA
#define LED_PIN												 GPIO_PIN_1
/* Definition for GPIO Input */
#define BUTTON_PORT 									 GPIOA
#define BUTTON_PIN										 GPIO_PIN_0
#define BUTTON_IRQn										 EXTI0_1_IRQn

/* Definition for AM2303 (temperature and humidity) sensor */
#define AM2303_CLK_ENABLE() 					 __GPIOA_CLK_ENABLE()
#define AM2303_PORT 									 GPIOA
#define AM2303_PIN										 GPIO_PIN_1
#define AM2303_IRQn										 EXTI0_1_IRQn


/* Definition for TIMx clock resources */
#define TIMx                           TIM2
#define TIMx_CLK_ENABLE()              __TIM2_CLK_ENABLE()		
/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM2_IRQn
#define TIMx_IRQHandler                TIM2_IRQHandler

/* Definition for I2Cx clock resources */
#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_CLK_ENABLE()               __I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()              __I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_6
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_7
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF1_I2C1

/* Definition for I2Cx's NVIC */
#define I2Cx_IRQn                        I2C1_IRQn
#define I2Cx_IRQHandler                  I2C1_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)

/* Exported functions ------------------------------------------------------- */

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
