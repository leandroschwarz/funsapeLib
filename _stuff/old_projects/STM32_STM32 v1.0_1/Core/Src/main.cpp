#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "funsapeLibGpio.hpp"
#include "funsapeLibSpi.hpp"
#include "funsapeLibTwi.hpp"
#include "funsapeLibStopwatch.hpp"
#include "funsapeLibDs1307.hpp"
#include "funsapeLibMax3010x.hpp"
#include "funsapeLibMpu.hpp"


#ifdef __cplusplus
extern "C" {
#endif

#include "new_inv_mpu.h"
#include "stuff.h"


#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <string.h>

// #define RTC_CONFIGURED_VALUE 0x22

Gpio	led(NLED_GPIO_Port, NLED_Pin);
// Gpio	spiCs(SPI1_CS_GPIO_Port, SPI1_CS_Pin);
// Spi		spi1;
Twi		twi1;
// CircularBuffer<uint16_t> buff(30);
// vbool	updateDateTime;
// Ds1307		rtc;
// Max3010x	ppgSensor(Max3010x::PartId::MAX30102);

void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	MX_TIM4_Init();
	MX_USART3_UART_Init();

	HAL_TIM_Base_Start_IT(&htim4);

	debug.init(&huart3, true, true, (0));
//	debug.init(&huart3, false, true, (0xFFFFFFFF));

//	spi1.init(&hspi1, 20);
//	spi1.setDevice(&spiCs);
//	spi1.changeMode(Spi::SpiMode::MODE_1);

	/*
		float floatValue = -123.40;
		double doubleValue = -123.40;

		Int32Q16_t value1;
		Int32Q16_t value2(-100000);

		Int32Q16_t value3 = value2;
		Int32Q16_t value4 = -20000;
		Int32Q16_t value5 = floatValue;
		Int32Q16_t value6 = doubleValue;
	*/
	twi1.init(&hi2c1, 20);

	// rtc.init(&twi1);
	// ppgSensor.init(&twi1);

	char uartTxBuffer[100];
	uint8_t twiData[10];
	// uint8_t aux8;
	// char uart3TxData[150];
	// twiData[0] = 0x00;
	// twiData[1] = 0x51;
	// twiData[2] = 0x17;
	// twiData[3] = 0x05;
	// twiData[4] = 0x18;
	// twiData[5] = 0x02;
	// twiData[6] = 0x21;
	// twiData[7] = 1 << 4;
	MY_MPL_LOGE("Uart initialized.\r");

	delayMs(1000);
//	inv_error_t result;
//	struct int_param_s int_param;
//	result = new_mpu_init(&int_param);
//	if(result) {
//		MY_MPL_LOGE("Could not initialize gyro.\r");
//	}

#ifdef COMPASS_ENABLED
//	new_mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
#else
//	new_mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
#endif
	twi1.setDevice(0x68);
	int16_t rawAccel[3];

	while(1) {
		twi1.readReg(0x3B, twiData, 6);
		rawAccel[0] = ((int16_t)(twiData[0]) << 8) | (int16_t)(twiData[1]);
		rawAccel[1] = ((int16_t)(twiData[2]) << 8) | (int16_t)(twiData[3]);
		rawAccel[2] = ((int16_t)(twiData[4]) << 8) | (int16_t)(twiData[5]);
		sprintf(uartTxBuffer,
				"%4d, %4d, %4d\r",
				rawAccel[0],
				rawAccel[1],
				rawAccel[2]
		);
		// sprintf(uartTxBuffer,
		// 		"%02x, %02x, %02x, %02x, %02x, %02x\r",
		// 		twiData[0], twiData[1],
		// 		twiData[2], twiData[3],
		// 		twiData[4], twiData[5]
		// );
		HAL_UART_Transmit(&huart3, (uint8_t *)uartTxBuffer, strlen(uartTxBuffer), 100);
		delayMs(500);
	}


	// Checks RTC configuration
	// delayMs(100);
	// if(!rtc.getRamData(0, &aux8, 1)) {
	// 	debugMessage(rtc.getLastError());
	// }
	// if(aux8 != RTC_CONFIGURED_VALUE) {
	// 	if(!rtc.setTime(19, 58, 17, DateTime::TimeFormat::FORMAT_24_HOURS)) {
	// 		debugMessage(rtc.getLastError());
	// 	}
	// 	if(!rtc.setDate(2021, 2, 22)) {
	// 		debugMessage(rtc.getLastError());
	// 	}
	// 	aux8 = RTC_CONFIGURED_VALUE;
	// 	if(!rtc.setRamData(0, &aux8, 1)) {
	// 		debugMessage(rtc.getLastError());
	// 	}
	// }
	// if(!rtc.setSquareWaveGenerator(Ds1307::SquareWave::CLOCK_1_HZ)) {
	// 	debugMessage(rtc.getLastError());
	// }

	// while(1) {
	// 	if(updateDateTime) {
	// 		uint8_t seconds;
	// 		uint8_t minutes;
	// 		uint8_t hours;
	// 		uint8_t weekDay;
	// 		uint8_t day;
	// 		uint8_t month;
	// 		uint16_t year;
	// 		if(!rtc.getDate(&year, &month, &day, &weekDay)) {
	// 			debugMessage(rtc.getLastError());
	// 		}
	// 		if(!rtc.getTime(&hours, &minutes, &seconds, DateTime::TimeFormat::FORMAT_24_HOURS)) {
	// 			debugMessage(rtc.getLastError());
	// 		}
	// 		sprintf(uart3TxData, "%02d/%02d/%04d %u %02d:%02d:%02d\r",
	// 				day, month, year, weekDay, hours, minutes, seconds
	// 		);
	// 		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	// 		updateDateTime = false;
	// 	}
	// }
}

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
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4) {
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_8) { // If The INT Source Is EXTI Line9 (A9 Pin)
//		led.cpl();
		// updateDateTime = true;
	}
}

void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while(1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

// =============================================================================
// END OF FILE
// =============================================================================
