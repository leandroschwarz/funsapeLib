/* =============================================================================
 * Project:			bioSignalGrabber
 * File name:		main.cpp
 * Module:			bioSignalGrabber
 * Author:			Leandro Schwarz
 * Build:			201908192216
 * Last edition:	2019-09-21 00:29
 * ========================================================================== */

// =============================================================================
// Hardware pin allocation
// =============================================================================

// -----------------------------------------------------------------------------
// Pin id		Module		Pin name			Mode			Def. State
// -----------------------------------------------------------------------------
// PA0			ADC1		ADC1_ECG0			Analog
// PA1			ADC1		ADC1_ECG1			Analog
// PA2			ADC1		ADC1_ECG2			Analog
// PA3			ADC1		ADC1_BELT0			Analog
// PA4			ADC1		ADC1_BELT1			Analog
// PA5			SPI1		SPI1_SCK			Alt. SPI		-
// PA6			SPI1		SPI1_MISO			Alt. SPI		-
// PA7			SPI1		SPI1_MOSI			Alt. SPI		-
// PA8			RCC			RCC_MCO
// PA9			GPIO		SPI2_CS_MPU1		Dig. Output		High
// PA10			GPIO		SPI2_CS_MPU2		Dig. Output		High
// PA11			USB			USB_DM				Alt. USB		-
// PA12			USB			USB_DP				Alt. USB		-
// PA13			SYS			SYS_JTMS_SWDIO		Alt. SWD		-
// PA14			SYS			SYS_JTCK_SWCLK		Alt. SWD		-
// PA15			GPIO		SPI2_CS_MPU3		Dig. Output		High
// PB0			GPIO		SPI1_CD_SD			Dig. Output		High
// PB1			ADC1		ADC1_BAT
// PB2			GPIO		BOOT1				Dig. Input		Tristate
// PB3			GPIO		DS1302_CE
// PB4			GPIO		USART1_BT_STATE
// PB5			GPIO		USART1_BT_EN
// PB6			USART1		USART1_BT_TX		Alt. UART		-
// PB7			USART1		USART1_BT_RX		Alt. UART		-
// PB8			I2C1		I2C1_SCL
// PB9			I2C1		I2C1_SDA
// PB10			GPIO		DS1302_SCK
// PB11			GPIO		DS1302_DIO
// PB12			GPIO		SPI2_CS_MPU0		Dig. Output		High
// PB13			SPI2		SPI2_SCK			Alt. SPI		-
// PB14			SPI2		SPI2_MISO			Alt. SPI		-
// PB15			SPI2		SPI2_MOSI			Alt. SPI		-
// PC13			GPIO		NLED				Dig. Output		High
// PC14			RCC			RCC_OSC32_IN		Crystal			-
// PC15			RCC			RCC_OSC32_OUT		Crystal			-
// PD0			RCC			RCC_OSC_IN			Crystal			-
// PD1			RCC			RCC_OSC_OUT			Crystal			-
// -----------------------------------------------------------------------------

// =============================================================================
// Include files
// =============================================================================

// -----------------------------------------------------------------------------
// Standard C library headers --------------------------------------------------

#include <stdio.h>
#include <string.h>

// -----------------------------------------------------------------------------
// STM32CubeMX headers ---------------------------------------------------------

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "usb_device.h"

// -----------------------------------------------------------------------------
// FunSAPE library headers -----------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
// #include "funsapeLibSpi.hpp"
// #include "devices/funsapeLibMpu9250.hpp"
#include "util/funsapeLibCircularBuffer.hpp"
#include "util/funsapeLibDebug.hpp"

// -----------------------------------------------------------------------------
// FatFs library headers -------------------------------------------------------

// #include "ff.h"
// #include "diskio.h"

// =============================================================================
// File exclusive - Constant definitions
// =============================================================================

#define CIRC_BUFF_MULTIPLIER			3

// -----------------------------------------------------------------------------
// ADC constants ---------------------------------------------------------------

