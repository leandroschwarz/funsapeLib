/* =============================================================================
 * Project:			dataGrabber
 * File name:		main.cpp
 * Module:			Main source code file
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

/* =============================================================================
 * NOTES
 * =============================================================================
 * Voltage calculation of CR2032 Battery.
 *
 * ADC reference 4096 c.u. = 3300 mV
 *
 * 3300 ----------- 4096
 * Vadc(mV) -------- ADC
 * 0 ----------------- 0
 *
 * Vadc(mV) / 3300 = ADC / 4096
 * Vadc(mV) = 3300 * ADC / 4096
 * Vadc(mV) = (825 * ADC) / 1024
 ** =============================================================================
 * Voltage calculation of 18650 Battery.
 *
 * ADC reference 4096 c.u. = 3300 mV
 * Vadc(mV) = (825 * ADC) / 1024
 *
 * Voltage is divided by two resistor in voltage divider configuration.
 *
 * Top Resistor (R1)		= 12k
 * Bottom Resistor (R2)		= 33k
 *
 * Vadc(mV) = Vbat(mV) * R2 / (R1 + R2)
 * Vadc(mV) * (R1 + R2) = Vbat(mV) * R2
 * Vbat(mV) * = Vadc(mV) * (R1 + R2) / R2
 *
 * Mixing both equations, resuts in
 *
 * Vbat(mV) * = Vadc(mV) * (R1 + R2) / R2
 * Vbat(mV) * = ((825 * ADC) / 1024) * (R1 + R2) / R2
 * Vbat(mV) * = 825 * ADC * (R1 + R2) / (R2 * 1024)
 * =============================================================================
 * Voltage calculation of STEP UP converter.
 *
 * ADC reference 4096 c.u. = 3300 mV
 * Vadc(mV) = (825 * ADC) / 1024
 *
 * Voltage is divided by two resistor in voltage divider configuration.
 *
 * Top Resistor (R3)		= 18k
 * Bottom Resistor (R4)		= 15k
 *
 * Vbat(mV) * = 825 * ADC * (R3 + R4) / (R4 * 1024)
 * =============================================================================
 * SD card data package format (must have 512 bytes in length)
 *
 * 	Byte 0			Start delimiter 1 (0x22)	(1 byte)
 * 	Byte 1			Start delimiter 2 (0x03)	(1 byte)
 * 	Byte 2			Start delimiter 3 (0x19)	(1 byte)
 * 	Byte 3			Start delimiter 4 (0x82)	(1 byte)
 * 	Byte 4-11		Sample information			(8 bytes)
 * 	Byte 12-13		Sample time index			(2 bytes)
 * 		-> 13 Sequential samples, each sample is composed of:
 * 				ECG1 (2 bytes)
 * 				ECG2 (2 bytes)
 * 				ECG3 (2 bytes)
 * 				MPU1_X (2 bytes)
 * 				MPU1_Y (2 bytes)
 * 				MPU1_Z (2 bytes)
 * 				MPU2_X (2 bytes)
 * 				MPU2_Y (2 bytes)
 * 				MPU2_Z (2 bytes)
 * 				MPU3_X (2 bytes)
 * 				MPU3_Y (2 bytes)
 * 				MPU3_Z (2 bytes)
 * 				MPU4_X (2 bytes)
 * 				MPU4_Y (2 bytes)
 * 				MPU4_Z (2 bytes)
 * 				PPG1_IR (2 bytes)
 * 				PPG1_RED (2 bytes)
 * 				PPG2_IR (2 bytes)
 * 				PPG2_RED (2 bytes)
 * 				Total 13 x 38 bytes = 494 bytes
 * 	Bytes 508-510	Blank (0x00)		(3 bytes)
 * 	Byte 511			Checksum			(1 byte)
 * =============================================================================
 * Tempo de comunicação para a leitura de 6 bytes de todos os MPU por SPI
 *
 * 4 MPUs em 400 Hz, prescaler = 128			=> 200ms a cada segundo
 *
 * Verificar se funciona com prescreler = 64, f = 1.125 MHz (MPU suporta 1 MHz
 * segundo o datasheet)
 * ============================================================================
 * Tempo de comunicação para a leitura de 6 bytes do MAX por I2C
 *
 * 1 MAX em 100 Hz, com fscl = 400 kHz			=> 22ms a cada segundo
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// STM32CubeMx and HAL library header files ------------------------------------

#include "main.h"
#include "adc.h"
#include "dma.h"
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

// -----------------------------------------------------------------------------
// FatFs Library header files --------------------------------------------------

#include "ff.h"

// =============================================================================
// File exclusive - Constants
// =============================================================================

// -----------------------------------------------------------------------------
// ADC related constants -------------------------------------------------------

#define ECG_ADC1_BYTES_PER_CHANNEL		2				// Bytes per channel
#define ECG_ADC1_CHANNELS_PER_SAMPLE	3				// Number of channels per sample
#define ECG_ADC1_SAMPLES_PER_BUFFER		8				// Number of samples per buffer (Must be even)
#define ECG_ADC1_CHANNELS_PER_BUFFER	(ECG_ADC1_SAMPLES_PER_BUFFER * ECG_ADC1_CHANNELS_PER_SAMPLE)

#define BAT_ADC3_BYTES_PER_CHANNEL		2				// Bytes per channel
#define BAT_ADC3_CHANNELS_PER_SAMPLE	3				// Number of channels per sample
#define BAT_ADC3_SAMPLES_PER_BUFFER		2				// Number of samples per buffer (Must be even)
#define BAT_ADC3_CHANNELS_PER_BUFFER	(BAT_ADC3_SAMPLES_PER_BUFFER * BAT_ADC3_CHANNELS_PER_SAMPLE)

#define MAIN_BAT_RES_TOP				12
#define MAIN_BAT_RES_BOTTOM				33
#define STEP_UP_RES_TOP					18
#define STEP_UP_RES_BOTTOM				15

// -----------------------------------------------------------------------------
// Debug constants -------------------------------------------------------------

#define ALLOW_FULL_DEBUG				1
#define ALLOW_VERBOSE_DEBUG				1

// -----------------------------------------------------------------------------
// Peripheral buffer sizes -----------------------------------------------------

#define ECG_ADC1_DMA_BUFFER_SIZE		ECG_ADC1_CHANNELS_PER_BUFFER
#define ECG_ADC1_DMA_BUFFER_SIZE_HALF	(ECG_ADC1_DMA_BUFFER_SIZE / 2)
#define BAT_ADC3_DMA_BUFFER_SIZE		BAT_ADC3_CHANNELS_PER_BUFFER
#define BAT_ADC3_DMA_BUFFER_SIZE_HALF	(BAT_ADC3_DMA_BUFFER_SIZE / 2)
#define I2C1_BUFFER_SIZE				8				// EEPROM (X), RTC (8), PPG (6)
#define I2C2_BUFFER_SIZE				6				// RESP (6)
#define SPI1_BUFFER_SIZE				6				// MPU (6)
#define SPI2_BUFFER_SIZE				512				// SD (512)
#define USART1_RX_BUFFER_SIZE			10
#define USART1_TX_BUFFER_SIZE			10
#define USART2_RX_BUFFER_SIZE			10
#define USART2_TX_BUFFER_SIZE			10
#define USART3_RX_BUFFER_SIZE			2				// DMA (2)
#define USART3_TX_BUFFER_SIZE			100				// BLOCKING (100)
#define USB_RX_BUFFER_SIZE				2				// DMA (2)
#define USB_TX_BUFFER_SIZE				100				// BLOCKING (100)

// -----------------------------------------------------------------------------
// Sd Card constants -----------------------------------------------------------

#define SD_CARD_BUFFER_SIZE				SPI2_BUFFER_SIZE

// =============================================================================
// File exclusive - New data types
// =============================================================================

typedef union {
	uint32_t allFlags;
	struct {
		bool		batMeasurementReady			: 1;
		uint32_t	unusedFlags					: 31;
	};
} systemFlags_f;

typedef struct {
	uint32_t		rtcBatery;
	uint32_t		mainBattery;
	uint32_t		stepUpConverter;
} systemVoltages_t;

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

// =============================================================================
// File exclusive - Inline function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Debug related functions -----------------------------------------------------

#if ALLOW_VERBOSE_DEBUG == 1
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
		} HAL_UART_Transmit(&huart3, (uint8_t *)buff, strlen(buff), 100);
	} while(0);
}
#else
void verboseStatus(uint16_t errorCode, const char *strMessage)
{
	return;
}
#endif

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// ADC variables ---------------------------------------------------------------

CircularBuffer<uint16_t>				ecgCircBuffer(300, true);
uint16_t								ecgDmaBuffer[ECG_ADC1_DMA_BUFFER_SIZE];
uint16_t								batDmaBuffer[BAT_ADC3_DMA_BUFFER_SIZE];

// -----------------------------------------------------------------------------
// Generic auxiliary variables -------------------------------------------------

char									auxBuff[1000];
char									auxBuff2[512];
Error									funsapeStatus;
HAL_StatusTypeDef						halStatus;
uint32_t								ticksCounter = 0;
systemFlags_f							systemFlags;
systemVoltages_t						systemVoltages;
vuint16_t								auxTimer = 0;

// -----------------------------------------------------------------------------
// I2C variables ---------------------------------------------------------------

char	 								i2c1Buffer[I2C1_BUFFER_SIZE];
char	 								i2c2Buffer[I2C2_BUFFER_SIZE] __attribute__((unused));

// -----------------------------------------------------------------------------
// Import external variables ---------------------------------------------------

extern __IO uint32_t					uwTick;
extern HAL_TickFreqTypeDef				uwTickFreq;

// -----------------------------------------------------------------------------
// SD Card global variables ----------------------------------------------------

char __attribute__((aligned(4)))		sdCardBuffer[SD_CARD_BUFFER_SIZE];
CircularBuffer<uint8_t>					sdCardCircBuffer(2000, true);
FIL										sdCardFile;
FRESULT									sdCardResult;
FATFS									sdCardVolume;

// -----------------------------------------------------------------------------
// SPI variables ---------------------------------------------------------------

uint8_t 								spi1Buffer[SPI1_BUFFER_SIZE] __attribute__((unused));
uint8_t 								spi2Buffer[SPI2_BUFFER_SIZE] __attribute__((unused));

// -----------------------------------------------------------------------------
// USART variables -------------------------------------------------------------

uint8_t									usart1RxDmaBuffer[USART1_RX_BUFFER_SIZE] __attribute__((unused));
uint8_t 								usart1TxBuffer[USART1_TX_BUFFER_SIZE] __attribute__((unused));
uint8_t									usart2RxDmaBuffer[USART2_RX_BUFFER_SIZE] __attribute__((unused));
uint8_t 								usart2TxBuffer[USART2_TX_BUFFER_SIZE] __attribute__((unused));
CircularBuffer<char>					usart3RxCircBuffer;
uint8_t									usart3RxDmaBuffer[USART3_RX_BUFFER_SIZE];
uint8_t 								usart3TxBuffer[USART3_TX_BUFFER_SIZE] __attribute__((unused));

// -----------------------------------------------------------------------------
// USB variables ---------------------------------------------------------------

uint8_t									usbRxDmaBuffer[USB_RX_BUFFER_SIZE] __attribute__((unused));
uint8_t 								usbTxBuffer[USB_TX_BUFFER_SIZE] __attribute__((unused));

// =============================================================================
// General function declarations
// =============================================================================

// -----------------------------------------------------------------------------
// STM32CubeMX functions -------------------------------------------------------

void Error_Handler(void);
void SystemClock_Config(void);

// -----------------------------------------------------------------------------
// SD Card functions -----------------------------------------------------------

void disk_timerproc(void);

// -----------------------------------------------------------------------------
// System status and configuration functions -----------------------------------

void adcConfiguration(void);
void myErrorHandler(uint16_t errorCode_p);
void sdCardConfiguration(void);
void usartConfiguration(void);

// =============================================================================
// Main function
// =============================================================================

int main(void)
{

	// -------------------------------------------------------------------------
	// Initialize variables
	// -------------------------------------------------------------------------

	systemFlags.allFlags = 0;
	systemVoltages.rtcBatery = 0;
	systemVoltages.mainBattery = 0;
	systemVoltages.stepUpConverter = 0;

	// -------------------------------------------------------------------------
	// HAL Library initialization
	// -------------------------------------------------------------------------

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

	// -------------------------------------------------------------------------
	// Trigger pulse for logic analyzer
	// -------------------------------------------------------------------------

	delayMs(100);
	debug7High();
	delayMs(10);
	debug7Low();
	delayMs(10);
	debug7High();
	delayMs(10);
	debug7Low();

	// -------------------------------------------------------------------------
	// FunSAPE Library debug configuration
	// -------------------------------------------------------------------------

#if ALLOW_FULL_DEBUG == 1
	debug.init(&huart3, false, false, true);
#else
	debug.init(&huart3);
#endif

	// -------------------------------------------------------------------------
	// Peripheral configurations
	// -------------------------------------------------------------------------

	usartConfiguration();
	adcConfiguration();
//	sdCardConfiguration();





	while(1) {
		ledToggle();
		delayMs(200);
	}


	// char spi1TxBuffer[100];
	// char spi1RxBuffer[100];

	// i2c1Buffer[0] = 0x00;
	// i2c1Buffer[1] = 0x34;
	// i2c1Buffer[2] = 0x12;
	// i2c1Buffer[3] = 0x04;
	// i2c1Buffer[4] = 0x17;
	// i2c1Buffer[5] = 0x20;
	// i2c1Buffer[6] = 0x06;
	// i2c1Buffer[7] = 1 << 4;

	// HAL_I2C_Mem_Write(&hi2c1, 0x68 << 1, 0x00, I2C_MEMADD_SIZE_8BIT, (uint8_t *)i2c1Buffer, 8, 10);
	sprintf(i2c1Buffer, "teste");

	delayMs(100);
	while(1) {
		i2c1Buffer[0] = 0x3B;
		// HAL_I2C_Master_Transmit(&hi2c1, 0x68 << 1, (uint8_t *)i2c1Buffer, 1, 10);
		debug7Low();
		HAL_I2C_Mem_Read(&hi2c1, 0x68 << 1, 0x3B, I2C_MEMADD_SIZE_8BIT, (uint8_t *)i2c1Buffer, 6, 10);
		// HAL_I2C_Master_Receive(&hi2c1, 0x68 << 1, (uint8_t *)i2c1Buffer, 6, 10);
		debug7High();
		HAL_UART_Transmit(&huart3, (uint8_t *)i2c1Buffer, 6, 100);
		delayMs(500);
	}

//	sdCardConfiguration();
//	adcConfiguration();

//	for(uint16_t i = 0; i < 512; i++) {
	// 	auxBuff2[i] = (uint8_t)i;
	// }


	while(1) {
		ledToggle();
		delayMs(200);
	}


	// hspi1.Instance = SPI1;
	// hspi1.Init.Mode = SPI_MODE_MASTER;
	// hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	// hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	// hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	// hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	// hspi1.Init.NSS = SPI_NSS_SOFT;
	// hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	// hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	// hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	// hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	// hspi1.Init.CRCPolynomial = 10;
	// if(dryHalSpiInit(&hspi1) != HAL_OK) {
	// 	Error_Handler();
	// }


	//delayMs(1000);
	// debug0High();
	// delayMs(1);
	// debug0Low();
	// delayMs(1);
	// debug0High();
	// delayMs(1);
	// debug0Low();
	// delayMs(1);
	// debug0High();
	// delayMs(1);
	// debug0Low();
	// delayMs(1);
	// debug0High();
	// delayMs(10);
// 	debug0Low();
// 	//dryHalSpiTransmitReceive(&hspi1, (uint8_t *)spi1TxBuffer, (uint8_t *)spi1RxBuffer, 5, 100);

// #define SPI_READ_COMMAND			0x80
// #define SPI_WRITE_COMMAND			0x00

// 	spi1TxBuffer[0] = 0x75 | SPI_READ_COMMAND;
// 	spi1TxBuffer[1] = 0xFF;
// 	spi1TxBuffer[2] = 0xFF;
// 	spi1TxBuffer[3] = 0xFF;
// 	spi1TxBuffer[4] = 0xFF;
// 	spi1TxBuffer[5] = 0xFF;
// 	spi1TxBuffer[6] = 0xFF;


// 	while(1) {
// 		delayMs(100);
// 		ledToggle();
// 		debug0Low();
// 		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi1TxBuffer, (uint8_t *)spi1RxBuffer, 7, 2);
// 		debug0High();
// 		debug1Low();
// 		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi1TxBuffer, (uint8_t *)spi1RxBuffer, 7, 2);
// 		debug1High();
// 		debug2Low();
// 		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi1TxBuffer, (uint8_t *)spi1RxBuffer, 7, 2);
// 		debug2High();
// 		debug3Low();
// 		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi1TxBuffer, (uint8_t *)spi1RxBuffer, 7, 2);
// 		debug3High();
// 		ledToggle();
// 	}

// =========================================================================
// Main loop
// =========================================================================

	while(1) {

		// ---------------------------------------------------------------------
		// Transfer data samples to SD Card cicular buffer ---------------------

		if(ecgCircBuffer.getOccupation() >= 64) {
			// if(ecgCircBuffer.getOccupation() >= (13 * 3)) {

			// uint16_t timeIndex = 0;
			// uint16_t packageIndex = 0;
			// uint16_t auxEcg[13 * 3];
			// uint16_t auxMpu[13 * 12];
			// uint16_t auxPpg[13 * 4];

			// Pop data from ECG buffer
			// ecgCircBuffer.popBuffer(auxEcg, (13 * 3));
			ecgCircBuffer.popBuffer((uint16_t *)auxBuff, 64);
			funsapeStatus = ecgCircBuffer.getLastError();
			// verboseStatus((uint16_t)funsapeStatus, "ECG CIRC => BUFFER");
			debugMessage((uint16_t)funsapeStatus);
			if((uint16_t)funsapeStatus) {
				myErrorHandler(10);
			}

			// Pop data from MPU buffer

			// Pop data from PPG buffer

			// sdCardBuffer[packageIndex++] = 0x22;			// Start delimiter 1
			// sdCardBuffer[packageIndex++] = 0x03;			// Start delimiter 2
			// sdCardBuffer[packageIndex++] = 0x19;			// Start delimiter 3
			// sdCardBuffer[packageIndex++] = 0x82;			// Start delimiter 4

			// sdCardBuffer[packageIndex++] = 0xAA;			// Sample information 1
			// sdCardBuffer[packageIndex++] = 0xBB;			// Sample information 2
			// sdCardBuffer[packageIndex++] = 0xCC;			// Sample information 3
			// sdCardBuffer[packageIndex++] = 0xDD;			// Sample information 4
			// sdCardBuffer[packageIndex++] = 0xDD;			// Sample information 5
			// sdCardBuffer[packageIndex++] = 0xCC;			// Sample information 6
			// sdCardBuffer[packageIndex++] = 0xBB;			// Sample information 7
			// sdCardBuffer[packageIndex++] = 0xAA;			// Sample information 8

			// timeIndex++;
			// sdCardBuffer[packageIndex++] = (uint8_t)(timeIndex & 0x00FF);			// Time index 1
			// sdCardBuffer[packageIndex++] = (uint8_t)(timeIndex >> 8);				// Time index 2

			// ecgCircBuffer.popBuffer(auxEcg, (13 * 3));
//		mpuCircBuffer.popBuffer(auxMpu, (13 * 12));
//		ppgCircBuffer.popBuffer(auxPpg, (13 * 4));

			// for(uint8_t i = 0; i < 13; i++) {
			// 	// ECG
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxEcg[(3 * i) + 0] & 0x00FF);		// ECG1
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxEcg[(3 * i) + 0] >> 8);			// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxEcg[(3 * i) + 1] & 0x00FF);		// ECG2
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxEcg[(3 * i) + 1] >> 8);			// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxEcg[(3 * i) + 2] & 0x00FF);		// ECG3
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxEcg[(3 * i) + 2] >> 8);			// ...
			// 	// MPU
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 0] & 0x00FF);	// MPU1_x
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 0] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 1] & 0x00FF);	// MPU1_Y
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 1] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 2] & 0x00FF);	// MPU1_Z
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 2] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 3] & 0x00FF);	// MPU2_x
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 3] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 4] & 0x00FF);	// MPU2_Y
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 4] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 5] & 0x00FF);	// MPU2_Z
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 5] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 6] & 0x00FF);	// MPU3_x
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 6] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 7] & 0x00FF);	// MPU3_Y
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 7] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 8] & 0x00FF);	// MPU3_Z
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 8] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 9] & 0x00FF);	// MPU4_x
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 9] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 10] & 0x00FF);	// MPU4_Y
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 10] >> 8);		// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 11] & 0x00FF);	// MPU4_Z
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxMpu[(12 * i) + 11] >> 8);		// ...
			// 	// PPG
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 0] & 0x00FF);		// PPG1_IR
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 0] >> 8);			// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 1] & 0x00FF);		// PPG1_RED
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 1] >> 8);			// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 2] & 0x00FF);		// PPG2_IR
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 2] >> 8);			// ...
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 3] & 0x00FF);		// PPG2_RED
			// 	sdCardBuffer[packageIndex++] = (uint8_t)(auxPpg[(4 * i) + 3] >> 8);			// ...
			// }

			// sdCardBuffer[packageIndex++] = 0x00;											// Blank 1
			// sdCardBuffer[packageIndex++] = 0x00;											// Blank 2
			// sdCardBuffer[packageIndex++] = 0x00;											// Blank 3
			// sdCardBuffer[packageIndex++] = 0x00;											// Blank 4
			// sdCardBuffer[packageIndex++] = 0x00;											// Blank 5

			// sdCardBuffer[packageIndex++] = 0x7E;											// Checksum
			// Temporary fixed in 0x7E to test

			sdCardCircBuffer.pushBuffer((uint8_t *)auxBuff, 128);
			funsapeStatus = sdCardCircBuffer.getLastError();
			// verboseStatus((uint16_t)funsapeStatus, "BUFFER => SD CIRC");
			debugMessage((uint16_t)funsapeStatus);
			if((uint16_t)funsapeStatus) {
				myErrorHandler(11);
			}
		}
		if(sdCardCircBuffer.getOccupation() >= 512) {
			auxTimer = 0;
			UINT auxUint;

			sdCardCircBuffer.popBuffer((uint8_t *)auxBuff, 512);
			funsapeStatus = sdCardCircBuffer.getLastError();
			// verboseStatus((uint16_t)funsapeStatus, "SD CIRC => BUFFER");
			debugMessage((uint16_t)funsapeStatus);
			if((uint16_t)funsapeStatus) {
				myErrorHandler(12);
			}
			sdCardResult = f_write(&sdCardFile, auxBuff2, 512, (UINT *)&auxUint);
			// verboseStatus((uint16_t)sdCardResult, "BUFFER => SD");
			debugMessage((uint16_t)sdCardResult);
			if((uint16_t)funsapeStatus) {
				myErrorHandler(13);
			}
			sdCardResult = f_sync(&sdCardFile);
			verboseStatus((uint16_t)sdCardResult, "SAVE SD");
			debugMessage((uint16_t)sdCardResult);
			if((uint16_t)funsapeStatus) {
				myErrorHandler(14);
			}
			sprintf(auxBuff, "timer = %ums\r", auxTimer);
			HAL_UART_Transmit(&huart3, (uint8_t *)auxBuff, strlen(auxBuff), 100);
		}

		// ---------------------------------------------------------------------
		// Evaluate voltage levels ---------------------------------------------

		if(systemFlags.batMeasurementReady) {
			// Calculate CR2032 voltage
			systemVoltages.rtcBatery *= 825;
			systemVoltages.rtcBatery /= 1024;

			// Calculate Main Battery voltage
			systemVoltages.mainBattery *= 825;
			systemVoltages.mainBattery *= (MAIN_BAT_RES_TOP + MAIN_BAT_RES_BOTTOM);
			systemVoltages.mainBattery /= (1024 * MAIN_BAT_RES_BOTTOM);

			// Calculae Step Up converter voltage
			systemVoltages.stepUpConverter *= 825;
			systemVoltages.stepUpConverter *= (STEP_UP_RES_TOP + STEP_UP_RES_BOTTOM);
			systemVoltages.stepUpConverter /= (1024 * STEP_UP_RES_BOTTOM);

			// Print results
			// sprintf(auxBuff, "CR2032 = %u.%03u V, BAT = %u.%03u V, STEP = %u.%03u V\r",
			// 		(uint16_t)(voltageCr2032 / 1000), (uint16_t)(voltageCr2032 % 1000),
			// 		(uint16_t)(voltageBattery / 1000), (uint16_t)(voltageBattery % 1000),
			// 		(uint16_t)(voltageStepUp / 1000), (uint16_t)(voltageStepUp % 1000));
			// HAL_UART_Transmit(&huart3, (uint8_t *)auxBuff, strlen(auxBuff), 100);

			// Clear flag before leave
			systemFlags.batMeasurementReady = false;
		}
	}
	return 0;
}

// =============================================================================
// Private function definitions
// =============================================================================

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	*/
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
	/** Initializes the CPU, AHB and APB busses clocks
	*/
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
	disk_timerproc();
	ticksCounter++;
	auxTimer++;
}

