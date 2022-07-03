/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Karolina Majstrovic
  * @version 
  * @date    09-May-2015
  * @brief   Functions for fast init of I2C communication.
  ******************************************************************************
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f091xc.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_nucleo.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_i2c_ex.h"
#include "main.h"

void I2C_Init();
void I2C_Send();
