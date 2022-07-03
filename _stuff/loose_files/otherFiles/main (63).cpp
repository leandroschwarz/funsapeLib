/* =============================================================================
 * Project:			serialPlotter
 * File name:		main.cpp
 * Module:			Main source code file
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ===========================================================================*/

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// STM32CubeMx and HAL library header files ------------------------------------

#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

// -----------------------------------------------------------------------------
// FunSAPE Library header files ------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "funsapeLibTwi.hpp"
#include "funsapeLibFixedPoint.hpp"
#include "max30102.hpp"

// -----------------------------------------------------------------------------
// Standard library header files -----------------------------------------------

#include <stdlib.h>
#include <string.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Peripheral buffer sizes -----------------------------------------------------

// User editable settings
#define I2C1_BUFF_SIZE					8				// EEPROM (X), RTC (8), PPG (6)
#define PPG_BYTES_PER_CHANNEL			3				// PPG - channel size (in bytes)
#define PPG_CHANNELS_PER_SAMPLE			2				// PPG - number of channels per sample
#define PPG_SAMPLES_PER_BUFFER			20				// PPG - number os samples per buffer
#define UART1_RX_CIRC_BUFF				100				// Circular buffer size for reception
#define UART1_TX_CIRC_BUFF				100				// Circular buffer size for transmission
#define UART1_TX_DMA_BUFF_SIZE			100				// Buffer size for transmission
#define UART3_RX_CIRC_BUFF				100				// Circular buffer size for reception
#define UART3_TX_CIRC_BUFF				100				// Circular buffer size for transmission
#define UART3_TX_DMA_BUFF_SIZE			300				// Buffer size for transmission

// Non-editable settings
#define PPG_CIRC_BUFF_SIZE				(PPG_SAMPLES_PER_BUFFER * PPG_CHANNELS_PER_SAMPLE * PPG_BYTES_PER_CHANNEL)
#define UART1_RX_DMA_BUFF_SIZE			2				// Must be 2
#define UART3_RX_DMA_BUFF_SIZE			2				// Must be 2

// =============================================================================
// File exclusive - New data types
// =============================================================================

// -----------------------------------------------------------------------------
// System control and status flags ---------------------------------------------

typedef union {
	uint32_t allFlags;
	struct {
		// Devices operational
		bool ppgSensorEnabled					: 1;

		// PPG sensor interrupt flags
		bool ppgGeneralInterruptFlag			: 1;
		bool ppgAmbientLightInterruptFlag		: 1;
		bool ppgDieTempReadyInterruptFlag		: 1;
		bool ppgFifoAlmostFullInterruptFlag		: 1;
		bool ppgNewFifoDataReadyInterruptFlag	: 1;
		bool ppgPowerReadyInterruptFlag			: 1;

		// Debug flags
		bool testFlag							: 1;
		bool error								: 1;
		bool delay1s							: 1;

		// Unused flags
		uint32_t unusedFlags					: 22;
	};
} systemFlags_f;

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// -----------------------------------------------------------------------------
// Debug level related functions -----------------------------------------------

#if DEBUG_ALLOW_VERBOSE == 1
inline void verboseStatus(uint16_t errorCode, const char *strMessage)
{
	char buff[1000];
	do {
		if(errorCode) {
			strcpy(buff, "[FAILED] ");
		} else {
			strcpy(buff, "[  OK  ] ");
		} strcat(buff, strMessage);
		if(errorCode) {
			sprintf(buff, "%s! Error = %u.\r", buff, errorCode);
		} else {
			strcat(buff, "!\r");
		}
//		HAL_UART_Transmit(&huart3, (uint8_t *)buff, strlen(buff), UART3_TIMEOUT);
	} while(0);
}
#else
inline void verboseStatus(uint16_t errorCode, const char *strMessage)
{
	return;
}
#endif

// =============================================================================
// File exclusive - Inline function definitions
// =============================================================================

#define ledBlink(delayMs_p) 			do {delayMs(delayMs_p);HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);} while(1)

// =============================================================================
// Global variables
// =============================================================================

//     //////////////////////    SYSTEM STATUS     //////////////////////     //
Error									funsapeStatus;
HAL_StatusTypeDef						halStatus;
// Stopwatch								timer1s(1000);
volatile systemFlags_f					systemFlags;
volatile uint8_t testChar[7] = {0, 0, 0, 0, 0, 0, 0};
volatile uint8_t testIndex = 0;

