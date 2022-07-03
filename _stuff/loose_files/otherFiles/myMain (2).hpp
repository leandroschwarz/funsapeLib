/* =============================================================================
 * Project:			STM32
 * File name:		myMain.hpp
 * Module:			Main definitions
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __MY_MAIN_HPP
#define __MY_MAIN_HPP							__BUILD_TO_REPLACE__

// =============================================================================
// Module
// =============================================================================
// 	ADC power chain
// 		-
// 	ADC ECG channels
// 		-
// 	ECG + RESP
// 		-
// 	MPU9250
// 		-
// 	MAX30102
// 		-
// 	SD Card
// 		-
// 	TFT Display
// 		-
// =============================================================================

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// STM32CubeMx and HAL library header files ------------------------------------

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "main.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"

// -----------------------------------------------------------------------------
// FunSAPE Library header files ------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "funsapeLibGpio.hpp"
#include "funsapeLibSpi.hpp"
#include "funsapeLibStopwatch.hpp"
#include "funsapeLibTm1637.hpp"
#include "funsapeLibTwi.hpp"
#include "funsapeLibMpu.hpp"

// -----------------------------------------------------------------------------
// Other Library header files --------------------------------------------------

#include "ff.h"

// =============================================================================
// Constant definitions
// =============================================================================

//     //////////////////////     SYSTEM DEBUG     //////////////////////     //
static cuint16_t constDebugTriggerPulseDelay	= 10;

//     //////////////////////////     ADC1     //////////////////////////     //
static cuint16_t constAdc1BytesPerChannel		= 2;	// Bytes per channel
static cuint16_t constAdc1ChannelsPerSample		= 3;	// Number of channels per sample
static cuint16_t constAdc1SamplesPerBuffer		= 8;	// Number of samples per buffer (must be even)
static cuint16_t constAdc1ChannelsPerBuffer		= (constAdc1ChannelsPerSample *constAdc1SamplesPerBuffer);
static cuint16_t constAdc1DmaBuffSizeFull		= constAdc1ChannelsPerBuffer;
static cuint16_t constAdc1DmaBuffSizeHalf		= (constAdc1DmaBuffSizeFull / 2);
static cuint16_t constAdc1CircBuffSize			= 300;	// Half word (uint16_t) elements

//     //////////////////////////     ADC3     //////////////////////////     //
static cuint16_t constAdc3BytesPerChannel		= 2;	// Bytes per channel
static cuint16_t constAdc3ChannelsPerSample		= 3;	// Number of channels per sample
static cuint16_t constAdc3SamplesPerBuffer		= 2;	// Number of samples per buffer (must be even)
static cuint16_t constAdc3ChannelsPerBuffer		= (constAdc3ChannelsPerSample *constAdc3SamplesPerBuffer);
static cuint16_t constAdc3DmaBuffSizeFull		= constAdc3ChannelsPerBuffer;
static cuint16_t constAdc3DmaBuffSizeHalf		= (constAdc3DmaBuffSizeFull / 2);

//     ////////////////////////    BLUETOOTH     ////////////////////////     //
static cuint16_t constBlutoothCircBufferSize	= 20;

//     /////////////////////////    ENCODER     /////////////////////////     //
static cuint16_t constEncoderButtonDebounce		= 50;	// Value in miliseconds
static cuint16_t constEncoderClockDebounce		= 50;	// Value in miliseconds

//     ///////////////////////     SPI1 (MPU)     ///////////////////////     //
static cuint16_t constMpuBuffSize				= 7;	// MPU (7)
static cuint16_t constMpuCircBuffSize			= 70;	// MPU (7)
static cuint16_t constSpi1Timeout				= 10;	// Value in miliseconds

//     /////////////////////     SPI2 (SD CARD)     /////////////////////     //
static cuint16_t constSdCardCircBuffSize		= 2048;
static cuint16_t constSdCardBuffSize			= 512;	// SD (512)
static cuint16_t constSpi2Timeout				= 10;	// Value in miliseconds
#define FATFS_FORCE_MOUNT				1

//     /////////////////    TWI1 (PPG, RTC, EEPROM)     /////////////////     //
static cuint16_t constPpg1CircBufferSize		= 20;
static cuint16_t constTwi1BuffSize				= 8;	// EEPROM (X), RTC (8), PPG (6)
static cuint16_t constTwi1Timeout				= 10;	// Value in miliseconds

//     //////////////////////////     TWI2     //////////////////////////     //
static cuint16_t constTwi2BuffSize				= 6;	// RESP (6)
static cuint16_t constTwi2Timeout				= 10;	// Value in miliseconds

//     //////////////////////////    UART1     //////////////////////////     //
static cuint16_t constUart1RxBuffSize			= 2;
static cuint16_t constUart1Timeout				= 10;	// Value in miliseconds
static cuint16_t constUart1TxBuffSize			= 100;

//     //////////////////////////    UART2     //////////////////////////     //
static cuint16_t constUart2RxBuffSize			= 2;
static cuint16_t constUart2Timeout				= 10;	// Value in miliseconds
static cuint16_t constUart2TxBuffSize			= 100;

//     //////////////////////////    UART3     //////////////////////////     //
static cuint16_t constUart3RxBuffSize			= 2;	// DMA (2)
static cuint16_t constUart3RxCircBuffSize		= 100;	// BLOCKING (100)
static cuint16_t constUart3Timeout				= 10;	// Value in miliseconds
static cuint16_t constUart3TxBuffSize			= 1000;	// BLOCKING (100)

//     ///////////////////////////    USB     ///////////////////////////     //
static cuint16_t constUsbRxBuffSize				= 2;	// DMA (2)
static cuint16_t constUsbTxBuffSize				= 100;	// BLOCKING (100)

//     //////////////////////     POWER SYSTEM     //////////////////////     //
static cuint16_t constPowerMainResistorTop			= 12;	// Value in kiloohms
static cuint16_t constPowerMainResistorBottom		= 33;	// Value in kiloohms
static cuint16_t constPowerStepUpResistorTop		= 18;	// Value in kiloohms
static cuint16_t constPowerStepUpResistorBottom		= 15;	// Value in kiloohms

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

#define debugStatus(code)				do{display.writeDisplayData(code);}while(0)

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

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
		// bool		encoderValueUpdated			: 1;
		// bool		encoderButtonPressed		: 1;
		bool		rtcUpdated					: 1;

		// Error control
		bool		errorAnimationUpdate		: 1;
		// bool		encoderButtonPressed		: 1;

		// Free to use
		uint32_t	unusedFlags					: 26;
	};
} systemFlags_f;

// -----------------------------------------------------------------------------
// Rotary encoder --------------------------------------------------------------

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

// =============================================================================
// Extern global variables
// =============================================================================

//     //////////////////////////     ADC1     //////////////////////////     //
extern CircularBuffer<uint16_t> adc1CircBuffer;
extern uint16_t adc1DmaBuffer[];

//     //////////////////////////     ADC3     //////////////////////////     //
extern uint16_t adc3DmaBuffer[];

//     ////////////////////////    BLUETOOTH     ////////////////////////     //
// extern CircularBuffer<uint8_t> bluetoothCircBuffer;
// extern Gpio bluetoothEnable;
// extern Gpio bluetoothStatus;

//     /////////////////////    ERROR AND DEBUG     /////////////////////     //
extern vuint64_t chronometer;
extern vuint16_t globalError;
extern Gpio debug1;
extern Gpio debug2;
extern Gpio debug3;
extern Gpio debug4;
extern Gpio debug5;
extern Gpio debug6;
extern Gpio debug7;
extern Gpio debug8;
extern Gpio led;
extern systemFlags_f systemFlags;
extern RTC_DateTypeDef currentDate;
extern RTC_TimeTypeDef currentTime;
extern Error funsapeStatus;
extern HAL_StatusTypeDef halStatus;
extern Gpio tm1637Clk;
extern Gpio tm1637Dio;
extern Tm1637 display;

//     /////////////////////////    ENCODER     /////////////////////////     //
extern encoderData_s encoder;

//     ///////////////////////     SPI1 (MPU)     ///////////////////////     //
// extern Mpu mpu1;
extern Gpio mpu1Cs;
extern CircularBuffer<uint16_t> mpu1CircBuffer;
// extern Mpu mpu2;
extern Gpio mpu2Cs;
extern CircularBuffer<uint16_t> mpu2CircBuffer;
// extern Mpu mpu3;
extern Gpio mpu3Cs;
extern CircularBuffer<uint16_t> mpu3CircBuffer;
// extern Mpu mpu4;
extern Gpio mpu4Cs;
extern CircularBuffer<uint16_t> mpu4CircBuffer;
extern uint16_t mpuStaticBuffer[];
extern Spi spi1;

//     /////////////////////     SPI2 (SD CARD)     /////////////////////     //
extern sdCard_s sdCard;
extern CircularBuffer<uint8_t> sdCardCircBuffer;
extern uint8_t sdCardBuffer[];
extern Spi spi2;

//     ///////////////////////    TFT DISPLAY     ///////////////////////     //
// extern Gpio tftCs;
// extern Gpio tftRd;
// extern Gpio tftRs;
// extern Gpio tftRst;
// extern Gpio tftWr;

//     /////////////////    TWI1 (PPG, RTC, EEPROM)     /////////////////     //
// extern Twi twi1;
// extern CircularBuffer<uint32_t> ppg1CircBuffer;

//     //////////////////////////     TWI2     //////////////////////////     //
// extern Twi twi2;

//     //////////////////////////    UART1     //////////////////////////     //
// extern Uart uart1;
extern char uart1RxBuffer[];
extern char uart1TxBuffer[];

//     //////////////////////////    UART2     //////////////////////////     //
// extern Uart uart2;
extern char uart2RxBuffer[];
extern char uart2TxBuffer[];

//     //////////////////////////    UART3     //////////////////////////     //
// extern Uart uart3;
extern uint8_t uart3RxBuffer[];
extern CircularBuffer<uint8_t> uart3RxCircBuffer;
extern char uart3TxBuffer[];

//     ///////////////////////////    USB     ///////////////////////////     //
// extern Usb usb1;

// =============================================================================
// General public functions declarations
// =============================================================================


void delayUs(uint16_t delay_p);
void Error_Handler(void);						// HAL Library
void Error_Handler(uint16_t errorCode_p);
void sdCardMount(void);

void setupAdc1(void);
void setupAdc3(void);
void setupDebug(void);
void setupHal(void);
void setupRtc(void);
void setupSpi1(void);
void setupSpi2(void);
void setupTimer1(void);
void setupTimer3(void);
void setupTimer7(void);
void setupTimer8(void);
void setupTwi1(void);
void setupTwi2(void);
void setupUart1(void);
void setupUart2(void);
void setupUart3(void);
void SystemClock_Config(void);					// HAL Library

// =============================================================================
// General inline functions definitions
// =============================================================================

inlined void debugTriggerPulse(void)
{
	debug8.low();
	delayMs(constDebugTriggerPulseDelay);
	debug8.high();
	delayMs(constDebugTriggerPulseDelay);
	debug8.low();
	delayMs(constDebugTriggerPulseDelay);
	debug8.high();
	delayMs(constDebugTriggerPulseDelay);
	debug8.low();

	return;
}

inlined uint64_t markChronometer(void)
{
	uint64_t value = 50000 * chronometer + (uint64_t)(htim7.Instance->CNT);
	return value;
}

inlined void startChronometer(void)
{
	htim7.Instance->CNT = 0;							// Reset counter
	chronometer = 0;
	htim7.Instance->ARR = 49999;
	setBit(htim7.Instance->CR1, TIM_CR1_CEN_Pos);		// Start counting
}

inlined uint32_t stopChronometer(void)
{
	clrBit(htim7.Instance->CR1, TIM_CR1_CEN_Pos);		// Stop counting
	uint64_t value = 50000 * chronometer + (uint64_t)(htim7.Instance->CNT);
	return value;
}

#endif // __MY_MAIN_HPP

// =============================================================================
// END OF FILE
// =============================================================================