#define ADC_CHA_SIZE_BYTES				2
#define ADC_REG_CHA_PER_SAM				5
#define ADC_REG_SAM_PER_BUF				20
#define ADC_REG_BUF_SIZE_BYTES			(ADC_REG_CHA_PER_SAM * ADC_REG_SAM_PER_BUF * ADC_CHA_SIZE_BYTES)
#define ADC_REG_BUF_SIZE_HWORDS			(ADC_REG_BUF_SIZE_BYTES / 2)
// #define ADC_REG_BUF_SIZE_WORDS			(ADC_REG_BUF_SIZE_BYTES / 4)
// #define ADC_REG_HALF_BUF_SIZE_BYTES		(ADC_REG_BUF_SIZE_BYTES / 2)
// #define ADC_REG_HALF_BUF_SIZE_HWORDS	(ADC_REG_BUF_SIZE_HWORDS / 2)
// #define ADC_REG_HALF_BUF_SIZE_WORDS		(ADC_REG_BUF_SIZE_WORDS / 2)
// #define ADC_REG_CIRC_BUF_SIZE_BYTES		(CIRC_BUFF_MULTIPLIER * ADC_REG_BUF_SIZE_BYTES)
#define ADC_REG_CIRC_BUF_SIZE_HWORDS	(CIRC_BUFF_MULTIPLIER * ADC_REG_BUF_SIZE_HWORDS)
// #define ADC_REG_CIRC_BUF_SIZE_WORDS		(CIRC_BUFF_MULTIPLIER * ADC_REG_BUF_SIZE_WORDS)
// #define ADC_INJ_CHA_PER_SAM				3
// #define ADC_INJ_SAM_PER_BUF				1
// #define ADC_INJ_BUF_SIZE_BYTES			(ADC_INJ_CHA_PER_SAM * ADC_INJ_SAM_PER_BUF * ADC_CHA_SIZE_BYTES)
// #define ADC_INJ_BUF_SIZE_HWORDS			(ADC_INJ_BUF_SIZE_BYTES / 2)

// -----------------------------------------------------------------------------
// I2C constants ---------------------------------------------------------------

// #define I2C1_BUFFER_SIZE				14
// #define I2C1_DEFAULT_TIMEOUT			10

// -----------------------------------------------------------------------------
// MPU9250 constants -----------------------------------------------------------

// #define MPU9250_I2C_BASE_SLAVE_ADDRESS	0x68
// #define MPU9250_I2C_SLAVE_ADDRESS		(MPU9250_I2C_BASE_SLAVE_ADDRESS << 1)
// #define MPU9250_REG_ACCEL_CONFIG		0x1C
// #define MPU9250_REG_ACCEL_CONFIG2		0x1D
// #define MPU9250_REG_ACCEL_XOUT_H		0x3B
// #define MPU9250_REG_CONFIG				0x1A
// #define MPU9250_REG_GYRO_CONFIG			0x1B
// #define MPU9250_REG_WHO_AM_I			0x75
// #define MPU9250_WHO_AM_I				0x71
#define MPU_CHA_SIZE_BYTES				2
#define MPU_CHA_PER_DEV					3
#define MPU_DEV_PER_SAM					4
#define MPU_SAM_PER_BUF					20
#define MPU_BUF_SIZE_BYTES				(MPU_CHA_PER_DEV * MPU_DEV_PER_SAM * MPU_SAM_PER_BUF * MPU_CHA_SIZE_BYTES)
#define MPU_BUF_SIZE_HWORDS				(MPU_BUF_SIZE_BYTES / 2)
// #define MPU_BUF_SIZE_WORDS				(MPU_BUF_SIZE_BYTES / 4)
// #define MPU_HALF_BUF_SIZE_BYTES			(MPU_BUF_SIZE_BYTES / 2)
// #define MPU_HALF_BUF_SIZE_HWORDS		(MPU_BUF_SIZE_HWORDS / 2)
// #define MPU_HALF_BUF_SIZE_WORDS			(MPU_BUF_SIZE_WORDS / 2)
// #define MPU_CIRC_BUF_SIZE_BYTES			(CIRC_BUFF_MULTIPLIER * MPU_BUF_SIZE_BYTES)
#define MPU_CIRC_BUF_SIZE_HWORDS		(CIRC_BUFF_MULTIPLIER * MPU_BUF_SIZE_HWORDS)
// #define MPU_CIRC_BUF_SIZE_WORDS			(CIRC_BUFF_MULTIPLIER * MPU_BUF_SIZE_WORDS)

// -----------------------------------------------------------------------------
// MAX30100 constants ----------------------------------------------------------