//     /////////////////////    UART PERIPHERAL     /////////////////////     //
CircularBuffer<char>					uart1RxCircBuffer(UART1_RX_CIRC_BUFF, true);
uint8_t									uart1RxDmaBuffer[UART1_RX_DMA_BUFF_SIZE];
char									uart1TxDmaBuffer[UART1_TX_DMA_BUFF_SIZE];
CircularBuffer<char>					uart3RxCircBuffer(UART3_RX_CIRC_BUFF, true);
uint8_t									uart3RxDmaBuffer[UART3_RX_DMA_BUFF_SIZE];
char									uart3TxDmaBuffer[UART3_TX_DMA_BUFF_SIZE];

//     ///////////////////////     PPG SIGNAL     ///////////////////////     //
CircularBuffer<uint8_t>					ppgCircBuffer(PPG_CIRC_BUFF_SIZE, true);
Max30102								ppgSensor;
Twi										twi1;

// =============================================================================
// Import external variables
// =============================================================================

// NONE

// =============================================================================
// Function declarations
// =============================================================================

// -----------------------------------------------------------------------------
// Internally definied ---------------------------------------------------------

void Error_Handler(void);						// HAL Library
void myErrorHandler(uint16_t errorCode_p);
void processPpgAmbientLightCallback(void);
void processPpgDieTempReadyCallback(void);
void processPpgFifoAlmostFullCallback(void);
void processPpgNewFifoDataReadyCallback(void);
void processPpgPowerReadyCallback(void);
void SystemClock_Config(void);					// HAL Library

// -----------------------------------------------------------------------------
// Externally definied ---------------------------------------------------------

// NONE

// =============================================================================
// Main function
// =============================================================================

/**
 * @brief Main function
 *
 * @return int
 */
