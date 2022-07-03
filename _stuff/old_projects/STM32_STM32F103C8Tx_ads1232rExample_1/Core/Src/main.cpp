
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibSpi.hpp"
#include <string.h>
#include <stdio.h>
#include "ads1292r.hpp"

void selectDevice(void);
void deselectDevice(void);

char uart3TxData[100];
char spiData[10];

Spi spi1;
Error funsapeError;

void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_TIM4_Init();
	MX_SPI1_Init();
	MX_USART3_UART_Init();

	HAL_TIM_Base_Start_IT(&htim4);
	strcpy(uart3TxData, "[  OK  ] Uart 3 is working!\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	// -------------------------------------------------------------------------
	// SPI configuration
	// -------------------------------------------------------------------------

	// Handler creation
	funsapeError = spi1.getLastError();
	if((uint16_t)funsapeError) {
		sprintf(uart3TxData, "[ FAIL ] Spi 1 handler not created! [0x%04x]\r", (uint16_t)funsapeError);
	} else {
		sprintf(uart3TxData, "[  OK  ] Spi 1 handler created!\r");
	}
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	// Handler initialization
	if(spi1.init(&hspi1)) {
		sprintf(uart3TxData, "[  OK  ] Spi 1 handler initialized!\r");
	} else {
		funsapeError = spi1.getLastError();
		sprintf(uart3TxData, "[ FAIL ] Spi 1 handler not initialized! [0x%04x]\r", (uint16_t)funsapeError);
	}
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);



	// Ads1292r device(selectDevice, deselectDevice);
	// device.init();
	// if(device.getId()) {
	// 	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
	// } else {
	// 	HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
	// }
	// device.readDataMode(true);

	while(1) {

	}

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
	/** Initializes the CPU, AHB and APB buses clocks
	*/
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

void selectDevice(void)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void deselectDevice(void)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void Error_Handler(void)
{
	__disable_irq();
	while(1) {
	}
}