#define PPG_CHA_SIZE_BYTES				4
#define PPG_CHA_PER_DEV					1
#define PPG_DEV_PER_SAM					1
#define PPG_SAM_PER_BUF					20
#define PPG_BUF_SIZE_BYTES				(PPG_CHA_PER_DEV * PPG_DEV_PER_SAM * PPG_SAM_PER_BUF * PPG_CHA_SIZE_BYTES)
// #define PPG_BUF_SIZE_HWORDS				(PPG_BUF_SIZE_BYTES / 2)
#define PPG_BUF_SIZE_WORDS				(PPG_BUF_SIZE_BYTES / 4)
// #define PPG_HALF_BUF_SIZE_BYTES			(PPG_BUF_SIZE_BYTES / 2)
// #define PPG_HALF_BUF_SIZE_HWORDS		(PPG_BUF_SIZE_HWORDS / 2)
// #define PPG_HALF_BUF_SIZE_WORDS			(PPG_BUF_SIZE_WORDS / 2)
// #define PPG_CIRC_BUF_SIZE_BYTES			(CIRC_BUFF_MULTIPLIER * PPG_BUF_SIZE_BYTES)
// #define PPG_CIRC_BUF_SIZE_HWORDS		(CIRC_BUFF_MULTIPLIER * PPG_BUF_SIZE_HWORDS)
#define PPG_CIRC_BUF_SIZE_WORDS			(CIRC_BUFF_MULTIPLIER * PPG_BUF_SIZE_WORDS)

// -----------------------------------------------------------------------------
// SD card constants -----------------------------------------------------------

// #define SD_CARD_BUFFER_SIZE				512
// #define SD_CARD_CIRC_BUF_SIZE_BYTES		1024

// -----------------------------------------------------------------------------
// SPI constants ---------------------------------------------------------------

// #define SPI1_DEFAULT_TIMEOUT			10
// #define SPI2_DEFAULT_TIMEOUT			10

// -----------------------------------------------------------------------------
// UART constants --------------------------------------------------------------

// #define UART1_RX_BUFFER_SIZE			2
#define UART1_TX_BUFFER_SIZE			100
// #define UART1_DEFAULT_TIMEOUT			10

// =============================================================================
// File exclusive - New data types
// =============================================================================

// typedef union systemFlags_f {
// 	uint32_t allFlags;
// 	struct {
// 		bool		acquireData			: 1;
// 		bool		adcSampleReady		: 1;
// 		bool		ppgSampleReady		: 1;
// 		bool		mpuSampleReady		: 1;
// 		bool		startingRecord		: 1;
// 		bool		stopingRecord		: 1;
// 		uint8_t		probeLines			: 7;
// 		uint32_t	unusedBits			: 19;
// 	};
// } systemFlags_f;

// typedef struct RTC_TimeTypeDef {
// 	uint8_t Hours;
// 	uint8_t Minutes;
// 	uint8_t Seconds;
// } RTC_TimeTypeDef;

// typedef struct RTC_DateTypeDef {
// 	uint8_t WeekDay;
// 	uint8_t Month;
// 	uint8_t Date;
// 	uint8_t Year;
// } RTC_DateTypeDef;


// =============================================================================
// File exclusive - Macro-functions definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Stopwatch macrofunctions ----------------------------------------------------

// #define stopwatchStop()					do {clrBit(TIM3->CR1, TIM_CR1_CEN_Pos);} while(0)
// #define stopwatchReset()				do {tim3Counter = 0;TIM3->CNT = 0;} while(0)
// #define stopwatchResume()				do {setBit(TIM3->CR1, TIM_CR1_CEN_Pos);} while(0)
// #define stopwatchStart()				do {stopwatchStop();stopwatchReset();stopwatchResume();} while(0)
// #define stopwatchValue()				(uint32_t)(tim3Counter / 10)

// -----------------------------------------------------------------------------
// I/O pin handle macrofunctions -----------------------------------------------

#define setIoPin(GPIOx, GPIO_Pin)		do {GPIOx->BSRR = (uint32_t)(GPIO_Pin);} while(0)
#define clrIoPin(GPIOx, GPIO_Pin)		do {GPIOx->BRR = (uint32_t)(GPIO_Pin);} while(0)
#define cplIoPin(GPIOx, GPIO_Pin)		do {((GPIOx->ODR & GPIO_Pin) != 0x00u) ? (GPIOx->BRR = (uint32_t)(GPIO_Pin)) : (GPIOx->BSRR = (uint32_t)(GPIO_Pin));} while(0)

// -----------------------------------------------------------------------------
// LED macrofunctions ----------------------------------------------------------

#define ledTurnOff()					setIoPin(NLED_GPIO_Port, NLED_Pin);
#define ledTurnOn()						clrIoPin(NLED_GPIO_Port, NLED_Pin);
#define ledToggle()						cplIoPin(NLED_GPIO_Port, NLED_Pin);

// =============================================================================
// File exclusive - Private functions declarations
// =============================================================================

// -----------------------------------------------------------------------------
// STM32CubeMX functions -------------------------------------------------------

void SystemClock_Config(void);