int main(void)
{
	//     ///////////////////    LOCAL VARIABLES     ///////////////////     //
	// NONE

	//     ////////////////////     SYSTEM FLAGS     ////////////////////     //
	systemFlags.allFlags = 0;

	//     /////////////////////    HAL LIBRARY     /////////////////////     //
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_USART3_UART_Init();
	MX_TIM1_Init();
	MX_TIM7_Init();

	//     //////////     PULSE FOR LOGIC ANALIZER     //////////     //
	ledTurnOff();
	delayMs(100);						// Time to HAL finishes configuring stuff
	debugTriggerPulse();
	delayMs(10);						// Time to HAL finishes configuring stuff

	debug0Low();
	debug1Low();
	debug2Low();
	debug3Low();
	debug4Low();
	debug5Low();
	debug6Low();
	debug7Low();

	//     //////////    CONFIGURE UART3 RECEPTION     //////////     //
	/*
		halStatus = HAL_UART_Receive_DMA(&huart3, uart3RxDmaBuffer, 2);
		if((uint16_t)halStatus) {
			myErrorHandler(UART3_DMA_RX_CONFIG);
		}
	*/

	//     //////////    CONFIGURE DEBUGGING TOOLS (UART1)     //////////     //

#if DEBUG_ALLOW_FULL == 1
	// init => handler, hideSuccess, stopOnError, showMarks
	debug.init(&huart3, true, false, false);
#else
	debug.init(&huart3);
#endif
	do {
		char auxBuff[2];
		auxBuff[0] = '\r';
		auxBuff[1] = '\r';

		halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)auxBuff, 2, UART3_TIMEOUT);
		if((uint16_t)halStatus) {
			myErrorHandler(UART3_TX_TRANSFER);
		}
		halStatus = HAL_UART_Receive_DMA(&huart3, uart3RxDmaBuffer, 2);
		verboseStatus((uint16_t)halStatus, "UART3 reception configured to use DMA");
		debugMessage((uint16_t)halStatus);
		if((uint16_t)halStatus) {
			myErrorHandler(UART3_DMA_RX_CONFIG);
		}
	} while(0);

	//     //////////////////     CONFIGURE TIMERS     //////////////////     //
	halStatus = HAL_TIM_Base_Start_IT(&htim1);
	verboseStatus((uint16_t)halStatus, "TIMER1 configured");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(TIMER1_CONFIG);
	}

	//     ///////////////////     CONFIGURE I2C1     ///////////////////     //
	if(!twi1.init(&hi2c1)) {
		funsapeStatus = twi1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(TWI_CLASS_INIT);
	}
	verboseStatus((uint16_t)Error::NONE, "I2C1 configured");

	//     /////////////////     CONFIGURE MAX30102     /////////////////     //
	// Device - Initialization
	if(!ppgSensor.init(&twi1)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_CONFIG_INIT);
	}
	// Device - FIFO configuration
	if(!ppgSensor.configFifo(Max30102::SampleAveraging::SAMPLE_AVERAGING_OFF, true, 15)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_CONFIG_FIFO);
	}
	if(!ppgSensor.flushFifo()) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_FIFO_FLUSH);
	}

	// Device - Operation parameters
	if(!ppgSensor.setFullScale(Max30102::FullScale::FULL_SCALE_2048_NA)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_FULL_SCALE);
	}
	if(!ppgSensor.setResolution(Max30102::Resolution::RESOLUTION_18_BITS)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_RESOLUTION);
	}
	if(!ppgSensor.setSampleRate(Max30102::SampleRate::SAMPLE_RATE_50_HZ)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_SAMPLE_RATE);
	}

	// Device - Interrupt configuration
	if(!ppgSensor.setAmbLightCancOverflowInterruptCallback(processPpgAmbientLightCallback)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_INTERRUPT_CALLBACK);
	}
	if(!ppgSensor.setDieTempReadyInterruptCallback(processPpgDieTempReadyCallback)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_INTERRUPT_CALLBACK);
	}
	if(!ppgSensor.setFifoAlmostFullInterruptCallback(processPpgFifoAlmostFullCallback)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_INTERRUPT_CALLBACK);
	}
	if(!ppgSensor.setNewFifoDataReadyInterruptCallback(processPpgNewFifoDataReadyCallback)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_INTERRUPT_CALLBACK);
	}
	if(!ppgSensor.setPowerReadyInterruptCallback(processPpgPowerReadyCallback)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_INTERRUPT_CALLBACK);
	}
	if(!ppgSensor.enableInterrupts(Max30102::InterruptFlags::ALL_FLAGS)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_ENABLE_INTERRUPT);
	}
	systemFlags.ppgSensorEnabled = true;

	// Device - Get die temperature
	if(!ppgSensor.startTemperatureRead()) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_SPO2_MODE);
	}

	// Device - mode selection
	if(!ppgSensor.setSpo2Mode(Max30102::LedCurrent::LED_CURRENT_30_MA, Max30102::LedCurrent::LED_CURRENT_30_MA)) {
		funsapeStatus = ppgSensor.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(PPG_SENSOR_1_SET_SPO2_MODE);
	}
	verboseStatus((uint16_t)Error::NONE, "PPG initialized");

	// =========================================================================
	// Main loop - Start
	// =========================================================================

	while(1) {

		// =====================================================================
		// PPG sensor processing
		// =====================================================================

		// ---------------------------------------------------------------------
		// General interrupt handler -------------------------------------------
		if(systemFlags.ppgGeneralInterruptFlag) {
			// Call PPG sensor IRQ handler
			if(!ppgSensor.generalIrqHandler()) {
				funsapeStatus = ppgSensor.getLastError();
				debugMessage((uint16_t)funsapeStatus);
				myErrorHandler(PPG_SENSOR_1_SET_SPO2_MODE);	// FIX: CHANGE ERROR CODE
			}

			// Clearing flag before leaving
			systemFlags.ppgGeneralInterruptFlag = false;
		}

		// ---------------------------------------------------------------------
		// ALC Overflow interrupt handler --------------------------------------
		if(systemFlags.ppgAmbientLightInterruptFlag) {
			// Process ALC Overflow
			// NOTHING TO DO

			// Clearing flag before leaving
			systemFlags.ppgAmbientLightInterruptFlag = false;
		}

		// ---------------------------------------------------------------------
		// Die Temperature Ready interrupt handler -----------------------------
		if(systemFlags.ppgDieTempReadyInterruptFlag) {
			// Process temperature
			int8_t auxInt = 0;
			uint8_t auxFrac = 0;
			ppgSensor.getTemperature(&auxInt, &auxFrac);
			uint16_t fracPart = (uint16_t)auxFrac;
			fracPart *= 625;
			sprintf(uart3TxDmaBuffer, "TEMP = %d.%04u\r", auxInt, auxFrac);
			halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxDmaBuffer, strlen(uart3TxDmaBuffer), 100);

			// Clearing flag before leaving
			systemFlags.ppgDieTempReadyInterruptFlag = false;
		}

		// ---------------------------------------------------------------------
		// FIFO Almost Full interrupt handler ----------------------------------
		if(systemFlags.ppgFifoAlmostFullInterruptFlag) {
			// Process ALC Overflow
			// NOTHING TO DO

			// Clearing flag before leaving
			systemFlags.ppgFifoAlmostFullInterruptFlag = false;
		}

		// ---------------------------------------------------------------------
		// New Fifo Data Ready interrupt handler -------------------------------
		if(systemFlags.ppgNewFifoDataReadyInterruptFlag) {
			ledToggle();

			uint8_t buffAux[6];
			// uint32_t redValue = 0;
			// uint32_t infraredValue = 0;

			ppgSensor._getFifoInfo();
			ppgSensor._read(0x07, buffAux, 6);

			// // Prepare data to uart3
			// redValue = (uint32_t)buffAux[0];
			// redValue <<= 8;
			// redValue += (uint32_t)buffAux[1];
			// redValue <<= 8;
			// redValue += (uint32_t)buffAux[2];
			// infraredValue = (uint32_t)buffAux[3];
			// infraredValue <<= 8;
			// infraredValue += (uint32_t)buffAux[4];
			// infraredValue <<= 8;
			// infraredValue += (uint32_t)buffAux[5];
			// sprintf(uart3TxDmaBuffer, "%02u/%02u/%02u %08lX %08lX\r",
			// 		ppgSensor._fifoWritePointer,
			// 		ppgSensor._fifoOverflowCounter,
			// 		ppgSensor._fifoReadPointer,
			// 		redValue,
			// 		infraredValue
			// );
			// halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxDmaBuffer, strlen(uart3TxDmaBuffer), 100);

			// Prepare data to uart1
			uart1TxDmaBuffer[0] = 0xAA;
			uart1TxDmaBuffer[1] = 0xBB;
			uart1TxDmaBuffer[2] = 0xCC;
			uart1TxDmaBuffer[3] = 0x00;			// RED[31:24]
			uart1TxDmaBuffer[4] = buffAux[0];	// RED[23:16]
			uart1TxDmaBuffer[5] = buffAux[1];	// RED[15:8]
			uart1TxDmaBuffer[6] = buffAux[2];	// RED[7:0]
			uart1TxDmaBuffer[7] = 0x00;			//  IR[31:24]
			uart1TxDmaBuffer[8] = buffAux[3];	//  IR[23:16]
			uart1TxDmaBuffer[9] = buffAux[4];	//  IR[15:8]
			uart1TxDmaBuffer[10] = buffAux[5];	//  IR[7:0]
			halStatus = HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, 11, 100);

			/*
						// Process New FIFO Data Ready
						uint32_t redValue = 0;
						uint32_t infraredValue = 0;

						if(!ppgSensor.getFifoNewestSample(&redValue, &infraredValue)) {
							funsapeStatus = ppgSensor.getLastError();
							debugMessage((uint16_t)funsapeStatus);
							myErrorHandler(PPG_SENSOR_1_SET_SPO2_MODE);	// FIX: CHANGE ERROR CODE
						}

						ledToggle();
						sprintf(uart3TxDmaBuffer, "\r%02u/%02u/%02u %02u/%02u/%02u DATA = %08lX %08lX\r",
								ppgSensor._fifoWritePointerOld,
								ppgSensor._fifoOverflowCounterOld,
								ppgSensor._fifoReadPointerOld,
								ppgSensor._fifoWritePointerNew,
								ppgSensor._fifoOverflowCounterNew,
								ppgSensor._fifoReadPointerNew,
								redValue, infraredValue
						);
						halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxDmaBuffer, strlen(uart3TxDmaBuffer), 100);
			*/

			// Clearing flag before leaving
			systemFlags.ppgNewFifoDataReadyInterruptFlag = false;
		}

		// ---------------------------------------------------------------------
		// Power Ready interrupt handler ---------------------------------------
		if(systemFlags.ppgPowerReadyInterruptFlag) {
			// Process Power Ready
			// NOTHING TO DO

			// Clearing flag before leaving
			systemFlags.ppgPowerReadyInterruptFlag = false;
		}

	} // End of main loop
}

