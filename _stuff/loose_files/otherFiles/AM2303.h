/**
  ******************************************************************************
  * @file    AM2303.h
  * @author  Karolina Majstrovic
  * @version 
  * @date    20-May-2015
  * @brief   Header for AM2303 (temperature and humidity) sensor.
  ******************************************************************************
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f091xc.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_nucleo.h"
#include "stm32f0xx_it.h"
#include "gpio.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void AM2303_Init(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);
void AM2303_Measure(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);