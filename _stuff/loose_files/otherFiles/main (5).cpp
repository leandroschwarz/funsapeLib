/* =============================================================================
 * Project:			dataGrabber_Part_1
 * File name:		main.cpp
 * Module:			Main source code file
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// STM32CubeMx and HAL library header files ------------------------------------

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

// -----------------------------------------------------------------------------
// FunSAPE Library header files ------------------------------------------------

#include "../funsapeLib/funsapeLibGlobalDefines.hpp"
#include "../funsapeLib/devices/lcd.hpp"
#include "../funsapeLib/util/funsapeLibDebug.hpp"

// -----------------------------------------------------------------------------
// FatFs Library header files --------------------------------------------------

// #include "ff.h"

// =============================================================================
// File exclusive - Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Debug level control constants -----------------------------------------------

#define DEBUG_DISABLE_UART1				0
#define DEBUG_DISABLE_UART2				0
#define DEBUG_DISABLE_UART3				0

#define DEBUG_ALLOW_FULL				1
#define DEBUG_ALLOW_VERBOSE				1
#define DEBUG_DISABLE_SD				1
#define DEBUG_DISABLE_ADC				0
#define DEBUG_DISABLE_MPU				1

// -----------------------------------------------------------------------------
// System voltages related constants -------------------------------------------

// -----------------------------------------------------------------------------
// Timeouts / debounce times ---------------------------------------------------

#define ENCODER_BUTTON_DEBOUNCE			200				// Value in miliseconds
#define ENCODER_CLOCK_DEBOUNCE			100				// Value in miliseconds
#define UART1_TIMEOUT					100				// Value in miliseconds
#define UART2_TIMEOUT					100				// Value in miliseconds
#define UART3_TIMEOUT					100				// Value in miliseconds

// -----------------------------------------------------------------------------
// Peripheral buffer sizes -----------------------------------------------------

#define UART1_RX_BUFFER_SIZE			2
#define UART1_TX_BUFFER_SIZE			100
#define UART2_RX_BUFFER_SIZE			2
#define UART2_TX_BUFFER_SIZE			100
#define UART3_RX_BUFFER_SIZE			2
#define UART3_TX_BUFFER_SIZE			100

// -----------------------------------------------------------------------------
// ECG related constants -------------------------------------------------------

// -----------------------------------------------------------------------------
// SD Card related constants ---------------------------------------------------

// =============================================================================
// File exclusive - New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Roraty encoder --------------------------------------------------------------

typedef enum {
	COMMAND_NONE						= 0,
	COMMAND_BUTTON						= 1,
	COMMAND_LEFT						= 2,
	COMMAND_RIGTH						= 3,
} command_t;

typedef struct {
	uint32_t	ticksSinceLastEdge;
	uint32_t	ticksSinceLastPress;
	command_t	lastCommand				: 2;
	int16_t		value					: 12;
	Rotation	rotation;
} encoderData_s;

// -----------------------------------------------------------------------------
// System control and status flags ---------------------------------------------

typedef union {
	uint32_t allFlags;
	struct {
		// Interrupt management
		// bool		systemVoltagesUpdated		: 1;

		// Acquisition control
		// bool		acquisitionStartNew			: 1;
		// bool		acquisitionRunning			: 1;
		// bool		acquisitionStopping			: 1;

		// Encoder control
		bool		encoderUpdated				: 1;

		// Display control
		bool		displayRefresh				: 1;

		// Free to use
		uint32_t	unusedFlags					: 30;
	};
} systemFlags_f;

// -----------------------------------------------------------------------------
// System voltages -------------------------------------------------------------

// -----------------------------------------------------------------------------
// SD Card ---------------------------------------------------------------------

// -----------------------------------------------------------------------------
// System error codes ----------------------------------------------------------

typedef enum {
	HAL_INTERNAL_ERROR					= 1,

	HAL_UART3_RX_START_DMA_ERROR,
	HAL_UART3_TRANSMIT_ERROR,
} systemError_e;

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Inline function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Debug level related functions -----------------------------------------------

// #if DEBUG_ALLOW_VERBOSE == 1
// inline void verboseStatus(uint16_t errorCode, const char *strMessage)
// {
// 	char buff[1000];
// 	do {
// 		if(errorCode) {
// 			strcpy(buff, "[FAILED] ");
// 		} else {
// 			strcpy(buff, "[  OK  ] ");
// 		} strcat(buff, strMessage);
// 		if(errorCode) {
// 			sprintf(buff, "%s! Error = %u.\r", buff, errorCode);
// 		} else {
// 			strcat(buff, "!\r");
// 		} HAL_UART_Transmit(&huart3, (uint8_t *)buff, strlen(buff), UART3_TIMEOUT);
// 	} while(0);
// }
// #else
// inline void verboseStatus(uint16_t errorCode, const char *strMessage)
// {
// 	return;
// }
// #endif

// -----------------------------------------------------------------------------
// System LED related functions ------------------------------------------------

inlined void ledToggle(void)
{
	HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
}

inlined void ledTurnOn(void)
{
	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
}

inlined void ledTurnOff(void)
{
	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
}

// -----------------------------------------------------------------------------
// System DEBUG pins related functions -----------------------------------------

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Status variables ------------------------------------------------------------

Error									funsapeStatus;
HAL_StatusTypeDef						halStatus;
systemFlags_f							systemFlags;

// -----------------------------------------------------------------------------
// Encoder and LCD variables ---------------------------------------------------

Lcd										display;
char									displayString[20];
encoderData_s							encoder;

// -----------------------------------------------------------------------------
// UART variables --------------------------------------------------------------

#if !DEBUG_DISABLE_UART1
char									uart1RxBuffer[UART1_RX_BUFFER_SIZE];
char									uart1TxBuffer[UART1_TX_BUFFER_SIZE];
#endif
#if !DEBUG_DISABLE_UART2
char									uart2RxBuffer[UART2_RX_BUFFER_SIZE];
char									uart2TxBuffer[UART2_TX_BUFFER_SIZE];
#endif
#if !DEBUG_DISABLE_UART3
char									uart3RxBuffer[UART3_RX_BUFFER_SIZE];
char									uart3TxBuffer[UART3_TX_BUFFER_SIZE];
#endif

// =============================================================================
// Import external variables
// =============================================================================

extern __IO uint32_t					uwTick;
extern HAL_TickFreqTypeDef				uwTickFreq;

// =============================================================================
// Function declarations
// =============================================================================

// -----------------------------------------------------------------------------
// Internally definied ---------------------------------------------------------

void Error_Handler(void);						// HAL Library
void myErrorHandler(uint16_t errorCode_p);
void processEventEncoderUpdate(void);
void SystemClock_Config(void);					// HAL Library

// -----------------------------------------------------------------------------
// Externally definied ---------------------------------------------------------

// NONE

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	//     ////////////////////    SYSTEM FLAGS     /////////////////////     //
	systemFlags.allFlags = 0;

	//     /////////////////////    HAL LIBRARY     /////////////////////     //
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	// MX_ADC1_Init();
	// MX_ADC3_Init();
	MX_DMA_Init();
	// MX_FATFS_Init();
	// MX_I2C1_Init();
	// MX_I2C2_Init();
	// MX_RTC_Init();
	// MX_SPI1_Init();
	// MX_SPI2_Init();
	MX_TIM3_Init();
	MX_TIM8_Init();
#if !DEBUG_DISABLE_UART1
	MX_USART1_UART_Init();
#endif
#if !DEBUG_DISABLE_UART2
	MX_USART2_UART_Init();
#endif
#if !DEBUG_DISABLE_UART3
	MX_USART3_UART_Init();
#endif
	// MX_USB_DEVICE_Init();

	delayMs(1000);

	//     //////////    CONFIGURE DEBUGGING TOOLS (UART3)     //////////     //
// 	debugTriggerPulse();
// #if DEBUG_ALLOW_FULL == 1
// 	debug.init(&huart3, true, false, true);
// #else
// 	debug.init(&huart3);
// #endif
// 	do {
// 		char auxBuff[2];
// 		auxBuff[0] = '\r';
// 		auxBuff[1] = '\r';

// 		halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)auxBuff, 2, UART3_TIMEOUT);
// 		if((uint16_t)halStatus) {
// 			myErrorHandler(UART3_TX_TRANSFER);
// 		}
// 		halStatus = HAL_UART_Receive_DMA(&huart3, uart3RxDmaBuffer, 2);
// 		verboseStatus((uint16_t)halStatus, "UART3 reception configured to use DMA");
// 		debugMessage((uint16_t)halStatus);
// 		if((uint16_t)halStatus) {
// 			myErrorHandler(UART3_DMA_RX_CONFIG);
// 		}
// 	} while(0);

	//     ///////////////////    CONFIGURE UART3     ///////////////////     //
#if !DEBUG_DISABLE_UART3
	strcpy(uart3TxBuffer, "[  OK  ] UART3 transmission configuration\r");
	halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxBuffer, strlen(uart3TxBuffer), UART3_TIMEOUT);
	if(!halStatus) {
		myErrorHandler(HAL_UART3_TRANSMIT_ERROR);
	}

	halStatus = HAL_UART_Receive_DMA(&huart3, (uint8_t *)uart3RxBuffer, UART3_RX_BUFFER_SIZE);
	if(!halStatus) {
		myErrorHandler(halStatus);
	}
	strcpy(uart3TxBuffer, "[  OK  ] UART3 reception configuration\r");
	halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxBuffer, strlen(uart3TxBuffer), UART3_TIMEOUT);
	if(!halStatus) {
		myErrorHandler(HAL_UART3_TRANSMIT_ERROR);
	}

#endif

	//     //////////////     CONFIGURE ROTARY ENCODER     //////////////     //
	encoder.lastCommand					= COMMAND_NONE;
	encoder.rotation					= Rotation::COUNTERCLOCKWISE;
	encoder.ticksSinceLastEdge			= 0;
	encoder.ticksSinceLastPress			= 0;
	encoder.value						= 0;

	//     //////////////////     CONFIGURE TIMERS     //////////////////     //
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim8);

	//     ////////////////////    CONFIGURE LCD     ////////////////////     //
	display.setControlPort(GPIOD, GPIO_PIN_3, GPIO_PIN_2);
	display.setDataPort(GPIOE, GPIO_PIN_0, false);
	display.init(Lcd::DisplaySize::DISPLAY_16X2, Lcd::FontSize::FONT_5X8);
	sprintf(displayString, "Iniciar teste!\n\n");
	systemFlags.displayRefresh = true;

	// =========================================================================
	// Main loop - Start
	// =========================================================================

	while(1) {

		// ---------------------------------------------------------------------
		// Process the events
		// ---------------------------------------------------------------------

		//     //////////////    ROTARY ENCODER UPDATE     //////////////     //
		if(systemFlags.encoderUpdated) {
			processEventEncoderUpdate();

			// Clearing flag before leave
			systemFlags.encoderUpdated = false;
		}

		//     /////////////////    DISPLAY REFRESH     /////////////////     //
		if(systemFlags.displayRefresh) {
			display.print(displayString, strlen(displayString));

			// Clearing flag before leave
			systemFlags.displayRefresh = false;
		}

	} // End of main loop

	return 0;
}

// =============================================================================
// Interrupt callback function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// UART interrupt callbacks ----------------------------------------------------

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}

// -----------------------------------------------------------------------------
// ADC interrupt callbacks -----------------------------------------------------

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	return;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	return;
}

// -----------------------------------------------------------------------------
// TIMER interrupt callbacks ---------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) {
		// Nothing to do
	} else if(htim->Instance == TIM8) {
		// Nothing to do
	}
}

// -----------------------------------------------------------------------------
// EXTI interrupt callbacks ----------------------------------------------------

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// Handling the encoder CLK interrupt
	if(GPIO_Pin == ENC_A_Pin) {
		uint32_t currentTick = HAL_GetTick();

		if((currentTick - encoder.ticksSinceLastEdge) > ENCODER_CLOCK_DEBOUNCE) {
			if(HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin)) {
				encoder.value++;
				encoder.rotation = Rotation::CLOCKWISE;
				encoder.lastCommand = COMMAND_RIGTH;
			} else {
				encoder.value--;
				encoder.rotation = Rotation::COUNTERCLOCKWISE;
				encoder.lastCommand = COMMAND_LEFT;
			}
			encoder.ticksSinceLastEdge = currentTick;
			systemFlags.encoderUpdated = true;
		}
	}

	// Handling the encoder BTN interrupt
	else if(GPIO_Pin == ENC_BTN_Pin) {
		uint32_t currentTick = HAL_GetTick();

		if((currentTick - encoder.ticksSinceLastPress) > ENCODER_BUTTON_DEBOUNCE) {
			encoder.ticksSinceLastPress = currentTick;
			systemFlags.encoderUpdated = true;
			encoder.lastCommand = COMMAND_BUTTON;
		}
	}

	// Handling other pin interrupt
	else if(GPIO_Pin == RTC_SWO_Pin) {
		// TODO Handle interrupt from the RTC SWO pin
	}

	return;
}

// =============================================================================
// Function definitions
// =============================================================================

void Error_Handler(void)
{
	myErrorHandler(HAL_INTERNAL_ERROR);
}

void myErrorHandler(uint16_t errorCode_p)
{
	__disable_irq();
	ledTurnOff();
	while(1) {
		for(uint16_t i = 0; i < errorCode_p; i++) {
			delayMs(200);
			ledTurnOn();
			delayMs(200);
			ledTurnOff();
		}
		delayMs(1000);
	}
}

void processEventEncoderUpdate(void)
{
	if(encoder.lastCommand == COMMAND_BUTTON) {
		display.print("B", 1);
	} else if(encoder.lastCommand == COMMAND_LEFT) {
		display.print("L", 1);
	} else if(encoder.lastCommand == COMMAND_RIGTH) {
		display.print("R", 1);
	}
	// TODO: Action when encoder is rotated

	return;
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC
			| RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

// =============================================================================
// Weak functions overriding
// =============================================================================

void HAL_IncTick(void)
{
	uwTick += uwTickFreq;
}
