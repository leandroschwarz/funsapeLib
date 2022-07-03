/* =============================================================================
 * Project:			bioSignalGrabber
 * File name:		main.cpp
 * Module:			bioSignalGrabber
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */


// =============================================================================
// Hardware pin allocation
// =============================================================================

// -----------------------------------------------------------------------------
// Pin id		Module		Pin name
// -----------------------------------------------------------------------------
// PA0			.			.
// PA1			.			.
// PA2			.			.
// PA3			.			.
// PA4			.			.
// PA5			SPI1		SPI1_SCK
// PA6			SPI1		SPI1_MISO
// PA7			SPI1		SPI1_MOSI
// PA8			.			.
// PA9			.			.
// PA10			.			.
// PA11			USB			USB_DM
// PA12			USB			USB_DP
// PA13			SYS			SYS_JTMS_SWIO
// PA14			SYS			SYS_JTCK_SWCLK
// PA15			GPIO		SPI2_CS_MPU0
// PB0			ADC1		ADC1_IN8
// PB1			ADC1		ADC1_IN9
// PB2			GPIO		BOOT1
// PB3			GPIO		SPI2_CS_MPU1
// PB4			GPIO		SPI2_CS_MPU2
// PB5			GPIO		SPI2_CS_MPU3
// PB6			I2C1		I2C1_SCL
// PB7			I2C1		I2C1_SDA
// PB8			.			.
// PB9			.			.
// PB10			USART3		USART3_TX
// PB11			USART3		USART3_RX
// PB12			GPIO		SPI1_CS_SD
// PB13			SPI2		SPI2_SCK
// PB14			SPI2		SPI2_MISO
// PB15			SPI2		SPI2_MOSI
// PC13			GPIO		NLED
// PC14			RCC			RCC_OSC32_IN
// PC15			RCC			RCC_OSC32_OUT
// PD0			RCC			RCC_OSC_IN
// PD1			RCC			RCC_OSC_OUT
// -----------------------------------------------------------------------------

// =============================================================================
// Include files
// =============================================================================

// Standard C library headers
#include <string.h>
#include <stdio.h>

// STM32CubeMX headers
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

// FunSAPE library headers
// #include "funsapeLibCircularBuffer32.hpp"
// #include "funsapeLibCircularBuffer16.hpp"
// #include "funsapeLibCircularBuffer8.hpp"
#include "funsapeLibSpi.hpp"
#include "devices/funsapeLibMpu9250.hpp"

// FatFs library headers
#include "ff.h"
#include "diskio.h"

// =============================================================================
// File exclusive - Constant definitions
// =============================================================================

// ADC constants
#define ADC_SAMPLES_PER_BUFFER			50
#define ADC_CHANNELS_PER_SAMPLE			2
#define ADC_CHANNELS_PER_BUFFER			(ADC_CHANNELS_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER)
#define ADC_DMA_BUFFER_SIZE_HWORDS		ADC_CHANNELS_PER_BUFFER
#define ADC_DMA_HALF_BUFFER_SIZE_HWORDS	(ADC_DMA_BUFFER_SIZE_HWORDS / 2)
#define ADC_DMA_BUFFER_SIZE_BYTES		(2 * ADC_CHANNELS_PER_BUFFER)
#define ADC_CIRCULAR_BUFFER_SIZE		(5 * ADC_DMA_BUFFER_SIZE_HWORDS)

// I2C constants
#define I2C1_BUFFER_SIZE				14
#define I2C1_DEFAULT_TIMEOUT			10

// MPU9250 constants
#define MPU9250_I2C_BASE_SLAVE_ADDRESS	0x68
#define MPU9250_I2C_SLAVE_ADDRESS		(MPU9250_I2C_BASE_SLAVE_ADDRESS << 1)
#define MPU9250_REG_ACCEL_CONFIG		0x1C
#define MPU9250_REG_ACCEL_CONFIG2		0x1D
#define MPU9250_REG_ACCEL_XOUT_H		0x3B
#define MPU9250_REG_CONFIG				0x1A
#define MPU9250_REG_GYRO_CONFIG			0x1B
#define MPU9250_REG_WHO_AM_I			0x75
#define MPU9250_WHO_AM_I				0x71

// SD card constants
#define SD_CARD_BLOCK_SIZE				512

