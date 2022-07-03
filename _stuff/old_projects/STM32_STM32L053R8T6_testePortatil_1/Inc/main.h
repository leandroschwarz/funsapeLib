/* -----------------------------------------------------------------------------
 * File:			main.h
 * Author:			Leandro Schwarz
 * Last edition:	May 1, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Include guard ---------------------------------------------------------------

#ifndef __MAIN_H__
#define __MAIN_H__

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "stm32l0xx_hal.h"

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

// ADC defines
#define ADC0_GPIO_Port			GPIOA
#define ADC0_Pin				GPIO_PIN_0
#define ADC1_GPIO_Port			GPIOA
#define ADC1_Pin				GPIO_PIN_1
#define ADC2_GPIO_Port			GPIOB
#define ADC2_Pin				GPIO_PIN_0
#define ADC3_GPIO_Port			GPIOC
#define ADC3_Pin				GPIO_PIN_0
#define ADC4_GPIO_Port			GPIOC
#define ADC4_Pin				GPIO_PIN_1
#define ADC5_GPIO_Port			GPIOC
#define ADC5_Pin				GPIO_PIN_2
#define ADC6_GPIO_Port			GPIOC
#define ADC6_Pin				GPIO_PIN_3

// BUTTON defines
#define B1_GPIO_Port			GPIOC
#define B1_Pin					GPIO_PIN_13

// DAC defines
#define DAC_GPIO_Port			GPIOA
#define DAC_Pin					GPIO_PIN_4

// USART defines
#define USART_TX_GPIO_Port		GPIOA
#define USART_TX_Pin			GPIO_PIN_2
#define USART_RX_GPIO_Port		GPIOA
#define USART_RX_Pin			GPIO_PIN_3

// SPI defines
#define SPI1_SCLK_GPIO_Port		GPIOA
#define SPI1_SCLK_Pin			GPIO_PIN_5
#define SPI1_MISO_GPIO_Port		GPIOA
#define SPI1_MISO_Pin			GPIO_PIN_6
#define SPI1_SS0_GPIO_Port		GPIOC
#define SPI1_SS0_Pin			GPIO_PIN_9
#define SPI1_SS1_GPIO_Port		GPIOC
#define SPI1_SS1_Pin			GPIO_PIN_8
#define SPI1_SS2_GPIO_Port		GPIOB
#define SPI1_SS2_Pin			GPIO_PIN_8
#define SPI1_SS3_GPIO_Port		GPIOC
#define SPI1_SS3_Pin			GPIO_PIN_6
#define SPI1_SS4_GPIO_Port		GPIOB
#define SPI1_SS4_Pin			GPIO_PIN_9
#define SPI1_SS5_GPIO_Port		GPIOC
#define SPI1_SS5_Pin			GPIO_PIN_5
#define SPI1_SS6_GPIO_Port		GPIOC
#define SPI1_SS6_Pin			GPIO_PIN_7

// DEBUGWIRE defines
#define TMS_GPIO_Port			GPIOA
#define TMS_Pin					GPIO_PIN_13
#define TCK_GPIO_Port			GPIOA
#define TCK_Pin					GPIO_PIN_14

// Use of full asserts
// #define USE_FULL_ASSERT			1U

// -----------------------------------------------------------------------------
// Macro-functions definitions -------------------------------------------------

#define Error_Handler()			_Error_Handler(__FILE__, __LINE__)

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------


// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

#endif
