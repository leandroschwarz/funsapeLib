
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mpu9250.hpp"
#include "circularBuffer16.hpp"
#include <string.h>
#include "setup.h"
#include "myFunctions.hpp"



// Global variables
uint16_t adcDataBufferDma[ADC_CHANNELS_IN_SAMPLE];
CircularBuffer16 adcDataBufferCicular(ADC_CHANNELS_IN_CIRCULAR_BUFFER, true);
I2c i2cHandle(&hi2c1);

// Main function
int main(void)
{
	// Local variavles
	FATFS sdCard;
	FIL file;
	char auxStr[200];
	UINT auxUint;
	FRESULT res;

	error_e error;
	uint16_t errorDet;

//	uint16_t aux16;


	uint16_t flags;
	uint16_t occupation;
	uint16_t rdIndex;
	uint16_t wrIndex;
	uint16_t maxSize;









	// Reset of all peripherals, Initializes the Flash interface and the Systick
	HAL_SystemInit();

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	/**/HAL_Delay(10);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	// SD Card - Initializing driver
	sdCardInit(&sdCard);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	/**/HAL_Delay(12);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	// SD Card - Open header file
	res = f_open(&file, "FORMULA.HDR", FA_CREATE_ALWAYS | FA_WRITE);
	if (res != FR_OK) {
		Error_Handler();
	}
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	/**/HAL_Delay(14);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	// SD Card - Write test info
	sprintf(auxStr, "Teste número %d\r\n", 1802);
	res = f_write(&file, auxStr, strlen(auxStr), &auxUint);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	/**/HAL_Delay(16);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	// SD Card - Close header file
	if (f_close(&file) != FR_OK) {
		Error_Handler();
	}
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	/**/HAL_Delay(18);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);

	// Accelerometer
	Mpu9250 mpu;
	mpu.initialize(&i2cHandle);
	mpu.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ,
	        MPU9250_ACCEL_DLPF_1046_HZ);

	
	
	/**/while (1) {
		int16_t accelData[3];
		int16_t gyroData[3];
		mpu.getAccelData(accelData, AXIS_ALL);
		/**/HAL_Delay(40);
		sprintf(auxStr, "%8d %8d %8d\r", accelData[0], accelData[1], accelData[2]);
		HAL_UART_Transmit(&huart2, (uint8_t *)auxStr, strlen(auxStr), 30);
		/**/}


	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	/**/HAL_Delay(20);
	/**/HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);

	// SD Card - Open data file
	res = f_open(&file, "FORMULA.DAT", FA_CREATE_ALWAYS | FA_WRITE);
	if (res != FR_OK) {
		Error_Handler();
	}



	while (1) {
	}
	/*
	adcDataBufferCicular.getStatus(&flags, &occupation, &rdIndex, &wrIndex,
	        &maxSize, &lastError);

	sprintf(auxStr,
	        "fla=0x%02X occ=0x%02X rdI=0x%02X wrI=0x%02X max=0x%02X err=0x%02X\r",
	        flags, occupation, rdIndex, wrIndex, maxSize,
	        lastError);
	//		HAL_UART_Transmit(&huart2, (uint8_t *)auxStr, strlen(auxStr), 30);

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_ADC_Start_DMA(&hadc, (uint32_t *)adcDataBufferDma,
	        ADC_CHANNELS_IN_SAMPLE);

	while (1) {
	//		HAL_Delay(1000);
		if (adcDataBufferCicular.getOccupation() > 42) {
			// Gets data
			adcDataBufferCicular.popBuffer((uint16_t *)auxStr, 100, &aux16);
			// Does something
			adcDataBufferCicular.getStatus(&flags, &occupation, &rdIndex,
			        &wrIndex, &maxSize, &lastError);
		}

	}
	 */
	while (1) {

	}
}


/* USER CODE BEGIN 4 */

///////////////////////////////////////////// BEGIN INTERRUPT



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{

}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	adcDataBufferCicular.pushBuffer(adcDataBufferDma, ADC_CHANNELS_IN_SAMPLE);
}










