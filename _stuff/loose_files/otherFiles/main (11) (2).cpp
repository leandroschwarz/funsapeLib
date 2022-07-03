/* =============================================================================
 * Project:		dataGrabber
 * File name:	main.cpp
 * Module:		Main code
 * Author:		Leandro Schwarz
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

// Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project specific files

#include "peripherals.h"

// GPDSE ARM Library
#include "gpdseArmGlobalDefines.h"
#include "gpdseArmSpi.h"
#include "devices/mpu9250.h"
//#include "devices/sdCard.h"
#include "devices/fatFs.h"
#include "util/packageApi.h"
#include "util/circularBuffer8.h"
#include "util/circularBuffer16.h"

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define ADC_CHANNELS_PER_SAMPLE			4
#define ADC_SAMPLES_PER_BUFFER			2
#define		ADC_BYTES_PER_CHANNEL		sizeof(uint16_t)
#define		ADC_BYTES_PER_SAMPLE		ADC_CHANNELS_PER_SAMPLE * ADC_BYTES_PER_CHANNEL
#define		ADC_BYTES_PER_BUFFER		ADC_BYTES_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER
#define		ADC_CHANNELS_PER_BUFFER		ADC_SAMPLES_PER_BUFFER * ADC_CHANNELS_PER_SAMPLE
#define		ADC_DMA_BUFFER_SIZE			ADC_CHANNELS_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER

#define NUM_ACC_CHANNELS_PER_SAMPLE		4 * 3
#define NUM_ECG_CHANNELS_PER_SAMPLE		2
#define NUM_PPG_CHANNELS_PER_SAMPLE		2
#define SD_BUFFER_OVERHEAD_PER_SAMPLE	1
#define NUM_MAX_SAMPLES_IN_MEMORY		25

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// *INDENT-OFF*
// =============================================================================

// System global variables
uint16_t		batteryStatus = 0;
uint16_t		chargerStatus = 0;

// ADC global variables
uint16_t		adcBufferDmaData[ADC_CHANNELS_PER_BUFFER];
uint16_t *		adcBufferDmaPointer = adcBufferDmaData;

// Circular buffer objects
CircularBuffer16	ecgBuffer((
	NUM_ECG_CHANNELS_PER_SAMPLE * NUM_MAX_SAMPLES_IN_MEMORY) , false);
CircularBuffer16	accBuffer((
	NUM_ACC_CHANNELS_PER_SAMPLE * NUM_MAX_SAMPLES_IN_MEMORY) , false);
CircularBuffer16	ppgBuffer((
	NUM_PPG_CHANNELS_PER_SAMPLE * NUM_MAX_SAMPLES_IN_MEMORY), false);
CircularBuffer8		sdCardBuffer((
	(sizeof(uint8_t) * SD_BUFFER_OVERHEAD_PER_SAMPLE) +
	(sizeof(uint16_t) * NUM_ACC_CHANNELS_PER_SAMPLE * NUM_MAX_SAMPLES_IN_MEMORY) +
	(sizeof(uint16_t) * NUM_ECG_CHANNELS_PER_SAMPLE * NUM_MAX_SAMPLES_IN_MEMORY) +
	(sizeof(uint16_t) * NUM_PPG_CHANNELS_PER_SAMPLE * NUM_MAX_SAMPLES_IN_MEMORY)),
	false);

// SPI objects
Spi spi1;
Spi spi2;

// MPU global variables
#define MPU_DEVICES						4
#define MPU_ACC_AXIS					3
#define MPU_GYR_AXIS					3

Mpu9250 mpuDevice[MPU_DEVICES];
int16_t mpuAccelData[MPU_DEVICES][MPU_ACC_AXIS];
int16_t mpuAccelOffset[MPU_DEVICES][MPU_ACC_AXIS];

// *INDENT-ON*

// =============================================================================
// Static functions declarations
// =============================================================================

void activateMpuDevice1(void);
void deactivateMpuDevice1(void);
void activateMpuDevice2(void);
void deactivateMpuDevice2(void);
void activateMpuDevice3(void);
void deactivateMpuDevice3(void);
void activateMpuDevice4(void);
void deactivateMpuDevice4(void);

bool mpuReadData();
bool mpuReadOffset(void);
bool mpuProcessData(void);

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	// -------------------------------------------------------------------------
	// Local variables ---------------------------------------------------------
	char auxString[100];

	// -------------------------------------------------------------------------
	// Variable initialization -------------------------------------------------
	systemFlags.allFlags = 0;

	// -------------------------------------------------------------------------
	// Peripherals initialization ----------------------------------------------
	initPeripherals();									// General HAL peripherals
	setPin(BLE_CS_GPIO_Port, BLE_CS_Pin);				// Set CS signals - BLE
	setPin(CARD_CS_GPIO_Port, CARD_CS_Pin);				// Set CS signals - SD CARD
	setPin(CHARGER_CS_GPIO_Port, CHARGER_CS_Pin);		// Set CS signals - CHARGER
	setPin(LCD_CS_GPIO_Port, LCD_CS_Pin);				// Set CS signals - LCD
	deactivateMpuDevice1();								// Set CS signals - MPU1
	deactivateMpuDevice2();								// Set CS signals - MPU2
	deactivateMpuDevice3();								// Set CS signals - MPU3
	deactivateMpuDevice4();								// Set CS signals - MPU4
	setPin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin);			// Set CS signals - TOUCH SCREEN

	// GPDSE SPI initialization

	spi1.init(&hspi1, 200);
	spi2.init(&hspi2, 200);

	sprintf(auxString, "Testing Usart\r");
	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);

	// -------------------------------------------------------------------------
	// Configure circular buffers ----------------------------------------------
	do {		// Inside block to release 'error' after test
		error_e error;
		// Check ACC buffer
		if ((error = accBuffer.getLastError()) != ERROR_NONE) {
			die(error);
		}
		// Check ECG buffer
		if ((error = ecgBuffer.getLastError()) != ERROR_NONE) {
			die(error);
		}
		// Check PPG buffer
		if ((error = ppgBuffer.getLastError()) != ERROR_NONE) {
			die(error);
		}
		// Check SD card buffer
		if ((error = sdCardBuffer.getLastError()) != ERROR_NONE) {
			die(error);
		}
	} while (0);

	// -------------------------------------------------------------------------
	// Configure MPU9250 devices -----------------------------------------------


	//

	if (!mpuDevice[0].initSpi(&spi1, activateMpuDevice1, deactivateMpuDevice1)) {
		sprintf(auxString, "mpu9250 Device 1 Error = %d initSpi()\r", mpuDevice[0].getLastError());
		HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	}
	if (!mpuDevice[1].initSpi(&spi1, activateMpuDevice2, deactivateMpuDevice2)) {
		sprintf(auxString, "mpu9250 Device 2 Error = %d initSpi()\r", mpuDevice[1].getLastError());
		HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	}
	if (!mpuDevice[2].initSpi(&spi1, activateMpuDevice3, deactivateMpuDevice3)) {
		sprintf(auxString, "mpu9250 Device 3 Error = %d initSpi()\r", mpuDevice[2].getLastError());
		HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	}
	if (!mpuDevice[3].initSpi(&spi1, activateMpuDevice4, deactivateMpuDevice4)) {
		sprintf(auxString, "mpu9250 Device 4 Error = %d initSpi()\r", mpuDevice[3].getLastError());
		HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	}

	for (uint8_t i = 0; i < MPU_DEVICES; i++) {
		if (!mpuDevice[i].configGyro(AXIS_NONE, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_32_KHZ, MPU9250_GYRO_DLPF_8800_HZ)) {
			sprintf(auxString, "mpu9250 Device %d Error = %d configGyro()\r", i, mpuDevice[i].getLastError());
			HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
		}
		if (!mpuDevice[i].configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_4_KHZ, MPU9250_ACCEL_DLPF_1046_HZ)) {
			sprintf(auxString, "mpu9250 Device %d Error = %d configAccel()\r", i, mpuDevice[i].getLastError());
			HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
		}
	}




	// int16_t accelBuff[3] = {0, 0, 0};
	// if (!mpuDevice1.setAccelOffset(accelBuff)) {
	// 	sprintf(auxString, "mpu9250 Device 1 Error = %d setAccelOffset()\r", mpuDevice1.getLastError());
	// 	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	// }
	// if (!mpuDevice2.setAccelOffset(accelBuff)) {
	// 	sprintf(auxString, "mpu9250 Device 2 Error = %d setAccelOffset()\r", mpuDevice2.getLastError());
	// 	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	// }
	// if (!mpuDevice3.setAccelOffset(accelBuff)) {
	// 	sprintf(auxString, "mpu9250 Device 3 Error = %d setAccelOffset()\r", mpuDevice3.getLastError());
	// 	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	// }
	// if (!mpuDevice4.setAccelOffset(accelBuff)) {
	// 	sprintf(auxString, "mpu9250 Device 4 Error = %d setAccelOffset()\r", mpuDevice4.getLastError());
	// 	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	// }

	while (1) {

		mpuReadData();
		mpuProcessData();
		HAL_Delay(500);
	}

// Configure SysTick
//	- Configure Jobs
// Configure I2C
// 	- Configure PPG sensor
// Configure SPI 1
// 	- Configure BLE
//	- Configure MPU 1
//	- Configure MPU 2
//	- Configure MPU 3
//	- Configure MPU 4
// Configure SPI 2
//	- Configure SD card

// Configure GPIO
//	- Configure LCD
//	- Configure TOUCH
//	- Configure CHARGER

// -------------------------------------------------------------------------
// Main loop ---------------------------------------------------------------
	while (1) {

		// ---------------------------------------------------------------------
		// Start a new acquisition cycle
		// ---------------------------------------------------------------------
		if (systemFlags.triggerAcquisition) {

			// Start ADC acquisition
			//			HAL_ADC_Start_DMA(&hadc, (uint32_t *)adcBufferDmaData, ADC_DMA_BUFFER_SIZE);
			// Start SPI communication
			// Start I2C communication

			// Clear flag
			systemFlags.triggerAcquisition = false;
		} // systemFlags.triggerAcquisition
		// ---------------------------------------------------------------------

		// ---------------------------------------------------------------------
		// ADC data ready
		// ---------------------------------------------------------------------
		if (systemFlags.adcDataReady) {
			// Get ECG channel 1
			if (!ecgBuffer.push(*adcBufferDmaPointer)) {
				die(ecgBuffer.getLastError());
			}
			// Get ECG channel 2
			if (!ecgBuffer.push(*(adcBufferDmaPointer + 1))) {
				die(ecgBuffer.getLastError());
			}
			// Get battery and charger status
			batteryStatus = *(adcBufferDmaPointer + 2);
			chargerStatus = *(adcBufferDmaPointer + 3);
			// Clear flag
			systemFlags.adcDataReady = false;
		} // systemFlags.adcDataReady
		// ---------------------------------------------------------------------

		// ---------------------------------------------------------------------
		// ACC data ready
		// ---------------------------------------------------------------------
		if (systemFlags.accDataReady) {
			// Get data from devices
			//	- MPU 1
			//	- MPU 2
			//	- MPU 3
			//	- MPU 4
			// Clear flag
			systemFlags.accDataReady = false;
		} // systemFlags.accDataReady
		// ---------------------------------------------------------------------

		// ---------------------------------------------------------------------
		// PPG data ready
		// ---------------------------------------------------------------------
		if (systemFlags.ppgDataReady) {
			// Get data from devices
			//	- PPG 1
			//	- PPG 2
			// Clear flag
			systemFlags.ppgDataReady = false;
		} // systemFlags.ppgDataReady
		// ---------------------------------------------------------------------










		// ---------------------------------------------------------------------
		// Send data to SD card
		// ---------------------------------------------------------------------
		if ((systemFlags.sdCardWaitingToSend) && (!systemFlags.sdCardBusy)) {
			// Clear flag
			systemFlags.sdCardWaitingToSend = false;
		} // systemFlags.sdCardWaitingToSend
		// ---------------------------------------------------------------------

		// ---------------------------------------------------------------------
		// SD card transmission finished
		// ---------------------------------------------------------------------
		if (0) {
			systemFlags.sdCardBusy = false;
		}
		// ---------------------------------------------------------------------




		// ---------------------------------------------------------------------
		// Send data to BLE card
		// ---------------------------------------------------------------------
		if ((systemFlags.bleWaitingToSend) && (!systemFlags.bleBusy)) {
			// Clear flag
			systemFlags.bleWaitingToSend = false;
		} // systemFlags.bleWaitingToSend
		// ---------------------------------------------------------------------

		// ---------------------------------------------------------------------
		// BLE transmission finished
		// ---------------------------------------------------------------------
		if (0) {
			systemFlags.bleBusy = false;
		}
		// ---------------------------------------------------------------------

		// ---------------------------------------------------------------------
		// BLE received data
		// ---------------------------------------------------------------------
		if (systemFlags.bleDataReceived) {
			// Clear flag
			systemFlags.bleDataReceived = false;
		}
		// ---------------------------------------------------------------------







		// ---------------------------------------------------------------------
		// TFT display update
		// ---------------------------------------------------------------------
		if (systemFlags.tftUpdate) {
			// Clear flag
			systemFlags.tftUpdate = false;
		}
		// ---------------------------------------------------------------------





		// ---------------------------------------------------------------------
		// Process data
		// ---------------------------------------------------------------------
		if (systemFlags.processData) {


			//	- PPG AGC
			//	- Create Package
			//	- Store data (SD) signal
			//	- Send data (BLE) signal
			//	- Show ECG (TFT) signal

			// Clear flag
			systemFlags.processData = false;
		} // systemFlags.processData
		// ---------------------------------------------------------------------


	} // while(1)

} // int main(void)

// =============================================================================
// General public functions definitions
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

void activateMpuDevice1(void)
{
	clrPin(MPU1_CS_GPIO_Port, MPU1_CS_Pin);
}

void deactivateMpuDevice1(void)
{
	setPin(MPU1_CS_GPIO_Port, MPU1_CS_Pin);
}

void activateMpuDevice2(void)
{
	clrPin(MPU2_CS_GPIO_Port, MPU2_CS_Pin);
}

void deactivateMpuDevice2(void)
{
	setPin(MPU2_CS_GPIO_Port, MPU2_CS_Pin);
}

void activateMpuDevice3(void)
{
	clrPin(MPU3_CS_GPIO_Port, MPU3_CS_Pin);
}

void deactivateMpuDevice3(void)
{
	setPin(MPU3_CS_GPIO_Port, MPU3_CS_Pin);
}

void activateMpuDevice4(void)
{
	clrPin(MPU4_CS_GPIO_Port, MPU4_CS_Pin);
}

void deactivateMpuDevice4(void)
{
	setPin(MPU4_CS_GPIO_Port, MPU4_CS_Pin);
}







bool mpuReadData(void)
{
	for (uint8_t i = 0; i < MPU_DEVICES; i++) {
		if (!mpuDevice[i].getAccelData(mpuAccelData[i])) {
			return false;
		}
	}
	return true;
}

bool mpuReadOffset(void)
{

	// if (!mpuDevice1.getAccelOffset(accOffset)) {
	// 	sprintf(auxString, "mpu9250 Device 1 Error = %d getAccelOffset()\r", mpuDevice1.getLastError());
	// 	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	return true;
}

bool mpuProcessData(void)
{
	int32_t aux32Buff[MPU_ACC_AXIS];
	char auxString[200];

	strcpy(auxString, "");
	for (uint8_t i = 0; i < MPU_DEVICES; i++) {
		for (uint8_t j = 0; j < MPU_ACC_AXIS; j++) {
			aux32Buff[j] = mpuAccelData[i][j];
			aux32Buff[j] *= 1000;
			aux32Buff[j] /= 16384;
			mpuAccelData[i][j] = (int16_t)aux32Buff[j];
		}
		sprintf(auxString, "%smpu9250 Device %d Data = %6d %6d %6d\r", auxString, i, mpuAccelData[i][0], mpuAccelData[i][1], mpuAccelData[i][2]);
	}
	sprintf(auxString, "%s\r", auxString);
	HAL_UART_Transmit(&huart2, (uint8_t *)auxString, strlen(auxString), 100);
	return true;
}


// =============================================================================
// Interruption callbacks
// =============================================================================

void HAL_SYSTICK_Callback(void)
{
	systemFlags.ticksCounter++;
	if (systemFlags.ticksCounter == 200) {
		systemFlags.ticksCounter = 0;
		systemFlags.triggerAcquisition = true;
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{
	adcBufferDmaPointer = adcBufferDmaData;
	systemFlags.adcDataReady = true;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	adcBufferDmaPointer = adcBufferDmaData + ADC_CHANNELS_PER_SAMPLE;
	systemFlags.adcDataReady = true;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef * huart)
{

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{

}
