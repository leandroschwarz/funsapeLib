/* =============================================================================
 * Project:			bioSignalGrabber
 * File name:		main.cpp
 * Module:			Main project file
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"
#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibSpi.hpp"
#include "funsapeLibMpu9250.hpp"

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define ADC_HALF_WORDS_PER_SAMPLE		4
#define ADC_SAMPLES_PER_BUFFER			10
#define ADC_PERIPH_BUFFER_SIZE			(ADC_HALF_WORDS_PER_SAMPLE * ADC_SAMPLES_PER_BUFFER)
#define SPI1_PERIPH_BUFFER_SIZE			30
#define SPI2_PERIPH_BUFFER_SIZE			30
#define I2C2_RX_PERIPH_BUFFER_SIZE		30
#define I2C2_TX_PERIPH_BUFFER_SIZE		30
#define UART1_RX_PERIPH_BUFFER_SIZE		40
#define UART1_TX_PERIPH_BUFFER_SIZE		40

#define RTC_TIME_SECONDS_REGISTER		0x00
#	define RTC_TIME_SECONDS_BIT					0
#		define RTC_TIME_SECONDS_DEFAULT					0x56
#	define RTC_CLOCK_CTRL_BIT					7
#	define RTC_CLOCK_CTRL_HALT							1
#	define RTC_CLOCK_CTRL_RESUME						0
#define RTC_TIME_MINUTES_REGISTER		0x01
#	define RTC_TIME_MINUTES_BIT					0
#		define RTC_TIME_MINUTES_DEFAULT					0x34
#define RTC_TIME_HOURS_REGISTER			0x02
#	define RTC_TIME_HOURS_BIT					0
#		define RTC_TIME_HOURS_DEFAULT					0x12
#	define RTC_TIME_MODE_BIT					6
#		define RTC_TIME_MODE_24_H						0
#		define RTC_TIME_MODE_12_H						1
#define RTC_DATE_WEEK_DAY_REGISTER		0x03
#	define RTC_DATE_WEEK_DAY_BIT				0
#		define RTC_DATE_WEEK_DAY_DEFAULT				0x07
#define RTC_DATE_MONTH_DAY_REGISTER		0x04
#	define RTC_DATE_MONTH_DAY_BIT				0
#		define RTC_DATE_MONTH_DAY_DEFAULT				0x27
#define RTC_DATE_MONTH_REGISTER			0x05
#	define RTC_DATE_MONTH_BIT					0
#		define RTC_DATE_MONTH_DEFAULT					0x04
#define RTC_DATE_YEAR_REGISTER			0x06
#	define RTC_DATE_YEAR_BIT					0
#		define RTC_DATE_YEAR_DEFAULT					0x19
#define RTC_CTRL_REGISTER				0x07
#	define RTC_CTRL_SWO_LEVEL_BIT				7
#		define RTC_CTRL_SWO_LEVEL_LOW					0
#		define RTC_CTRL_SWO_LEVEL_HIGH					1
#	define RTC_CTRL_SWO_OSC_BIT					4
#		define RTC_CTRL_SWO_OSC_OFF						0
#		define RTC_CTRL_SWO_OSC_ON						1
#	define RTC_CTRL_SWO_FREQ_BIT				0
#		define RTC_CTRL_SWO_FREQ_1_HZ					0
#		define RTC_CTRL_SWO_FREQ_4096_HZ				1
#		define RTC_CTRL_SWO_FREQ_8192_HZ				2
#		define RTC_CTRL_SWO_FREQ_32768_HZ				3

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
// =============================================================================

Spi spi2;
Mpu9250 mpuDevice[4];

void (*activateDevice[4])(void);
void (*deactivateDevice[4])(void);
volatile uint8_t errorCode = 0;

// =============================================================================
// Static functions declarations
// =============================================================================

static void SystemClock_Config(void);
static void activateMpu1(void);
static void deactivateMpu1(void);
static void activateMpu2(void);
static void deactivateMpu2(void);
static void activateMpu3(void);
static void deactivateMpu3(void);
static void activateMpu4(void);
static void deactivateMpu4(void);

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	// -------------------------------------------------------------------------
	// Local Variables ---------------------------------------------------------

	uint16_t adcPeriphBuffer[ADC_PERIPH_BUFFER_SIZE];
	char i2c2RxPeriphBuffer[I2C2_RX_PERIPH_BUFFER_SIZE];
	char i2c2TxPeriphBuffer[I2C2_TX_PERIPH_BUFFER_SIZE];
	uint8_t spi2PeriphBuffer[SPI2_PERIPH_BUFFER_SIZE];
//	uint8_t spi1PeriphBuffer[SPI1_PERIPH_BUFFER_SIZE];
//	char uart1RxPeriphBuffer[UART1_RX_PERIPH_BUFFER_SIZE];
	char uart1TxPeriphBuffer[UART1_TX_PERIPH_BUFFER_SIZE];

	// -------------------------------------------------------------------------
	// Initialize Valiables ----------------------------------------------------

	activateDevice[0] = activateMpu1;
	activateDevice[1] = activateMpu2;
	activateDevice[2] = activateMpu3;
	activateDevice[3] = activateMpu4;
	deactivateDevice[0] = deactivateMpu1;
	deactivateDevice[1] = deactivateMpu2;
	deactivateDevice[2] = deactivateMpu3;
	deactivateDevice[3] = deactivateMpu4;

	// -------------------------------------------------------------------------
	// MCU Configuration -------------------------------------------------------

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	MX_I2C2_Init();
	MX_USB_PCD_Init();

	// -------------------------------------------------------------------------
	// Deactivate All MPU9250 Devices ------------------------------------------

	for (uint8_t i = 0; i < 4; i++) {
		(*activateDevice[i])();
		(*deactivateDevice[i])();
	}

	// -------------------------------------------------------------------------
	// Trigger for Debug -------------------------------------------------------

	HAL_Delay(10);
	ledTurnOn();
	HAL_Delay(10);
	ledTurnOff();

	// -------------------------------------------------------------------------
	// TIMER3 Configuration ----------------------------------------------------

	HAL_TIM_Base_Start_IT(&htim3);

	// -------------------------------------------------------------------------
	// ADC1 Configuration ------------------------------------------------------

	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcPeriphBuffer, ADC_PERIPH_BUFFER_SIZE);

	// -------------------------------------------------------------------------
	// UART Configuration ------------------------------------------------------

	sprintf(uart1TxPeriphBuffer, "Teste da UART1!\r\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxPeriphBuffer, strlen(uart1TxPeriphBuffer), 30);

	// -------------------------------------------------------------------------
	// SPI2 Configuration ------------------------------------------------------

	if (!spi2.setTimeOut(10)) {
		errorCode = ERROR_CODE_SPI_SET_TIME_OUT;
		Error_Handler();
	}
	if (!spi2.setDefaultDevice(activateMpu1, deactivateMpu1)) {
		errorCode = ERROR_CODE_SPI_SET_DEFAULT_DEVICE;
		Error_Handler();
	}
	if (!spi2.init(&hspi2)) {
		errorCode = ERROR_CODE_SPI_INIT;
		Error_Handler();
	}

	// -------------------------------------------------------------------------
	// DS1307 Configuration ----------------------------------------------------

	//	 Verify if RTC is outdated
	if (HAL_I2C_Mem_Read(&hi2c2, 0x68 << 1, 0x08, I2C_MEMADD_SIZE_8BIT, (uint8_t *)i2c2RxPeriphBuffer, 8, 30) != HAL_OK) {
		errorCode = ERROR_CODE_RTC_UPDATE_VERIFICATION;
		Error_Handler();
	}
	//	 If outdated
	if (strcmp(i2c2RxPeriphBuffer, "updated") != 0) {
		i2c2TxPeriphBuffer[0] = 0x00									|
			(RTC_CLOCK_CTRL_RESUME		<< RTC_CLOCK_CTRL_BIT)			|
			(RTC_TIME_SECONDS_DEFAULT	<< RTC_TIME_SECONDS_BIT);
		i2c2TxPeriphBuffer[1] = 0x00									|
			(RTC_TIME_MINUTES_DEFAULT	<< RTC_TIME_MINUTES_BIT);
		i2c2TxPeriphBuffer[2] = 0x00									|
			(RTC_TIME_MODE_24_H			<< RTC_TIME_MODE_BIT)			|
			(RTC_TIME_HOURS_DEFAULT		<< RTC_TIME_HOURS_BIT);
		i2c2TxPeriphBuffer[3] = 0x00									|
			(RTC_DATE_WEEK_DAY_DEFAULT	<< RTC_DATE_WEEK_DAY_BIT);
		i2c2TxPeriphBuffer[4] = 0x00									|
			(RTC_DATE_MONTH_DAY_DEFAULT	<< RTC_DATE_MONTH_DAY_BIT);
		i2c2TxPeriphBuffer[5] = 0x00 |
			(RTC_DATE_MONTH_DEFAULT		<< RTC_DATE_MONTH_BIT);
		i2c2TxPeriphBuffer[6] = 0x00									|
			(RTC_DATE_YEAR_DEFAULT		<< RTC_DATE_YEAR_BIT);
		i2c2TxPeriphBuffer[7] = 0x00									|
			(RTC_CTRL_SWO_LEVEL_LOW		<< RTC_CTRL_SWO_LEVEL_BIT)		|
			(RTC_CTRL_SWO_OSC_ON		<< RTC_CTRL_SWO_OSC_BIT)		|
			(RTC_CTRL_SWO_FREQ_1_HZ		<< RTC_CTRL_SWO_FREQ_BIT);
		//	 Update RTC
		if (HAL_I2C_Mem_Write(&hi2c2, 0x68 << 1, 0x00, I2C_MEMADD_SIZE_8BIT, (uint8_t *)i2c2TxPeriphBuffer, 8, 30) != HAL_OK) {
			errorCode = ERROR_CODE_RTC_UPDATE_DATA;
			Error_Handler();
		}
		//	 Mark as updated
		strcpy(i2c2TxPeriphBuffer, "updated");
		if (HAL_I2C_Mem_Write(&hi2c2, 0x68 << 1, 0x08, I2C_MEMADD_SIZE_8BIT, (uint8_t *)i2c2TxPeriphBuffer,
				strlen(i2c2TxPeriphBuffer) + 1, 30) != HAL_OK) {
			errorCode = ERROR_CODE_RTC_MARK_UPDATED;
			Error_Handler();
		}
	}

	// -------------------------------------------------------------------------
	// MPU9250 Configuration ---------------------------------------------------

	for (uint8_t i = 0; i < 4; i++) {
		if (!mpuDevice[i].initialize(&spi2, activateDevice[i], deactivateDevice[i])) {
			errorCode = ERROR_CODE_MPU_INITIALIZE;
			Error_Handler();
		}
	}

	// -------------------------------------------------------------------------
	// Main Loop ---------------------------------------------------------------

	while (1) {
		if (HAL_I2C_Mem_Read(&hi2c2, 0x68 << 1, 0x00, I2C_MEMADD_SIZE_8BIT, (uint8_t *)i2c2RxPeriphBuffer, 7, 30) != HAL_OK) {
			errorCode = 20;	// DEBUG: Error Code 20
			Error_Handler();
		}
		sprintf(uart1TxPeriphBuffer, "RTC = %02x/%02x/20%02x %d %02x:%02x:%02x\t", i2c2RxPeriphBuffer[4], i2c2RxPeriphBuffer[5],
			i2c2RxPeriphBuffer[6], i2c2RxPeriphBuffer[3], i2c2RxPeriphBuffer[2], i2c2RxPeriphBuffer[1], i2c2RxPeriphBuffer[0]);
		sprintf(uart1TxPeriphBuffer, "%sVbat = %d\r", uart1TxPeriphBuffer, adcPeriphBuffer[0]);
		if (HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxPeriphBuffer, strlen(uart1TxPeriphBuffer), 30) != HAL_OK) {
			errorCode = 21;	// DEBUG: Error Code 21
			Error_Handler();
		}
		HAL_Delay(1000);
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks */
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
	/** Initializes the CPU, AHB and APB busses clock */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{

}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	// debug1Toggle();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	// debug2Toggle();
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{

}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{

}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{

}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// debug0Toggle();
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}