// -----------------------------------------------------------------------------
// FatFs functions -------------------------------------------------------------

// DWORD get_fattime(void);

// -----------------------------------------------------------------------------
// MPU-9250 functions ----------------------------------------------------------

// void mpu0Select(void);
// void mpu0Deselect(void);
// void mpu1Select(void);
// void mpu1Deselect(void);
// void mpu2Select(void);
// void mpu2Deselect(void);
// void mpu3Select(void);
// void mpu3Deselect(void);

// =============================================================================
// External functions declarations
// =============================================================================

// -----------------------------------------------------------------------------
// FatFs functions -------------------------------------------------------------

// void disk_timerproc(void);

// =============================================================================
// Global variables
// =============================================================================

CircularBuffer<uint16_t>	adcCircBuffer(ADC_REG_CIRC_BUF_SIZE_HWORDS, true);
vuint16_t					adcDmaBuffer[ADC_REG_BUF_SIZE_HWORDS];
// vuint16_t					adcInjBuffer[ADC_INJ_BUF_SIZE_HWORDS];
// CircularBuffer<uint8_t>		millisCircBuffer(100, true);
// vuint32_t					millisenconds = 0;
CircularBuffer<uint16_t>	mpuCircBuffer(MPU_CIRC_BUF_SIZE_HWORDS, true);
CircularBuffer<uint32_t>	ppgCircBuffer(PPG_CIRC_BUF_SIZE_WORDS, true);
// volatile systemFlags_f		systemFlags;

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	// =========================================================================
	// Local variables
	// =========================================================================

	// -------------------------------------------------------------------------
	// SD card variables -------------------------------------------------------

	// UINT					sdCardBytesWritten;
	// CircularBuffer<uint8_t>	sdCardCircBuffer(SD_CARD_CIRC_BUF_SIZE_BYTES, true);
	// FIL						sdCardFileHandler;
	// uint16_t				sdCardFileNumber = 0;
	// FRESULT					sdCardStatus;
	// FATFS					sdCardVolumeHandler;
	// char					sdCardWriteBuffer[SD_CARD_BUFFER_SIZE];

	// -------------------------------------------------------------------------
	// Status and error variables ----------------------------------------------

	Error				funsapeStatus;
	HAL_StatusTypeDef	halStatus;

	// -------------------------------------------------------------------------
	// Spi related variables ---------------------------------------------------

	// Spi					spi2;

	// -------------------------------------------------------------------------
	// UART related variables --------------------------------------------------

	char				uart1TxBuffer[UART1_TX_BUFFER_SIZE];

	// -------------------------------------------------------------------------
	// Mpu9250 devices ---------------------------------------------------------

	// Mpu9250				mpu[4];

	// =========================================================================
	// Initializations
	// =========================================================================

	// -------------------------------------------------------------------------
	// System flags initialization ---------------------------------------------

	// systemFlags.allFlags = 0;

	// -------------------------------------------------------------------------
	// HAL layer initialization ------------------------------------------------

	HAL_Init();							// HAL library initialization
	SystemClock_Config();				// MCU clock configuration
	MX_GPIO_Init();						// GPIO initial configuration
	MX_DMA_Init();						// DMA initial configuration
	MX_ADC1_Init();						// ADC1 initial configuration
	MX_I2C1_Init();						// I2C1 initial configuration
	MX_SPI1_Init();						// SPI1 initial configuration
	MX_SPI2_Init();						// SPI2 initial configuration
	MX_TIM1_Init();						// TIM1 initial configuration
	MX_TIM2_Init();						// TIM2 initial configuration
	MX_TIM3_Init();						// TIM3 initial configuration
	MX_USART1_UART_Init();				// USART1 initial configuration
	MX_USB_DEVICE_Init();				// USB Device initial configuration

	// =========================================================================
	// PROBE pins configuration
	// =========================================================================

	// setIoPin(PROBE0_GPIO_Port, PROBE0_Pin);						// Set I/O state
	// setIoPin(PROBE1_GPIO_Port, PROBE1_Pin);						// ...
	// setIoPin(PROBE2_GPIO_Port, PROBE2_Pin);						// ...
	// setIoPin(PROBE3_GPIO_Port, PROBE3_Pin);						// ...
	// setIoPin(PROBE4_GPIO_Port, PROBE4_Pin);						// ...
	// setIoPin(PROBE5_GPIO_Port, PROBE5_Pin);						// ...
	// setIoPin(PROBE6_GPIO_Port, PROBE6_Pin);						// ...
	// setIoPin(PROBE7_GPIO_Port, PROBE7_Pin);						// ...
	// clrMask(PROBE0_GPIO_Port->CRL, 0x03, GPIO_CRL_CNF3_Pos);	// Config as general purpose output push-pull
	// clrMask(PROBE1_GPIO_Port->CRL, 0x03, GPIO_CRL_CNF4_Pos);	// ...
	// clrMask(PROBE2_GPIO_Port->CRL, 0x03, GPIO_CRL_CNF5_Pos);	// ...
	// clrMask(PROBE3_GPIO_Port->CRH, 0x03, GPIO_CRH_CNF8_Pos);	// ...
	// clrMask(PROBE4_GPIO_Port->CRH, 0x03, GPIO_CRH_CNF9_Pos);	// ...
	// clrMask(PROBE5_GPIO_Port->CRH, 0x03, GPIO_CRH_CNF10_Pos);	// ...
	// clrMask(PROBE6_GPIO_Port->CRH, 0x03, GPIO_CRH_CNF11_Pos);	// ...
	// clrMask(PROBE7_GPIO_Port->CRH, 0x03, GPIO_CRH_CNF13_Pos);	// ...
	// setMask(PROBE0_GPIO_Port->CRL, 0x03, GPIO_CRL_MODE3_Pos);	// Config as output mode, max speed 50 MHz
	// setMask(PROBE1_GPIO_Port->CRL, 0x03, GPIO_CRL_MODE4_Pos);	// ...
	// setMask(PROBE2_GPIO_Port->CRL, 0x03, GPIO_CRL_MODE5_Pos);	// ...
	// setMask(PROBE3_GPIO_Port->CRH, 0x03, GPIO_CRH_MODE8_Pos);	// ...
	// setMask(PROBE4_GPIO_Port->CRH, 0x03, GPIO_CRH_MODE9_Pos);	// ...
	// setMask(PROBE5_GPIO_Port->CRH, 0x03, GPIO_CRH_MODE10_Pos);	// ...
	// setMask(PROBE6_GPIO_Port->CRH, 0x03, GPIO_CRH_MODE11_Pos);	// ...
	// setMask(PROBE7_GPIO_Port->CRH, 0x03, GPIO_CRH_MODE13_Pos);	// ...
	// clrIoPin(PROBE0_GPIO_Port, PROBE0_Pin);						// Clear I/O state
	// clrIoPin(PROBE1_GPIO_Port, PROBE1_Pin);						// ...
	// clrIoPin(PROBE2_GPIO_Port, PROBE2_Pin);						// ...
	// clrIoPin(PROBE3_GPIO_Port, PROBE3_Pin);						// ...
	// clrIoPin(PROBE4_GPIO_Port, PROBE4_Pin);						// ...
	// clrIoPin(PROBE5_GPIO_Port, PROBE5_Pin);						// ...
	// clrIoPin(PROBE6_GPIO_Port, PROBE6_Pin);						// ...
	// clrIoPin(PROBE7_GPIO_Port, PROBE7_Pin);						// ...

	// =========================================================================
	// Object DEBUG
	// =========================================================================

	debug.init(&huart1, false, true);

	// =========================================================================
	// UART1 configuration
	// =========================================================================

	strcpy(uart1TxBuffer, "UART1 is working!\r");
	halStatus = HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxBuffer, strlen(uart1TxBuffer), 10);
	if(halStatus) {
		Error_Handler();
	}

	// =========================================================================
	// Circular buffers configuration
	// =========================================================================

	funsapeStatus = adcCircBuffer.getLastError();
	if(funsapeStatus != Error::NONE) {
		debugMessage(funsapeStatus);
	}
	funsapeStatus = mpuCircBuffer.getLastError();
	if(funsapeStatus != Error::NONE) {
		debugMessage(funsapeStatus);
	}
	funsapeStatus = ppgCircBuffer.getLastError();
	if(funsapeStatus != Error::NONE) {
		debugMessage(funsapeStatus);
	}
	// funsapeStatus = sdCardCircBuffer.getLastError();
	// if(funsapeStatus != Error::NONE) {
	// 	debugMessage(funsapeStatus);
	// }

	// =========================================================================
	// ADC configuration
	// =========================================================================

	// ADC calibration
	halStatus = HAL_ADCEx_Calibration_Start(&hadc1);
	if(halStatus) {
		debugMessage(halStatus);
	}
	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adcDmaBuffer, ADC_REG_BUF_SIZE_HWORDS);
	if(halStatus) {
		debugMessage(halStatus);
	}
	halStatus = HAL_ADCEx_InjectedStart_IT(&hadc1);
	if(halStatus) {
		debugMessage(halStatus);
	}

	// =========================================================================
	// TIMER1 configuration
	// =========================================================================

	halStatus = HAL_TIM_Base_Start_IT(&htim1);
	if(halStatus) {
		debugMessage(halStatus);
	}

	// =========================================================================
	// TIMER2 configuration
	// =========================================================================

	halStatus = HAL_TIM_Base_Start_IT(&htim2);
	if(halStatus) {
		debugMessage(halStatus);
	}

	// =========================================================================
	// TIMER3 configuration
	// =========================================================================

	halStatus = HAL_TIM_Base_Start_IT(&htim3);
	if(halStatus) {
		debugMessage(halStatus);
	}

	// =========================================================================
	// SPI configuration
	// =========================================================================

	// spi2.init(&hspi2, 20);

	// =========================================================================
	// MPU-9250 configuration
	// =========================================================================

	// Set device selection functions
	// mpu[0].setDevice(mpu0Select, mpu0Deselect);
	// mpu[1].setDevice(mpu1Select, mpu1Deselect);
	// mpu[2].setDevice(mpu2Select, mpu2Deselect);
	// mpu[3].setDevice(mpu3Select, mpu3Deselect);

	// // Set handler pointer
	// for(uint8_t i = 0; i < 4; i++) {
	// 	mpu[i].setHandler(&spi2);
	// }

	// // Initialize devices
	// for(uint8_t i = 0; i < 4; i++) {
	// 	// mpu[i].init();
	// }

	// =========================================================================
	// SD Card configuration
	// =========================================================================

	// debugMessage(0);
	// // Mount SD Card volume
	// sdCardStatus = f_mount(&sdCardVolumeHandler, "", 0);
	// if(sdCardStatus) {
	// 	debugMessage(sdCardStatus);
	// }

	// do {
	// 	char fileName[13];
	// 	bool keepTrying = true;

	// 	// Searching for last file created
	// 	do {
	// 		if(sdCardFileNumber == 9999) {
	// 			debugMessage(9999);
	// 		}
	// 		sprintf(fileName, "file%04d.dat", ++sdCardFileNumber);
	// 		sdCardStatus = f_open(&sdCardFileHandler, (const TCHAR *)fileName, (FA_OPEN_EXISTING | FA_READ));
	// 		switch(sdCardStatus) {
	// 		case FR_OK:					// File openned successfully, so, it already exists -> Keep trying
	// 			keepTrying = true;
	// 			break;
	// 		case FR_NO_FILE:			// File does not exists -> Stop trying
	// 			keepTrying = false;
	// 			break;
	// 		case FR_DISK_ERR:			// Operation failed!
	// 		case FR_INT_ERR:
	// 		case FR_NOT_READY:
	// 		case FR_NO_PATH:
	// 		case FR_INVALID_NAME:
	// 		case FR_DENIED:
	// 		case FR_EXIST:
	// 		case FR_INVALID_OBJECT:
	// 		case FR_WRITE_PROTECTED:
	// 		case FR_INVALID_DRIVE:
	// 		case FR_NOT_ENABLED:
	// 		case FR_NO_FILESYSTEM:
	// 		case FR_TIMEOUT:
	// 		case FR_LOCKED:
	// 		case FR_NOT_ENOUGH_CORE:
	// 		case FR_TOO_MANY_OPEN_FILES:
	// 		default:
	// 			debugMessage(sdCardStatus);
	// 			keepTrying = false;
	// 			break;
	// 		}
	// 	} while(keepTrying);

	// 	// Create the new file
	// 	sdCardStatus = f_open(&sdCardFileHandler, (const TCHAR *)fileName, (FA_OPEN_ALWAYS | FA_WRITE));
	// 	if(sdCardStatus) {
	// 		debugMessage(sdCardStatus);
	// 	}

	// 	// Saving file
	// 	sdCardStatus = f_sync(&sdCardFileHandler);
	// 	if(sdCardStatus) {
	// 		debugMessage(sdCardStatus);
	// 	}
	// } while(0);
	// debugMessage(0);

	// systemFlags.acquireData = true;

	// =========================================================================
	// Main loop
	// =========================================================================

	// uint8_t aux8 = 0;

	while(1) {
		// Checks if one complete sample is available in each circular buffer
		// systemFlags.adcSampleReady = (adcCircBuffer.getOccupation() >= ADC_REG_BUF_SIZE_HWORDS) ? true : false;
		// systemFlags.mpuSampleReady = (mpuCircBuffer.getOccupation() >= MPU_BUF_SIZE_HWORDS) ? true : false;
		// systemFlags.ppgSampleReady = (ppgCircBuffer.getOccupation() >= PPG_BUF_SIZE_HWORDS) ? true : false;

		// sprintf(uart1TxBuffer, "adcCirc=%d/%d mpuCirc=%d/%d ppgCirc=%d/%d sdCardCirc=%d/%d, [%d|%d|%d]\r",
		// 		adcCircBuffer.getOccupation(), ADC_REG_CIRC_BUF_SIZE_HWORDS,
		// 		mpuCircBuffer.getOccupation(), MPU_CIRC_BUF_SIZE_HWORDS,
		// 		ppgCircBuffer.getOccupation(), PPG_CIRC_BUF_SIZE_WORDS,
		// 		sdCardCircBuffer.getOccupation(), SD_CARD_CIRC_BUF_SIZE_BYTES,
		// 		systemFlags.adcSampleReady, systemFlags.mpuSampleReady, systemFlags.ppgSampleReady
		// );
		// HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxBuffer, strlen(uart1TxBuffer), 30);
		// HAL_Delay(200);

		// sdCardCircBuffer.push(aux8++);

		// if(systemFlags.adcSampleReady) {
		// 	PROBE6_HIGH();
		// 	for(uint8_t i = 0; i < ADC_REG_SAM_PER_BUF; i ++) {
		// 		uint8_t auxBuffer[50];
		// 		if(!adcCircBuffer.popBuffer((uint16_t *)auxBuffer, ADC_REG_CHA_PER_SAM)) {
		// 			debugMessage(adcCircBuffer.getLastError());
		// 		}
		// 		if(!sdCardCircBuffer.pushBuffer(auxBuffer, 2 * ADC_REG_CHA_PER_SAM)) {
		// 			debugMessage(adcCircBuffer.getLastError());
		// 		}
		// 	}
		// 	PROBE6_LOW();
		// 	// sprintf(uart1TxBuffer, "adcCirc=%d/%d mpuCirc=%d/%d ppgCirc=%d/%d sdCardCirc=%d/%d, [%d|%d|%d]\r",
		// 	// 		adcCircBuffer.getOccupation(), ADC_REG_CIRC_BUF_SIZE_HWORDS,
		// 	// 		mpuCircBuffer.getOccupation(), MPU_CIRC_BUF_SIZE_HWORDS,
		// 	// 		ppgCircBuffer.getOccupation(), PPG_CIRC_BUF_SIZE_WORDS,
		// 	// 		sdCardCircBuffer.getOccupation(), SD_CARD_CIRC_BUF_SIZE_BYTES,
		// 	// 		systemFlags.adcSampleReady, systemFlags.mpuSampleReady, systemFlags.ppgSampleReady
		// 	// );
		// 	// HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxBuffer, strlen(uart1TxBuffer), 30);
		// 	// HAL_Delay(200);
		// }
		// if(sdCardCircBuffer.getOccupation() >= SD_CARD_BUFFER_SIZE) {
		// 	PROBE7_HIGH();
		// 	if(!sdCardCircBuffer.popBuffer((uint8_t *)sdCardWriteBuffer, SD_CARD_BUFFER_SIZE)) {
		// 		debugMessage(sdCardCircBuffer.getLastError());
		// 	}
		// 	sdCardStatus = f_write(&sdCardFileHandler, sdCardWriteBuffer, SD_CARD_BUFFER_SIZE, &sdCardBytesWritten);
		// 	if(sdCardStatus) {
		// 		debugMessage(sdCardStatus);
		// 	}
		// 	if(sdCardBytesWritten != SD_CARD_BUFFER_SIZE) {
		// 		debugMessage(99);
		// 	}
		// 	HAL_Delay(1);
		// 	sdCardStatus = f_sync(&sdCardFileHandler);
		// 	if(sdCardStatus) {
		// 		debugMessage(sdCardStatus);
		// 	}
		// 	HAL_Delay(1);
		// 	PROBE7_LOW();
		// }
	}
}
// =============================================================================
// Interrupt handlers
// =============================================================================