// =============================================================================
// Callback function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// UART interrupt callbacks ----------------------------------------------------

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	usart3RxCircBuffer.push(usart3RxDmaBuffer[0]);
	return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	usart3RxCircBuffer.push(usart3RxDmaBuffer[1]);
	return;
}

// -----------------------------------------------------------------------------
// ADC interrupt callbacks -----------------------------------------------------

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1) {
//		debug2High();
		if(!ecgCircBuffer.pushBuffer(&ecgDmaBuffer[0], ECG_ADC1_DMA_BUFFER_SIZE_HALF)) {
			funsapeStatus = ecgCircBuffer.getLastError();
			verboseStatus((uint16_t)funsapeStatus, "ERRO HALF");
			debugMessage((uint16_t)funsapeStatus);
		}
//		debug2Low();
	} else if(hadc->Instance == ADC3) {
		systemVoltages.rtcBatery = (uint32_t)batDmaBuffer[0];
		systemVoltages.mainBattery = (uint32_t)batDmaBuffer[1];
		systemVoltages.stepUpConverter = (uint32_t)batDmaBuffer[2];
		systemFlags.batMeasurementReady = true;
	}

	return;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1) {
//		debug3High();
		if(!ecgCircBuffer.pushBuffer(&ecgDmaBuffer[ECG_ADC1_DMA_BUFFER_SIZE_HALF], ECG_ADC1_DMA_BUFFER_SIZE_HALF)) {
			funsapeStatus = ecgCircBuffer.getLastError();
			verboseStatus((uint16_t)funsapeStatus, "ERRO FULL");
			debugMessage((uint16_t)funsapeStatus);
		}
//		debug3Low();
	} else if(hadc->Instance == ADC3) {
		systemVoltages.rtcBatery = (uint32_t)batDmaBuffer[0 + BAT_ADC3_DMA_BUFFER_SIZE_HALF];
		systemVoltages.mainBattery = (uint32_t)batDmaBuffer[1 + BAT_ADC3_DMA_BUFFER_SIZE_HALF];
		systemVoltages.stepUpConverter = (uint32_t)batDmaBuffer[2 + BAT_ADC3_DMA_BUFFER_SIZE_HALF];
		systemFlags.batMeasurementReady = true;
	}

	return;
}

