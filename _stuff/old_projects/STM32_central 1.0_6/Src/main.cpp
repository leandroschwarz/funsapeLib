
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "mpu9250.hpp"
#include "circularBuffer16.hpp"
#include <string.h>






void SystemClock_Config(void);





#define ADC_CHANNELS_IN_SAMPLE				6
#define ADC_SAMPLES_IN_CIRCULAR_BUFFER		10
#define ADC_BYTES_IN_CHANNEL				2

#define ADC_CHANNELS_IN_CIRCULAR_BUFFER		(ADC_CHANNELS_IN_SAMPLE * ADC_SAMPLES_IN_CIRCULAR_BUFFER)
#define ADC_BYTES_IN_SAMPLE					(ADC_BYTES_IN_CHANNEL * ADC_CHANNELS_IN_SAMPLE)
#define ADC_BYTES_IN_CIRCULAR_BUFFER		(ADC_BYTES_IN_SAMPLE * ADC_SAMPLES_IN_CIRCULAR_BUFFER)




uint16_t adcDataBufferDma[ADC_CHANNELS_IN_SAMPLE];
CircularBuffer16 adcDataBufferCicular(ADC_CHANNELS_IN_CIRCULAR_BUFFER, true);
I2c i2cHandle(&hi2c1);






int main(void)
{
	/* USER CODE BEGIN 1 */
	char auxStr[200];
	uint16_t flags;
	uint16_t occupation;
	uint16_t rdIndex;
	uint16_t wrIndex;
	uint16_t maxSize;
	error_e lastError;
	uint16_t aux16;

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_ADC_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	MX_USART3_UART_Init();
	MX_USART4_UART_Init();
	MX_USART1_UART_Init();
	MX_USART5_UART_Init();

	Mpu9250 mpu;
	

//	mpu.initialize(&i2cHandle);

	error_e error;
	uint16_t errorDet;


	error = i2cHandle.getLastError(&errorDet);
	if (error) {
		auxStr[0] = sizeof(&i2cHandle);
		auxStr[1] = (uint8_t)(errorDet & 0xFF);
		auxStr[2] = (uint8_t)(errorDet >> 8);
		HAL_UART_Transmit(&huart2, (uint8_t *)auxStr, 3, 30);
		HAL_GPIO_WritePin(CHANNEL_1_GPIO_Port, CHANNEL_1_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(CHANNEL_1_GPIO_Port, CHANNEL_1_Pin, GPIO_PIN_RESET);
		sprintf(auxStr, "HDL=%ld/%ld BUF=%ld/%ld\r", i2cHandle.getHandle(),
		        (uint32_t)&hi2c1, i2cHandle.getBufAdd(), (uint32_t)auxStr);
		HAL_UART_Transmit(&huart2, (uint8_t *)auxStr, strlen(auxStr), 30);
		systemHalt();
	}

	if (!i2cHandle.sendData(0x68, TWI_READ, 0x75, (uint8_t *)auxStr, 1)) {
		error = i2cHandle.getLastError(&errorDet);
		HAL_GPIO_WritePin(CHANNEL_1_GPIO_Port, CHANNEL_1_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(CHANNEL_1_GPIO_Port, CHANNEL_1_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(CHANNEL_1_GPIO_Port, CHANNEL_1_Pin, GPIO_PIN_SET);
		sprintf(auxStr, "HDL=%ld/%ld BUF=%ld/%ld\r", i2cHandle.getHandle(),
		        (uint32_t)&hi2c1, i2cHandle.getBufAdd(), (uint32_t)auxStr);
		HAL_UART_Transmit(&huart2, (uint8_t *)auxStr, strlen(auxStr), 30);
		systemHalt();
	}
	
	while (1) {
	}
	
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
}


void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
	        | RCC_OSCILLATORTYPE_HSI14;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	        | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3
	        | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

///////////////////////////////////////////// BEGIN INTERRUPT



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	HAL_GPIO_TogglePin(CHANNEL_1_GPIO_Port, CHANNEL_1_Pin);
	HAL_GPIO_WritePin(CHANNEL_2_GPIO_Port, CHANNEL_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHANNEL_3_GPIO_Port, CHANNEL_3_Pin, GPIO_PIN_SET);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{
	HAL_GPIO_WritePin(CHANNEL_2_GPIO_Port, CHANNEL_2_Pin, GPIO_PIN_RESET);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	HAL_GPIO_WritePin(CHANNEL_3_GPIO_Port, CHANNEL_3_Pin, GPIO_PIN_RESET);
	adcDataBufferCicular.pushBuffer(adcDataBufferDma, ADC_CHANNELS_IN_SAMPLE);
}











///////////////////////////////////////////// END INTERRUPT

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
void assert_failed(char *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
uint32_t getHandle(void);
