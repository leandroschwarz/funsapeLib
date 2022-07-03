/* =============================================================================
 * Project:			dataGrabber
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

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "funsapeLibDebug.hpp"
#include "funsapeLibSpi.hpp"
#include "funsapeLibTwi.hpp"
#include "funsapeLibStopWatch.hpp"
#include "mpu9250.hpp"

// -----------------------------------------------------------------------------
// FatFs Library header files --------------------------------------------------

#include "ff.h"

// =============================================================================
// File exclusive - Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Debug level control constants -----------------------------------------------

#define DEBUG_ALLOW_FULL				1
#define DEBUG_ALLOW_VERBOSE				1
#define DEBUG_DISABLE_SD				1
#define DEBUG_DISABLE_ADC				0
#define DEBUG_DISABLE_MPU				1

// -----------------------------------------------------------------------------
// System voltages related constants -------------------------------------------

#define BAT_ADC_BYTES_PER_CHANNEL		2				// Bytes per channel
#define BAT_ADC_CHANNELS_PER_SAMPLE		3				// Number of channels per sample
#define BAT_ADC_SAMPLES_PER_BUFFER		2				// Number of samples per buffer (Must be even)
#define BAT_ADC_CHANNELS_PER_BUFFER		(BAT_ADC_SAMPLES_PER_BUFFER * BAT_ADC_CHANNELS_PER_SAMPLE)
#define BAT_ADC_DMA_BUFF_SIZE			BAT_ADC_CHANNELS_PER_BUFFER
#define BAT_ADC_DMA_BUFF_SIZE_HALF		(BAT_ADC_DMA_BUFF_SIZE / 2)

#define POWER_MAIN_BAT_RES_TOP			12				// Value in kiloohms
#define POWER_MAIN_BAT_RES_BOTTOM		33				// Value in kiloohms
#define POWER_STEP_UP_RES_TOP			18				// Value in kiloohms
#define POWER_STEP_UP_RES_BOTTOM		15				// Value in kiloohms

// -----------------------------------------------------------------------------
// Timeouts / debounce times ---------------------------------------------------

#define I2C1_TIMEOUT					10				// Value in miliseconds
#define I2C2_TIMEOUT					10				// Value in miliseconds
#define SPI1_TIMEOUT					10				// Value in miliseconds
#define SPI2_TIMEOUT					10				// Value in miliseconds
#define UART1_TIMEOUT					10				// Value in miliseconds
#define UART2_TIMEOUT					10				// Value in miliseconds
#define UART3_TIMEOUT					10				// Value in miliseconds
#define ENCODER_BUTTON_DEBOUNCE			50				// Value in miliseconds
#define ENCODER_CLOCK_DEBOUNCE			50				// Value in miliseconds

// -----------------------------------------------------------------------------
// Peripheral buffer sizes -----------------------------------------------------

#define I2C1_BUFF_SIZE					8				// EEPROM (X), RTC (8), PPG (6)
#define I2C2_BUFF_SIZE					6				// RESP (6)
#define SPI1_BUFF_SIZE					6				// MPU (6)
#define SPI2_BUFF_SIZE					512				// SD (512)
#define UART1_RX_BUFF_SIZE				10
#define UART1_TX_BUFF_SIZE				10
#define UART2_RX_BUFF_SIZE				10
#define UART2_TX_BUFF_SIZE				10
#define UART3_RX_BUFF_SIZE				2				// DMA (2)
#define UART3_TX_BUFF_SIZE				100				// BLOCKING (100)
#define USB_RX_BUFF_SIZE				2				// DMA (2)
#define USB_TX_BUFF_SIZE				100				// BLOCKING (100)

// -----------------------------------------------------------------------------
// ECG related constants -------------------------------------------------------

#define ECG_ADC_BYTES_PER_CHANNEL		2				// Bytes per channel
#define ECG_ADC_CHANNELS_PER_SAMPLE		3				// Number of channels per sample
#define ECG_ADC_SAMPLES_PER_BUFFER		8				// Number of samples per buffer (Must be even)
#define ECG_ADC_CHANNELS_PER_BUFFER		(ECG_ADC_SAMPLES_PER_BUFFER * ECG_ADC_CHANNELS_PER_SAMPLE)
#define ECG_ADC_DMA_BUFF_SIZE			ECG_ADC_CHANNELS_PER_BUFFER
#define ECG_ADC_DMA_BUFF_SIZE_HALF		(ECG_ADC_DMA_BUFF_SIZE / 2)
#define ECG_ADC_CIRC_BUFF_SIZE			300				// Half word (uint16_t) elements

// -----------------------------------------------------------------------------
// SD Card related constants ---------------------------------------------------

#define SD_CARD_CIRCULAR_BUFFER_SIZE	2048
#define SD_CARD_BUFF_SIZE				SPI2_BUFF_SIZE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// -----------------------------------------------------------------------------
// System control and status flags ---------------------------------------------

typedef union {
	uint32_t allFlags;
	struct {
		// Interrupt management
		bool		systemVoltagesUpdated		: 1;

		// Acquisition control
		bool		acquisitionStartNew			: 1;
		bool		acquisitionRunning			: 1;
		bool		acquisitionStopping			: 1;

		// Encoder control
		bool		encoderValueUpdated			: 1;
		bool		encoderButtonPressed		: 1;

		// Free to use
		uint32_t	unusedFlags					: 26;
	};
} systemFlags_f;

// -----------------------------------------------------------------------------
// System voltages -------------------------------------------------------------

typedef struct {
	uint16_t	rtcBattery;
	uint16_t	mainBattery;
	uint16_t	stepUpConverter;
} systemVoltages_t;

// -----------------------------------------------------------------------------
// Roraty encoder --------------------------------------------------------------

typedef struct {
	uint32_t	ticksSinceLastEdge;
	uint32_t	ticksSinceLastPress;
	int16_t		value					: 14;
	Rotation	rotation;
} encoderData_s;

// -----------------------------------------------------------------------------
// SD Card ---------------------------------------------------------------------

typedef struct {
	FATFS		disk;
	bool		diskIsMounted;
	FIL			fileHandler;
	bool		fileIsOpenned;
	char		fileName[13];
	uint16_t	fileNumber				: 14;
	FRESULT		result;
} sdCard_s;

// -----------------------------------------------------------------------------
// System error codes ----------------------------------------------------------

typedef enum {
	HAL_INTERNAL_ERROR					= 1,

	ADC1_DMA_CONFIG						= 2,
	ADC3_DMA_CONFIG						= 3,
	ECG_AD8232_CIRC_BUFF_POP_DATA		= 4,
	ECG_AD8232_CIRC_BUFF_PUSH_DATA		= 5,
	PPG__MAX30102_CIRC_BUFF_POP_DATA	= 6,
	PPG__MAX30102_CIRC_BUFF_PUSH_DATA	= 7,
	RESP_MAX30102_CIRC_BUFF_POP_DATA	= 8,
	RESP_MAX30102_CIRC_BUFF_PUSH_DATA	= 9,
	RESP_MPU9250_CIRC_BUFF_POP_DATA		= 10,
	RESP_MPU9250_CIRC_BUFF_PUSH_DATA	= 11,
	RESP_MPU9250_INIT					= 12,
	RESP_MPU9250_CONFIG_ACCEL			= 13,
	SD_CARD_CLOSE_FILE					= 14,
	SD_CARD_MOUNT						= 15,
	SD_CARD_OPEN_FILE					= 16,
	SD_CARD_SAVE_FILE					= 17,
	SD_CARD_WRITE_FILE					= 18,
	SD_CIRC_BUFF_INIT					= 19,
	SD_CIRC_BUFF_POP_DATA				= 20,
	SD_CIRC_BUFF_PUSH_DATA				= 21,
	SPI_CLASS_DEVICE_SET				= 22,
	SPI_CLASS_INIT						= 23,
	TIMER3_CONFIG						= 24,
	TIMER8_CONFIG						= 25,
	TWI_CLASS_INIT						= 26,

	UART_RX_CIRC_BUFFER_PUSH_DATA		= 27,
	UART3_DMA_RX_CONFIG					= 28,
	UART3_TX_TRANSFER					= 29,
} systemError_e;

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// -----------------------------------------------------------------------------
// System LED macrofunctions ---------------------------------------------------

#define ledTurnOn()						HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET)
#define ledTurnOff()					HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET)
#define ledToggle()						HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin)

// -----------------------------------------------------------------------------
// System DEBUG pins macrofunctions --------------------------------------------

#define debug0High()					HAL_GPIO_WritePin(DEBUG0_GPIO_Port, DEBUG0_Pin, GPIO_PIN_SET)
#define debug0Low()						HAL_GPIO_WritePin(DEBUG0_GPIO_Port, DEBUG0_Pin, GPIO_PIN_RESET)
#define debug0Toggle()					HAL_GPIO_TogglePin(DEBUG0_GPIO_Port, DEBUG0_Pin)
#define debug1High()					HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_SET)
#define debug1Low()						HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_RESET)
#define debug1Toggle()					HAL_GPIO_TogglePin(DEBUG1_GPIO_Port, DEBUG1_Pin)
#define debug2High()					HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_SET)
#define debug2Low()						HAL_GPIO_WritePin(DEBUG2_GPIO_Port, DEBUG2_Pin, GPIO_PIN_RESET)
#define debug2Toggle()					HAL_GPIO_TogglePin(DEBUG2_GPIO_Port, DEBUG2_Pin)
#define debug3High()					HAL_GPIO_WritePin(DEBUG3_GPIO_Port, DEBUG3_Pin, GPIO_PIN_SET)
#define debug3Low()						HAL_GPIO_WritePin(DEBUG3_GPIO_Port, DEBUG3_Pin, GPIO_PIN_RESET)
#define debug3Toggle()					HAL_GPIO_TogglePin(DEBUG3_GPIO_Port, DEBUG3_Pin)
#define debug4High()					HAL_GPIO_WritePin(DEBUG4_GPIO_Port, DEBUG4_Pin, GPIO_PIN_SET)
#define debug4Low()						HAL_GPIO_WritePin(DEBUG4_GPIO_Port, DEBUG4_Pin, GPIO_PIN_RESET)
#define debug4Toggle()					HAL_GPIO_TogglePin(DEBUG4_GPIO_Port, DEBUG4_Pin)
#define debug5High()					HAL_GPIO_WritePin(DEBUG5_GPIO_Port, DEBUG5_Pin, GPIO_PIN_SET)
#define debug5Low()						HAL_GPIO_WritePin(DEBUG5_GPIO_Port, DEBUG5_Pin, GPIO_PIN_RESET)
#define debug5Toggle()					HAL_GPIO_TogglePin(DEBUG5_GPIO_Port, DEBUG5_Pin)
#define debug6High()					HAL_GPIO_WritePin(DEBUG6_GPIO_Port, DEBUG6_Pin, GPIO_PIN_SET)
#define debug6Low()						HAL_GPIO_WritePin(DEBUG6_GPIO_Port, DEBUG6_Pin, GPIO_PIN_RESET)
#define debug6Toggle()					HAL_GPIO_TogglePin(DEBUG6_GPIO_Port, DEBUG6_Pin)
#define debug7High()					HAL_GPIO_WritePin(DEBUG7_GPIO_Port, DEBUG7_Pin, GPIO_PIN_SET)
#define debug7Low()						HAL_GPIO_WritePin(DEBUG7_GPIO_Port, DEBUG7_Pin, GPIO_PIN_RESET)
#define debug7Toggle()					HAL_GPIO_TogglePin(DEBUG7_GPIO_Port, DEBUG7_Pin)
#define debugTriggerPulse()				do {debug7Low();delayMs(10);debug7High();delayMs(10);debug7Low();} while(0);

// =============================================================================
// File exclusive - Inline function definitions
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
		} HAL_UART_Transmit(&huart3, (uint8_t *)buff, strlen(buff), UART3_TIMEOUT);
	} while(0);
}
#else
inline void verboseStatus(uint16_t errorCode, const char *strMessage)
{
	return;
}
#endif

// =============================================================================
// Global variables
// =============================================================================

uint16_t								batteryVoltagesDmaBuffer[BAT_ADC_DMA_BUFF_SIZE];
CircularBuffer<uint16_t>				ecgCircBuffer(ECG_ADC_CIRC_BUFF_SIZE, true);
uint16_t								ecgDmaBuffer[ECG_ADC_DMA_BUFF_SIZE];
encoderData_s							encoder;
Error									funsapeStatus;
HAL_StatusTypeDef						halStatus;
Mpu9250									mpu0;
Mpu9250									mpu1;
Mpu9250									mpu2;
Mpu9250									mpu3;
sdCard_s								sdCard;
CircularBuffer<uint8_t>					sdCardCircBuffer(SD_CARD_CIRCULAR_BUFFER_SIZE, true);
systemFlags_f							systemFlags;
systemVoltages_t						systemVoltages;
Stopwatch								timer1s(1000);
CircularBuffer<char>					uart3RxCircBuffer;
uint8_t									uart3RxDmaBuffer[UART3_RX_BUFF_SIZE];
Spi										spi1;
Twi										twi1;

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

void adc1DmaProcessData(bool isFirstBank_p);
void adc3DmaProcessData(bool isFirstBank_p);
void Error_Handler(void);						// HAL Library
void mpu0DeselectDevice(void);
void mpu0SelectDevice(void);
void mpu1DeselectDevice(void);
void mpu1SelectDevice(void);
void mpu2DeselectDevice(void);
void mpu2SelectDevice(void);
void mpu3DeselectDevice(void);
void mpu3SelectDevice(void);
void myErrorHandler(uint16_t errorCode_p);
void processEventAcquisitonStartNew(void);
void processEventEncoderButtonPress(void);
void processEventEncoderRotation(void);
void processEventSdCardWriteData(void);
void processEventSystemVoltagesUpdated(void);
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
	MX_ADC1_Init();
	MX_ADC3_Init();
	MX_DMA_Init();
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_RTC_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_TIM3_Init();
	MX_TIM8_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_DEVICE_Init();
	MX_FATFS_Init();

	//     //////////    CONFIGURE DEBUGGING TOOLS (UART3)     //////////     //
	debugTriggerPulse();
#if DEBUG_ALLOW_FULL == 1
	debug.init(&huart3, true, false, true);
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

	//     /////////////////    ECG (ADC1 + TIMER3)     /////////////////     //
#if !(DEBUG_DISABLE_ADC)
	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ecgDmaBuffer), ECG_ADC_DMA_BUFF_SIZE);
	verboseStatus((uint16_t)halStatus, "ADC1 configured to use DMA");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(ADC1_DMA_CONFIG);
	}
	halStatus = HAL_TIM_Base_Start_IT(&htim3);
	verboseStatus((uint16_t)halStatus, "TIMER3 configured");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(TIMER3_CONFIG);
	}
#endif

	//     ///////////    SYSTEM VOLTAGES (ADC3 + TIMER8)     ///////////     //
#if !(DEBUG_DISABLE_ADC)
	halStatus = HAL_ADC_Start_DMA(&hadc3, (uint32_t *)(batteryVoltagesDmaBuffer), BAT_ADC_DMA_BUFF_SIZE);
	verboseStatus((uint16_t)halStatus, "ADC3 configured to use DMA");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(ADC3_DMA_CONFIG);
	}
	halStatus = HAL_TIM_Base_Start_IT(&htim8);
	verboseStatus((uint16_t)halStatus, "TIMER8 configured");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(TIMER8_CONFIG);
	}
	systemVoltages.rtcBattery = 0;
	systemVoltages.mainBattery = 0;
	systemVoltages.stepUpConverter = 0;
#endif

	//     ///////////////////     ROTARY ENCODER     ///////////////////     //
	encoder.value = 0;
	encoder.ticksSinceLastEdge = 0;
	encoder.ticksSinceLastPress = 0;
	encoder.rotation = Rotation::CLOCKWISE;

	//     ///////////////////     CONFIGURE SPI1     ///////////////////     //

	if(!spi1.init(&hspi1)) {
		funsapeStatus = spi1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(SPI_CLASS_INIT);
	}

	//     ///////////////////////    SD CARD     ///////////////////////     //
#if !(DEBUG_DISABLE_SD)
	funsapeStatus = sdCardCircBuffer.getLastError();
	if(funsapeStatus != Error::NONE) {
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(SD_CIRC_BUFF_INIT);
	}
	// SD card mounting
	sdCard.result = f_mount(&sdCard.disk, "", 1);
	verboseStatus((uint16_t)sdCard.result, "SD Card volume mounted");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		sdCard.diskIsMounted = false;
		myErrorHandler(SD_CARD_MOUNT);
	} else {
		sdCard.diskIsMounted = true;
	}
	// Search for last created file
	sdCard.fileNumber = 1;
	do {
		sprintf(sdCard.fileName, "file%04u.txt", sdCard.fileNumber);
		sdCard.result = f_open(&sdCard.fileHandler, sdCard.fileName, (BYTE)(FA_WRITE | FA_CREATE_NEW));
		if(!sdCard.result) {
			break;
		}
	} while((++sdCard.fileNumber) <= 9999);
	verboseStatus((uint16_t)sdCard.result, "New file created");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		sdCard.fileIsOpenned = false;
		myErrorHandler(SD_CARD_OPEN_FILE);
	} else {
		sdCard.fileIsOpenned = true;
	}
	// Saving empty file
	sdCard.result = f_sync(&sdCard.fileHandler);
	verboseStatus((uint16_t)sdCard.result, "Empty file saved");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_SAVE_FILE);
	}
	// Closing file
	sdCard.result = f_close(&sdCard.fileHandler);
	verboseStatus((uint16_t)sdCard.result, "Empty file closed");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_CLOSE_FILE);
		sdCard.fileIsOpenned = false;
	}
	// SD card umounting
	sdCard.result = f_mount(NULL, "", 0);
	verboseStatus((uint16_t)sdCard.result, "SD Card umounted");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_CLOSE_FILE);
		sdCard.diskIsMounted = false;
	}
#endif

	//     ///////////////////     RESP (MPU9250)     ///////////////////     //
#if !(DEBUG_DISABLE_MPU)
	if(!mpu0.init(&spi1, mpu0SelectDevice, mpu0DeselectDevice)) {
		funsapeStatus = mpu0.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_INIT);
	}
	if(!mpu1.init(&spi1, mpu1SelectDevice, mpu1DeselectDevice)) {
		funsapeStatus = mpu1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_INIT);
	}
	if(!mpu2.init(&spi1, mpu2SelectDevice, mpu2DeselectDevice)) {
		funsapeStatus = mpu2.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_INIT);
	}
	if(!mpu3.init(&spi1, mpu3SelectDevice, mpu3DeselectDevice)) {
		funsapeStatus = mpu3.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_INIT);
	}

	if(!mpu0.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
		funsapeStatus = mpu0.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
	}
	if(!mpu1.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
		funsapeStatus = mpu1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
	}
	if(!mpu2.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
		funsapeStatus = mpu2.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
	}
	if(!mpu3.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
		funsapeStatus = mpu3.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
	}
#endif

	//     ///////////////////     CONFIGURE I2C1     ///////////////////     //
	if(!twi1.init(&hi2c1)) {
		funsapeStatus = twi1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(TWI_CLASS_INIT);
	}
	if(!twi1.setAddress(0x68, false)) {
		funsapeStatus = twi1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(TWI_CLASS_INIT);
	}

	while(1) {
		uint8_t aux[4];

		if(!twi1.readReg(0x75, aux, 1)) {
			funsapeStatus = twi1.getLastError();
			debugMessage((uint16_t)funsapeStatus);
			myErrorHandler(TWI_CLASS_INIT);
		}
		debugMessage((uint16_t)aux[0]);
		ledToggle();
		delayMs(100);
	}

	//     //////////////////////////////////////////////////////////////     //
	//     ////////////////////     TESTING CODE     ////////////////////     //
	//     //////////////////////////////////////////////////////////////     //
#if !(DEBUG_DISABLE_MPU)
	for(uint16_t i = 0; i < 1000; i++) {
		sdCardCircBuffer.push((uint8_t)i);
	}
	char auxString[1000];
	sprintf(auxString, "%u\r", sdCardCircBuffer.getOccupation());
	HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), UART3_TIMEOUT);

	while(1) {
		char auxString[1000];
		int16_t accelBuff[3] = {0, 0, 0};

		if(!mpu0.getAccelData(accelBuff)) {
			myErrorHandler(4);
		}
		sprintf(auxString, "%08d %08d %08d \r", accelBuff[0], accelBuff[1], accelBuff[2]);
		HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), UART3_TIMEOUT);

		if(!mpu0.getAccelOffset(accelBuff)) {
			myErrorHandler(5);
		}
		sprintf(auxString, "%08d %08d %08d \r\r", accelBuff[0], accelBuff[1], accelBuff[2]);
		HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), UART3_TIMEOUT);

		ledToggle();
		delayMs(200);
	}
#endif

	while(1) {
		ledToggle();
		delayMs(1000);
	}

	// =========================================================================
	// Main loop - Start
	// =========================================================================

	while(1) {

		// ---------------------------------------------------------------------
		// Process the events
		// ---------------------------------------------------------------------

		//     ////////////    ROTARY ENCODER - ROTATION     ////////////     //
		if(systemFlags.encoderValueUpdated) {
			processEventEncoderRotation();

			// Clearing flag before leave
			systemFlags.encoderValueUpdated = false;
		}

		//     //////////    ROTARY ENCODER - BUTTON PRESS     //////////     //
		if(systemFlags.encoderButtonPressed) {
			processEventEncoderButtonPress();

			// Clearing flag before leave
			systemFlags.encoderButtonPressed = false;
		}

		//     /////////////    SYSTEM VOLTAGES UPDATED     /////////////     //
		if(systemFlags.systemVoltagesUpdated) {
			processEventSystemVoltagesUpdated();

			// Clearing flag before leave
			systemFlags.systemVoltagesUpdated = false;
		}

		//     /////////////    ACQUISITION - START NEW     /////////////     //
		if(systemFlags.acquisitionStartNew) {
			processEventAcquisitonStartNew();

			// Clearing flag before leave
			systemFlags.acquisitionStartNew = false;
		}

		//     //////////////     SD CARD - WRITE DATA     //////////////     //
		if(systemFlags.acquisitionRunning && (sdCardCircBuffer.getOccupation() >= SD_CARD_BUFF_SIZE)) {
			processEventSdCardWriteData();

			// No flag to clear (just leave)
		}

		//     //////////////////////////////////////////////////////////     //
		//     //////////////////     TESTING CODE     //////////////////     //
		//     //////////////////////////////////////////////////////////     //
		if(timer1s.isTimedOut()) {
			char auxString[1000];

			sprintf(auxString, "%u\r", sdCardCircBuffer.getOccupation());
			HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), UART3_TIMEOUT);
			ledToggle();
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
	if(!uart3RxCircBuffer.push(uart3RxDmaBuffer[0])) {
		myErrorHandler(UART_RX_CIRC_BUFFER_PUSH_DATA);
	}

	return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(!uart3RxCircBuffer.push(uart3RxDmaBuffer[1])) {
		myErrorHandler(UART_RX_CIRC_BUFFER_PUSH_DATA);
	}

	return;
}

// -----------------------------------------------------------------------------
// ADC interrupt callbacks -----------------------------------------------------

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1) {
		adc1DmaProcessData(true);
	} else if(hadc->Instance == ADC3) {
		adc3DmaProcessData(true);
	}

	return;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1) {
		adc1DmaProcessData(false);
	} else if(hadc->Instance == ADC3) {
		adc3DmaProcessData(false);
	}

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
	if(GPIO_Pin == GPIO_PIN_5) {
		uint32_t currentTick = HAL_GetTick();

		if((currentTick - encoder.ticksSinceLastEdge) > ENCODER_CLOCK_DEBOUNCE) {
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
				encoder.value++;
				encoder.rotation = Rotation::CLOCKWISE;
			} else {
				encoder.value--;
				encoder.rotation = Rotation::COUNTERCLOCKWISE;
			}
			encoder.ticksSinceLastEdge = currentTick;
			systemFlags.encoderValueUpdated = true;
		}
	}

	// Handling the encoder BTN interrupt
	else if(GPIO_Pin == GPIO_PIN_6) {
		uint32_t currentTick = HAL_GetTick();

		if((currentTick - encoder.ticksSinceLastPress) > ENCODER_BUTTON_DEBOUNCE) {
			systemFlags.encoderButtonPressed = true;
			encoder.ticksSinceLastPress = currentTick;
		}
	}

	// Handling other pin interrupt
	else if(GPIO_Pin == GPIO_PIN_8) {
		// TODO Handle interrupt from the RTC SWO pin
	}

}

// =============================================================================
// Function definitions
// =============================================================================

void adc1DmaProcessData(bool isFirstBank_p)
{
	uint16_t index = (isFirstBank_p) ? 0 : ECG_ADC_DMA_BUFF_SIZE_HALF;

	if(!ecgCircBuffer.pushBuffer(&ecgDmaBuffer[index], ECG_ADC_DMA_BUFF_SIZE_HALF)) {
		funsapeStatus = ecgCircBuffer.getLastError();
		verboseStatus((uint16_t)funsapeStatus, "ERRO ADC1 DMA");
		debugMessage((uint16_t)funsapeStatus);
		myErrorHandler(ECG_AD8232_CIRC_BUFF_PUSH_DATA);
	}
}

void adc3DmaProcessData(bool isFirstBank_p)
{
	// Initializing variables
	uint32_t rawRtcBatteryVoltage = 0;
	uint32_t rawMainBatteryVoltage = 0;
	uint32_t rawStepUpConverterVoltage = 0;

	// Reading data from DMA buffer
	if(isFirstBank_p) {
		rawRtcBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[0];
		rawMainBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[1];
		rawStepUpConverterVoltage = (uint32_t)batteryVoltagesDmaBuffer[2];
	} else {
		rawRtcBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[0 + BAT_ADC_DMA_BUFF_SIZE_HALF];
		rawMainBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[1 + BAT_ADC_DMA_BUFF_SIZE_HALF];
		rawStepUpConverterVoltage = (uint32_t)batteryVoltagesDmaBuffer[2 + BAT_ADC_DMA_BUFF_SIZE_HALF];
	}

	// Calculate CR2032 voltage
	rawRtcBatteryVoltage *= 825;
	rawRtcBatteryVoltage /= 1024;
	systemVoltages.rtcBattery = (uint16_t)rawRtcBatteryVoltage;

	// Calculate Main Battery voltage
	rawMainBatteryVoltage *= 825;
	rawMainBatteryVoltage *= (POWER_MAIN_BAT_RES_TOP + POWER_MAIN_BAT_RES_BOTTOM);
	rawMainBatteryVoltage /= (1024 * POWER_MAIN_BAT_RES_BOTTOM);
	systemVoltages.mainBattery = (uint16_t)rawMainBatteryVoltage;

	// Calculate Step Up converter voltage
	rawStepUpConverterVoltage *= 825;
	rawStepUpConverterVoltage *= (POWER_STEP_UP_RES_TOP + POWER_STEP_UP_RES_BOTTOM);
	rawStepUpConverterVoltage /= (1024 * POWER_STEP_UP_RES_BOTTOM);
	systemVoltages.stepUpConverter = (uint16_t)rawStepUpConverterVoltage;

	// Sinilizing that new voltage values are available
	systemFlags.systemVoltagesUpdated = true;
}

void Error_Handler(void)
{
	myErrorHandler(HAL_INTERNAL_ERROR);
}

void mpu0DeselectDevice(void)
{
	setIoPin(SPI1_CS_MPU0_GPIO_Port, SPI1_CS_MPU0_Pin);
}

void mpu0SelectDevice(void)
{
	clrIoPin(SPI1_CS_MPU0_GPIO_Port, SPI1_CS_MPU0_Pin);
}

void mpu1DeselectDevice(void)
{
	setIoPin(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin);
}

void mpu1SelectDevice(void)
{
	clrIoPin(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin);
}

void mpu2DeselectDevice(void)
{
	setIoPin(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin);
}

void mpu2SelectDevice(void)
{
	clrIoPin(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin);
}

void mpu3DeselectDevice(void)
{
	setIoPin(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin);
}

void mpu3SelectDevice(void)
{
	clrIoPin(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin);
}

void myErrorHandler(uint16_t errorCode_p)
{
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

void processEventEncoderRotation(void)
{
	// TODO: Action when encoder is rotated

	return;
}

void processEventEncoderButtonPress(void)
{
	// TODO: Action when encoder button is pressed

	return;
}

void processEventSystemVoltagesUpdated(void)
{
	// TODO: Action when new values of system voltage is available

	return;
}

void processEventAcquisitonStartNew(void)
{
	// TODO: Start the devices and create a new file

	return;
}

void processEventSdCardWriteData(void)
{
	// Pop data from circular buffer
	uint8_t sdCardBuff[SD_CARD_BUFF_SIZE];
	sdCardCircBuffer.popBuffer(sdCardBuff, SD_CARD_BUFF_SIZE);
	// SD card mounting
	sdCard.result = f_mount(&sdCard.disk, "", 1);
	verboseStatus((uint16_t)sdCard.result, "SD Card volume mounted");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		sdCard.diskIsMounted = false;
		myErrorHandler(SD_CARD_MOUNT);
	} else {
		sdCard.diskIsMounted = true;
	}
	// Open last created file for append
	sdCard.result = f_open(&sdCard.fileHandler, sdCard.fileName, (BYTE)(FA_WRITE | FA_OPEN_ALWAYS));
	verboseStatus((uint16_t)sdCard.result, "File opened for write operation");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		sdCard.fileIsOpenned = false;
		myErrorHandler(SD_CARD_OPEN_FILE);
	} else {
		sdCard.fileIsOpenned = true;
	}
	sdCard.result = f_lseek(&sdCard.fileHandler, f_size(&sdCard.fileHandler));
	verboseStatus((uint16_t)sdCard.result, "Moving pointer to append data");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		sdCard.fileIsOpenned = false;
		myErrorHandler(SD_CARD_OPEN_FILE);
	} else {
		sdCard.fileIsOpenned = true;
	}
	// Writing data
	UINT bytesWrote;
	sdCard.result = f_write(&sdCard.fileHandler, sdCardBuff, SD_CARD_BUFF_SIZE, &bytesWrote);
	verboseStatus((uint16_t)sdCard.result, "Data written to file");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_WRITE_FILE);
	}
	// Saving empty file
	sdCard.result = f_sync(&sdCard.fileHandler);
	verboseStatus((uint16_t)sdCard.result, "File saved");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_SAVE_FILE);
	}
	// Closing file
	sdCard.result = f_close(&sdCard.fileHandler);
	verboseStatus((uint16_t)sdCard.result, "File closed");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_CLOSE_FILE);
		sdCard.fileIsOpenned = false;
	}
	// SD card umounting
	sdCard.result = f_mount(NULL, "", 0);
	verboseStatus((uint16_t)sdCard.result, "SD Card umounted");
	debugMessage((uint16_t)sdCard.result);
	if(sdCard.result) {
		myErrorHandler(SD_CARD_CLOSE_FILE);
		sdCard.diskIsMounted = false;
	}

	return;
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	// Initializes the CPU, AHB and APB busses clocks
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	// Initializes the CPU, AHB and APB busses clocks
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
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
