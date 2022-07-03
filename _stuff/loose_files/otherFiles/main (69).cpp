/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibSpi.hpp"
#include "funsapeLibMpu9250.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// *INDENT-OFF*

// *INDENT-ON*
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

Spi spi1;
Mpu9250 mpu[4];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void mpu0Select(void);
void mpu0Deselect(void);
void mpu1Select(void);
void mpu1Deselect(void);
void mpu2Select(void);
void mpu2Deselect(void);
void mpu3Select(void);
void mpu3Deselect(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
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
	/* USER CODE BEGIN 2 */

	// =========================================================================
	// Logic analyzer synchronization pulse
	// =========================================================================

	ledPulseOn(20);

	// =========================================================================
	// Debug configuration
	// =========================================================================

	debug.init(&huart3, false, true);

	// =========================================================================
	// SPI configuration
	// =========================================================================

	spi1.init(&hspi1, 20);

	// =========================================================================
	// MPU configuration
	// =========================================================================

	// Set device selection functions
	mpu[0].setDevice(mpu0Select, mpu0Deselect);
	mpu[1].setDevice(mpu1Select, mpu1Deselect);
	mpu[2].setDevice(mpu2Select, mpu2Deselect);
	mpu[3].setDevice(mpu3Select, mpu3Deselect);
	// Set handler pointer
	for (uint8_t i = 0; i < 4; i++) {
		mpu[i].setHandler(&spi1);
	}
	// Initialize devices
	//while (1) {
	HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
	for (uint8_t i = 0; i < 4; i++) {
		mpu[i].init();
	}
	HAL_Delay(1000);
	//}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */


	while (1) {

		int16_t accel[3][4];
		//int16_t gyro[3][4];
		int16_t temp[4];



		HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
		for (uint8_t i = 0; i < 4; i++) {
			mpu[i].getAccelRawData(&accel[0][i], true);
			mpu[i].getTempRawData(&temp[i], false);
		}

		//mpu[i].getAccelRawData(&accel[0][i], true);





		// mpu[0].getTempRawData(&temp[0], true);
		// mpu[1].getTempRawData(&temp[1], true);
		// mpu[2].getTempRawData(&temp[2], true);
		// mpu[3].getTempRawData(&temp[3], true);

		HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);

		HAL_Delay(1000);

		// HAL_GPIO_WritePin(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin, GPIO_PIN_RESET);
		// mpu1.getAccelData(accel, true);
		// mpu1.getGyroData(gyro, false);
		// mpu1.getTempData(&temp, false);
		// HAL_GPIO_WritePin(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin, GPIO_PIN_SET);

		// HAL_GPIO_WritePin(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin, GPIO_PIN_RESET);
		// mpu2.getAccelData(accel, true);
		// mpu2.getGyroData(gyro, false);
		// mpu2.getTempData(&temp, false);
		// HAL_GPIO_WritePin(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin, GPIO_PIN_SET);

		// HAL_GPIO_WritePin(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin, GPIO_PIN_RESET);
		// mpu3.getAccelData(accel, true);
		// mpu3.getGyroData(gyro, false);
		// mpu3.getTempData(&temp, false);
		// HAL_GPIO_WritePin(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin, GPIO_PIN_SET);

		// HAL_GPIO_WritePin(SPI1_CS_MPU4_GPIO_Port, SPI1_CS_MPU4_Pin, GPIO_PIN_RESET);
		// mpu4.getAccelData(accel, true);
		// mpu4.getGyroData(gyro, false);
		// mpu4.getTempData(&temp, false);
		// HAL_GPIO_WritePin(SPI1_CS_MPU4_GPIO_Port, SPI1_CS_MPU4_Pin, GPIO_PIN_SET);


		//uint8_t errorCode = (uint8_t)mpu1.getLastError();



		//  else {
		// 	HAL_Delay(1000);
		// 	for (uint8_t i = 0; i < errorCode; i++) {
		// 		HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
		// 		HAL_Delay(100);
		// 		HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
		// 		HAL_Delay(100);
		// 	}
	}
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
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

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC
		| RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV128;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void mpu0Select(void)
{
	HAL_GPIO_WritePin(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin, GPIO_PIN_RESET);
}

void mpu0Deselect(void)
{
	HAL_GPIO_WritePin(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin, GPIO_PIN_SET);
}

void mpu1Select(void)
{
	HAL_GPIO_WritePin(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin, GPIO_PIN_RESET);
}

void mpu1Deselect(void)
{
	HAL_GPIO_WritePin(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin, GPIO_PIN_SET);
}

void mpu2Select(void)
{
//	SPI1_CS_MPU3_GPIO_Port->ODR = (SPI1_CS_MPU3_Pin << 16u);
	HAL_GPIO_WritePin(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin, GPIO_PIN_RESET);
}

void mpu2Deselect(void)
{
//	SPI1_CS_MPU3_GPIO_Port->ODR = SPI1_CS_MPU3_Pin;
	HAL_GPIO_WritePin(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin, GPIO_PIN_SET);
}

void mpu3Select(void)
{
	SPI1_CS_MPU4_GPIO_Port->BRR = SPI1_CS_MPU4_Pin;
	// HAL_GPIO_WritePin(SPI1_CS_MPU4_GPIO_Port, SPI1_CS_MPU4_Pin, GPIO_PIN_RESET);
}

void mpu3Deselect(void)
{
	SPI1_CS_MPU4_GPIO_Port->BSRR = SPI1_CS_MPU4_Pin;
	// HAL_GPIO_WritePin(SPI1_CS_MPU4_GPIO_Port, SPI1_CS_MPU4_Pin, GPIO_PIN_SET);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