// -----------------------------------------------------------------------------
// TIMER interrupt callbacks ---------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) {
		//debug0Toggle();
	} else if(htim->Instance == TIM8) {
//		debug1Toggle();
	}
}

// =============================================================================
// General function definitions
// =============================================================================

void adcConfiguration(void)
{
	// -------------------------------------------------------------------------
	// ADC1 configuration (ECG)
	// -------------------------------------------------------------------------

	// ECG DMA configuration
	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ecgDmaBuffer), ECG_ADC1_DMA_BUFFER_SIZE);
	verboseStatus((uint16_t)halStatus, "ADC1 configured to use DMA");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(6);
	}

	// -------------------------------------------------------------------------
	// ADC3 configuration (BAT)
	// -------------------------------------------------------------------------

	// BAT DMA configuration
	halStatus = HAL_ADC_Start_DMA(&hadc3, (uint32_t *)(batDmaBuffer), BAT_ADC3_DMA_BUFFER_SIZE);
	verboseStatus((uint16_t)halStatus, "ADC3 configured to use DMA");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(7);
	}

	// -------------------------------------------------------------------------
	// TIMER3 configuration (ADC1 Trigger)
	// -------------------------------------------------------------------------

	halStatus = HAL_TIM_Base_Start_IT(&htim3);
	verboseStatus((uint16_t)halStatus, "TIMER3 configured");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(8);
	}

	// -------------------------------------------------------------------------
	// TIMER8 configuration (ADC3 Trigger)
	// -------------------------------------------------------------------------

	halStatus = HAL_TIM_Base_Start_IT(&htim8);
	verboseStatus((uint16_t)halStatus, "TIMER8 configured");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(9);
	}

	return;
}