// SPI constants
#define SPI1_DEFAULT_TIMEOUT			10
#define SPI2_DEFAULT_TIMEOUT			10

// UART constants
#define UART3_RX_DMA_BUFFER_SIZE		2
#define UART3_TX_DMA_BUFFER_SIZE		100
#define UART3_DEFAULT_TIMEOUT			10

// =============================================================================
// File exclusive - New data types
// =============================================================================

typedef union systemFlags_f {
	uint32_t allFlags;
	struct {
		bool startingRecord				: 1;
		bool stopingRecord				: 1;
	};
} systemFlags_f;

// =============================================================================
// File exclusive - Macro-functions definitions
// =============================================================================

// Stopwatch macrofunctions
#define stopwatchStop()					do {clrBit(TIM3->CR1, TIM_CR1_CEN_Pos);} while(0)
#define stopwatchReset()				do {tim3Counter = 0;TIM3->CNT = 0;} while(0)
#define stopwatchResume()				do {setBit(TIM3->CR1, TIM_CR1_CEN_Pos);} while(0)
#define stopwatchStart()				do {stopwatchStop();stopwatchReset();stopwatchResume();} while(0)
#define stopwatchValue()				(uint32_t)(tim3Counter / 10)

// I/O pin handle macrofunctions
#define setIoPin(GPIOx, GPIO_Pin)		do {GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;} while(0)
#define clrIoPin(GPIOx, GPIO_Pin)		do {GPIOx->BSRR = GPIO_Pin;} while(0)
#define cplIoPin(GPIOx, GPIO_Pin)		do {((GPIOx->ODR & GPIO_Pin) != 0x00u) ? (GPIOx->BRR = (uint32_t)GPIO_Pin) : (GPIOx->BSRR = (uint32_t)GPIO_Pin);} while(0)

// LED macrofunctions
#define ledTurnOff()					setIoPin(NLED_GPIO_Port, NLED_Pin);
#define ledTurnOn()						clrIoPin(NLED_GPIO_Port, NLED_Pin);
#define ledToggle()						cplIoPin(NLED_GPIO_Port, NLED_Pin);

// =============================================================================
// File exclusive - Private functions declarations
// =============================================================================

// STM32CubeMX functions
void SystemClock_Config(void);

// FatFs functions
DWORD get_fattime(void);

// MPU-9250 functions
void mpu0Select(void);
void mpu0Deselect(void);
void mpu1Select(void);
void mpu1Deselect(void);
void mpu2Select(void);
void mpu2Deselect(void);
void mpu3Select(void);
void mpu3Deselect(void);

// =============================================================================
// External functions declarations
// =============================================================================

// FatFs functions
void disk_timerproc(void);

// =============================================================================
// Global variables
// =============================================================================

