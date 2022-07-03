/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	projectConfig.hpp
 * Module:		System definitions for GPDSE++ Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

// Include guard
#ifndef GPDSE_PROJECT_CONFIG_HPP
#define GPDSE_PROJECT_CONFIG_HPP		 1002

// =============================================================================
// User may edit this section
// BEGIN - USER EDITABLE AREA
// =============================================================================

// -----------------------------------------------------------------------------
// System clock and delay ------------------------------------------------------

#define GPDSE_CPU_CLOCK							16000000UL
#define GPDSE_DEBOUNCE_TIME						20

// -----------------------------------------------------------------------------
// Microcontroller selection ---------------------------------------------------

#define GPDSE_CURRENT_DEVICE_ATMEGA_48x			0
#define GPDSE_CURRENT_DEVICE_ATMEGA_88x			0
#define GPDSE_CURRENT_DEVICE_ATMEGA_168x		0
#define GPDSE_CURRENT_DEVICE_ATMEGA_328x		0
#define GPDSE_CURRENT_DEVICE_ATMEGA_2560		0
#define GPDSE_CURRENT_DEVICE_STM32L053			1

// -----------------------------------------------------------------------------
// Debug configuration ---------------------------------------------------------

// Modules selection
#define GPDSE_DEBUG_MODULE_ALL					0
#define GPDSE_DEBUG_MODULE_CIRCULAR_BUFFER_8	0
#define GPDSE_DEBUG_MODULE_DATA_TIME			0
#define GPDSE_DEBUG_MODULE_PACKAGE_API			0

// Debug mode
#define GPDSE_DEBUG_MODE_SERIAL					1

// =============================================================================
// END - USER EDITABLE AREA
// From this point on, don't change anything
// =============================================================================

// *INDENT-OFF*

// -----------------------------------------------------------------------------
// System configuration --------------------------------------------------------

#if GPDSE_CPU_CLOCK > 4294967295UL
#	error	Error 1 - System frequency is too high.
#endif

// -----------------------------------------------------------------------------
// Microcontroller selection ---------------------------------------------------

// Selecting microcontroller
	// ATmega328
#if GPDSE_CURRENT_DEVICE_ATMEGA_48x == 1
#	if defined(GPDSE_MCU)
#		define GPDSE_MCU_ERROR_CONFLIC
#	endif
#	define GPDSE_MCU							1
#endif
#if GPDSE_CURRENT_DEVICE_ATMEGA_88x == 1
#	if defined(GPDSE_MCU)
#		define GPDSE_MCU_ERROR_CONFLIC
#	endif
#	define GPDSE_MCU							2
#endif
#if GPDSE_CURRENT_DEVICE_ATMEGA_168x == 1
#	if defined(GPDSE_MCU)
#		define GPDSE_MCU_ERROR_CONFLIC
#	endif
#	define GPDSE_MCU							3
#endif
#if GPDSE_CURRENT_DEVICE_ATMEGA_328x == 1
#	if defined(GPDSE_MCU)
#		define GPDSE_MCU_ERROR_CONFLIC
#	endif
#	define GPDSE_MCU							4
#endif
#if GPDSE_CURRENT_DEVICE_ATMEGA_2560 == 1
#	if defined(GPDSE_MCU)
#		define GPDSE_MCU_ERROR_CONFLIC
#	endif
#	define GPDSE_MCU							5
#endif

#if GPDSE_CURRENT_DEVICE_STM32L053 == 1
#	if defined(GPDSE_MCU)
#		define GPDSE_MCU_ERROR_CONFLIC
#	endif
#	define GPDSE_MCU							501
#endif

	// Verify microcontroller selection
#if !defined(GPDSE_MCU)
#	warning	Verify your selection in "Microcontroller Selection" section!
#	error	Error 2 - No device selected.
#endif
#if defined(GPDSE_MCU_ERROR_CONFLIC)
#	warning	Verify your selection in "Microcontroller Selection" section!
#	error	Error 3 - More than one device are selected!
#endif