void Error_Handler(void)
{
	myErrorHandler(1);
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

void sdCardConfiguration(void)
{
	// -------------------------------------------------------------------------
	// SD Card configuration
	// -------------------------------------------------------------------------

	// SD card mounting
	sdCardResult = f_mount(&sdCardVolume, "", 1);
	verboseStatus((uint16_t)sdCardResult, "SD Card volume mounted");
	debugMessage((uint16_t)sdCardResult);
	if(sdCardResult) {
		myErrorHandler(3);
	}

	// Searching for last created file
	char fileName[13];
	uint16_t fileNumber = 1;
	do {
		sprintf(fileName, "file%04u.txt", fileNumber);
		sdCardResult = f_open(&sdCardFile, fileName, (BYTE)(FA_WRITE | FA_CREATE_NEW));
		if(!sdCardResult) {
			break;
		}
	} while((++fileNumber) < 9999);
	verboseStatus((uint16_t)sdCardResult, "New file created");
	debugMessage((uint16_t)sdCardResult);
	if(sdCardResult) {
		myErrorHandler(4);
	}

	// Saving empty file
	sdCardResult = f_sync(&sdCardFile);
	verboseStatus((uint16_t)sdCardResult, "Empty file saved");
	debugMessage((uint16_t)sdCardResult);
	if(sdCardResult) {
		myErrorHandler(5);
	}

	return;
}

void usartConfiguration(void)
{
	// -------------------------------------------------------------------------
	// Sending two <CR> bytes to clear buffer
	// -------------------------------------------------------------------------

	auxBuff[0] = '\r';
	auxBuff[1] = '\r';
	HAL_UART_Transmit(&huart3, (uint8_t *)auxBuff, 2, 100);

	// -------------------------------------------------------------------------
	// Configuring UART receiver to use DMA
	// -------------------------------------------------------------------------

	halStatus = HAL_UART_Receive_DMA(&huart3, usart3RxDmaBuffer, 2);
	verboseStatus((uint16_t)halStatus, "UART3 reception configured to use DMA");
	debugMessage((uint16_t)halStatus);
	if((uint16_t)halStatus) {
		myErrorHandler(2);
	}

	return;
}
