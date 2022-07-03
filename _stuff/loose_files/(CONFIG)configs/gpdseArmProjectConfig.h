/* =============================================================================
 * Project:		GPSDE++ ARM Library
 * File name:	gpdseArmProjectConfig.h
 * Module:		Project Configurations for GPDSE++ ARM Library project
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
//
// >>>>> >>>>> >>>>>      SYSTEM CONFIGURATION AREA      <<<<< <<<<< <<<<< <<<<<
// >>>>> >>>>> >>>>>             DO NOT EDIT             <<<<< <<<<< <<<<< <<<<<
//
// =============================================================================

// -----------------------------------------------------------------------------
// System preprocessor pragmas
// -----------------------------------------------------------------------------

#pragma anon_unions

// -----------------------------------------------------------------------------
// File build
// -----------------------------------------------------------------------------

#ifndef __GPDSE_ARM_PROJECT_CONFIG_H
#define __GPDSE_ARM_PROJECT_CONFIG_H			__BUILD_MACRO_TO_BE_CHANGED__

// -----------------------------------------------------------------------------
// System file dependencies
// -----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
//
// >>>>> >>>>> >>>>>   BEGIN - USER CONFIGURATION AREA   <<<<< <<<<< <<<<< <<<<<
// >>>>> >>>>> >>>>>     USER MAY EDIT  THIS SECTION     <<<<< <<<<< <<<<< <<<<<
//
// ATTENTION:
//		Options are indicated with commands to indicate how to edit this file:
//
//		- CUSTOM VALUE => user may change the value of the option
//
//		- ENABLE ONE => user must uncomment one option and comment the rest
//			of the group. Only the first enabled option will be considered, the
//			others options of the group will be ignored. A letter in parentesis
//			will indicate the elements of that group.
// =============================================================================

// -----------------------------------------------------------------------------
// User pragmas
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// User file dependencies
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Microcontroller selection
// -----------------------------------------------------------------------------

#define GPDSE_USE_DEVICE_STM32L053						// ENABLE ONE (A)
// #define GPDSE_USE_DEVICE_STM32F030						// ENABLE ONE (A)
// #define GPDSE_USE_DEVICE_STM32F091						// ENABLE ONE (A)

// -----------------------------------------------------------------------------
// Delays and timeouts
// -----------------------------------------------------------------------------

#define GPDSE_DEFAULT_DEBOUNCE_TIME				30
#define GPDSE_DEFAULT_SPI_TIMEOUT				30
#define GPDSE_DEFAULT_I2C_TIMEOUT				30
#define GPDSE_DEFAULT_UART_TIMEOUT				30

// -----------------------------------------------------------------------------
// Clock configuration - Base clock values
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_LSE					32768UL			// CUSTOM VALUE
#define GPDSE_CLOCK_LSI					37000UL			// CUSTOM VALUE
#define GPDSE_CLOCK_HSE					8000000UL		// CUSTOM VALUE
#define GPDSE_CLOCK_HSI16				16000000UL		// CUSTOM VALUE
#define GPDSE_CLOCK_HSI48				48000000UL		// CUSTOM VALUE

// #define GPDSE_CLOCK_MSI					65536UL			// ENABLE ONE (A)
// #define GPDSE_CLOCK_MSI					131072UL		// ENABLE ONE (A)
// #define GPDSE_CLOCK_MSI					262144UL		// ENABLE ONE (A)
// #define GPDSE_CLOCK_MSI					524288UL		// ENABLE ONE (A)
// #define GPDSE_CLOCK_MSI					1048576UL		// ENABLE ONE (A)
#define GPDSE_CLOCK_MSI					2097152UL		// ENABLE ONE (A)
// #define GPDSE_CLOCK_MSI					4194304UL		// ENABLE ONE (A)

// -----------------------------------------------------------------------------
// Clock configuration - System and PLL clock
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_SYSCLK_SOURCE_MSI					// ENABLE ONE (B)
// #define GPDSE_CLOCK_SYSCLK_SOURCE_HSI16					// ENABLE ONE (B)
// #define GPDSE_CLOCK_SYSCLK_SOURCE_HSE					// ENABLE ONE (B)
// #define GPDSE_CLOCK_SYSCLK_SOURCE_PLLCLK				// ENABLE ONE (B)

#define GPDSE_CLOCK_PLLCLK_SOURCE_HSE					// ENABLE ONE (C)
// #define GPDSE_CLOCK_PLLCLK_SOURCE_HSI16					// ENABLE ONE (C)

#define GPDSE_CLOCK_HSI16_DIVIDER				1		// ENABLE ONE (D)
// #define GPDSE_CLOCK_HSI16_DIVIDER				4		// ENABLE ONE (D)

// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			3		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			4		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			6		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			8		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			12		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			16		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			24		// ENABLE ONE (E)
#define GPDSE_CLOCK_PLLCLK_MULTIPLIER			32		// ENABLE ONE (E)
// #define GPDSE_CLOCK_PLLCLK_MULTIPLIER			48		// ENABLE ONE (E)

// #define GPDSE_CLOCK_PLLCLK_DIVIDER				2		// ENABLE ONE (F)
#define GPDSE_CLOCK_PLLCLK_DIVIDER				3		// ENABLE ONE (F)
// #define GPDSE_CLOCK_PLLCLK_DIVIDER				4		// ENABLE ONE (F)

// -----------------------------------------------------------------------------
// Clock configuration - RTC / LCD
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_RTC_LCD_SOURCE_HSE					// ENABLE ONE (G)
#define GPDSE_CLOCK_RTC_LCD_SOURCE_LSE					// ENABLE ONE (G)
#define GPDSE_CLOCK_RTC_LCD_SOURCE_LSI					// ENABLE ONE (G)

#define GPDSE_CLOCK_RTC_LCD_HSE_DIVIDER			2		// ENABLE ONE (H)
// #define GPDSE_CLOCK_RTC_LCD_HSE_DIVIDER			4		// ENABLE ONE (H)
// #define GPDSE_CLOCK_RTC_LCD_HSE_DIVIDER			8		// ENABLE ONE (H)
// #define GPDSE_CLOCK_RTC_LCD_HSE_DIVIDER			16		// ENABLE ONE (H)

// -----------------------------------------------------------------------------
// Clock configuration - USB / RNG
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_USB_RNG_SOURCE_PLLDIV2				// ENABLE ONE (I)
// #define GPDSE_CLOCK_USB_RNG_SOURCE_HSE					// ENABLE ONE (I)

#define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			1		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			2		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			4		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			8		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			16		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			32		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			64		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			128		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			256		// ENABLE ONE (J)
// #define GPDSE_CLOCK_HCLB_FCLK_DIVIDER			512		// ENABLE ONE (J)

// -----------------------------------------------------------------------------
// Clock configuration - Cortex clock
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_CORTEX_TIMER_DIVIDER			1		// ENABLE ONE (K)
// #define GPDSE_CLOCK_CORTEX_TIMER_DIVIDER			8		// ENABLE ONE (K)

// -----------------------------------------------------------------------------
// Clock configuration - Peripherals clocks
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_ABP1_DIVIDER				1		// ENABLE ONE (L)
// #define GPDSE_CLOCK_ABP1_DIVIDER				2		// ENABLE ONE (L)
// #define GPDSE_CLOCK_ABP1_DIVIDER				4		// ENABLE ONE (L)
// #define GPDSE_CLOCK_ABP1_DIVIDER				8		// ENABLE ONE (L)
// #define GPDSE_CLOCK_ABP1_DIVIDER				16		// ENABLE ONE (L)

#define GPDSE_CLOCK_ABP1_TIMER_MULTIPLIER		1		// ENABLE ONE (M)
// #define GPDSE_CLOCK_ABP1_TIMER_MULTIPLIER		2		// ENABLE ONE (M)

#define GPDSE_CLOCK_ABP2_DIVIDER				1		// ENABLE ONE (N)
// #define GPDSE_CLOCK_ABP2_DIVIDER				2		// ENABLE ONE (N)
// #define GPDSE_CLOCK_ABP2_DIVIDER				4		// ENABLE ONE (N)
// #define GPDSE_CLOCK_ABP2_DIVIDER				8		// ENABLE ONE (N)
// #define GPDSE_CLOCK_ABP2_DIVIDER				16		// ENABLE ONE (N)

#define GPDSE_CLOCK_ABP2_TIMER_MULTIPLIER		1		// ENABLE ONE (N)
// #define GPDSE_CLOCK_ABP2_TIMER_MULTIPLIER		2		// ENABLE ONE (N)

// -----------------------------------------------------------------------------
// Clock configuration - USART
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_USART1_SOURCE_APB2					// ENABLE ONE (O)
// #define GPDSE_CLOCK_USART1_SOURCE_SYSTEM				// ENABLE ONE (O)
// #define GPDSE_CLOCK_USART1_SOURCE_HSI16					// ENABLE ONE (O)
// #define GPDSE_CLOCK_USART1_SOURCE_LSE					// ENABLE ONE (O)

// #define GPDSE_CLOCK_USART2_SOURCE_SYSTEM				// ENABLE ONE (P)
// #define GPDSE_CLOCK_USART2_SOURCE_HSI16					// ENABLE ONE (P)
// #define GPDSE_CLOCK_USART2_SOURCE_LSE					// ENABLE ONE (P)
#define GPDSE_CLOCK_USART2_SOURCE_APB1					// ENABLE ONE (P)

#define GPDSE_CLOCK_LPUART_SOURCE_APB1					// ENABLE ONE (Q)
#define GPDSE_CLOCK_LPUART_SOURCE_LSE					// ENABLE ONE (Q)
#define GPDSE_CLOCK_LPUART_SOURCE_HSI16					// ENABLE ONE (Q)
#define GPDSE_CLOCK_LPUART_SOURCE_SYSTEM				// ENABLE ONE (Q)

// -----------------------------------------------------------------------------
// Clock configuration - LPTIMER
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_LPTIMER_SOURCE_LSI					// ENABLE ONE (R)
#define GPDSE_CLOCK_LPTIMER_SOURCE_HSI16				// ENABLE ONE (R)
#define GPDSE_CLOCK_LPTIMER_SOURCE_LSE					// ENABLE ONE (R)
#define GPDSE_CLOCK_LPTIMER_SOURCE_APB1					// ENABLE ONE (R)

// -----------------------------------------------------------------------------
// Clock configuration - I2C
// -----------------------------------------------------------------------------

#define GPDSE_CLOCK_I2C_SOURCE_APB1						// ENABLE ONE (S)
// #define GPDSE_CLOCK_I2C_SOURCE_HSI16					// ENABLE ONE (S)
// #define GPDSE_CLOCK_I2C_SOURCE_SYSTEM					// ENABLE ONE (S)





// =============================================================================
//
// >>>>> >>>>> >>>>>    END - USER CONFIGURATION AREA    <<<<< <<<<< <<<<< <<<<<
// >>>>> >>>>> >>>>>    DO NOT EDIT  BELOW THIS POINT    <<<<< <<<<< <<<<< <<<<<
//
// =============================================================================

// -----------------------------------------------------------------------------
// Microcontroller selection
// -----------------------------------------------------------------------------

// Declaring devices
#define GPDSE_MCU_STM32L053				1
#define GPDSE_MCU_STM32F030				2
#define GPDSE_MCU_STM32F091				3

// Selecting microcontroller
// STM32L053
#if defined(GPDSE_USE_DEVICE_STM32L053)
#	define GPDSE_MCU							GPDSE_MCU_STM32L053
#elif defined(GPDSE_USE_DEVICE_STM32F030)
#	define GPDSE_MCU							GPDSE_MCU_STM32F030
#elif defined(GPDSE_USE_DEVICE_STM32F091)
#	define GPDSE_MCU							GPDSE_MCU_STM32F091
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No device selected clock!
#endif

// -----------------------------------------------------------------------------
// Clock sources
// -----------------------------------------------------------------------------

// Gets system clock from MSI
#if defined(GPDSE_CLOCK_SYSCLK_SOURCE_MSI)
#	define GPDSE_CLOCK_SYSTEM					GPDSE_CLOCK_MSI
// Gets system clock from HSI16
#elif defined(GPDSE_CLOCK_SYSCLK_SOURCE_HSI16)
#	define GPDSE_CLOCK_SYSTEM					GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets system clock from HSE
#elif defined(GPDSE_CLOCK_SYSCLK_SOURCE_HSE)
#	define GPDSE_CLOCK_SYSTEM					GPDSE_CLOCK_HSE
// Gets system clock from PLL
#elif defined(GPDSE_CLOCK_SYSCLK_SOURCE_PLLCLK)
// Gets PLL clock from HSE
#	if defined(GPDSE_CLOCK_PLLCLK_SOURCE_HSE)
#		define GPDSE_CLOCK_PLLCLK_VALUE			GPDSE_CLOCK_HSE * GPDSE_CLOCK_PLLCLK_MULTIPLIER / GPDSE_CLOCK_PLLCLK_DIVIDER
// Gets PLL clock from HSI16
#	elif defined(GPDSE_CLOCK_PLLCLK_SOURCE_HSI16)
#		define GPDSE_CLOCK_PLLCLK_VALUE			(GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER) * GPDSE_CLOCK_PLLCLK_MULTIPLIER / GPDSE_CLOCK_PLLCLK_DIVIDER
#	else
// No PLL clock
#		warning	Verify your choices in "gpdseArmConfig.h" file!
#		error	No clock source defined for PLL clock!
#	endif
#	define GPDSE_CLOCK_SYSTEM					GPDSE_CLOCK_PLLCLK_VALUE
// No system clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for SYSTEM clock!
#endif

// Gets ADC clock from HSI16
#define GPDSE_CLOCK_ADC							GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER

// Gets USB/NRG clock from PLL (PLL_SOURCE * MULT / 2)
#if defined(GPDSE_CLOCK_USB_RNG_SOURCE_PLLDIV2)
#	define GPDSE_CLOCK_RNG						GPDSE_CLOCK_PLLCLK_VALUE * GPDSE_CLOCK_PLLCLK_DIVIDER / 2
#	define GPDSE_CLOCK_USB						GPDSE_CLOCK_PLLCLK_VALUE * GPDSE_CLOCK_PLLCLK_DIVIDER / 2
// Gets USB/NRG clock from HSE
#elif defined(GPDSE_CLOCK_USB_RNG_SOURCE_HSE)
#	define GPDSE_CLOCK_RNG						GPDSE_CLOCK_HSE
#	define GPDSE_CLOCK_USB						GPDSE_CLOCK_HSE
// No USB/NRG clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for USB/NRG clock!
#endif

// Gets RTC/LCD clock from HSE
#if defined(GPDSE_CLOCK_RTC_LCD_SOURCE_HSE)
#	define GPDSE_CLOCK_LCD						GPDSE_CLOCK_HSE / GPDSE_CLOCK_RTC_LCD_HSE_DIVIDER
#	define GPDSE_CLOCK_RTC						GPDSE_CLOCK_HSE / GPDSE_CLOCK_RTC_LCD_HSE_DIVIDER
// Gets RTC/LCD clock from HSI16
#elif defined(GPDSE_CLOCK_RTC_LCD_SOURCE_LSE)
#	define GPDSE_CLOCK_LCD						GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
#	define GPDSE_CLOCK_RTC						GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets RTC/LCD clock from LSI
#elif defined(GPDSE_CLOCK_SYSCLK_SOURCE_HSE)
#	define GPDSE_CLOCK_LCD						GPDSE_CLOCK_LSI
#	define GPDSE_CLOCK_RTC						GPDSE_CLOCK_LSI
// No RTC/LCD clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for RTC/LCD clock!
#endif

// Gets IWDG clock from LSI
#define GPDSE_CLOCK_IWDG						GPDSE_CLOCK_LSI

// Gets PWR clock from SYSTEM
#define GPDSE_CLOCK_PWR							GPDSE_CLOCK_SYSTEM

// Gets FCLK/HCLK clock from SYSTEM
#define GPDSE_CLOCK_FCLK						GPDSE_CLOCK_SYSTEM / GPDSE_CLOCK_HCLB_FCLK_DIVIDER
#define GPDSE_CLOCK_HCLK						GPDSE_CLOCK_SYSTEM / GPDSE_CLOCK_HCLB_FCLK_DIVIDER

// Gets FCLK/HCLK clock from HCLK
#define GPDSE_CLOCK_CORTEX_SYSTEM_TIMER			GPDSE_CLOCK_HCLK / GPDSE_CLOCK_CORTEX_TIMER_DIVIDER

// Gets ABP1/ABP2 Peripheral clock from HCLK
#define GPDSE_CLOCK_APB1_PERIPHERAL				GPDSE_CLOCK_HCLK / GPDSE_CLOCK_ABP1_DIVIDER
#define GPDSE_CLOCK_APB2_PERIPHERAL				GPDSE_CLOCK_HCLK / GPDSE_CLOCK_ABP2_DIVIDER

#define GPDSE_CLOCK_APB1_TIMER					GPDSE_CLOCK_APB1_PERIPHERAL * GPDSE_CLOCK_ABP1_TIMER_MULTIPLIER
#define GPDSE_CLOCK_APB2_TIMER					GPDSE_CLOCK_APB2_PERIPHERAL * GPDSE_CLOCK_ABP2_TIMER_MULTIPLIER

// Gets USART1 clock from APB2
#if defined(GPDSE_CLOCK_USART1_SOURCE_APB2)
#	define GPDSE_CLOCK_USART1					GPDSE_CLOCK_APB2_PERIPHERAL
// Gets USART1 clock from SYSTEM
#elif defined(GPDSE_CLOCK_USART1_SOURCE_SYSTEM)
#	define GPDSE_CLOCK_USART1					GPDSE_CLOCK_SYSTEM
// Gets USART1 clock from HSI16
#elif defined(GPDSE_CLOCK_USART1_SOURCE_HSI16)
#	define GPDSE_CLOCK_USART1					GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets USART1 clock from LSE
#elif defined(GPDSE_CLOCK_USART1_SOURCE_LSE)
#	define GPDSE_CLOCK_USART1					GPDSE_CLOCK_LSE
// No USART1 clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for USART1 clock!
#endif

// Gets USART2 clock from APB1
#if defined(GPDSE_CLOCK_USART2_SOURCE_APB1)
#	define GPDSE_CLOCK_USART2					GPDSE_CLOCK_APB1_PERIPHERAL
// Gets USART2 clock from SYSTEM
#elif defined(GPDSE_CLOCK_USART2_SOURCE_SYSTEM)
#	define GPDSE_CLOCK_USART2					GPDSE_CLOCK_SYSTEM
// Gets USART2 clock from HSI16
#elif defined(GPDSE_CLOCK_USART2_SOURCE_HSI16)
#	define GPDSE_CLOCK_USART2					GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets USART2 clock from LSE
#elif defined(GPDSE_CLOCK_USART2_SOURCE_LSE)
#	define GPDSE_CLOCK_USART2					GPDSE_CLOCK_LSE
// No USART2 clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for USART2 clock!
#endif

// Gets LPUART clock from APB1
#if defined(GPDSE_CLOCK_LPUART_SOURCE_APB1)
#	define GPDSE_CLOCK_LPUART					GPDSE_CLOCK_APB1_PERIPHERAL
// Gets LPUART clock from LSE
#elif defined(GPDSE_CLOCK_LPUART_SOURCE_LSE)
#	define GPDSE_CLOCK_LPUART					GPDSE_CLOCK_LSE
// Gets LPUART clock from HSI16
#elif defined(GPDSE_CLOCK_LPUART_SOURCE_HSI16)
#	define GPDSE_CLOCK_LPUART					GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets LPUART clock from SYSTEM
#elif defined(GPDSE_CLOCK_LPUART_SOURCE_SYSTEM)
#	define GPDSE_CLOCK_LPUART					GPDSE_CLOCK_SYSTEM
// No LPUART clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for LPUART clock!
#endif

// Gets LPTIMER clock from LSI
#if defined(GPDSE_CLOCK_LPTIMER_SOURCE_LSI)
#	define GPDSE_CLOCK_LPTIMER					GPDSE_CLOCK_LSI
// Gets LPTIMER clock from HSI16
#elif defined(GPDSE_CLOCK_LPTIMER_SOURCE_HSI16)
#	define GPDSE_CLOCK_LPTIMER					GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets LPTIMER clock from LSE
#elif defined(GPDSE_CLOCK_LPTIMER_SOURCE_LSE)
#	define GPDSE_CLOCK_LPTIMER					GPDSE_CLOCK_LSE
// Gets LPTIMER clock from APB1
#elif defined(GPDSE_CLOCK_LPTIMER_SOURCE_APB1)
#	define GPDSE_CLOCK_LPTIMER					GPDSE_CLOCK_APB1_PERIPHERAL
// No LPTIMER clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for LPTIMER clock!
#endif

// Gets I2C clock from APB1
#if defined(GPDSE_CLOCK_I2C_SOURCE_APB1)
#	define GPDSE_CLOCK_I2C					GPDSE_CLOCK_APB1_PERIPHERAL
// Gets I2C clock from HSI16
#elif defined(GPDSE_CLOCK_I2C_SOURCE_HSI16)
#	define GPDSE_CLOCK_I2C					GPDSE_CLOCK_HSI16 / GPDSE_CLOCK_HSI16_DIVIDER
// Gets LPUART clock from SYSTEM
#elif defined(GPDSE_CLOCK_I2C_SOURCE_SYSTEM)
#	define GPDSE_CLOCK_I2C					GPDSE_CLOCK_SYSTEM
// No I2C clock
#else
#	warning	Verify your choices in "gpdseArmConfig.h" file!
#	error	No clock source defined for I2C clock!
#endif

// -----------------------------------------------------------------------------
// MCU file dependencies
// -----------------------------------------------------------------------------

#if GPDSE_MCU == GPDSE_MCU_STM32L053
#	include "stm32l0xx_hal.h"
#	include "stm32l053xx.h"
#	include "stm32l0xx_hal_adc.h"
#	include "stm32l0xx_hal_adc_ex.h"
#	include "stm32l0xx_hal_comp.h"
#	include "stm32l0xx_hal_comp_ex.h"
#	include "stm32l0xx_hal_cortex.h"
#	include "stm32l0xx_hal_crc.h"
#	include "stm32l0xx_hal_crc_ex.h"
#	include "stm32l0xx_hal_cryp.h"
#	include "stm32l0xx_hal_cryp_ex.h"
#	include "stm32l0xx_hal_dac.h"
#	include "stm32l0xx_hal_dac_ex.h"
#	include "stm32l0xx_hal_def.h"
#	include "stm32l0xx_hal_dma.h"
#	include "stm32l0xx_hal_firewall.h"
#	include "stm32l0xx_hal_flash.h"
#	include "stm32l0xx_hal_flash_ex.h"
#	include "stm32l0xx_hal_flash_ramfunc.h"
#	include "stm32l0xx_hal_gpio.h"
#	include "stm32l0xx_hal_gpio_ex.h"
#	include "stm32l0xx_hal_i2c.h"
#	include "stm32l0xx_hal_i2c_ex.h"
#	include "stm32l0xx_hal_i2s.h"
#	include "stm32l0xx_hal_irda.h"
#	include "stm32l0xx_hal_irda_ex.h"
#	include "stm32l0xx_hal_iwdg.h"
#	include "stm32l0xx_hal_lcd.h"
#	include "stm32l0xx_hal_lptim.h"
#	include "stm32l0xx_hal_lptim_ex.h"
#	include "stm32l0xx_hal_pcd.h"
#	include "stm32l0xx_hal_pcd_ex.h"
#	include "stm32l0xx_hal_pwr.h"
#	include "stm32l0xx_hal_pwr_ex.h"
#	include "stm32l0xx_hal_rcc.h"
#	include "stm32l0xx_hal_rcc_ex.h"
#	include "stm32l0xx_hal_rng.h"
#	include "stm32l0xx_hal_rtc.h"
#	include "stm32l0xx_hal_rtc_ex.h"
#	include "stm32l0xx_hal_smartcard.h"
#	include "stm32l0xx_hal_smartcard_ex.h"
#	include "stm32l0xx_hal_smbus.h"
#	include "stm32l0xx_hal_spi.h"
#	include "stm32l0xx_hal_tim.h"
#	include "stm32l0xx_hal_tim_ex.h"
#	include "stm32l0xx_hal_tsc.h"
#	include "stm32l0xx_hal_uart.h"
#	include "stm32l0xx_hal_uart_ex.h"
#	include "stm32l0xx_hal_usart.h"
#	include "stm32l0xx_hal_usart.h"
#	include "stm32l0xx_hal_wwdg.h"
#elif GPDSE_MCU == GPDSE_MCU_STM32F030
#	include "stm32f030xx.h"
#	include "stm32f0xx_hal.h"
#elif GPDSE_MCU == GPDSE_MCU_STM32F091
#	include "stm32f091xx.h"
#	include "stm32f0xx_hal.h"
#endif

#endif // GPDSE_ARM_PROJECT_CONFIG_H