// -----------------------------------------------------------------------------
// ADC interrupt handler functions ---------------------------------------------

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	// if(systemFlags.acquireData) {
	// 	PROBE3_HIGH();
	// 	if(!adcCircBuffer.pushBuffer((uint16_t *)&adcDmaBuffer[0], ADC_REG_HALF_BUF_SIZE_HWORDS)) {
	// 		debugMessage(adcCircBuffer.getLastError());
	// 	}
	// 	PROBE3_LOW();
	// }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	// if(systemFlags.acquireData) {
	// 	PROBE4_HIGH();
	// 	if(!adcCircBuffer.pushBuffer((uint16_t *)&adcDmaBuffer[ADC_REG_HALF_BUF_SIZE_HWORDS], ADC_REG_HALF_BUF_SIZE_HWORDS)) {
	// 		debugMessage(adcCircBuffer.getLastError());
	// 	}
	// 	PROBE4_LOW();
	// }
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	// PROBE5_HIGH();
	// for(uint8_t i = 0; i < ADC_INJ_BUF_SIZE_HWORDS; i++) {
	// 	HAL_ADCEx_InjectedGetValue(&hadc1, (i + ADC_INJECTED_RANK_1));
	// }
	// PROBE5_LOW();
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
}

// -----------------------------------------------------------------------------
// SysTick interrupt handler functions -----------------------------------------