// Platform
#if GPDSE_MCU < 500
#	define GPDSE_PLATFORM_AVR8
#elif GPDSE_MCU < 1000
#	define GPDSE_PLATFORM_STM32
#else
#	warning	The file must be outdated or corrupted!
#	error	Error 4 - The selected microcontroller is not supported!
#endif

// Debug checking
#if GPDSE_DEBUG_MODULE_ALL || GPDSE_DEBUG_MODULE_CIRCULAR_BUFFER_8 ||	\
	GPDSE_DEBUG_MODULE_DATA_TIME || GPDSE_DEBUG_MODULE_PACKAGE_API
#	if !GPDSE_DEBUG_MODE_SERIAL
#		warning	Verify your choices in "Debug Configuration" section!
#		error	Error 5 - Debug is enabled, but no output method is selected!
#	endif
#endif

// Header files
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(GPDSE_PLATFORM_AVR8)
#	include <avr/io.h>
#elif defined(GPDSE_PLATFORM_STM32)
#	if (GPDSE_MCU > 500) && (GPDSE_MCU < 600)
#		include "stm32l0xx_hal.h"
#	elif  (GPDSE_MCU > 600) && (GPDSE_MCU < 700)
#		include "stm32f0xx_hal.h"
#	elif  (GPDSE_MCU > 700) && (GPDSE_MCU < 800)
#		include "stm32f1xx_hal.h"
#	elif  (GPDSE_MCU > 800) && (GPDSE_MCU < 900)
#		include "stm32f4xx_hal.h"
#	elif  (GPDSE_MCU > 900) && (GPDSE_MCU < 1000)
#		include "stm32f7xx_hal.h"
#	endif
#endif

// Debug - New data types
typedef enum debFunctionId_e{
	// main
	DEB_FUN_INIT_SETUP								= 0,
	DEB_FUN_MAIN_LOOP								= 1,
	// Circular buffer 8
	// Date time
	// Package API
	DEB_FUN_PACKAGE_API__CONSTRUCTOR				= 0,
	DEB_FUN_PACKAGE_API_CHECK_INITIALIZED			= 1,
	DEB_FUN_PACKAGE_API_CONFIG_CHAR_SET_1			= 2,
	DEB_FUN_PACKAGE_API_CONFIG_CHAR_SET_2			= 3,
	DEB_FUN_PACKAGE_API_CONFIG_CHAR_SET_3			= 4,
	DEB_FUN_PACKAGE_API_GET_LAST_ERROR				= 5,
	DEB_FUN_PACKAGE_API_RX_ADD_DATA					= 6,
	DEB_FUN_PACKAGE_API_RX_FREE_PACKAGE				= 7,
	DEB_FUN_PACKAGE_API_RX_GET_DESTINATION_ADDRESS	= 8,
	DEB_FUN_PACKAGE_API_RX_GET_ID					= 9,
	DEB_FUN_PACKAGE_API_RX_GET_PAYLOAD				= 10,
	DEB_FUN_PACKAGE_API_RX_GET_SOURCE_ADDRESS		= 11,
	DEB_FUN_PACKAGE_API_RX_IS_READY					= 12,
	DEB_FUN_PACKAGE_API_RX_PROCESS_ESCAPE_CHARACTER	= 13,
	DEB_FUN_PACKAGE_API_RX_PROCESS_FRAME_FORMAT		= 14,
	DEB_FUN_PACKAGE_API_RX_PROCESS_NEXT_STATE		= 15,
	DEB_FUN_PACKAGE_API_RX_PROCESS_RAW_ADD_DATA		= 16,
	DEB_FUN_PACKAGE_API_RX_RESET_PACKAGE			= 17,
	DEB_FUN_PACKAGE_API_TX_ADD_DATA					= 18,
	DEB_FUN_PACKAGE_API_TX_CREATE_PACKAGE			= 19,
	DEB_FUN_PACKAGE_API_TX_ENCODE_PACKAGE			= 20,
	DEB_FUN_PACKAGE_API_TX_SET_DESTINATION_ADDRESS	= 21,
	DEB_FUN_PACKAGE_API_TX_SET_ID					= 22,
	DEB_FUN_PACKAGE_API_TX_SET_PAYLOAD				= 23,
	DEB_FUN_PACKAGE_API_TX_SET_SOURCE_ADDRESS		= 24,
} debFunctionId_e;

