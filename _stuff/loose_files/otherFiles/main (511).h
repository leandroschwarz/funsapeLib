/*
  ******************************************************************************
  * File Name          : main.h
  * Description        : Header for main.c file.
  *                      This file contains the common defines of the application.
  * COPYRIGHT(c) 2018 STMicroelectronics
  ******************************************************************************
*/

// -----------------------------------------------------------------------------
// Include guard

#ifndef __MAIN_H__
#define __MAIN_H__

// -----------------------------------------------------------------------------
// Header files

#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"

#pragma anon_unions

// -----------------------------------------------------------------------------
// Constant definitions

// Button
#define BUTTON_PIN			GPIO_PIN_13
#define BUTTON_GPIO			GPIOC
// LED
#define LED_PIN				GPIO_PIN_5
#define LED_GPIO			GPIOA
// USART
#define USART_TX_Pin		GPIO_PIN_2
#define USART_TX_GPIO_Port	GPIOA
#define USART_RX_Pin		GPIO_PIN_3
#define USART_RX_GPIO_Port	GPIOA
// DEBUGWIRE
#define TMS_Pin				GPIO_PIN_13
#define TMS_GPIO_Port		GPIOA
#define TCK_Pin				GPIO_PIN_14
#define TCK_GPIO_Port		GPIOA

// -----------------------------------------------------------------------------
// Macro-function definitions

#define Error_Handler()			_Error_Handler(__FILE__, __LINE__)
#define setIoPin(gpio, pin)		HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
#define clrIoPin(gpio, pin)		HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
#define cplIoPin(gpio, pin)		HAL_GPIO_TogglePin(gpio, pin);

// -----------------------------------------------------------------------------
// New data types

typedef enum bool_t {
	FALSE	= 0,
	TRUE	= 1
} bool_t;

typedef union lsSystemConfigFlags_f {
	struct {
		bool_t		gpioClockAEnabled	: 1;
		bool_t		gpioClockBEnabled	: 1;
		bool_t		gpioClockCEnabled	: 1;
		bool_t		gpioClockDEnabled	: 1;
		bool_t		gpioClockHEnabled	: 1;
	};
	uint32_t allFlags;
} lsSystemConfigFlags_f;

typedef enum lsGpioPinNumber_e {
	LS_GPIO_PIN_0		= ((uint16_t)0x0001U),
	LS_GPIO_PIN_1		= ((uint16_t)0x0002U),
	LS_GPIO_PIN_2		= ((uint16_t)0x0004U),
	LS_GPIO_PIN_3		= ((uint16_t)0x0008U),
	LS_GPIO_PIN_4		= ((uint16_t)0x0010U),
	LS_GPIO_PIN_5		= ((uint16_t)0x0020U),
	LS_GPIO_PIN_6		= ((uint16_t)0x0040U),
	LS_GPIO_PIN_7		= ((uint16_t)0x0080U),
	LS_GPIO_PIN_8		= ((uint16_t)0x0100U),
	LS_GPIO_PIN_9		= ((uint16_t)0x0200U),
	LS_GPIO_PIN_10		= ((uint16_t)0x0400U),
	LS_GPIO_PIN_11		= ((uint16_t)0x0800U),
	LS_GPIO_PIN_12		= ((uint16_t)0x1000U),
	LS_GPIO_PIN_13		= ((uint16_t)0x2000U),
	LS_GPIO_PIN_14		= ((uint16_t)0x4000U),
	LS_GPIO_PIN_15		= ((uint16_t)0x8000U),
	LS_GPIO_PIN_All		= ((uint16_t)0xFFFFU)
} lsGpioPinNumber_e;

typedef enum lsGpioPinMode_e {
	LS_GPIO_MODE_INPUT					= ((uint32_t)0x00000000U),
	LS_GPIO_MODE_OUTPUT_PP				= ((uint32_t)0x00000001U),
	LS_GPIO_MODE_OUTPUT_OD				= ((uint32_t)0x00000011U),
	LS_GPIO_MODE_AF_PP					= ((uint32_t)0x00000002U),
	LS_GPIO_MODE_AF_OD					= ((uint32_t)0x00000012U),
	LS_GPIO_MODE_ANALOG					= ((uint32_t)0x00000003U),
	LS_GPIO_MODE_IT_RISING				= ((uint32_t)0x10110000U),
	LS_GPIO_MODE_IT_FALLING				= ((uint32_t)0x10210000U),
	LS_GPIO_MODE_IT_RISING_FALLING		= ((uint32_t)0x10310000U),
	LS_GPIO_MODE_EVT_RISING				= ((uint32_t)0x10120000U),
	LS_GPIO_MODE_EVT_FALLING			= ((uint32_t)0x10220000U),
	LS_GPIO_MODE_EVT_RISING_FALLING		= ((uint32_t)0x10320000U)
} lsGpioPinMode_e;

typedef enum lsGpioPinSpeed_e {
	LS_GPIO_SPEED_FREQ_LOW			= ((uint32_t)0x00000000U),
	LS_GPIO_SPEED_FREQ_MEDIUM		= ((uint32_t)0x00000001U),
	LS_GPIO_SPEED_FREQ_HIGH			= ((uint32_t)0x00000002U),
	LS_GPIO_SPEED_FREQ_VERY_HIGH	= ((uint32_t)0x00000003U)
} lsGpioPinSpeed_e;

typedef enum lsGpioPinPullUp_e {
	LS_GPIO_NOPULL				= ((uint32_t)0x00000000U),
	LS_GPIO_PULLUP				= ((uint32_t)0x00000001U),
	LS_GPIO_PULLDOWN			= ((uint32_t)0x00000002U)
} lsGpioPinPullUp_e;

typedef enum lsGpioPort_e {
	LS_GPIOA			= 0,
	LS_GPIOB			= 1,
	LS_GPIOC			= 2,
	LS_GPIOD			= 3,
	LS_GPIOH			= 4,
} lsGpioPort_e;

// -----------------------------------------------------------------------------
// Function declarations

void SysTick_Handler(void);
void HAL_MspInit(void);
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle);
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle);
void SystemClock_Config(void);
void _Error_Handler(char *, int);

void LS_GPIOA_Clock_Enable(void);
void LS_GPIOB_Clock_Enable(void);
void LS_GPIOC_Clock_Enable(void);
void LS_GPIOD_Clock_Enable(void);
void LS_GPIOH_Clock_Enable(void);
void LS_GPIO_Config_Pin(GPIO_TypeDef * port, lsGpioPinNumber_e pin, lsGpioPinMode_e mode, lsGpioPinPullUp_e pull, lsGpioPinSpeed_e speed);
void LS_UART_Config(UART_HandleTypeDef * huart);

// -----------------------------------------------------------------------------
// Extern global variables

extern lsSystemConfigFlags_f lsSystemConfigFlags;



#endif // END_#ifndef __MAIN_H__