// =============================================================================
// Interrupt callback function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// UART interrupt callbacks ----------------------------------------------------

/**
 * @brief
 * // TODO: document this function
 * @param huart
 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {
		if(!uart1RxCircBuffer.push(uart1RxDmaBuffer[0])) {
			myErrorHandler(UART1_RX_CIRC_BUFFER_PUSH_DATA);
		}
	} else  if(huart->Instance == USART3) {
		if(!uart3RxCircBuffer.push(uart3RxDmaBuffer[0])) {
			myErrorHandler(UART3_RX_CIRC_BUFFER_PUSH_DATA);
		}
	}

	return;
}

/**
 * @brief
 * // TODO: document this function
 * @param huart
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {
		if(!uart1RxCircBuffer.push(uart1RxDmaBuffer[1])) {
			myErrorHandler(UART1_RX_CIRC_BUFFER_PUSH_DATA);
		}
	} else  if(huart->Instance == USART3) {
		if(!uart3RxCircBuffer.push(uart3RxDmaBuffer[1])) {
			myErrorHandler(UART3_RX_CIRC_BUFFER_PUSH_DATA);
		}
	}

	return;
}

// -----------------------------------------------------------------------------
// TIMER interrupt callbacks ---------------------------------------------------

/**
 * @brief
 * // TODO: document this function
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1) {
		systemFlags.delay1s = 1;
		// // Read PPG data
		// uint8_t aux[4];
		// if(!twi1.readReg(0xFF, aux, 1)) {
		// 	funsapeStatus = twi1.getLastError();
		// 	debugMessage((uint16_t)funsapeStatus);
		// 	myErrorHandler(TWI_CLASS_INIT);
		// }
		// debugMessage((uint16_t)aux[0]);
		// delayMs(100);
		// sprintf(uart3TxDmaBuffer, "AABB\r");
		// HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxDmaBuffer, strlen(uart3TxDmaBuffer), UART3_TIMEOUT);
	} else if(htim->Instance == TIM7) {
	}
}
// -----------------------------------------------------------------------------
// GPIO interrupt callbacks ----------------------------------------------------

/**
 * @brief
	 * // TODO: document this function
	 * @param GPIO_Pin
	 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BT_ST_Pin) {			// ppgSensor IN 		// Entered in interrupt
		systemFlags.testFlag = true;
		debug5Toggle();

		if(systemFlags.ppgSensorEnabled) {
			debug6Toggle();

			// Must treat General interrupt
			systemFlags.ppgGeneralInterruptFlag = true;
		}
	}

	// Handling the encoder CLK interrupt 	if(GPIO_Pin == BT_ST_Pin) {
	// uint32_t currentTick = HAL_GetTick();

	// if((currentTick - encoder.ticksSinceLastEdge) > ENCODER_CLOCK_DEBOUNCE) {
	// 	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
	// 		encoder.value++;
	// 		encoder.rotation = Rotation::CLOCKWISE;
	// 	} else {
	// 		encoder.value--;
	// 		encoder.rotation = Rotation::COUNTERCLOCKWISE;
	// 	}
	// 	encoder.ticksSinceLastEdge = currentTick;
	// 	systemFlags.encoderValueUpdated = true;
	// }
}

// =============================================================================
// Function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// PPG interrupt process functions ---------------------------------------------

/**
 * @brief
	 * // TODO: document this function
	 */