typedef enum debModule_e {
	DEB_MOD_MAIN				= 0,
	DEB_MOD_CIRCULAR_BUFFER_8	= 1,
	DEB_MOD_DATE_TIME			= 2,
	DEB_MOD_PACKAGE_API			= 3
} debModule_e;

// Debug - External global variables

// Debug - Macro-functions
#if defined(GPDSE_PLATFORM_STM32)
#	if GPDSE_DEBUG_MODE_SERIAL == 1
#		define debugSendMessage()												\
			uartSafeSend(&huart2, debBuf, 35)
#	endif
#elif defined(GPDSE_PLATFORMA_AVR8)
#	if GPDSE_DEBUG_MODE_SERIAL == 1
#		define debugSendMessage()												\
			usartTransmit(debBuf, 35)
#	endif
#endif

#define debugPackageApi(watchPoint)												\
	do{																			\
		debBuf[0] = debModule;													\
		debBuf[1] = debFunctionId;												\
		debBuf[2] = watchPoint;													\
		debBuf[3] = this->initialized_;											\
		debBuf[4] = this->lastError_;											\
		debBuf[5] = wordHighByte(this->rawMaxSize_);							\
		debBuf[6] = wordLowByte(this->rawMaxSize_);								\
		debBuf[7] = this->payloadMaxSize_;										\
		debBuf[8] = this->configEscapeEnabled_;									\
		debBuf[9] = this->configStartDelimiter_;								\
		debBuf[10] = this->configEscapeCharacter_;								\
		debBuf[11] = this->configEscapeXorDecoded_;								\
		debBuf[12] = this->configEscapeCharSize_;								\
		debBuf[13] = this->rxRawEscape_;										\
		debBuf[14] = this->rxRawReady_;											\
		debBuf[15] = this->rxRawState_;											\
		debBuf[16] = wordHighByte(this->rxRawSize_);							\
		debBuf[17] = wordLowByte(this->rxRawSize_);							\
		debBuf[18] = wordHighByte(this->rxRawIndex_);							\
		debBuf[19] = wordLowByte(this->rxRawIndex_);							\
		debBuf[20] = this->rxRawData_[this->rxRawIndex_];						\
		debBuf[21] = this->rxDecodedAuxData_;									\
		debBuf[22] = this->rxRawChecksum_;										\
		debBuf[23] = this->rxDecodedPayloadIndex_;								\
		debBuf[24] = this->rxDecodedPayloadSize_;								\
		debBuf[25] = this->rxDecodedPayloadData_[this->rxDecodedPayloadIndex_];	\
		debBuf[26] = wordHighByte(this->txRawSize_);							\
		debBuf[27] = wordLowByte(this->txRawSize_);								\
		debBuf[28] = wordHighByte(this->txRawIndex_);							\
		debBuf[29] = wordLowByte(this->txRawIndex_);							\
		debBuf[30] = this->txRawData_[this->txRawIndex_];						\
		debBuf[31] = this->txDecodedPayloadIndex_;								\
		debBuf[32] = this->txDecodedPayloadSize_;								\
		debBuf[33] = 0x18;														\
		debBuf[34] = 0x02;														\
		debugSendMessage();														\
		_delay_ms(300);															\
	} while (0)

// *INDENT-ON*

#endif // GPDSE_PROJECT_CONFIG_HPP