// void HAL_SYSTICK_Callback(void)
// {
// 	if(systemFlags.acquireData) {
// 		millisenconds++;
// 	} else {
// 		millisenconds = 0;
// 	}
// }

// -----------------------------------------------------------------------------
// TIMER interrupt handler functions -------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// 	static uint16_t millis = 0;
	// if(htim->Instance == TIM1) {		// Inected ADC
	// 	PROBE0_TOGGLE();
	// } else if(htim->Instance == TIM2) {	// SD card
	// 	PROBE1_TOGGLE();
	// 	disk_timerproc();
	// } else if(htim->Instance == TIM3) {	// Regular ADC
	// 	PROBE2_TOGGLE();
//	 		tim3Counter++;
// }
}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
}

// -----------------------------------------------------------------------------
// SPI interrupt handler functions ---------------------------------------------

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
}

// -----------------------------------------------------------------------------
// I2C interrupt handler functions ---------------------------------------------

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
}

// -----------------------------------------------------------------------------
// UART interrupt handler functions --------------------------------------------

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
}

// =============================================================================
// Private functions definitions
// =============================================================================

// -----------------------------------------------------------------------------
// MPU-9250 device select and deselect functions -------------------------------

void mpu0Select(void)
{
	clrIoPin(SPI2_CS_MPU0_GPIO_Port, SPI2_CS_MPU0_Pin);
}