void processPpgAmbientLightCallback(void)
{
	debug0Toggle();
	systemFlags.ppgAmbientLightInterruptFlag = true;
}

/**
* @brief
 * // TODO: document this function
 */
void processPpgDieTempReadyCallback(void)
{
	debug1Toggle();
	systemFlags.ppgDieTempReadyInterruptFlag = true;
}

/**
* @brief
 * // TODO: document this function
 */
void processPpgFifoAlmostFullCallback(void)
{
	debug2Toggle();
	systemFlags.ppgFifoAlmostFullInterruptFlag = true;
}

/**
* @brief
 * // TODO: document this function
 */
void processPpgNewFifoDataReadyCallback(void)
{
	debug3Toggle();
	systemFlags.ppgNewFifoDataReadyInterruptFlag = true;
}

/**
* @brief
 * // TODO: document this function
 */
void processPpgPowerReadyCallback(void)
{
	debug4Toggle();
	systemFlags.ppgPowerReadyInterruptFlag = true;
}

// -----------------------------------------------------------------------------
// Generic error handler -------------------------------------------------------

/**
* @brief
	 *
	 * @param errorCode_p
	 */
void myErrorHandler(uint16_t errorCode_p)
{
	//	ledTurnOff(); 	while(1) {
	// for(uint16_t i = 0; i < errorCode_p; i++) {
	// 	delayMs(200);
	// 	ledTurnOn();
	// 	delayMs(200);
	// 	ledTurnOff();
	// }
	// delayMs(1000);
}

/**
 * @brief
 * // TODO: document this function
 */
void Error_Handler(void)
{
	myErrorHandler(HAL_INTERNAL_ERROR);
}

/**
* @brief
* // TODO: document this function
*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

// =============================================================================
// Weak functions overriding
// =============================================================================

/**
 * @brief
 * // TODO: document this function
 */
void HAL_IncTick(void)
{
	uwTick += uwTickFreq;
}
