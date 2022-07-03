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
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void blinkLed(void);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum errorCode_e
{
    // General error codes
    ERROR_CODE_NONE										= 0x0000,

    // ADC related error codes
    ERROR_CODE_ADC_UNKNOWN								= 0x0010,
    ERROR_CODE_ADC_CALIBRATION_FAILED					= 0x0011,
    ERROR_CODE_ADC_START_POOLING_FAILED					= 0x0012,
    ERROR_CODE_ADC_START_INTERRUPT_FAILED				= 0x0013,
    ERROR_CODE_ADC_START_DMA_FAILED						= 0x0014,

    // TIMER related error codes
    ERROR_CODE_TIMER_UNKNOWN							= 0x0020,
    ERROR_CODE_TIMER_BASETIME_START_POOLING_FAILED		= 0x0021,
    ERROR_CODE_TIMER_BASETIME_START_INTERRUPT_FAILED	= 0x0022,

    // UART related error codes
    ERROR_CODE_UART_UNKNOWN								= 0x0030,
    ERROR_CODE_UART_TRANSMIT_POLLING_FAILED				= 0x0031,
    ERROR_CODE_UART_TRANSMIT_INTERRUPT_FAILED			= 0x0032,
    ERROR_CODE_UART_TRANSMIT_DMA_FAILED					= 0x0033,
    ERROR_CODE_UART_RECEIVE_POLLING_FAILED				= 0x0034,
    ERROR_CODE_UART_RECEIVE_INTERRUPT_FAILED			= 0x0035,
    ERROR_CODE_UART_RECEIVE_DMA_FAILED					= 0x0036,

    // Unknow error - Ops... Something really bad happened, and we don't have a
    // 		clue of what the hell caused it! Good luck, bro!
    ERROR_CODE_GENERAL_UNKNOWN							= 0xFFFF
} errorCode_e;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define ADC_CHANNELS_PER_SAMPLE			8
#define ADC_SAMPLES_PER_BUFFER			10		// Must be an even number
#define ADC_CHANNELS_PER_BUFFER			(ADC_CHANNELS_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER)
#define ADC_DMA_BUFFER_ELEMENTS			ADC_CHANNELS_PER_BUFFER

#define UART1_RX_DMA_BUFFER_ELEMENTS	40		// Must be an even number
#define UART1_TX_DMA_BUFFER_ELEMENTS	50		// Must be an even number
#define UART2_RX_DMA_BUFFER_ELEMENTS	20		// Must be an even number
#define UART2_TX_DMA_BUFFER_ELEMENTS	50		// Must be an even number
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define showErrorStatus(code)				printErrorStatus(code, (uint8_t *)__FILE__, __LINE__)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void printErrorStatus(uint16_t errorCode, uint8_t* file, uint32_t line);

/* USER CODE BEGIN EFP */

extern uint16_t adcDmaBuffer[ADC_DMA_BUFFER_ELEMENTS];
extern char uart1RxDmaBuffer[UART1_RX_DMA_BUFFER_ELEMENTS];
extern char uart1TxDmaBuffer[UART1_TX_DMA_BUFFER_ELEMENTS];
//	extern char uart2RxDmaBuffer[UART2_RX_DMA_BUFFER_ELEMENTS];
//	extern char uart2TxDmaBuffer[UART2_TX_DMA_BUFFER_ELEMENTS];
extern volatile errorCode_e errorCode;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NLED_Pin GPIO_PIN_13
#define NLED_GPIO_Port GPIOC
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_15
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_8
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_9
#define LED_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
