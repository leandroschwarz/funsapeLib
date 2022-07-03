/**
  ******************************************************************************
  * @file    gpio.h
  * @author  Karolina Majstrovic
  * @version 
  * @date    09-May-2015
  * @brief   Header for timer.c module
  ******************************************************************************
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f091xc.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_nucleo.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GPIO_Init_Output(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);
void GPIO_On(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);
void GPIO_Off(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);
void GPIO_Init_Input(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);