void mpu0Deselect(void)
{
	setIoPin(SPI2_CS_MPU0_GPIO_Port, SPI2_CS_MPU0_Pin);
}

void mpu1Select(void)
{
	clrIoPin(SPI2_CS_MPU1_GPIO_Port, SPI2_CS_MPU1_Pin);
}

void mpu1Deselect(void)
{
	setIoPin(SPI2_CS_MPU1_GPIO_Port, SPI2_CS_MPU1_Pin);
}

void mpu2Select(void)
{
	clrIoPin(SPI2_CS_MPU2_GPIO_Port, SPI2_CS_MPU2_Pin);
}

void mpu2Deselect(void)
{
	setIoPin(SPI2_CS_MPU2_GPIO_Port, SPI2_CS_MPU2_Pin);
}

void mpu3Select(void)
{
	clrIoPin(SPI2_CS_MPU3_GPIO_Port, SPI2_CS_MPU3_Pin);
}

void mpu3Deselect(void)
{
	setIoPin(SPI2_CS_MPU3_GPIO_Port, SPI2_CS_MPU3_Pin);
}

// -----------------------------------------------------------------------------
// FatFs date and time functions -----------------------------------------------

// DWORD get_fattime(void)
// {
// 	RTC_TimeTypeDef rtcTime;
// 	RTC_DateTypeDef rtcDate;

// 	// Fixed timestamp - for now // FIXME: Implement DS1302 interface
// 	rtcDate.Year = 19;
// 	rtcDate.Month = 8;
// 	rtcDate.Date = 19;
// 	rtcDate.WeekDay = 2;
// 	rtcTime.Hours = 22;
// 	rtcTime.Minutes = 44;
// 	rtcTime.Seconds = 20;

// 	return (((DWORD)(rtcDate.Year + 20) << 25)			|
// 					((DWORD)rtcDate.Month << 21)		|
// 					((DWORD)rtcDate.Date << 16)			|
// 					((DWORD)rtcTime.Hours << 11)		|
// 					((DWORD)rtcTime.Minutes << 5)		|
// 					((DWORD)rtcTime.Seconds >> 1)
// 			);
// }

// -----------------------------------------------------------------------------
// HAL private functions -------------------------------------------------------

void Error_Handler(void)
{
	while(1) {
		HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
		HAL_Delay(200);
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}