// CircularBuffer16 adcCircBuffer(200);
// CircularBuffer8 mpuCircBuffer(200);
uint16_t adcDmaBuffer[ADC_DMA_BUFFER_SIZE_HWORDS];
uint8_t i2c1Buffer[I2C1_BUFFER_SIZE];
volatile uint32_t tim3Counter = 0;

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	// -------------------------------------------------------------------------
	// Local variables
	// -------------------------------------------------------------------------

	char dataFileName[13];
	char sdCardWriteBuffer[SD_CARD_BLOCK_SIZE];
	char uart3RxDmaBuffer[UART3_RX_DMA_BUFFER_SIZE];
	char uart3TxDmaBuffer[UART3_TX_DMA_BUFFER_SIZE];
	FATFS sdCardVolume;
	FIL dataFileHandler;
	FRESULT fatStatus;
	HAL_StatusTypeDef halStatus;
	Mpu9250 mpu[4];
	Spi spi1;
	UINT sdCardBytesWritten;
	uint16_t dataFileNumber = 0;

	// -------------------------------------------------------------------------
	// HAL library initialization
	// -------------------------------------------------------------------------

	HAL_Init();

	// -------------------------------------------------------------------------
	// MCU clock configuration
	// -------------------------------------------------------------------------

	SystemClock_Config();

	// -------------------------------------------------------------------------
	// MCU peripheral initial configuration
	// -------------------------------------------------------------------------

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_RTC_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_USART3_UART_Init();
	MX_USB_DEVICE_Init();

	// =========================================================================
	// Object DEBUG
	// =========================================================================

	debug.init(&huart3, false, true);

	// =========================================================================
	// UART3 configuration
	// =========================================================================

	strcpy(uart3TxDmaBuffer, "UART3 is working!\r");
	halStatus = HAL_UART_Receive_DMA(&huart3, (uint8_t *)uart3RxDmaBuffer, UART3_RX_DMA_BUFFER_SIZE);
	if(halStatus) {
		debugMessage(halStatus);
	}
	halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxDmaBuffer, strlen(uart3TxDmaBuffer), 10);
	if(halStatus) {
		debugMessage(halStatus);
	}

	do {
		get_fattime();
		HAL_Delay(3000);
	} while(1);


	// =========================================================================
	// TIMER1 configuration
	// =========================================================================

	halStatus = HAL_TIM_Base_Start_IT(&htim1);
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
	// ADC configuration
	// =========================================================================

	// ADC calibration
	halStatus = HAL_ADCEx_Calibration_Start(&hadc1);
	if(halStatus) {
		debugMessage(halStatus);
	}
	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adcDmaBuffer, ADC_DMA_BUFFER_SIZE_HWORDS);
	if(halStatus) {
		debugMessage(halStatus);
	}

	// =========================================================================
	// SPI configuration
	// =========================================================================

	spi1.init(&hspi1, 20);

	// =========================================================================
	// MPU-9250 configuration
	// =========================================================================
	/*
		// Set device selection functions
		mpu[0].setDevice(mpu0Select, mpu0Deselect);
		mpu[1].setDevice(mpu1Select, mpu1Deselect);
		mpu[2].setDevice(mpu2Select, mpu2Deselect);
		mpu[3].setDevice(mpu3Select, mpu3Deselect);

		// Set handler pointer
		for(uint8_t i = 0; i < 4; i++) {
			mpu[i].setHandler(&spi1);
		}

		// Initialize devices
		for(uint8_t i = 0; i < 4; i++) {
			mpu[i].init();
		}
	*/
	// =========================================================================
	// SD Card configuration
	// =========================================================================


	// Mount SD Card volume
	fatStatus = f_mount(&sdCardVolume, "", 0);
	if(fatStatus) {
		debugMessage(fatStatus);
	}

	// Searching for last file created
	do {
		sprintf(dataFileName, "file%04d.dat", ++dataFileNumber);
		fatStatus = f_open(&dataFileHandler, (const TCHAR *)dataFileName, (FA_OPEN_EXISTING | FA_READ));
	} while(fatStatus != FR_OK);

	// Create the new file
	fatStatus = f_open(&dataFileHandler, (const TCHAR *)dataFileName, (FA_OPEN_ALWAYS | FA_WRITE));
	if(fatStatus) {
		debugMessage(fatStatus);
	}

	// Saving file
	fatStatus = f_sync(&dataFileHandler);
	if(fatStatus) {
		debugMessage(fatStatus);
	}

	// =========================================================================
	// Main loop
	// =========================================================================

	while(1) {
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t millis = 0;

	if(htim->Instance == TIM1) {
		disk_timerproc();
		if((millis %= 500) == 0) {
//		HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
		}
		millis++;
	} else if(htim->Instance == TIM3) {
		tim3Counter++;
	}
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

DWORD get_fattime(void)
{
	char uart3TxDmaBuffer[UART3_TX_DMA_BUFFER_SIZE];

	RTC_TimeTypeDef rtcTime;
	RTC_DateTypeDef rtcDate;
	HAL_StatusTypeDef halStatus;

	halStatus = HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
	if(halStatus) {
		debugMessage(halStatus);
	}
	halStatus = HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
	if(halStatus) {
		debugMessage(halStatus);
	}

	sprintf(uart3TxDmaBuffer, "20%02d/%02d/%02d %d %02d:%02d:%02d\r", rtcDate.Year, rtcDate.Month, rtcDate.Date,
			rtcDate.WeekDay, rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxDmaBuffer, strlen(uart3TxDmaBuffer), 10);

	return ((DWORD)(rtcDate.Year + 20) << 25)
			| ((DWORD)rtcDate.Month << 21)
			| ((DWORD)rtcDate.Date << 16)
			| ((DWORD)rtcTime.Hours << 11)
			| ((DWORD)rtcTime.Minutes << 5)
			| ((DWORD)rtcTime.Seconds >> 1);
}

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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}