void activateMpu1(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU1_GPIO_Port, SPI2_SS_MPU1_Pin, GPIO_PIN_RESET);
	return;
}

void deactivateMpu1(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU1_GPIO_Port, SPI2_SS_MPU1_Pin, GPIO_PIN_SET);
	return;
}

void activateMpu2(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU2_GPIO_Port, SPI2_SS_MPU2_Pin, GPIO_PIN_RESET);
	return;
}

void deactivateMpu2(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU2_GPIO_Port, SPI2_SS_MPU2_Pin, GPIO_PIN_SET);
	return;
}

void activateMpu3(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU3_GPIO_Port, SPI2_SS_MPU3_Pin, GPIO_PIN_RESET);
	return;
}

void deactivateMpu3(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU3_GPIO_Port, SPI2_SS_MPU3_Pin, GPIO_PIN_SET);
	return;
}

void activateMpu4(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU4_GPIO_Port, SPI2_SS_MPU4_Pin, GPIO_PIN_RESET);
	return;
}

void deactivateMpu4(void)
{
	HAL_GPIO_WritePin(SPI2_SS_MPU4_GPIO_Port, SPI2_SS_MPU4_Pin, GPIO_PIN_SET);
	return;
}

void Error_Handler(void)
{
	while (1) {
		for (uint8_t i = 0; i < errorCode; i++) {
			ledTurnOn();
			HAL_Delay(10);
			ledTurnOff();
			HAL_Delay(10);
		}
		HAL_Delay(100);
	}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
