/**
  ******************************************************************************
  * @file    BH1750FVI.h
  * @author  Karolina Majstrovic
  * @version 
  * @date    09-May-2015
  * @brief   Functions for BH1750FVI sensor.
  ******************************************************************************
  ******************************************************************************
  */
#ifndef __BH1750FVI_H
#define __BH1750FVI_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#define POWER_DOWN                       0b00000000  //No active state.
#define POWER_ON                         0b00000001  // Waiting for measurement command.
#define RESET                            0b00000111 // Reset Data register value. Reset command is not acceptable in Power Down mode.
#define CONTINUOUSLY_HRESOLUTION_MODE    0b00010000 // Start measurement at 1lx resolution. Measurement Time is typically 120ms.
#define CONTINUOUSLY_HRESOLUTION_MODE2   0b00010001 // Start measurement at 0.5lx resolution. Measurement Time is typically 120ms.
#define CONTINUOUSLY_LRESOLUTION_MODE    0b00010011 // Start measurement at 4lx resolution. Measurement Time is typically 16ms.
#define ONE_TIME_H_RESOLUTION_MODE       0b00100000 // Start measurement at 1lx resolution. Measurement Time is typically 120ms. It is automatically set to Power Down mode after measurement.
#define ONE_TIME_H_RESOLUTION_MODE2      0b00100001 // Start measurement at 0.5lx resolution. Measurement Time is typically 120ms. It is automatically set to Power Down mode after measurement.
#define ONE_TIME_L_RESOLUTION_MODE       0b00100011 // Start measurement at 4lx resolution. Measurement Time is typically 16ms. It is automatically set to Power Down mode after measurement. 
#define CHANGE_MEASUREMENT_TIME_HIGH_BIT 0b01000000 // Change measurement time. Please refer "adjust measurement result for influence of optical window." Last three bits should be defined with or operator
#define CHANGE_MASUREMENT_TIME_LOW_BIT   0b01100000 // Change measurement time. Please refer "adjust measurement result for influence of optical window." Last five bits should be define with or operator.

/* Peripheral Control functions  **********************************************/
	 
	 
#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_HAL_H */
